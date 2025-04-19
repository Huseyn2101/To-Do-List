#include <iostream>
#include <cstring>
#include <cassert>
#include <ctime>
#include <string>
#include <limits> 
#include <nlohmann/json.hpp> // JSON kitabxanasını əlavə etmək üçün
#include <vector>
#include <iomanip>
#include <conio.h>
#include <fstream> // Add this include to resolve incomplete type "std::ofstream" error
#include <chrono>   
#include <thread>   
#include <windows.h>
#include <algorithm>
#include <cstdlib>
#include <random>
#include <stack>


using json = nlohmann::json;
using namespace std;
using namespace std::this_thread;
using namespace std::chrono;


#include "class.h"








int main()
{

	cout << boolalpha << endl;

	//welcomeMessage();

	//cout << "Welcome to the Task Management System!" << endl;
	//showPercentageLoading(3);
	//cout << "Loading completed!" << endl;

	//showDotsLoading(3);
	//cout << "Loading completed!" << endl;


	//cout << "1. Klasik ilerleme çubuğu:\n";
	//classicProgressBar(2);

	//cout << "\n2. Renkli ilerleme çubuğu:\n";
	//coloredProgressBar(2);

	//cout << "\n9. Binary ilerleme çubuğu:\n";
	//binaryProgressBar(2);

	//

	UserManager userManager("users.json");

	displayMenu(userManager);
	


	return 0;
}

