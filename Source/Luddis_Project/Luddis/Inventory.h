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

	int getChips() const;
	void setChips(int chips);
	void changeChips(int chips);

	int getDust() const;
	void setDust(int dust);
	void addDust(int dust);

	int getMaxDust() const;
	void setMaxDust(int maxDust);
	void increaseMaxDust(int maxDust);

	int getPowerUp() const;
	void setPowerUp(int pow);
	void changePowerUp(int pow);

	int getEggs() const;
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
	int mMaxDust;
	int mEggs;
	int mPowerUp;
};

#endif