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

/*
RenderingsFörslag


Entity{

enum RenderLayer{
LEVEL,
BACKGROUND,
ITEMS,
}



void Render(){
sortering efter den enumen

EntityList temp(mEntities);

for(e : temp){
if(e.getRenderLAyer() >


for(auto e : temp){
aWindow.draw(e);
*/
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