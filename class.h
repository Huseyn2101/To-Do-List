#pragma once

#include "constants.h"
#include "datetime.h"
#include "task.h"
#include "user.h"
#include "userManagerr.h"
#include "jsonHandler.h"




struct LoginAttempt {
	int failedAttempts = 0;
	std::chrono::system_clock::time_point blockUntil = std::chrono::system_clock::time_point{};

	bool isBlocked() const {
		auto now = std::chrono::system_clock::now();
		return now < blockUntil;
	}

	void registerFailure() {
		failedAttempts++;
		if (failedAttempts >= 3) {
			blockUntil = std::chrono::system_clock::now() + std::chrono::minutes(5);
			failedAttempts = 0;
		}
	}

	void resetAttempts() {
		failedAttempts = 0;
		blockUntil = std::chrono::system_clock::time_point{};
	}
};


enum Priority {
	LOW,
	MEDIUM,
	HIGH,
	CRITICAL,
	URGENT
};

string toLowerCase(const string& text) {
	string result = text;
	for (char& c : result) {
		c = tolower(c);
	}
	return result;
}

string getHiddenInput() {
	string password;
	char ch;
	while (true) {
		ch = _getch();
		if (ch == ENTER_KEY) {
			break;
		}
		else if (ch == 8) { // Backspace
			if (!password.empty()) {
				password.pop_back();
				cout << "\b \b";
			}
		}
		else {
			password.push_back(ch);
			cout << '*';
		}
	}
	cout << endl;
	return password;
}

Priority stringToPriority(string priority) {
	string lower = toLowerCase(priority);
	if (lower == "low") return Priority::LOW;
	if (lower == "medium") return Priority::MEDIUM;
	if (lower == "high") return Priority::HIGH;
	if (lower == "critical") return Priority::CRITICAL;
	if (lower == "urgent") return Priority::URGENT;
	return Priority::LOW;
}


class UserManager {
private:
	vector<User> _users;
	JsonHandler _jsonHandler;

public:
	UserManager(const string& datafile) : _jsonHandler(datafile) {
		_users = _jsonHandler.loadUsersFromFile();
	}

	~UserManager() {
		saveUsersToFile();
	}

	void addUser(const User& user) {
		for (const auto& u : _users) {
			if (u.getUsername() == user.getUsername()) {
				cout << "User already exists!" << endl;
				return;
			}
		}
		_users.push_back(user);
		saveUsersToFile();
		cout << "User added successfully!" << endl;
	}

	void loadUsersFromFile() {
		_users = _jsonHandler.loadUsersFromFile();
	}

	void saveUsersToFile() {
		_jsonHandler.saveUsersToFile(_users);
	}

	const vector<User>& getUsers() const {
		return _users;
	}

	User* findUser(const string& username, const string& password) {
		for (auto& user : _users) {
			if (user.getUsername() == username && user.verifyPassword(password)) {
				return &user;
			}
		}
		return nullptr;
	}
};


void hideCursor() {
	cout << "\033[?25l"; // Hide cursor
}

void showCursor() {
	cout << "\033[?25h"; // Show cursor
}

void clearLine() {
	cout << "\r" << string(100, ' ') << "\r"; // Clear the current line
}

void classicProgressBar(int duration = 3) {
	hideCursor();
	auto start = chrono::high_resolution_clock::now();
	auto end = start + chrono::seconds(duration);

	int barWidth = 50;
	while (chrono::high_resolution_clock::now() < end) {
		float progress = 1.0f - chrono::duration_cast<chrono::milliseconds>(end - chrono::high_resolution_clock::now()).count() / (duration * 1000.0f);
		int pos = barWidth * progress;

		cout << "\r[";
		for (int i = 0; i < barWidth; ++i) {
			if (i < pos) cout << "=";
			else if (i == pos) cout << ">";
			else cout << " ";
		}
		cout << "] " << int(progress * 100.0) << "%";
		cout.flush();
		this_thread::sleep_for(chrono::milliseconds(50));
	}
	clearLine();
	showCursor();
}

