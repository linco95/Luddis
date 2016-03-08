#include "GUIManager.h"
#include <cassert>
#include <map>

GUIManager::GUIManager() :
mElements(){

}

GUIManager::~GUIManager(){
	clearInterfaceElements();
}

void GUIManager::addInterfaceElement(InterfaceElement* ie){
	assert(ie != nullptr);
	mElements.push_back(ie);
}

void GUIManager::removeObsoleteElements(){
	InterfaceElementVector temp;
	for (auto e : mElements){
		if (e->isAlive()) {
			temp.push_back(e);
		}
		else{
			delete e;
		}
	}
	mElements = temp;
}

void GUIManager::clearInterfaceElements(){
	while (!mElements.empty()){
		delete mElements.back();
		mElements.pop_back();
	}
}

void GUIManager::renderElements(sf::RenderWindow& window){
	// sort the entities renderlayer
	std::map<InterfaceElement::Strata, InterfaceElementVector> renderMap;
	for (auto e : mElements) {
		renderMap[e->getRenderLayer()].push_back(e);
	}
	// render the entities
	for (auto it : renderMap){

		for (auto e : it.second){
			if (e->isActive()){
				window.draw(*e);
			}
		}
	}
}

void GUIManager::updateElements(const sf::Time& deltaTime){
	InterfaceElementVector temp(mElements);
	for (auto e : temp){
		if (e->isActive()){
			e->tick(deltaTime);
		}
	}
}