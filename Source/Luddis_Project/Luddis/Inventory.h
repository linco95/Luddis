#ifndef INCLUDED_INVENTORY
#define INCLUDED_INVENTORY

#include "Entity.h"
#include <vector>

class Inventory {
public:
	Inventory(Inventory&) = delete;
	Inventory& operator=(Inventory&) = delete;
	~Inventory();

	static Inventory& getInstance();

	int getChips();
	void setChips(int chips);
	void changeChips(int chips);

	int getDust();
	void setDust(int dust);
	void changeDust(int dust);

	int getEggs();
	void setEggs(int eggs);
	void changeEggs(int eggs);

	//void addItem(Entity* item);
	// Check mIsAlive
	//void loseItems();
private:
	Inventory();

	typedef std::vector<Entity*> InventoryVector;

	int mChips;
	int mDust;
	int mEggs;
};

#endif