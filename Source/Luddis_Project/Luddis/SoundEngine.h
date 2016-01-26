#ifndef _INCLUDED_SOUNDENGINE_
#define _INCLUDED_SOUNDENGINE_

class SoundEngine{
public:
	SoundEngine(SoundEngine&) = delete;
	SoundEngine& operator=(SoundEngine&) = delete;

	static SoundEngine& getInstance();

	
private:
	SoundEngine();

};

#endif // !_INCLUDED_SOUNDENGINE_
