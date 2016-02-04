#include "Inventory.h"

Inventory::Inventory():
mChips(0),
mDust(0)
{

}

Inventory::~Inventory(){

}

Inventory& Inventory::getInstance(){

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