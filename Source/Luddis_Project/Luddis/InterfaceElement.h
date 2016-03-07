#ifndef _INCLUDED_INTERFACEELEMENT_
#define _INCLUDED_INTERFACEELEMENT_

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>
#include <string>

class InterfaceElement: public sf::Drawable, public sf::Transformable{
public:
	//Used for render sorting.
	enum Strata{
		FIFTH,
		FOURTH,
		THIRD,
		SECOND,
		FIRST
	};
	InterfaceElement();
	virtual ~InterfaceElement();

	//Function for drawing the object.
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
	//Function for updating the object. Mostly timer related.
	virtual void tick(const sf::Time& deltaTime) = 0;
	//Returns the renderlayer. Used for sorting later.
	virtual Strata getRenderLayer() const = 0;
	//Return if the object is alive or not.
	//Dead objects get deallocated.
	virtual bool isAlive() const = 0;
	//Inactive objects will not have its
	//tick() function called.
	virtual bool isActive() const = 0;
	//Sets the active state of the object.
	virtual void setActive(const bool& active) = 0;
	//Used mostly by buttons to call a higher instanced
	//objects onClick() function. Not purely virtual
	//for the reason of having only a few interface elements
	//using it.
	virtual void onClick(std::string buttonFunc){}
};

#endif // !_INCLUDEDINTERFACE_ELEMENT_
