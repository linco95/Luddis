#include "Debug.h"
#include <iostream>
#include <SFML\Graphics\Shape.hpp>
namespace WINDOWS{
#include <windows.h>
}
using namespace std;
struct ErrorInfo{
	ErrorInfo(char* aString, const WINDOWS::WORD& aColor) : 
	asString(aString),
	color(aColor) {}

	char* asString;
	WINDOWS::WORD color;
};

const ErrorInfo getErrorLevelStr(const Debug::ERRORLEVEL& aErrLvl){
	switch (aErrLvl){
	case Debug::DEFAULT:
		return ErrorInfo(" DEFAULT ", 0x0F);
	case Debug::INFO:
		return ErrorInfo(" INFO ", 0x0F);
	case Debug::WARNING:
		return ErrorInfo(" WARNING ", 0x0E);
	case Debug::NONFATAL:
		return ErrorInfo(" ERROR ", 0x04);
	case Debug::FATAL:
		return ErrorInfo(" FATAL ", 0x0C);
	default:
		return ErrorInfo(" NOT DEFINED ", 0x0F);
	}
}
namespace WINDOWS{
	void printInColor(const WORD& aColor, const char* aText) {

		HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);
		HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

		// Remember how things were when we started
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(hstdout, &csbi);
		SetConsoleTextAttribute(hstdout, aColor);
		std::cout << aText;
		FlushConsoleInputBuffer(hstdin);

		// Keep users happy
		SetConsoleTextAttribute(hstdout, csbi.wAttributes);
	}
}
void Debug::log(const string& aMsg, const ERRORLEVEL& aLvl){
	
	cout << "[";
	WINDOWS::printInColor(getErrorLevelStr(aLvl).color, getErrorLevelStr(aLvl).asString);
	cout <<  "]: " << aMsg << endl;
}
