#include "EntityManager.h"

// Constructor for the entity manager (empty)
EntityManager::EntityManager(){
}

// Destructor for the entity manager
EntityManager::~EntityManager(){
	std::for_each(_objects.begin(), _objects.end(), ObjectDeallocator());
}

// Function to add entities to the manager (input "name" and an entity pointer)
void EntityManager::addEntity(std::string name, Entity* entity){
	_objects.insert(std::pair<std::string, Entity*>(name, entity));
}

// Function to remove entities from the manager and memory (input "name")
void EntityManager::removeEntity(std::string name){
	std::map<std::string, Entity*>::iterator results = _objects.find(name);
	if (results != _objects.end()){
		delete results->second;
		_objects.erase(results);
	}
}

// Retrieves an entity with a certain "name"
Entity* EntityManager::getEntity(std::string name){
	std::map<std::string, Entity*>::const_iterator results = _objects.find(name);
	if (results == _objects.end())
		return NULL;
	return results->second;
}

// Iterate through the entities and updates them
void EntityManager::updateEntities(){
	//TODO
}