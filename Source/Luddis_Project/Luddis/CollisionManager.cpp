#include "CollisionManager.h"

CollisionManager::CollisionManager(){

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

void CollisionManager::detectCollisions(){
	Collidable::CollidableVector collidables(mCollidables);
	for (Collidable::CollidableVector::size_type i = 0; i < collidables.size(); i++){
		Collidable *collidable0 = collidables[i];
		for (Collidable::CollidableVector::size_type j = i + 1; j < collidables.size(); j++){
			Collidable *collidable1 = collidables[j];
			if (collidable0->getHitBox().intersects(collidable1->getHitBox())){
				collidable0->collide();
				collidable1->collide();
			}
		}
	}
}