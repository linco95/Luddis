#include "Inventory.h"

Inventory::Inventory():
mChips(0),
mDust(0),
mEggs(0)
{

}

Inventory::~Inventory(){

}

Inventory& Inventory::getInstance(){
	static Inventory I;
	return I;
}

int Inventory::getChips(){
	return mChips;
}

void Inventory::setChips(int chips){
	mChips = chips;
}

void Inventory::changeChips(int chips){
	mChips += chips;
}


int Inventory::getDust(){
	return mDust;
}

void Inventory::setDust(int dust){
	mDust = dust;
}

void Inventory::changeDust(int dust){
	mDust += dust;
}

int Inventory::getEggs(){
	return mEggs;
}

void Inventory::setEggs(int eggs){
	mEggs = eggs;
}

void Inventory::changeEggs(int eggs){
	mEggs += eggs;
}