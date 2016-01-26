#include "EntityManager.h"

// Constructor for the entity manager (empty)
EntityManager::EntityManager(){
}

// Destructor for the entity manager
EntityManager::~EntityManager(){
	while (!mEntities.empty())
		mEntities.pop_back();
}

// Function to add entities to the manager (an entity pointer)
void EntityManager::addEntity(Entity* entity){
	mEntities.push_back(entity);
}

// Function to remove dead entities from the manager and memory
void EntityManager::removeDeadEntities(){
	EntitiesVector temp;
	for (auto e : mEntities){
		if (e->isAlive()) {
			temp.push_back(e);
		}
	}
	mEntities = temp;
	return;
}

// Iterate through the entities and updates them (input time)
void EntityManager::updateEntities(const sf::Time& deltaTime){
	for (auto e : mEntities){
		e->tick(deltaTime);
	}
}