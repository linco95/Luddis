#include "GameManager.h"

#include <rapidjson\document.h>
#include <iostream>

using namespace rapidjson;

void testingJSON(){
	const char* json = "{ \"hello\": \"world\", \"t\" : true, \"f\" : false, \"n\" : null, \"i\" : 123, \"pi\" : 3.1416, \"a\" : [1, 2, 3, 4] }";
	std::cout << "Original string: " << json << std::endl;

}

int main(){
	testingJSON();
	GameManager::getInstance().run();

	return 0;
}