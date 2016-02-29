#include "Inventory.h"
#include <cmath>

Inventory::Inventory():
mChips(0),
mDust(2),
mEggs(0),
mPowerUp(0),
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

#pragma region POWER-UP
int Inventory::getPowerUp() const{
	return mPowerUp;
}

void Inventory::setPowerUp(int pow){
	mPowerUp = pow;
}

void Inventory::changePowerUp(int pow){
	mPowerUp += pow;
}
#pragma endregion Power-up related functions