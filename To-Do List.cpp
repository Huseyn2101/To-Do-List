#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <chrono>
#include <iomanip>
#include <thread>
#include <functional>
#include <algorithm>
#include <conio.h>



using json = nlohmann::json;
using namespace std;


#include "class.h"


int main()
{
	cout << boolalpha << endl;

	string databaseFile = "Source Files/database.json";

	UserManager userManager(databaseFile);

	while (true)
	{
		displayMenu(userManager);
	}



	return 0;
}

