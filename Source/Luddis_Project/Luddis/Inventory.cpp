#include "Inventory.h"
#include <cmath>
//#include <algorithm>

Inventory::Inventory():
mChips(0),
mDust(0),
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

int Inventory::getChips() const{
	return mChips;
}

void Inventory::setChips(int chips){
	mChips = chips;
}

void Inventory::changeChips(int chips){
	mChips += chips;
}


int Inventory::getDust() const{
	return mDust;
}

void Inventory::setDust(int dust){
	mDust = dust;
	mDust = std::min(mDust, mMaxDust);
}

void Inventory::addDust(int dust){
	mDust += dust;
	mDust = std::min(mDust, mMaxDust);
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

int Inventory::getEggs() const{
	return mEggs;
}

void Inventory::setEggs(int eggs){
	mEggs = eggs;
}

void Inventory::changeEggs(int eggs){
	mEggs += eggs;
}

int Inventory::getPowerUp() const{
	return mPowerUp;
}

void Inventory::setPowerUp(int pow){
	mPowerUp = pow;
}

void Inventory::changePowerUp(int pow){
	mPowerUp += pow;
}