#include "CollisionManager.h"

CollisionManager::CollisionManager(){

}

CollisionManager::~CollisionManager(){

}

void CollisionManager::addCollidable(Collidable* collidable){
	mCollidables.push_back(collidable);
}

