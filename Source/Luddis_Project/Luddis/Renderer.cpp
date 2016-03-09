#include "Renderer.h"
#include <map>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Drawable.hpp>


Renderer::Renderer() { }

void Renderer::addDrawableToQueue(const sf::Drawable * a_Drawable, const RenderLayer& a_Layer) {
	m_Drawables[a_Layer].push_back(a_Drawable);
}

void Renderer::removeDrawableFromQueue(const sf::Drawable * a_Drawable) {
	for (auto itr : m_Drawables) {
		for (auto i = itr.second.begin(); i != itr.second.end(); i++) {
			if (*i == a_Drawable)
				itr.second.erase(i);
		}
	}
}


Renderer& Renderer::getInstance() {
	static Renderer rd;
	return rd;
}

void Renderer::render(sf::RenderWindow & a_Window) {
	a_Window.clear(sf::Color::Black);
	for (auto itr : m_Drawables) {
		for (auto d : itr.second)
			if (d != 0)
				a_Window.draw(*d);
	}
	a_Window.display();
	m_Drawables.clear();
}
