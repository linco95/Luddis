/*
* Class for handling of entities:
* Allocating/Creating
* Updating
* Retrieving
* Deallocating/Deleting
*/
#ifndef INCLUDED_ENTITYMANAGER
#define INCLUDED_ENTITYMANAGER

#include <SFML/System.hpp>
#include <vector>
#include "Entity.h"
#include <SFML/Graphics.hpp>

class EntityManager {
public:
	//Is no longer a singleton for the reason of having each state manage
	//a separate entity vector.
	typedef std::vector<Entity*> EntitiesVector;
	EntityManager();
	~EntityManager();

	void addEntity(Entity* entity);
	void removeDeadEntities();

	void updateEntities(const sf::Time& deltaTime);
	void renderEntities(sf::RenderWindow& window);
	void emptyVector();

	// Function that gets used to read all entities. (Added to be used by rendering)
	const EntitiesVector& getEntities() const;

	void stunEntities(const sf::Time deltatime);
private:
	
	EntitiesVector mEntities;
};
#endif