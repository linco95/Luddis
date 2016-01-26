#ifndef INCLUDED_EVENTSUBJECT
#define INCLUDED_EVENTSUBJECT

#include <vector>
#include <SFML\Window\Event.hpp>

class EventObserver;


class EventSubject{
public:
	virtual ~EventSubject();
	
	// Function that adds an observer that listens for the types in types. Observer will get notified if any of the specific types are occuring. if
	virtual void attatch(EventObserver *obs, const std::vector<const sf::Event::EventType&> &types) = 0;
	// same as above, but listens to a specific type
	virtual void attatch(EventObserver *obs, const sf::Event::EventType &type) = 0;
	// same as above, but listens to all kinds of events
	virtual void attatch(EventObserver *obs) = 0;



	// Notify all observers. (according to their interests)
	virtual const sf::Event& notify() const = 0;


	// Detatch this observer from the corresponding types
	virtual void detatch(EventObserver *obs, const std::vector<const sf::Event::EventType&> &types) = 0;
	// Detatch this observer from the specific type
	virtual void detatch(EventObserver *obs, const sf::Event::EventType &type) = 0;
	// Detatch this observer from all types
	virtual void detatch(EventObserver *obs) = 0;
};


#endif