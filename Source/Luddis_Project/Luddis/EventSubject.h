#ifndef INCLUDED_EVENTSUBJECT
#define INCLUDED_EVENTSUBJECT

#include <vector>

class Observer;
namespace sf{
	class Event{
	public:
		enum EventType;
	};
}


class EventSubject{
	virtual ~EventSubject();
	
	// Function that adds an observer that listens for the types in types. Observer will get notified if any of the specific types are occuring. if
	virtual void attatch(Observer *obs, const std::vector<const sf::Event::EventType&> &types) = 0;
	// same as above, but listens to a specific type
	virtual void attatch(Observer *obs, const sf::Event::EventType &type) = 0;
	// same as above, but listens to all kinds of events
	virtual void attatch(Observer *obs) = 0;



	// Notify all observers. (according to their interests)
	virtual const sf::Event& notifyAll() const = 0;


	// Detatch this observer from the corresponding types
	virtual void detatch(Observer *obs, const std::vector<const sf::Event::EventType&> &types) = 0;
	// Detatch this observer from the specific type
	virtual void detatch(Observer *obs, const sf::Event::EventType &type) = 0;
	// Detatch this observer from all types
	virtual void detatch(Observer *obs) = 0;
};


#endif