#ifndef INCLUDED_EVENTMANAGER
#define INCLUDED_EVENTMANAGER

#include "EventSubject.h"

/* TODO
	-	Implement observer pattern
*/

namespace sf{
	class Window;
}

class EventManager : public EventSubject {
public:
	EventManager();
	~EventManager();

	// Function that adds an observer that listens for the types in types. Observer will get notified if any of the specific types are occuring. if
	virtual void attatch(EventObserver *obs, const std::vector<const sf::Event::EventType&> &types) override;
	// same as above, but listens to a specific type
	virtual void attatch(EventObserver *obs, const sf::Event::EventType &type) override;
	// same as above, but listens to all kinds of events
	virtual void attatch(EventObserver *obs) override;



	// Notify all observers. (according to their interests)
	virtual const sf::Event& notify() const override;


	// Detatch this observer from the corresponding types
	virtual void detatch(EventObserver *obs, const std::vector<const sf::Event::EventType&> &types) override;
	// Detatch this observer from the specific type
	virtual void detatch(EventObserver *obs, const sf::Event::EventType &type) override;
	// Detatch this observer from all types
	virtual void detatch(EventObserver *obs) override;


};

#endif