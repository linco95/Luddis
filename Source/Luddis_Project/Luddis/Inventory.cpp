#include "Inventory.h"
#include <cmath>
#include "SpiderWeb.h"

Inventory::Inventory():
mChips(0),
mDust(2),
mEggs(0),
mMaxDust(15)
{
	
}

Inventory::~Inventory(){

}

Inventory& Inventory::getInstance(){
	static Inventory I;
	return I;
}

#pragma region CHIPS
int Inventory::getChips() const{
	return mChips;
}

void Inventory::setChips(int chips){
	mChips = chips;
}

void Inventory::changeChips(int chips){
	mChips += chips;
}
#pragma endregion Chips related functions

#pragma region DUST
int Inventory::getDust() const{
	return mDust;
}

void Inventory::setDust(int dust){
	mDust = std::min(mDust = dust, mMaxDust);
	mDust = std::max(mDust, 0);
}

void Inventory::addDust(int dust){
	mDust = std::min(mDust += dust, mMaxDust);
	mDust = std::max(mDust, 0);
}

int Inventory::getMaxDust() const{
	return mMaxDust;
}

void Inventory::setMaxDust(int maxDust){
	mMaxDust = maxDust;
}

void Inventory::increaseMaxDust(int maxDust){
	mMaxDust += maxDust;
}
#pragma endregion Dust related functions

#pragma region EGGS
int Inventory::getEggs() const{
	return mEggs;
}

void Inventory::setEggs(int eggs){
	mEggs = eggs;
}

void Inventory::changeEggs(int eggs){
	mEggs += eggs;
}
#pragma endregion Egg related functions

#pragma region POWER-UPS
void Inventory::activateFirst(sf::Time deltaTime) {
	//mPowerUps[0]->activate(deltaTime);
	mPowerOne->activate(deltaTime);
}

void Inventory::activateSecond(sf::Time deltaTime) {
	//mPowerUps[1]->activate(deltaTime);
}

void Inventory::activateThird(sf::Time deltaTime) {
	//mPowerUps[2]->activate(deltaTime);
}

void Inventory::choseFirst(PowerUps* powerUp) {
	//mPowerUps[0] = powerUp;
	mPowerOne = powerUp;

}

void Inventory::choseSecond(PowerUps* powerUp) {
	//mPowerUps[1] = powerUp;
}

void Inventory::choseThird(PowerUps* powerUp) {
	//mPowerUps[2] = powerUp;
}
#pragma endregion Power-up related functions

void Inventory::addEntityManager(EntityManager* entityManager) {
	mEntityManager = entityManager;
	// VERY QUICKFIX SOLUTION
	//mPowerUps[0] = new SpiderWeb(mEntityManager);
	//mPowerUps[1] = new SpiderWeb(mEntityManager);
	//mPowerUps[2] = new SpiderWeb(mEntityManager);
}
