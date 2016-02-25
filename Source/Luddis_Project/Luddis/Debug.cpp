#include "Debug.h"
#include <iostream>
#include <SFML\Graphics\Shape.hpp>
#include <string>
#include <fstream>
#include <ctime>
#include <sstream>

static const char* SAVEPATH = "Logs/";


#ifdef _WIN32
namespace WINDOWS{
#include <windows.h>
}
#endif
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
#ifdef _WIN32
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
#endif

void Debug::finalize() {
	/*mLog.
	if (mLog.message.empty() || mLogname.empty()) return;
	ofstream saveFile;
	string savepath = SAVEPATH;

	saveFile.open(savepath);*/
	
}

void Debug::initialize() {
	//
	//if (mLog.message.empty()) return;
	//time_t currentTime;
	//struct tm *localTime;

	//time(&currentTime);                   // Get the current time
	//localTime = localtime(&currentTime);  // Convert the current time to the local time

	//int day = localTime->tm_mday;
	//int month = localTime->tm_mon + 1;
	//int year = localTime->tm_year + 1900;
	//int hour = localTime->tm_hour;
	//int min = localTime->tm_min;
	//int sec = localTime->tm_sec;



	//mLog = "Luddis | Started " + day;
}

void Debug::log(const string& aMsg, const ERRORLEVEL& aLvl){
#if defined _DEBUG || LOG_IN_RELEASE
	string message = "[";
	message += getErrorLevelStr(aLvl).asString;
	message =+ "]: " + aMsg;
	mLog += message;

#ifdef _WIN32
	cout << "[";
	WINDOWS::printInColor(getErrorLevelStr(aLvl).color, getErrorLevelStr(aLvl).asString);
	cout <<  "]: " << aMsg << endl;
#else
	cout << message << endl;
#endif

#endif
}
