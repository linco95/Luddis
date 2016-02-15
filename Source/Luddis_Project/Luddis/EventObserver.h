#ifndef INCLUDED_EVENTOBSERVER
#define INCLUDED_EVENTOBSERVER

namespace sf{
	class Event;
}

class EventObserver{
public:
	virtual ~EventObserver();

	virtual void onEvent(const sf::Event &aEvent) = 0;
	
};

#endif