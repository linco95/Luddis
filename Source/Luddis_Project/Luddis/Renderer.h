#ifndef INCLUDED_RENDERER_H
#define INCLUDED_RENDERER_H

#include <map>
#include <vector>

namespace sf {
	class RenderWindow;
	class Drawable;
}

class Renderer {
public:
	enum RenderLayer {
		BACKGROUND,
		OBSTACLES,
		ITEM,
		PLAYER,
		FOREGROUND,
		GUI,
		MOUSE
	};

	//void addDrawableToQueue(const sf::Drawable* a_Drawable, const RenderLayer& a_Layer, const sf::View)

	// Add an object to be drawn
	void addDrawableToQueue(const sf::Drawable* a_Drawable, const RenderLayer& a_Layer);

	// Remove an object from the drawablequeue
	void removeDrawableFromQueue(const sf::Drawable* a_Drawable);
	
	// Draws the objects that has been assigned to drawing, and clear the vector
	void render(sf::RenderWindow& a_Window);


	static Renderer& getInstance();
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

private:
	typedef std::vector<const sf::Drawable*> DrawableVector;
	typedef std::map<RenderLayer, DrawableVector> DrawableMap;
	//typedef std::map<sf::View, DrawableMap> ViewMap;
	Renderer();

	DrawableMap m_Drawables;
};


#endif // INCLUDED_RENDERER_H