void welcomeMessage() {
	// ASCII Art
	cout << R"(
                                  _____         _          _____           _ 
                                 |_   _|       | |        |_   _|         | |
                                   | | __ _ ___| | __       | | ___   ___ | |
                                   | |/ _` / __| |/ /       | |/ _ \ / _ \| |
                                   | | (_| \__ \   <        | | (_) | (_) | |
                                   \_/\__,_|___/_|\_\       \_/\___/ \___/|_|
                                                                  
)" << endl;

	// Welcome message
	cout << "\033[1;36m" << "Welcome to TaskMaster Pro - Your Ultimate Task Management System!" << "\033[0m" << endl;
	cout << "\033[1;33m" << "Version 2.4.1 | Secure Login Enabled | Multi-User Support" << "\033[0m" << endl << endl;

	// Features Header
	cout << "\033[1;32m" << "Key Features:" << "\033[0m" << endl;

	// Features List
	cout << "  " << static_cast<char>(250) << " " << "\033[1;37m" << "Secure user authentication with password protection." << "\033[0m" << endl;
	cout << "  " << static_cast<char>(250) << " " << "\033[1;37m" << "Priority-based task organization for better productivity." << "\033[0m" << endl;
	cout << "  " << static_cast<char>(250) << " " << "\033[1;37m" << "Flexible DateTime scheduling with built-in validation." << "\033[0m" << endl;
	cout << "  " << static_cast<char>(250) << " " << "\033[1;37m" << "JSON data persistence to ensure your data is saved securely." << "\033[0m" << endl;
	cout << "  " << static_cast<char>(250) << " " << "\033[1;37m" << "Beautiful and intuitive console interface for easy navigation." << "\033[0m" << endl << endl;

	// User login/registration prompt
	cout << "\033[3m" << "Please login to access your tasks or register as a new user." << "\033[0m" << endl;
	cout << "----------------------------------------------------" << endl;
	cout << "\033[1;34m" << "Press Enter to continue..." << "\033[0m" << endl;
}


User* login(UserManager& userManager) {
	string username, password;
	cout << "\n=== LOGIN ===\n";
	cout << "Enter username: ";
	getline(cin, username);
	cout << "Enter password: ";
	password = getHiddenInput();

	User* user = userManager.findUser(username, password);
	if (user) {
		cout << "Login successful! Welcome, " << user->getUsername() << "!" << endl;
		return user;
	}
	else {
		cout << "Invalid username or password. Please try again." << endl;
		return nullptr;
	}
}

void prioritySetter(Task& task) {
	int choice = 0;
	const char* priorityNames[] = { "LOW", "MEDIUM", "HIGH", "CRITICAL", "URGENT" };


	while (true)
	{
		system("cls||clear");
		cout << "Use arrow keys to navigate and Enter to select." << endl;
		cout << "Priority: ";

		switch (choice) {
		case 0: cout << "\033[0;32m"; break;
		case 1: cout << "\033[0;33m"; break;
		case 2: cout << "\033[0;31m"; break;
		case 3: cout << "\033[1;31m"; break;
		case 4: cout << "\033[0;35m"; break;
		}

		cout << priorityNames[choice] << "\033[0m" << endl;

		int c = _getch();

		switch (c) {
		case KEY_LEFT:
			if (choice > 0) choice--;
			else choice = 4;
			break;
		case KEY_RIGHT:
			if (choice < 4) choice++;
			else choice = 0;
			break;
		case ENTER_KEY:
			if (choice >= 0 && choice < 5) {
				task.setPriority(static_cast<Priority>(choice));
				return;
			}
			break;
		default:
			break;
		}
	}
}


void registerUser(UserManager& userManager) {
	string username, password;
	cout << "\n=== REGISTER ===\n";
	cout << "Enter username: ";
	getline(cin, username);
	cout << "Enter password: ";
	password = getHiddenInput();
	User newUser(username, password);
	userManager.addUser(newUser);
	cout << "Registration successful! You can now log in." << endl;
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

void showDotsLoading(int durationSeconds = 3) {
	using namespace std::chrono_literals;

	auto start = std::chrono::steady_clock::now();
	auto end = start + std::chrono::seconds(durationSeconds);

	std::cout << "\t\t\tLoading";

	while (std::chrono::steady_clock::now() < end) {
		for (int i = 0; i < 3; i++) {
			if (std::chrono::steady_clock::now() >= end) break;
			std::cout << "." << std::flush;
			std::this_thread::sleep_for(300ms);
		}
		std::cout << "\b\b\b   \b\b\b" << std::flush;
	}
	cout << "\b\b\b\b\b";
	std::cout << "Loading Done!" << std::endl;
}


void showDotsExiting(int durationSeconds = 3) {
	using namespace std::chrono_literals;

	auto start = std::chrono::steady_clock::now();
	auto end = start + std::chrono::seconds(durationSeconds);

	std::cout << "App is closing";

	while (std::chrono::steady_clock::now() < end) {
		for (int i = 0; i < 3; i++) {
			if (std::chrono::steady_clock::now() >= end) break;
			std::cout << "." << std::flush;
			std::this_thread::sleep_for(300ms);
		}
		std::cout << "\b\b\b   \b\b\b" << std::flush;
	}
	cout << "\b\b\b\b\b";
	std::cout << "Closing Done!" << std::endl;
}



void displayUpdating(User* user) {
	int choice = 0;

	while (true) {
		system("cls||clear");
		cout << "Use arrow keys to navigate and Enter to select.\n\n";

		auto& tasks = user->getTasks();

		if (tasks.empty()) {
			cout << "No tasks available.\n";
			break;
		}

		for (size_t i = 0; i < tasks.size(); ++i) {
			if (choice == static_cast<int>(i)) {
				cout << COLOR_MAGENTA << i + 1 << ". " << tasks[i].getName() << COLOR_RESET << endl;
			}
			else {
				cout << i + 1 << ". " << tasks[i].getName() << endl;
			}
		}

		int c = _getch();
		switch (c) {
		case KEY_UP:
			choice = (choice > 0) ? choice - 1 : static_cast<int>(tasks.size()) - 1;
			break;
		case KEY_DOWN:
			choice = (choice < static_cast<int>(tasks.size()) - 1) ? choice + 1 : 0;
			break;
		case ENTER_KEY: {
			Task& selectedTask = tasks[choice];
			system("cls||clear");
			cout << "Selected task: " << selectedTask.getName() << "\n\n";

			cin.ignore(INT_MAX, '\n');  // ignore buffer before getline

			string newName;
			cout << "Enter new task name (current: " << selectedTask.getName() << "): ";
			getline(cin, newName);
			if (!newName.empty()) selectedTask.setName(newName);

			string newDescription;
			cout << "Enter new task description (current: " << selectedTask.getDescription() << "): ";
			getline(cin, newDescription);
			if (!newDescription.empty()) selectedTask.setDescription(newDescription);

			int newPriority;
			cout << "Enter new task priority (0-LOW, 1-MEDIUM, 2-HIGH, 3-CRITICAL, 4-URGENT): ";
			while (!(cin >> newPriority) || newPriority < 0 || newPriority > 4) {
				cout << "Invalid priority! Please enter a value between 0 and 4: ";
				cin.clear();
				cin.ignore(INT_MAX, '\n');
			}
			selectedTask.setPriority(static_cast<Priority>(newPriority));

			DateTime newStartDate;
			cout << "Enter new task start date (YYYY MM DD HH MM SS): ";
			while (!(cin >> newStartDate.year >> newStartDate.month >> newStartDate.day
				>> newStartDate.hour >> newStartDate.minute >> newStartDate.second) ||
				!newStartDate.isValid()) {
				cout << "Invalid start date! Please enter in format YYYY MM DD HH MM SS.\n";
				cin.clear();
				cin.ignore(INT_MAX, '\n');
			}
			selectedTask.setStartTime(newStartDate);

			DateTime newEndDate;
			cout << "Enter new task end date (YYYY MM DD HH MM SS): ";
			while (!(cin >> newEndDate.year >> newEndDate.month >> newEndDate.day
				>> newEndDate.hour >> newEndDate.minute >> newEndDate.second) ||
				!newEndDate.isValid() || newEndDate < newStartDate) {
				cout << "Invalid end date! It must be valid and later than the start date.\n";
				cin.clear();
				cin.ignore(INT_MAX, '\n');
			}
			selectedTask.setEndTime(newEndDate);

			int newStatus;
			cout << "Enter new task status (0 - Not Completed, 1 - Completed): ";
			while (!(cin >> newStatus) || (newStatus != 0 && newStatus != 1)) {
				cout << "Invalid input! Enter 0 or 1: ";
				cin.clear();
				cin.ignore(INT_MAX, '\n');
			}
			selectedTask.setStatus(newStatus);
			cin.ignore(INT_MAX, '\n');  // clean buffer after last input

			cout << "\nTask updated successfully!" << endl;
			cout << "Press any key to continue...";
			_getch();
			break;
		}
		case ESC:
			return;
		default:
			break;
		}
	}
}


void displayRemoving(User* user) {
	int choice = 0;

	while (true) {
		system("cls||clear");
		cout << "Use arrow keys to navigate and Enter to remove a task.\n\n";

		auto& tasks = user->getTasks();

		if (tasks.empty()) {
			cout << "No tasks available.\n";
			cout << "Press any key to return to the previous menu...";
			_getch();
			break;
		}

		for (size_t i = 0; i < tasks.size(); ++i) {
			if (choice == static_cast<int>(i)) {
				cout << COLOR_MAGENTA << i + 1 << ". " << tasks[i].getName() << COLOR_RESET << endl;
			}
			else {
				cout << i + 1 << ". " << tasks[i].getName() << endl;
			}
		}

		int c = _getch();
		switch (c) {
		case KEY_UP:
			choice = (choice > 0) ? choice - 1 : static_cast<int>(tasks.size()) - 1;
			break;
		case KEY_DOWN:
			choice = (choice < static_cast<int>(tasks.size()) - 1) ? choice + 1 : 0;
			break;
		case ENTER_KEY: {
			string removedName = tasks[choice].getName();
			user->removeTask(choice);
			cout << "\nTask \"" << removedName << "\" removed successfully!" << endl;
			cout << "Press any key to continue...";
			_getch();
			break;
		}
		case ESC:
			return;
		default:
			break;
		}
	}
}


void displayTasks(User* user) {
	int choice = 0;

	while (true) {
		system("cls||clear");

		auto& tasks = user->getTasks();

		cout << "Use arrow keys to navigate and Enter to select.\n\n";

		if (tasks.empty()) {
			cout << "No tasks available.\n";
			cout << "Press any key to return to the previous menu...";
			_getch();
			break;
		}

		for (size_t i = 0; i < tasks.size(); ++i) {
			if (static_cast<int>(i) == choice) {
				cout << COLOR_MAGENTA << i + 1 << ". " << tasks[i].getName() << COLOR_RESET << endl;
			}
			else {
				cout << i + 1 << ". " << tasks[i].getName() << endl;
			}
		}

		int c = _getch();
		switch (c) {
		case KEY_UP:
			choice = (choice > 0) ? choice - 1 : static_cast<int>(tasks.size()) - 1;
			break;

		case KEY_DOWN:
			choice = (choice < static_cast<int>(tasks.size()) - 1) ? choice + 1 : 0;
			break;

		case ENTER_KEY: {
			Task& selectedTask = tasks[choice];
			system("cls||clear");
			cout << "Selected task: " << selectedTask.getName() << "\n\n";
			selectedTask.display();
			cout << "\nPress any key to continue...";
			_getch();
			break;
		}

		case ESC:
			return;

		default:
			break;
		}
	}
}


static void addTaskToUser(User* user) {
	string name, description;
	DateTime start, end;

	cout << "Enter the task's name: ";
	getline(cin, name);

	cout << "Enter the task's description: ";
	getline(cin, description);

	while (true) {
		cout << "Enter start time (YYYY MM DD HH MM SS): ";
		if (!(cin >> start.year >> start.month >> start.day >> start.hour >> start.minute >> start.second) || (!start.isValid())) {
			cout << "Invalid input format! Please enter numbers only." << endl;
			cin.clear(); // səhv bayrağını sıfırla
			cin.ignore(); // bu sətirdəki qalan hər şeyi at
			continue;
		}
		if (start.isValid()) {
			break;
		}
		if (start < DateTime()) {
			cout << "Start time cannot be in the past! Please try again." << endl;
			continue;
		}
		else if (start > DateTime()) {
			break;
		}
		else {
			cout << "Invalid start date! Please try again." << endl;
		}
	}

	// END TIME
	while (true) {
		cout << "Enter end time (YYYY MM DD HH MM SS): ";
		if (!(cin >> end.year >> end.month >> end.day >> end.hour >> end.minute >> end.second) || (!start.isValid())) {
			cout << "Invalid input format! Please enter numbers only." << endl;
			cin.clear();
			cin.ignore();
			continue;
		}
		if (!end.isValid()) {
			cout << "Invalid end date! Please try again." << endl;
		}
		if (end < start) {
			cout << "End time cannot be earlier than start time! Please try again." << endl;
		}
		else if (end > start) {
			break;
		}
		else {
			break;
		}
		cin.ignore(); // bu sətirdəki qalan hər şeyi at
	}
	cin.ignore(); // Yeni sətir simvolunu təmizlə

	Task newTask(name, description, Priority::LOW, start, end); // Default prioritet olaraq LOW təyin edilir

	prioritySetter(newTask);

	user->addTask(newTask);
	cout << "Task added successfully!" << endl;
}



void displayFiltering(User* user) {
	int filterChoice = 0, priorityChoice = 0;
	DateTime startDate, endDate;

	while (true) {
		system("cls||clear");
		cout << "Use arrow keys to navigate, Enter to apply filter, and ESC to exit." << endl;

		cout << "Filter by:" << endl;
		cout << (filterChoice == 0 ? COLOR_MAGENTA : COLOR_RESET) << "1. Priority" << COLOR_RESET << endl;
		cout << (filterChoice == 1 ? COLOR_MAGENTA : COLOR_RESET) << "2. Date Range" << COLOR_RESET << endl;
		cout << (filterChoice == 2 ? COLOR_MAGENTA : COLOR_RESET) << "3. Back" << COLOR_RESET << endl;

		int c = _getch();
		switch (c) {
		case KEY_UP:
			filterChoice = (filterChoice > 0) ? filterChoice - 1 : 2;
			break;
		case KEY_DOWN:
			filterChoice = (filterChoice < 2) ? filterChoice + 1 : 0;
			break;
		case ENTER_KEY:
			switch (filterChoice) {
			case 0: { // Prioritetə görə süzmə
				while (true) {
					system("cls||clear");
					cout << "Select Priority:" << endl;
					for (int i = 0; i <= 4; ++i) {
						if (priorityChoice == i) {
							cout << COLOR_MAGENTA << "> " << i << " - " << Task::priorityToString(static_cast<Priority>(i)) << COLOR_RESET << endl;
						}
						else {
							cout << "  " << i << " - " << Task::priorityToString(static_cast<Priority>(i)) << endl;
						}
					}

					int pc = _getch();
					switch (pc) {
					case KEY_UP:
						priorityChoice = (priorityChoice > 0) ? priorityChoice - 1 : 4;
						break;
					case KEY_DOWN:
						priorityChoice = (priorityChoice < 4) ? priorityChoice + 1 : 0;
						break;
					case ENTER_KEY:
						user->filterByPriority(static_cast<Priority>(priorityChoice));
						cout << "Filtered tasks by priority." << endl;
						break;
					case ESC:
						return;
					}
				}
				break;
			}
			case 1: { // Tarix aralığına görə süzmə
				while (true) {
					system("cls||clear");
					cout << "Enter Start Date (YYYY MM DD HH MM SS): ";
					cin >> startDate.year >> startDate.month >> startDate.day
						>> startDate.hour >> startDate.minute >> startDate.second;

					if (!startDate.isValid()) {
						cout << "Invalid start date! Please try again." << endl;
						cin.clear();
						cin.ignore(INT_MAX, '\n');
						continue;
					}

					cout << "Enter End Date (YYYY MM DD HH MM SS): ";
					cin >> endDate.year >> endDate.month >> endDate.day
						>> endDate.hour >> endDate.minute >> endDate.second;

					if (!endDate.isValid() || endDate < startDate) {
						cout << "Invalid end date! End date must be valid and later than the start date." << endl;
						cin.clear();
						cin.ignore(INT_MAX, '\n');
						continue;
					}

					user->filterTasksByDate(startDate, endDate);
					cout << "Filtered tasks by date range." << endl;
					cout << "Press any key to return to filter selection..." << endl;
					(void)_getch();
					break;
				}
				break;
			}
			case 2:
				return;
			}
			break;
		case ESC:
			cout << "Exiting filter display..." << endl;
			return;
		}
	}
}






void displayMenu(UserManager& userManager) {
	int choice = 0;

	while (true) {
		system("cls||clear");
		welcomeMessage();
		cout << "Use arrow keys to navigate and Enter to select." << endl;

		cout << (choice == 0 ? COLOR_MAGENTA : COLOR_RESET) << "\v1. Login" << COLOR_RESET << endl;
		cout << (choice == 1 ? COLOR_MAGENTA : COLOR_RESET) << "2. Register" << COLOR_RESET << endl;
		cout << (choice == 2 ? COLOR_MAGENTA : COLOR_RESET) << "3. Exit" << COLOR_RESET << endl;

		int c = _getch();
		switch (c) {
		case KEY_UP:
			choice = (choice > 0) ? choice - 1 : 2;
			break;
		case KEY_DOWN:
			choice = (choice < 2) ? choice + 1 : 0;
			break;
		case ENTER_KEY:
			switch (choice) {
			case 0: {
				showDotsLoading();
				User* user = login(userManager);
				if (user != nullptr) {
					classicProgressBar();
					int userChoice = 0;
					while (true) {
						system("cls||clear");
						cout << "Use arrow keys to navigate and Enter to select." << endl;
						cout << "\033[1;36m" << "\n\nWelcome, " << user->getUsername() << "!" << "\033[0m\v\v" << endl;

						cout << (userChoice == 0 ? COLOR_MAGENTA : COLOR_RESET) << "1. Add Task" << COLOR_RESET << endl;
						cout << (userChoice == 1 ? COLOR_MAGENTA : COLOR_RESET) << "2. Remove Task" << COLOR_RESET << endl;
						cout << (userChoice == 2 ? COLOR_MAGENTA : COLOR_RESET) << "3. Update Task" << COLOR_RESET << endl;
						cout << (userChoice == 3 ? COLOR_MAGENTA : COLOR_RESET) << "4. Display Tasks" << COLOR_RESET << endl;
						cout << (userChoice == 4 ? COLOR_MAGENTA : COLOR_RESET) << "5. Filtering" << COLOR_RESET << endl;
						cout << (userChoice == 5 ? COLOR_MAGENTA : COLOR_RESET) << "6. Logout" << COLOR_RESET << endl;

						int userInput = _getch();
						switch (userInput) {
						case KEY_UP:
							userChoice = (userChoice > 0) ? userChoice - 1 : 5; // Əvvəlki seçimi seç
							break;
						case KEY_DOWN:
							userChoice = (userChoice < 5) ? userChoice + 1 : 0; // Növbəti seçimi seç
							break;
						case ENTER_KEY:
							switch (userChoice) {
							case 0:
								addTaskToUser(user); // Yeni tapşırıq əlavə et
								break;
							case 1:
								displayRemoving(user); // Tapşırıq silmə menyusu
								break;
							case 2:
								displayUpdating(user); // Tapşırıq yeniləmə menyusu
								break;
							case 3:
								displayTasks(user); // Tapşırıqları göstər
								break;
							case 4:
								displayFiltering(user); // Filtrləmə menyusu
								break;
							case 5: // Logout
								user = nullptr; // İstifadəçi məlumatlarını sıfırla
								return;
							}
							cout << "Press any key to continue..." << endl;
							(void)_getch(); // İstifadəçi girişini gözləyin
							break;
						}
					}
				}
				else {
					cout << "Press any key to return to the main menu..." << endl;
					(void)_getch();
				}
				break;
			}
			case 1:
				registerUser(userManager);
				break;
			case 2:
				showDotsExiting();
				exit(0);
				break;
			}
			break;
		default:
			break;
		}
	}
}