#ifndef _INCLUDED_VIEWUTILITY_
#define _INCLUDED_VIEWUTILITY_

#include <SFML/Graphics/View.hpp>

//Simply used on a low level to retrieve default view info.
namespace ViewUtility{
	static const int VIEW_WIDTH = 1920;
	static const int VIEW_HEIGHT = 1080;

	static sf::View getViewSize(){return sf::View(sf::FloatRect(0, 0, (float)VIEW_WIDTH, (float)VIEW_HEIGHT));}
}

#endif // !_INCLUDED_VIEWUTILITY_
