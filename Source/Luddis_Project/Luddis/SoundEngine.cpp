#include "SoundEngine.h"
#include "ResourceManager.h"
#include "Debug.h"
#include <cassert>

SoundEngine::SoundEngine() :
mMainVolume(100), mSoundVolume(100), mMusicVolume(100),
mStudioSystem(NULL), mLowLvlSystem(NULL),
	mSounds(), mChannel(NULL) {

	initialize();
}

SoundEngine::~SoundEngine() {
	finalize();
}

SoundEngine& SoundEngine::getInstance() {
	static SoundEngine se;
	return se;
}

void SoundEngine::initialize() {
	FMOD_RESULT result;
	void *extraDriverData1 = NULL, *extraDriverData2 = NULL;
	result = FMOD::Studio::System::create(&mStudioSystem);
	if (result != FMOD_OK) {
		Debug::log(("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result)), Debug::FATAL);
		exit(-1);
	}
	result = FMOD::System_Create(&mLowLvlSystem);
	if (result != FMOD_OK) {
		Debug::log(("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result)), Debug::FATAL);
		exit(-1);
	}
	result = mStudioSystem->initialize(MAX_SOUND_CHANNELS, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData1);
	result = mLowLvlSystem->init(MAX_SOUND_CHANNELS, FMOD_INIT_NORMAL, extraDriverData2);
}

void SoundEngine::finalize() {
	mLowLvlSystem->close();
	mLowLvlSystem->release();
	mStudioSystem->unloadAll();
	mStudioSystem->flushCommands();
	mStudioSystem->release();
}

void SoundEngine::setMainVolume(float volume) {
	mMainVolume = volume;
	setSoundVolume(mSoundVolume);
	setMusicVolume(mMusicVolume);
}

float SoundEngine::getMainVolume() const{
	return mMainVolume;
}

void SoundEngine::setSoundVolume(float volume) {
	mSoundVolume = volume;
	mFinalVolume = mMainVolume*mSoundVolume / 100;
	//Studio instances
	for (auto se : mSoundEventInstances) {
		se.second->setVolume(mFinalVolume);
	}
	FMOD_RESULT result;
	result = mChannel->setVolume(mFinalVolume);
	//Low level instances
	mChannel->setVolume(mFinalVolume);
}

float SoundEngine::getSoundVolume() const{
	return mSoundVolume;
}

void SoundEngine::setMusicVolume(float volume) {
	mMusicVolume = volume;
	mFinalVolume = mMainVolume*mMusicVolume / 100;
	//Studio instances
	for (auto s : mMusicEventInstances) {
		s.second->setVolume(mFinalVolume);
	}
}

float SoundEngine::getMusicVolume() const{
	return mMusicVolume;
}


int SoundEngine::playSound(const char* filename) {
	if (mSounds.find(filename) == mSounds.end()) {
		createSound(filename);
	}
	FMOD_RESULT result;
	result = mLowLvlSystem->playSound(mSounds[filename], 0, false, &mChannel);
	result = mChannel->setVolume(mMainVolume*mSoundVolume / 100);
	float volume = 0;
	mChannel->getVolume(&volume);
	int channelIndex = 0;
	mChannel->getIndex(&channelIndex);
	return channelIndex;
}

FMOD_RESULT SoundEngine::playEvent(const char * path) {
	FMOD_RESULT result;
	//See if it can be found in the sound map,
	//otherwise assume that its in the music map.
	int i = 0;
	i = 2;
	if (mMusicEventInstances.find(path) != mMusicEventInstances.end()) {
		result = mMusicEventInstances[path]->start();
		result = mMusicEventInstances[path]->setVolume(mMainVolume* mSoundVolume / 100);
	}
	else {
		result = mSoundEventInstances[path]->start();
		result = mSoundEventInstances[path]->setVolume(mMainVolume* mMusicVolume / 100);
	}

	return result;
}

FMOD_RESULT SoundEngine::stopSound(int channelIndex) {
	FMOD_RESULT result;
	FMOD::Channel* channel;
	result = mLowLvlSystem->getChannel(channelIndex, &channel);
	if (result != FMOD_ERR_INVALID_HANDLE)
		result = channel->stop();
	return result;
}

FMOD_RESULT SoundEngine::stopEvent(const char* path, FMOD_STUDIO_STOP_MODE stopMode) {
	//See if it can be found in the sound map,
	//otherwise assume that its in the music map.
	FMOD_RESULT result;
	if (mMusicEventInstances.find(path) != mMusicEventInstances.end())
		result = mMusicEventInstances[path]->stop(stopMode);
	else
		result = mSoundEventInstances[path]->stop(stopMode);
	return result;
}

FMOD_RESULT SoundEngine::setEventParameter(const char* path, const char* parameter, float value) {
	FMOD_RESULT result;
	//See if it can be found in the sound map,
	//otherwise assume that its in the music map.
	if (mMusicEventInstances.find(path) != mMusicEventInstances.end())
		result = mMusicEventInstances[path]->setParameterValue(parameter, value);
	else
		result = mSoundEventInstances[path]->setParameterValue(parameter, value);
	return result;
}

FMOD_RESULT SoundEngine::loadBank(const char* filename) {
	FMOD::Studio::Bank* bank = NULL;
	FMOD_RESULT result;
	result = mStudioSystem->loadBankFile(filename, FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);
	assert(result == FMOD_OK);
	return result;
}

FMOD::Studio::Bank* SoundEngine::getBank(const char* filename) {
	FMOD::Studio::Bank* bank = NULL;
	FMOD_RESULT result;
	result = mStudioSystem->getBank(filename, &bank);
	assert(result == FMOD_OK);
	loadBank(filename);
	return bank;
}

FMOD_RESULT SoundEngine::unloadBank(const char* filename) {
	FMOD::Studio::Bank* bank;
	FMOD_RESULT result = mStudioSystem->getBank(filename, &bank);
	result = bank->unload();
	assert(result == FMOD_OK);

	return result;
}

FMOD_RESULT SoundEngine::createEvent(const char* path, EventType eventType) {
	int bankCount;
	mStudioSystem->getBankCount(&bankCount);
	SoundEventDesc* desc = NULL;
	FMOD_RESULT result;
	result = mStudioSystem->getEvent(path, &desc);
	assert(result == FMOD_RESULT::FMOD_OK);
	SoundEventInst** inst = NULL;
	if (eventType == SOUND)
		inst = &mSoundEventInstances[path];
	else
		inst = &mMusicEventInstances[path];
	result = desc->createInstance(inst);
	assert(result == FMOD_OK);

	if (eventType == SOUND)
		(*inst)->setVolume(mMainVolume*mSoundVolume / 100);
	else
		(*inst)->setVolume(mMainVolume*mMusicVolume / 100);
	return result;
}

FMOD_RESULT SoundEngine::releaseSoundEvent(const char* path) {
	FMOD_RESULT result;
	result = mSoundEventInstances[path]->release();
	mSoundEventInstances.erase(path);
	return result;
}

FMOD_RESULT SoundEngine::releaseMusicEvent(const char* path) {
	FMOD_RESULT result;
	result = mMusicEventInstances[path]->release();
	mSoundEventInstances.erase(path);
	return result;
}

FMOD_RESULT SoundEngine::createSound(const char* filepath, bool loop) {
	FMOD_RESULT result;
	FMOD::Sound** sound = &mSounds[filepath];
	result = mLowLvlSystem->createSound(filepath, FMOD_DEFAULT, 0, sound);
	if (!loop)
		(*sound)->setMode(FMOD_LOOP_OFF);
	sound;
	return result;
}

FMOD_RESULT SoundEngine::releaseSound(const char* filepath) {
	FMOD_RESULT result;
	result = mSounds[filepath]->release();
	mSounds.erase(filepath);
	return result;
}

//static sf::Clock actualTime;
void SoundEngine::update(const sf::Time& deltaTime) {
	mStudioSystem->update();
	mLowLvlSystem->update();
}
