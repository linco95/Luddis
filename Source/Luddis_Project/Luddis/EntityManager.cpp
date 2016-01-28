#include "EntityManager.h"
#include <SFML\Graphics.hpp>

static const sf::Color BGCOLOR = sf::Color::Black;

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

// Sorted rendering depending on the enum of the entity
/*
* 0 - BACKGROUND,
* 1 - OBSTACLES,
* 2 - ITEM,
* 3 - PLAYER,
* 4 - FOREGROUND
* 5 - GUI
*/

void EntityManager::renderEntities(sf::RenderWindow& window){

	/*
	* TODO Fix the quickfix
	* Make looped sorting dynamic
	*/

	EntitiesVector unsorted(mEntities);
	EntitiesVector unsortedTwo;
	EntitiesVector sorted;
	// 0
	for (auto e : unsorted){
		if (e->getRenderLayer() == 0)
			sorted.push_back(e);
		else
			unsortedTwo.push_back(e);
	}
	unsorted.clear();
	// 1
	for (auto e : unsortedTwo){
		if (e->getRenderLayer() == 1)
			sorted.push_back(e);
		else
			unsorted.push_back(e);
	}
	unsortedTwo.clear();
	// 2
	for (auto e : unsorted){
		if (e->getRenderLayer() == 2)
			sorted.push_back(e);
		else
			unsortedTwo.push_back(e);
	}
	unsorted.clear();
	// 3
	for (auto e : unsortedTwo){
		if (e->getRenderLayer() == 3)
			sorted.push_back(e);
		else
			unsorted.push_back(e);

	}
	unsortedTwo.clear();
	// 4
	for (auto e : unsorted){
		if (e->getRenderLayer() == 4)
			sorted.push_back(e);
		else
			unsortedTwo.push_back(e);
	}
	unsorted.clear();
	// 5
	for (auto e : unsortedTwo){
		if (e->getRenderLayer() == 5)
			sorted.push_back(e);
		else
			unsorted.push_back(e);
	}
	//assert(unsorted.size() == 0);
	window.clear(BGCOLOR);
	for (auto e : sorted){
		window.draw(*e);
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