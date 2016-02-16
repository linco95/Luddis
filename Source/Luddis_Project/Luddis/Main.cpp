#include "GameManager.h"
//// THESE ARE JUST TEMPORARY
//// They are here for testing, will be removed later...
//#include <rapidjson\document.h>
//#include <iostream>
//#include <cassert>
//#include <fstream>
//#include <string>
//
//using namespace rapidjson;
//
//void testingJSON(){
//	const char* json = "{ \"hello\": \"world\", \"t\" : true, \"f\" : false, \"n\" : null, \"i\" : 123, \"pi\" : 3.1416, \"a\" : [1, 2, 3, 4] }";
//	std::cout << "Original string: " << json << std::endl;
//	Document doc;
//	doc.Parse(json);
//	assert(doc.IsObject());
//	assert(doc.HasMember("hello"));
//	assert(doc["hello"].IsString());
//	std::cout << "hello: " << doc["hello"].GetString() << std::endl;
//
//}
//
//void testingJSONFromFile(const char* filepath) {
	//// Testing loading file and parsing
	//std::ifstream input(filepath);
	//assert(input.is_open());
	//std::string inputText = "";
	//std::string temp = "";
	//while (std::getline(input, temp)){
	//	inputText += temp + "\n";
	//}
	//input.close();
	//std::cout << inputText << std::endl;
	//char* input = loadFile("filnamn");
//	Document doc;
//	doc.Parse(inputText.c_str());
//	assert(doc.IsObject());
//
//	// Testing getting values
//	assert(doc.HasMember("first name"));
//	assert(doc["first name"].IsString());
//	std::cout << "Name: " << doc["first name"].GetString() << " " << doc["last name"].GetString() << std::endl;
//
//	// Testing arrays
//	std::cout << "Coordinates: [";
//	assert(doc.HasMember("coordinates"));
//	assert(doc["coordinates"].IsArray());
//	const Value& a = doc["coordinates"];
//	for (SizeType i = 0; i < doc["coordinates"].Size(); i++){
//		std::cout << a[i].GetInt() << " ";
//	}
//	std::cout << "]" << std::endl;
//
//	// Testing objects in array
//	assert(doc.HasMember("Silverfish spawns"));
//	assert(doc["Silverfish spawns"].IsArray());
//	const Value& fishSpawn = doc["Silverfish spawns"];
//	assert(fishSpawn.Size() > 0);
//	//assert(temp.HasMember("x") && temp.HasMember("y") && temp.HasMember("angle"));
//	//assert(fishSpawn["x"].IsInt() && fishSpawn["y"].IsInt() && fishSpawn["angle"].IsDouble());
//	for (Value::ConstValueIterator itr = fishSpawn.Begin(); itr != fishSpawn.End(); itr++){
//		std::printf("Silverfish spawning at [%d, %d] with an angle of %.2f\n", (*itr)["x"].GetInt(), (*itr)["y"].GetInt(), (*itr)["angle"].GetDouble());
//	}
//
//}
// THESE ARE JUST TEMPORARY
// They are here for testing, will be removed later...

int main(){
	/*testingJSON();
	testingJSONFromFile("resources/configs/example/test.json");*/
	//ResourceManager::getInstance().loadJsonFile("resources/configs/example/test.json");
	GameManager::getInstance().run();

	return 0;
}