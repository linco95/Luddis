#ifndef _INCLUDED_GRAPHICALUSERINTERFACE_
#define _INCLUDED_GRAPHICALUSERINTERFACE_

#include "InterfaceElement.h"
#include <vector>

class GraphicalUserInterface{
public:
	//Since GUI elements will be limited, it might
	//be useful to change from vector to array at some point

	typedef std::vector<InterfaceElement*> InterfaceElementVector;
	GraphicalUserInterface();
	~GraphicalUserInterface();

	//Might not be needed, if it loads a default setting and adapts from there
	/*void addElement(InterfaceElement* interfaceElement);
	void clearElements();*/
	void drawElements();
private:
	InterfaceElementVector mElements;
	void setupInterface();
};

#endif // !_INCLUDED_GRAPHICALUSERINTERFACE_
