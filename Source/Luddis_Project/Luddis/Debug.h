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
		NONFATAL,
		FATAL
	};
	
	typedef std::vector<sf::Shape*> ShapeVector;
	
	// Functionality to draw debug shapes?
	//void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	//static ShapeVector mShapes;

	static void finalize();
	static void initialize();

	static void log(const std::string& aMsg, const ERRORLEVEL& aLvl = DEFAULT);

	Debug(const Debug&) = delete;
	Debug& operator=(const Debug&) = delete;
private:
	Debug();
	static std::string mLog;
	//static std::string mLog;


};

#endif //INCLUDED_DEBUG
