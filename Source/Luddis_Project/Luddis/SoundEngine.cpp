#include "SoundEngine.h"

SoundEngine::SoundEngine(){

}

SoundEngine& SoundEngine::getInstance(){
	static SoundEngine se;
	return se;
}

