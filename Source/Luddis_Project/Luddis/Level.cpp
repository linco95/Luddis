#include "Level.h"
#include "ResourceManager.h"
#include <SFML\Audio.hpp>


using namespace sf;

static const float X_OFFSET = 200.f,
				   SCROLLSPEED = 100.f;

static const Entity::RenderLayer LAYER = Entity::RenderLayer::BACKGROUND;
static const char* BGFILEPATH = "resources/images/Grafik_Bana1PrototypBakg_S1D2V4.png";
Level::Level(){

}

Level::~Level(){

}

void Level::initializeLevel(sf::RenderWindow& aWindow, Transformable* aTarget){
	//assert(aTarget != 0);

	mTarget = aTarget;
	mWindow = &aWindow;

	ResourceManager::getInstance().loadTexture(BGFILEPATH, IntRect(Vector2<int>(), Vector2<int>(Texture::getMaximumSize(), (int)mWindow->getView().getSize().y)));
	mBackground.setTexture(ResourceManager::getInstance().getTexture(BGFILEPATH));
	
	// QUICKFIX TODO make properiate sound resource management
	mLevelMusic = &ResourceManager::getInstance().getMusic("resources/audio/musik16.wav");
	mLevelMusic->play();


	mPointsOfNoReturn.push_back(mWindow->getSize().x / 2 + 1000.f);
	mCurrentPONR = mWindow->getView().getSize().x / 2;
	
}


void Level::tick(const sf::Time& deltaTime) {
	updateView(deltaTime);
	
}

void Level::updateView(const Time& deltaTime){
	View view = mWindow->getView();
	float xPos = mTarget->getPosition().x;
	float screenXPos = view.getCenter().x;
	float deltaMove = SCROLLSPEED * deltaTime.asSeconds();
	float minX = mCurrentPONR;
	float maxX = mBackground.getTextureRect().width - view.getSize().x / 2;

	
	if (xPos < screenXPos - X_OFFSET / 2){
		deltaMove *= -1;
	}
	else if (xPos < screenXPos + X_OFFSET / 2){
		deltaMove = 0;
	}
	if (screenXPos >= minX && screenXPos <= maxX) {
		if (!mPointsOfNoReturn.empty() && screenXPos >= mPointsOfNoReturn.back()){
			mCurrentPONR = mPointsOfNoReturn.back();
			mPointsOfNoReturn.pop_back();
		}
		view.move(deltaMove, 0);
		view.getCenter().x < minX ? view.setCenter(minX, view.getCenter().y) : 0;
		view.getCenter().x > maxX ? view.setCenter(maxX, view.getCenter().y) : 0;
	}
	mWindow->setView(view);
}
bool Level::isAlive() {
	return true;
}
Entity::RenderLayer Level::getRenderLayer() const {
	return LAYER;
}

void Level::draw(RenderTarget& target, RenderStates states) const {
	target.draw(mBackground);
}
