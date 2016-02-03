#pragma once
#ifndef INCLUDED_UTILS
#define INCLUDED_UTILS
#include <fstream>
#include <cassert>
#include <string>

struct LuddisUtilFuncs{
	static std::string loadJsonFile(const std::string filepath){
		assert(filepath.substr(filepath.length() - 5, filepath.length()) == ".json");
		std::ifstream input(filepath);
		assert(input.is_open());
		std::string inputText = "";
		std::string temp = "";
		while (std::getline(input, temp)){
			inputText += temp + "\n";
		}
		return inputText;
	}
};

#endif