#include "GUIManager.h"
#include <cassert>
#include <map>
#include "Renderer.h"
GUIManager::GUIManager() :
mElements(){

}

GUIManager::~GUIManager(){
	clearInterfaceElements();
}

GUIManager& GUIManager::operator+(const GUIManager& guiM)
{
	
	for (auto g : guiM.mElements) {
		this->addInterfaceElement(g);
	}
	return *this;
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
	static Renderer* renderer = &Renderer::getInstance();
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
				//renderer->addDrawableToQueue(e, Renderer::GUI);
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