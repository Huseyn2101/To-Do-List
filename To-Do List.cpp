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



using json = nlohmann::json;
using namespace std;



#include "class.h"

int main()
{

	cout << boolalpha << endl;

	UserManager userManager("users.json");

	string username, password;
	cout << "Enter username: ";
	getline(cin, username);
	cout << "Enter password: ";
	password = getHiddenİnput();

	try {
		UserManager userManager("users.json");

		// Authentication logic
		User* user = userManager.authenticateUser(username, password);
		if (!user) {
			cerr << "Authentication failed!\n";
			return 1;  // Proper error code
		}

		// Rest of your program...
		return 0;  // Success
	}
	catch (const exception& e) {
		cerr << "Fatal Error: " << e.what() << "\n";
		return 3;  // Match your observed error code
	}








	return 0;
}

