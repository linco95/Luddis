#ifndef INCLUDED_POWERUPS
#define INCLUDED_POWERUPS

class PowerUps {
public:
	PowerUps();
	~PowerUps();

	virtual void activate() = 0;
};

#endif // !INCLUDED_POWERUPS