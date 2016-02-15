#include "EventManager.h"
#include "EventObserver.h"

using namespace sf;

EventManager::EventManager(){

}

EventManager::~EventManager(){

}

#pragma region ObserverPattern

// Function that adds an observer that listens for the types in types. Observer will get notified if any of the specific types are occuring. if
void EventManager::attatch(EventObserver *obs, const std::vector<Event::EventType> &types){
	for (auto t : types){
		attatch(obs, t);
	}
}
// same as above, but listens to a specific type
void EventManager::attatch(EventObserver *obs, const Event::EventType &type){
	// Get the ObserverSet at the specific event, and insert observer
	mEventToObservers[type].insert(obs);
	// WEAK REFERENCE, do not delete object here. That is for the ObjectManager. Objects are responsible for detatching from the subject.
}


// Notify all observers. (according to their interests)
void EventManager::notify(const sf::Event& aEvent) const{
	if (mEventToObservers.find(aEvent.type) != mEventToObservers.end()){
		const ObserverSet& observers = mEventToObservers.at(aEvent.type);
		for (auto o : observers){
			o->onEvent(aEvent);
		}
	}
	
}


// Detatch this observer from the corresponding types
void EventManager::detatch(EventObserver *obs, const std::vector<Event::EventType> &types){
	for (auto t : types){
		detatch(obs, t);
	}
}
// Detatch this observer from the specific type
void EventManager::detatch(EventObserver *obs, const Event::EventType &type){
	mEventToObservers[type].erase(obs);
}
#pragma endregion Functions related to the observer pattern



EventManager& EventManager::getInstance(){
	static EventManager em;
	return em;
}