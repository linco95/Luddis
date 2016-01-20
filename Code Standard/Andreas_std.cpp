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

// Stor bokstav p� klassnamn
class ExampleClass {
public:

	ExampleClass();
	~ExampleClass();

	// Liten bokstav p� funktionsnamn
	void exampleFunction(const int& value);
	

private:
	// Liten bokstav p� variabler
	float mLength;
};

#endif

#include <SFML>
#include "ExampleClass.h"

// Caps p� konstanter och enums
// Statiska variabler i .cpp f�r att undvika "magiska v�rden" i koden.
static const float TWEAKABLEFLOAT = 0.0f;

// F�rs�k att skriva enkla kommentarer f�r funktionens funktionalitet (om det inte beskrivs tydligt i funktionsnamnet)
// Funktion som tar ett v�rde och multiplicerar det med tv�
void ExampleClass::exampleFunction(const int& value){
	value * 2;
}