#ifdef _DEBUG
#ifndef INCLUDED_DEBUG
#define INCLUDED_DEBUG

#include <SFML\Graphics\Drawable.hpp>
#include <vector>
#include <string>

namespace sf{
	class Shape;
}

class Debug : public sf::Drawable {
public:
	enum ERRORLEVEL{
		DEFAULT,
		INFO,
		WARNING,
		ERROR,
		FATAL
	};
	typedef std::vector<sf::Shape*> ShapeVector;


	void draw(const sf::RenderTarget& target, sf::RenderStates states) override;

	static void log(const std::string& aMsg, const ERRORLEVEL& aLvl);

	Debug() = delete;
	~Debug() = delete;
private:
	
	static ShapeVector mShapes;

};

#endif //INCLUDED_DEBUG
#endif // _DEBUG