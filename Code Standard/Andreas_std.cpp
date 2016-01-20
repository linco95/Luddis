// Textruta
/*
* Multi-line comment
* for lots of
* text by the author blabla
*/

// Bortkommenterad kod
/*
	cout << "Hello World!\n";
*/

#ifndef INCLUDED_FILENAME
#define INCLUDED_FILENAME

// Stor bokstav på klassnamn
class ExampleClass {
public:

	ExampleClass();
	~ExampleClass();

	// Liten bokstav på funktionsnamn
	void exampleFunction(const int& value);
	

private:
	// Liten bokstav på variabler
	float mLength;
};

#endif

#include <SFML>
#include "ExampleClass.h"

// Caps på konstanter och enums
// Statiska variabler i .cpp för att undvika "magiska värden" i koden.
static const float TWEAKABLEFLOAT = 0.0f;

// Försök att skriva enkla kommentarer för funktionens funktionalitet (om det inte beskrivs tydligt i funktionsnamnet)
// Funktion som tar ett värde och multiplicerar det med två
void ExampleClass::exampleFunction(const int& value){
	value * 2;
}