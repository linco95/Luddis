#include "CharacterPortrait.h"
#include "ResourceManager.h"

static const std::string DEFAULT_FONT = "Resources/fonts/arial.ttf";
static const int DEFAULT_FONTSIZE = 24;

CharacterPortrait::CharacterPortrait(std::string textureFilename, std::string characterName, sf::Vector2f pos) :
mSprite(ResourceManager::getInstance().getTexture(textureFilename)),
mIsAlive(true),
mIsActive(false),
mName(characterName, ResourceManager::getInstance().getFont(DEFAULT_FONT), DEFAULT_FONTSIZE){
	setPosition(pos);
	float spriteOrigoX = (float)mSprite.getTextureRect().width / 2;
	float spriteOrigoY = (float)mSprite.getTextureRect().height / 2;
	mSprite.setOrigin(spriteOrigoX, spriteOrigoY);
	float textOrigoX = mName.getGlobalBounds().width / 2;
	float textOrigoY = mName.getGlobalBounds().height / 2;
	mName.setColor(sf::Color::Black);
	mName.setOrigin(textOrigoX, textOrigoY);
	mName.move(0, spriteOrigoY*1.5f);
	move(spriteOrigoX*2.5f, -spriteOrigoY);
}

CharacterPortrait::~CharacterPortrait(){

}

void CharacterPortrait::tick(const sf::Time& deltaTime){

}

void CharacterPortrait::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mSprite, states);
	target.draw(mName, states);
}

CharacterPortrait::RenderLayer CharacterPortrait::getRenderLayer() const{
	return FOREGROUND;
}

bool CharacterPortrait::isActive() const{
	return mIsActive;
}

bool CharacterPortrait::isAlive() const{
	return mIsAlive;
}

void CharacterPortrait::setActive(const bool& active){
	mIsActive = active;
}

void CharacterPortrait::kill(){
	mIsAlive = false;
}