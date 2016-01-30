#ifdef _DEBUG
#ifndef INCLUDED_DEBUG
#define INCLUDED_DEBUG

#include <SFML\Graphics\Shape.hpp>
#include <vector>
#include <ostream>

class Debug{
public:
	typedef std::vector<sf::Shape*> ShapeVector;



	static void log(const std::ostream& aMsg);

	Debug() = delete;
	~Debug() = delete;
private:
	
	static ShapeVector mShapes;

};

#endif //INCLUDED_DEBUG
#endif // _DEBUG