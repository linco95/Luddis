#include "LuddisStateStunned.h"
#include "LuddisStatePlayable.h"
#include "EntityManager.h"
#include "CollidableEntity.h"
#include "Luddis.h"
#include "Inventory.h"
#include <SFML/Graphics/RenderWindow.hpp>

static const float INVINCIBILITY_TIMER = 0.75f;
static const std::string HIT_ANIMATION = "Resources/Images/Spritesheets/Luddis_hit";

LuddisStateStunned::LuddisStateStunned(Luddis* playerPtr, float stunDuration, sf::RenderWindow* window, EntityManager* entityManager):
mPlayerPtr(playerPtr),
mWindow(window),
mEntityManager(entityManager),
mInvincibility(INVINCIBILITY_TIMER),
mStunDuration(stunDuration){

}

LuddisStateStunned::~LuddisStateStunned(){

}

void LuddisStateStunned::tick(const sf::Time & deltaTime) {
	if (mStunDuration > 0) {
		mStunDuration -= deltaTime.asSeconds();
	}
	else {
		mPlayerPtr->setPlayerState(new LuddisStatePlayable(mPlayerPtr, mWindow, mEntityManager));
	}
	if (mInvincibility >= 0) {
		mInvincibility -= deltaTime.asSeconds();
	}
}

void LuddisStateStunned::collide(CollidableEntity * collidable, const sf::Vector2f & moveAway){
	if (mInvincibility <= 0) {
		// Collision with damaging object
		if (collidable->getCollisionCategory() == CollidableEntity::BG_DAMAGE || collidable->getCollisionCategory() == CollidableEntity::ENEMY) {
			mPlayerPtr->getAnimation()->replaceAnimation(HIT_ANIMATION);

			Inventory::getInstance().addDust(-1);
			if (Inventory::getInstance().getDust() == 0) {

				//TODO: add dead state here

			}
		}
		// Collision with a stunning entity
		if (collidable->getCollisionCategory() == CollidableEntity::ENEMY_STUN) {
			//TODO: add stunned state here
			mPlayerPtr->getAnimation()->replaceAnimation(HIT_ANIMATION);
		}
	}
}
