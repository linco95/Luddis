#include "LuddisStateDead.h"
#include "Luddis.h"
#include "GameManager.h"
#include "GameStateLevel.h"
#include "GameStatePaused.h"
#include "Menu.h"
#include <cmath>

static const std::string ANIMATION_ALMOSTDEAD = "Resources/Images/Spritesheets/luddis_CriticalHealth";

LuddisStateDead::LuddisStateDead(Luddis* playerPtr):
mPlayerPtr(playerPtr){
	GameManager::getInstance().setGameState(&GameStatePaused::getInstance());
	GameStateLevel::getInstance().setPlayable(false);
	GameStatePaused::getInstance().createMenu(Menu::MenuType::DEATHMENU, &GameStateLevel::getInstance());
}

LuddisStateDead::~LuddisStateDead(){

}

void LuddisStateDead::tick(const sf::Time & deltaTime){

}

void LuddisStateDead::collide(CollidableEntity * collidable, const sf::Vector2f & moveAway){

}