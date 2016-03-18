#ifndef _INCLUDED_SOUNDENGINE_
#define _INCLUDED_SOUNDENGINE_

#include "fmod_studio.h"
#include "fmod_studio.hpp"
#include "fmod_errors.h"
#include <SFML/System/Time.hpp>
#include <map>

class SoundEngine{
public:
	typedef FMOD::Studio::EventInstance SoundEventInst;
	typedef FMOD::Studio::EventDescription SoundEventDesc;
	typedef FMOD::Studio::Bank SoundBank;
	~SoundEngine();
	SoundEngine(const SoundEngine&) = delete;
	SoundEngine& operator=(const SoundEngine&) = delete;
	static SoundEngine& getInstance();

	void update(const sf::Time& deltaTime);
	FMOD_RESULT loadBank(const char* filename);
	SoundBank* getBank(const char* filename);
	FMOD_RESULT unloadBank(const char* filename);

	enum EventType {
		SOUND,
		MUSIC
	};

	//An event is used to play a sound or music, and has
	//parameters that can be changed with the
	//SetEventParameter() function.
	FMOD_RESULT createEvent(const char* path, EventType eventType);
	
	//Release events to free up memory allocated by them.
	FMOD_RESULT releaseSoundEvent(const char* path);
	FMOD_RESULT releaseMusicEvent(const char* path);

	//Creates a sound in the low level system that
	//does not have events.
	FMOD_RESULT createSound(const char* filepath, bool loop = false);
	
	//Frees up allocated memory.
	FMOD_RESULT releaseSound(const char* filepath);

	//Volume is a value from 0-1
	void setMainVolume(float volume);
	float getMainVolume() const;
	//Volume is a value from 0-1
	void setSoundVolume(float volume);
	float getSoundVolume() const;
	//Volume is a value from 0-1
	void setMusicVolume(float volume);
	float getMusicVolume() const;

	void mute(bool mute);

	//Plays a sound on the low level system, and returns
	//the channel index used to play it.
	int playSound(const char* filename);

	//Stops playing any sound on the given channel.
	//(On the low level system)
	FMOD_RESULT stopSound(int channelIndex);
	
	//Starts an event based sound or music track.
	//Path is the same as the one used in CreateEvent().
	FMOD_RESULT playEvent(const char* path);

	//Stops playing an event. Path is the same one
	//used in CreateEvent().
	FMOD_RESULT stopEvent(const char* path, FMOD_STUDIO_STOP_MODE stopMode = FMOD_STUDIO_STOP_IMMEDIATE);

	//Sets a parameter for an active event.
	//path is the same as the one used in CreateEvent(),
	//parameter is the name of the internal parameter,
	//value is the new value of the parameter.
	FMOD_RESULT setEventParameter(const char* path, const char* parameter, float value);
	//Fadetime in seconds
	
private:
	void initialize();
	void finalize();
	SoundEngine();
	
	typedef std::map<const char*, FMOD::Studio::EventInstance*> EventInstMap;
	EventInstMap mSoundEventInstances;
	EventInstMap mMusicEventInstances;
	typedef std::map<const char*, FMOD::Sound*> SoundMap;
	SoundMap mSounds;
	static const int MAX_SOUND_CHANNELS = 64;

	FMOD::Studio::System* mStudioSystem;
	FMOD::System* mLowLvlSystem;
	FMOD::Channel* mChannel;
	bool mMute;
	float mMainVolume;
	float mSoundVolume;
	float mMusicVolume;
};

#endif // !_INCLUDED_SOUNDENGINE_
