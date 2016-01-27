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
	typedef std::vector<Entity*> EntitiesVector;

	EntityManager(EntityManager&) = delete;
	EntityManager& operator=(EntityManager&) = delete;
	~EntityManager();

	static EntityManager& getInstance();
	
	void addEntity(Entity* entity);
	void removeDeadEntities();

	void updateEntities(const sf::Time& deltaTime);

	// Function that gets used to read all entities. (Added to be used by rendering)
	const EntitiesVector& getEntities() const;
private:
	EntityManager();
	EntitiesVector mEntities;
};
#endif