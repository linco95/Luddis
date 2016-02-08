#include "SoundEngine.h"
#include "ResourceManager.h"
#include <SFML/Audio.hpp>
#include <SFML/System/Time.hpp>

SoundEngine::SoundEngine():
mMainVolume(100), mSoundVolume(100), mMusicVolume(100), mFading(false){

}

SoundEngine& SoundEngine::getInstance(){
	static SoundEngine se;
	return se;
}

void SoundEngine::setMainVolume(float volume){
	mMainVolume = volume;
	for (int i = 0; i < MAX_SOUND_CHANNELS; i++){
		mSoundChannels[i].setVolume(mMainVolume*mSoundVolume / 100);
	}
	if (mCurrentMusic != nullptr){
		mCurrentMusic->setVolume(mMainVolume*mMusicVolume / 100);
	}
}

void SoundEngine::setSoundVolume(float volume){
	mSoundVolume = volume;
	for (int i = 0; i < MAX_SOUND_CHANNELS; i++){
		mSoundChannels[i].setVolume(mMainVolume*mSoundVolume / 100);
	}
}

void SoundEngine::setMusicVolume(float volume){
	mMusicVolume = volume;
	if (mCurrentMusic != nullptr){
		mCurrentMusic->setVolume(mMainVolume*mMusicVolume / 100);
	}
}

void SoundEngine::playSound(std::string filename){
	//Might have to change this around to disallow the same buffert from playing two different sounds
	for (int i = 0; i < MAX_SOUND_CHANNELS; i++){
		if (mSoundChannels[i].getStatus() != sf::Sound::Playing){
			mSoundChannels[i].setBuffer(ResourceManager::getInstance().getSoundBuffer(filename));
			mSoundChannels[i].play();
			return;
		}
	}
}

void SoundEngine::playMusic(std::string filename){
	if (mCurrentMusic != 0) {
		mCurrentMusic->stop();
	}
	mCurrentMusic = &ResourceManager::getInstance().getMusic(filename);
	mCurrentMusic->setLoop(true);
	setMusicVolume(mMusicVolume);
	mCurrentMusic->play();
}
static sf::Clock actualTime;
void SoundEngine::update(const sf::Time& deltaTime){
	//sf::Time aDT = actualTime.restart();
	//float pitch = deltaTime.asSeconds() / aDT.asSeconds();
	if (mFading == true){
		fadeTransition(deltaTime);
		//mFadingMusic->setPitch(1 + pitch);
	}
	//mCurrentMusic->setPitch(1 + pitch);
}

void SoundEngine::fadeTransition(const sf::Time& deltaTime){
	mFadeTimeLeft -= deltaTime.asSeconds();
	if (mFadeTimeLeft > 0){
		mFadingMusic->setVolume(mMainVolume*mMusicVolume*(mFadeTimeLeft / mFadeTime) / 100);
		mCurrentMusic->setVolume(mMainVolume*mMusicVolume*((mFadeTime - mFadeTimeLeft) / mFadeTime) / 100);
	}
	else{
		mFading = false;
		mCurrentMusic->setVolume(mMainVolume*mMusicVolume / 100);
		mFadingMusic->stop();
	}
}

void SoundEngine::fadeToNewMusic(std::string filename, float fadeTime){
	if (fadeTime != 0 && mFading==false){
		mFading = true;
		mFadeTime = fadeTime;
		mFadeTimeLeft = fadeTime;
		playMusic(filename);
		mFadingMusic->setVolume(0.0f);
	}
}