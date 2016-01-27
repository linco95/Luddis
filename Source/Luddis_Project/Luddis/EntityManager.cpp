#include "EntityManager.h"

// Constructor for the entity manager
EntityManager::EntityManager():
mEntities(){
}

// Destructor for the entity manager
EntityManager::~EntityManager(){
	while (!mEntities.empty()){
		delete mEntities.back();
		mEntities.pop_back();
	}
}

//Fetch the singleton instance
EntityManager& EntityManager::getInstance(){
	static EntityManager eM;
	return eM;
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
		else{
			delete e;
		}
	}
	mEntities = temp;
	return;
}

// Function that gets used to read all entities. (Added to be used by rendering)
const EntityManager::EntitiesVector& EntityManager::getEntities() const{
	return mEntities;
}

// Iterate through the entities and updates them (input time)
void EntityManager::updateEntities(const sf::Time& deltaTime){
	for (auto e : mEntities){
		e->tick(deltaTime);
	}
}