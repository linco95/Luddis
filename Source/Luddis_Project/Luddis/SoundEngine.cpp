#include "SoundEngine.h"
#include "ResourceManager.h"

SoundEngine::SoundEngine():
mMainVolume(100), mSoundVolume(100), mMusicVolume(100), mFading(false)
{
	mMusic.setLoop(true);
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
	mMusic.setVolume(mMainVolume*mMusicVolume / 100);
}

void SoundEngine::setSoundVolume(float volume){
	mSoundVolume = volume;
	for (int i = 0; i < MAX_SOUND_CHANNELS; i++){
		mSoundChannels[i].setVolume(mMainVolume*mSoundVolume / 100);
	}
}

void SoundEngine::setMusicVolume(float volume){
	mMusicVolume = volume;
	mMusic.setVolume(mMainVolume*mMusicVolume / 100);
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
	mCurrentMusicFile = filename;
	sf::Music* temp = &ResourceManager::getInstance().getMusic(filename);
	temp->setLoop(true);
	temp->play();
}

void SoundEngine::update(const sf::Time& deltaTime){
	if (mFading == true){
		fadeTransition(deltaTime);
	}
}

void SoundEngine::fadeTransition(const sf::Time& deltaTime){
	mFadeTimeLeft -= deltaTime.asSeconds();
	if (mFadeTimeLeft > 0){
		ResourceManager::getInstance().getMusic(mFadingMusicFile).setVolume(mMainVolume*mMusicVolume*(mFadeTimeLeft / mFadeTime) / 100);
		ResourceManager::getInstance().getMusic(mCurrentMusicFile).setVolume(mMainVolume*mMusicVolume*((mFadeTime - mFadeTimeLeft) / mFadeTime) / 100);
	}
	else{
		mFading = false;
		ResourceManager::getInstance().getMusic(mCurrentMusicFile).setVolume(mMainVolume*mMusicVolume / 100);
		ResourceManager::getInstance().getMusic(mFadingMusicFile).stop();
	}
}

void SoundEngine::fadeToNewMusic(std::string filename, float fadeTime){
	if (fadeTime != 0 && mFading==false){
		mFading = true;
		mFadeTime = fadeTime;
		mFadeTimeLeft = fadeTime;
		mFadingMusicFile = mCurrentMusicFile;
		playMusic(filename);
		ResourceManager::getInstance().getMusic(mFadingMusicFile).setVolume(0.0f);
	}
}