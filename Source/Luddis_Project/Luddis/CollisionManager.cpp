#include "CollisionManager.h"

CollisionManager::CollisionManager() :
mCollidables(){
}

CollisionManager::~CollisionManager(){

}

CollisionManager& CollisionManager::getInstance(){
	static CollisionManager cM;
	return cM;
}

void CollisionManager::addCollidable(Collidable* collidable){
	mCollidables.push_back(collidable);
}

void CollisionManager::removeDeadCollidables(){
	CollidableVector temp;
	for (auto e : mCollidables){
		if (e->isAlive()) {
			temp.push_back(e);
		}
	}
	mCollidables = temp;
	return;
}

void CollisionManager::detectCollisions(){
	
	CollidableVector collidables(mCollidables);
	for (CollidableVector::size_type i = 0; i < collidables.size(); i++){
		Collidable *collidable0 = collidables.at(i);
		for (CollidableVector::size_type j = i + 1; j < collidables.size(); j++){
			Collidable *collidable1 = collidables.at(j);
			if (collidable0->getHitBox().intersects(collidable1->getHitBox()) && (collidable0->getCollisionCategory() != collidable1->getCollisionCategory())){
					collidable0->collide(collidable1);
					collidable1->collide(collidable0);
			}
		}
	}
}