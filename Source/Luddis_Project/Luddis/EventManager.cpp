#include "EventManager.h"

using namespace sf;

EventManager::EventManager(){

}

EventManager::~EventManager(){

}

// Function that adds an observer that listens for the types in types. Observer will get notified if any of the specific types are occuring. if
void EventManager::attatch(EventObserver *obs, const std::vector<const Event::EventType&> &types){

}
// same as above, but listens to a specific type
void EventManager::attatch(EventObserver *obs, const Event::EventType &type){

}
// same as above, but listens to all kinds of events
void EventManager::attatch(EventObserver *obs){

}



// Notify all observers. (according to their interests)
const Event& EventManager::notify() const{

	// temp
	return *(new Event());
}


// Detatch this observer from the corresponding types
void EventManager::detatch(EventObserver *obs, const std::vector<const Event::EventType&> &types){

}
// Detatch this observer from the specific type
void EventManager::detatch(EventObserver *obs, const Event::EventType &type){

}
// Detatch this observer from all types
void EventManager::detatch(EventObserver *obs){

}