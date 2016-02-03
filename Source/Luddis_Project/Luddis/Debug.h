#ifdef _DEBUG
#ifndef INCLUDED_DEBUG
#define INCLUDED_DEBUG

#include <SFML\Graphics\Drawable.hpp>
#include <vector>
#include <string>

#include <SFML/Graphics/Shape.hpp>


class Debug /*: public sf::Drawable*/ {
public:
	enum ERRORLEVEL{
		DEFAULT,
		INFO,
		WARNING,
		ERROR,
		FATAL
	};
	typedef std::vector<sf::Shape*> ShapeVector;


	//void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	static void log(const std::string& aMsg, const ERRORLEVEL& aLvl);

	Debug(const Debug&) = delete;
	//Debug& Debug(const Debug&) = delete;
private:
	Debug();

	static ShapeVector mShapes;

};

#endif //INCLUDED_DEBUG
#endif // _DEBUG