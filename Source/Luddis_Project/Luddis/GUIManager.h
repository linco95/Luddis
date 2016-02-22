#ifndef _INCLUDED_GUIMANAGER_
#define _INCLUDED_GUIMANAGER_

#include "InterfaceElement.h"
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>

class GUIManager{
public:
	GUIManager();
	~GUIManager();

	void addInterfaceElement(InterfaceElement* ie);
	void removeObsoleteElements();
	void renderElements(sf::RenderWindow& window);
	void updateElements(const sf::Time& deltaTime);
	void clearInterfaceElements();
private:

	typedef std::vector<InterfaceElement*> InterfaceElementVector;
	InterfaceElementVector mElements;
};

#endif // !_INCLUDED_GUIMANAGER_
