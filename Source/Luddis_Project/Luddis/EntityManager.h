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

class EntityManager {
public:
	EntityManager();
	~EntityManager();
	
	void addEntity(Entity* entity);
	void removeDeadEntities();

	void updateEntities(const sf::Time& deltaTime);
private:
	typedef std::vector<Entity*> EntitiesVector;
	EntitiesVector mEntities;
};
#endif