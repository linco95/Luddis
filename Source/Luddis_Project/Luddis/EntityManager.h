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
#include <map>
#include "Entity.h"

class EntityManager {
public:
	EntityManager();
	~EntityManager();
	
	void addEntity(std::string name, Entity* entity);
	void removeEntity(std::string name);
	Entity* getEntity(std::string name);

	void updateEntities();
private:
	std::map<std::string, Entity*>_objects;

	struct ObjectDeallocator{
		void operator()(const std::pair<std::string, Entity*> & p) const{
			delete p.second;
		}
	};
};
#endif