#ifndef INCLUDED_SIDERWEB
#define INCLUDED_SIDERWEB

#include "PowerUps.h"

class SpiderWeb :public PowerUps {
public:
	SpiderWeb();
	~SpiderWeb();

	void activate() override;
private:

};


#endif // !INCLUDED_SIDERWEB
