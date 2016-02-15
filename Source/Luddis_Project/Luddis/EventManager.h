#ifndef INCLUDED_EVENTMANAGER
#define INCLUDED_EVENTMANAGER

#include "EventSubject.h"
#include <set>
#include <unordered_map>

/* TODO
	-	Implement observer pattern
	-	Implement singelton pattern
*/

namespace sf{
	class Window;
}

// Class that handles incoming events and notifies every observer which event was incoming.
class EventManager : public EventSubject {
public:
	EventManager();
	~EventManager();

	// Function that adds an observer that listens for the types in types. Observer will get notified if any of the specific types are occuring. if
	void attatch(EventObserver *obs, const std::vector<sf::Event::EventType> &types) override;
	// same as above, but listens to a specific type
	void attatch(EventObserver *obs, const sf::Event::EventType &type) override;

	// Notify all observers. (according to their interests)
	void notify(const sf::Event& aEvent) const override;

	// Detatch this observer from the corresponding types
	void detatch(EventObserver *obs, const std::vector<sf::Event::EventType> &types) override;
	// Detatch this observer from the specific type
	void detatch(EventObserver *obs, const sf::Event::EventType &type) override;

private:
	typedef std::set<EventObserver*> ObserverSet;
	typedef std::unordered_map<sf::Event::EventType, ObserverSet> EventToObservers;

	EventToObservers mEventToObservers;

};

#endif