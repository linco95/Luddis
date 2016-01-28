#include "Level.h"
#include "ResourceManager.h"

using namespace sf;

static const char* BGFILEPATH = "resources/images/leve_l1BG.png";
Level::Level(){

}

Level::~Level(){

}

void Level::initializeLevel(sf::RenderWindow& aWindow, Transformable* aTarget){
	//assert(aTarget != 0);

	mTarget = aTarget;
	ResourceManager::getInstance().loadTexture(BGFILEPATH, IntRect(Vector2<int>(), Vector2<int>(Texture::getMaximumSize(), Texture::getMaximumSize())));
	mBackground.setTexture(ResourceManager::getInstance().getTexture(BGFILEPATH));
	// assert(mBackground.getTextureRect().height() >= aWindow.getSize().y);
	mView = aWindow.getView();
	mView.setSize(mView.getSize().x, (float)mBackground.getTextureRect().height);
	mView.setCenter(mView.getSize().x / 2.0f, mView.getSize().y / 2.0f);
	////aWindow.
	//mView.setCenter(mVie)
	aWindow.setView(mView);
}

void Level::tick(const sf::Time& deltaTime) {

}
bool Level::isAlive() const {
	return true;
}
Entity::RenderLayer Level::getRenderLayer() const {
	return Entity::RenderLayer::BACKGROUND;
}

void Level::draw(RenderTarget& target, RenderStates states) const {
	target.draw(mBackground);
}
