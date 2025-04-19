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

void displayMainMenu() {
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "\t\tTask Management System" << endl;
	cout << "1. Register New User" << endl;
	cout << "2. Login" << endl;
	cout << "3. Exit" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "Enter your choice: " << endl;
}

void displayUserMenu() {
	cout << "\n===== USER MENU =====\n";
	cout << "1. Add New Task\n";
	cout << "2. View All Tasks\n";
	cout << "3. View Tasks by Priority\n";
	cout << "4. View Tasks by Date Range\n";
	cout << "5. Update Task\n";
	cout << "6. Delete Task\n";
	cout << "7. Logout\n";
	cout << "======================\n";
	cout << "Enter your choice: ";
}

void registerUser(UserManager& userManager) {
	string username, password;
	cout << "\n=== REGISTER NEW USER ===\n";
	cout << "Enter username: ";
	getline(cin, username);
	cout << "Enter password: ";
	password = getHiddenInput();

	User newUser(username, password);
	userManager.addUser(newUser);
}

User* login(UserManager& userManager) {
	string username, password;
	cout << "\n=== LOGIN ===\n";
	cout << "Enter username: ";
	getline(cin, username);
	cout << "Enter password: ";
	password = getHiddenInput();

	return userManager.authenticateUser(username, password);
}

void addTask(User* user) {
	cout << "\n=== ADD NEW TASK ===\n";
	Task newTask = Task::createUserInput();
	user->addTask(newTask);
	cout << "Task added successfully!\n";
}

void viewAllTasks(User* user) {
	cout << "\n=== ALL TASKS ===\n";
	user->displayTasks();
}

void viewTasksByPriority(User* user) {
	cout << "\n=== FILTER TASKS BY PRIORITY ===\n";
	cout << "Enter priority (0:LOW, 1:MEDIUM, 2:HIGH, 3:CRITICAL, 4:URGENT): ";
	int priority;
	cin >> priority;
	cin.ignore();

	if (priority >= 0 && priority <= 4) {
		user->filterByPriority(static_cast<Priority>(priority));
	}
	else {
		cout << "Invalid priority level!\n";
	}
}

void viewTasksByDateRange(User* user) {
	cout << "\n=== FILTER TASKS BY DATE RANGE ===\n";
	DateTime start, end;
	bool validStart = false, validEnd = false;

	do {
		cout << "Enter start date (YYYY MM DD HH MM SS): ";
		cin >> start.year >> start.month >> start.day >> start.hour >> start.minute >> start.second;
		if (!start.isValid()) {
			cout << "Invalid start date! Please try again.\n";
		}
		else {
			validStart = true;
		}
	} while (!validStart);

	do {
		cout << "Enter end date (YYYY MM DD HH MM SS): ";
		cin >> end.year >> end.month >> end.day >> end.hour >> end.minute >> end.second;
		if (!end.isValid()) {
			cout << "Invalid end date! Please try again.\n";
		}
		else if (end < start) {
			cout << "End date cannot be earlier than start date! Please try again.\n";
		}
		else {
			validEnd = true;
		}
	} while (!validEnd);
	cin.ignore();

	user->filterTasksByDate(start, end);
}

void updateTask(User* user) {
	cout << "\n=== UPDATE TASK ===\n";
	viewAllTasks(user);

	const vector<Task>& tasks = user->getTasks();
	if (tasks.empty()) {
		cout << "No tasks available to update.\n";
		return;
	}

	cout << "Enter task number to update: ";
	size_t index;
	cin >> index;
	cin.ignore();

	if (index >= 0 && index < tasks.size()) {
		user->updateTask(index);
		cout << "Task updated successfully!\n";
	}
	else {
		cout << "Invalid task number!\n";
	}
}

void deleteTask(User* user) {
	cout << "\n=== DELETE TASK ===\n";
	viewAllTasks(user);

	const vector<Task>& tasks = user->getTasks();
	if (tasks.empty()) {
		cout << "No tasks available to delete.\n";
		return;
	}

	cout << "Enter task number to delete: ";
	size_t index;
	cin >> index;
	cin.ignore();

	if (index >= 0 && index < tasks.size()) {
		user->removeTask(index);
		cout << "Task deleted successfully!\n";
	}
	else {
		cout << "Invalid task number!\n";
	}
}

void userMenu(UserManager& userManager, User* user) {
	int choice;
	do {
		displayUserMenu();
		cin >> choice;
		cin.ignore();

		switch (choice) {
		case 1: addTask(user); break;
		case 2: viewAllTasks(user); break;
		case 3: viewTasksByPriority(user); break;
		case 4: viewTasksByDateRange(user); break;
		case 5: updateTask(user); break;
		case 6: deleteTask(user); break;
		case 7: cout << "Logging out...\n"; break;
		default: cout << "Invalid choice! Please try again.\n";
		}
	} while (choice != 7);
}






int main()
{

	cout << boolalpha << endl;

	UserManager userManager("users.json");
	int choice;

	do
	{
		displayMainMenu();
		cin >> choice;
		cin.ignore();

		switch (choice)
		{
		case 1:
			registerUser(userManager);
			break;
		case 2: {
			User* user = login(userManager);
			if (user != nullptr) {
				userMenu(userManager, user);
			}
			break;
		}
		case 3:
			cout << "Exiting program...\n";
			break;
		default:
			cout << "Invalid choice! Please try again.\n";
		}
	} while (choice != 3);










	return 0;
}

