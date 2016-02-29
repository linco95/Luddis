#ifndef _INCLUDED_TUTORIALTEXT_
#define _INCLUDED_TUTORIALTEXT_

#include "Entity.h"
#include "TextBox.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <string>

class TutorialText: public Entity{
public:
	TutorialText(sf::RenderWindow* window, sf::Vector2f position, float activationPoint, std::string text, sf::Transformable* aTarget);
	~TutorialText();

	// Update the entity
	void tick(const sf::Time& deltaTime) override;
	// Draw the entity
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	// Check if an entity is alive
	bool isAlive() const override;
	// Determines in what order entities are drawn
	RenderLayer getRenderLayer() const override;
	// Check if an entity is active
	bool isActive() const override;
	// Set an entities "active" state
	void setActive(const bool& active) override;

	void stun(const sf::Time& deltatime) override;
private:
	void updateText(const sf::Time& deltaTime);

	sf::RenderWindow* mWindow;
	sf::Transformable* mTarget;
	TextBox mText;
	float mActivate;
	bool mIsAlive;
	bool mIsActive;
};

#endif // !_INCLUDED_TUTORIALTEXT_
