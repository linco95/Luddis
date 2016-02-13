#include "EntityManager.h"
#include <SFML\Graphics.hpp>

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

// Sorted rendering depending on the enum of the entity
/*
* 0 - BACKGROUND,
* 1 - OBSTACLES,
* 2 - ITEM,
* 3 - PLAYER,
* 4 - FOREGROUND
* 5 - GUI_BACKGROUND
* 6 - GUI_FOREGROUND
*/

void EntityManager::renderEntities(sf::RenderWindow& window){

	// sort the entities renderlayer
	std::map<Entity::RenderLayer, EntitiesVector> renderMap;
	for (auto e : mEntities) {
		renderMap[e->getRenderLayer()].push_back(e);
	}
	// render the entities
	
	for (auto it : renderMap){
		
		for (auto e : it.second){
			window.draw(*e);
		}
	}
}

// Function that gets used to read all entities. (Added to be used by rendering)
const EntityManager::EntitiesVector& EntityManager::getEntities() const{
	return mEntities;
}

// Iterate through the entities and updates them (input time)
void EntityManager::updateEntities(const sf::Time& deltaTime){
	EntitiesVector temp(mEntities);
	for (auto e : temp){
		e->tick(deltaTime);
	}
}
