#ifndef INCLUDED_INVENTORY
#define INCLUDED_INVENTORY

#include "Entity.h"
#include <vector>
#include "PowerUps.h"
#include "EntityManager.h"

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

	int getEggs() const;
	void setEggs(int eggs);
	void changeEggs(int eggs);

	void setAccessoryHead(std::string filename);
	std::string getAccessoryHead() const;
	void setAccessoryTail(std::string filename);
	std::string getAccessorTail() const;
	void setColorScheme(int index);
	int getColorScheme() const;

	void activateFirst(sf::Time deltaTime);
	void activateSecond(sf::Time deltaTime);
	void activateThird(sf::Time deltaTime);

	void choseFirst(PowerUps* powerUp);
	void choseSecond(PowerUps* powerUp);
	void choseThird(PowerUps* powerUp);

	//void addItem(Entity* item);
	// Check mIsAlive
	//void loseItems();
private:
	Inventory();

	typedef std::vector<Entity*> InventoryVector;
	typedef std::vector<PowerUps*> PowerUpVector;

	int mChips;
	int mDust;
	int mMaxDust;
	int mEggs;
	std::string mAccessoryHead, mAccessoryTail;
	int mColorScheme;
	PowerUpVector mPowerUps;
	PowerUps* mPowerOne;
};

#endif