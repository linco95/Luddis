#ifndef INCLUDED_COLLIDABLE
#define INCLUDED_COLLIDABLE

#include "Entity.h"
#include <vector>

class Collidable : public Entity{
public:

	enum Category{
		FRIEND,
		ENEMY,
		BG
	};

	enum Type{
		REC,
		CIRCLE
	};
	typedef std::vector<Collidable*> CollidableVector;
	Collidable();
	virtual ~Collidable();

	// Funktion för att uppdatera entiteten
	virtual Category getCategory() = 0;
	virtual Type getType() = 0;
	virtual int collide() = 0;
};

#endif