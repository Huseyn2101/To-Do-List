#pragma once


// Defining the arrows for the console
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_RIGHT 77
#define KEY_LEFT 75
#define ESC 27
#define ENTER_KEY 13
#define SHIFT 16

// Defining colors
#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"
#define COLOR_WHITE "\033[37m"
#define COLOR_BOLD "\033[1m"


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


struct DateTime {
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;

	DateTime() {
		year = 1900;
		month = 1;
		day = 1;
		hour = 0;
		minute = 0;
		second = 0;
	}

	DateTime(int y, int m, int d, int h, int min, int s)
		: year(y), month(m), day(d), hour(h), minute(min), second(s) {
	}

	DateTime(const tm& tm)
		: year(tm.tm_year + 1900), month(tm.tm_mon + 1), day(tm.tm_mday),
		hour(tm.tm_hour), minute(tm.tm_min), second(tm.tm_sec) {
	}

	DateTime(const DateTime& other) {
		year = other.year;
		month = other.month;
		day = other.day;
		hour = other.hour;
		minute = other.minute;
		second = other.second;
	}

	DateTime(DateTime&& other) noexcept {
		year = other.year;
		month = other.month;
		day = other.day;
		hour = other.hour;
		minute = other.minute;
		second = other.second;
		other.year = 0;
		other.month = 0;
		other.day = 0;
		other.hour = 0;
		other.minute = 0;
		other.second = 0;
	}

	DateTime& operator=(const DateTime& other) {
		if (this != &other) {
			year = other.year;
			month = other.month;
			day = other.day;
			hour = other.hour;
			minute = other.minute;
			second = other.second;
		}
		return *this;
	}

	DateTime& operator=(DateTime&& other) noexcept {
		if (this != &other) {
			year = other.year;
			month = other.month;
			day = other.day;
			hour = other.hour;
			minute = other.minute;
			second = other.second;
			other.year = 0;
			other.month = 0;
			other.day = 0;
			other.hour = 0;
			other.minute = 0;
			other.second = 0;
		}
		return *this;
	}

	bool operator<(const DateTime& other) const {
		if (year != other.year) return year < other.year;
		if (month != other.month) return month < other.month;
		if (day != other.day) return day < other.day;
		if (hour != other.hour) return hour < other.hour;
		if (minute != other.minute) return minute < other.minute;
		return second < other.second;
	}

	bool operator>(const DateTime& other) const {
		return other < *this;
	}

	bool operator==(const DateTime& other) const {
		return year == other.year && month == other.month && day == other.day &&
			hour == other.hour && minute == other.minute && second == other.second;
	}

	friend ostream& operator<<(ostream& os, const DateTime& dt) {
		os << (dt.day < 10 ? "0" : "") << dt.day << "."
			<< (dt.month < 10 ? "0" : "") << dt.month << "."
			<< dt.year << " "
			<< (dt.hour < 10 ? "0" : "") << dt.hour << ":"
			<< (dt.minute < 10 ? "0" : "") << dt.minute << ":"
			<< (dt.second < 10 ? "0" : "") << dt.second;
		return os;
	}

	int getdaysinmonth(int year, int month) const {
		if (month == 2) {
			return isleapyear(year) ? 29 : 28;
		}
		if (month == 4 || month == 6 || month == 9 || month == 11) {
			return 30;
		}
		return 31;
	}

	bool isleapyear(int year) const {
		return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
	}

	bool isValid() const {
		if (year < 1900 || year > 2100) return false;
		if (month < 1 || month > 12) return false;
		if (day < 1 || day > getdaysinmonth(year, month)) return false;
		if (hour < 0 || hour > 23) return false;
		if (minute < 0 || minute > 59) return false;
		if (second < 0 || second > 59) return false;
		return true;
	}

	void display() const {
		cout << (day < 10 ? "0" : "") << day << "."
			<< (month < 10 ? "0" : "") << month << "."
			<< year << " "
			<< (hour < 10 ? "0" : "") << hour << ":"
			<< (minute < 10 ? "0" : "") << minute << ":"
			<< (second < 10 ? "0" : "") << second;
	}

	// Convert to JSON
	json toJson() const {
		return json{
			{"year", year},
			{"month", month},
			{"day", day},
			{"hour", hour},
			{"minute", minute},
			{"second", second}
		};
	}

	// Convert from JSON
	static DateTime fromJson(const json& j) {
		return DateTime(
			j["year"],
			j["month"],
			j["day"],
			j["hour"],
			j["minute"],
			j["second"]
		);
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



class Task {
private:
	string _name;
	string _description;
	Priority _priority;
	DateTime _startTime;
	DateTime _endTime;
	bool _completed;
	char _symbol = 254;

public:
	Task() : _name(""), _description(""), _priority(Priority::LOW),
		_startTime(), _endTime(), _completed(false) {
	}

	Task(const string& n, const string& desc, Priority prio,
		const DateTime& start, const DateTime& end, bool comp = false)
		: _name(n), _description(desc), _priority(prio),
		_startTime(start), _endTime(end), _completed(comp) {
	}

	Task(const Task& other)
		:_name(other._name),
		_description(other._description),
		_priority(other._priority),
		_startTime(other._startTime),
		_endTime(other._endTime),
		_completed(other._completed),
		_symbol(other._symbol)
	{
	}

	Task(Task&& other) noexcept
		: _name(move(other._name)),
		_description(move(other._description)),
		_priority(move(other._priority)),
		_startTime(move(_startTime)),
		_endTime(move(other._endTime)),
		_completed(move(other._completed)),
		_symbol(move(other._symbol))
	{
	}

	Task& operator=(const Task& other) {
		if (this != &other) {
			_name = other._name;
			_description = other._description;
			_priority = other._priority;
			_startTime = other._startTime;
			_endTime = other._endTime;
			_completed = other._completed;
			_symbol = other._symbol;
		}
		return *this;
	}

	Task& operator=(Task&& other) noexcept {
		if (this != &other) {
			_name = std::move(other._name);
			_description = std::move(other._description);
			_priority = other._priority;
			_startTime = std::move(other._startTime);
			_endTime = std::move(other._endTime);
			_completed = other._completed;
			_symbol = other._symbol;
		}
		return *this;
	}

	void setName(const string& name) {
		_name = name;
	}

	void setDescription(const string& description) {
		_description = description;
	}

	void setPriority(Priority priority) {
		_priority = priority;
	}

	void setStartTime(const DateTime& start) {
		_startTime = start;
	}

	void setEndTime(const DateTime& end) {
		_endTime = end;
	}

	void setStatus(bool completed) {
		_completed = completed;
	}

	void display() const {
		cout << "\v~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\v" << endl;
		cout << "Name: " << _name << endl;
		cout << "Description: " << _description << endl;
		cout << "Start time: "; _startTime.display(); cout << endl;
		cout << "End time: "; _endTime.display(); cout << endl;
		cout << "Priority level: " << priorityToString(_priority) << endl;
		if (_completed) {
			cout << "Status: " << "\033[1;32m" << _symbol << "\033[0m" << endl;
		}
		else {
			cout << "Status: " << "\033[1;31m" << _symbol << "\033[0m" << endl;
		}
		cout << "\v~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\v" << endl;
	}

	string getName() const {
		return _name;
	}

	string getDescription() const {
		return _description;
	}

	Priority getPriority() const {
		return _priority;
	}

	DateTime getStartTime() const {
		return _startTime;
	}

	DateTime getEndTime() const {
		return _endTime;
	}

	bool getStatus() const {
		return _completed;
	}

	static string priorityToString(Priority priority) {
		switch (priority) {
		case Priority::LOW: return "Low";
		case Priority::MEDIUM: return "Medium";
		case Priority::HIGH: return "High";
		case Priority::CRITICAL: return "Critical";
		case Priority::URGENT: return "Urgent";
		default: return "Unknown";
		}
	}

	// Convert task to JSON
	json toJson() const {
		return json{
			{"name", _name},
			{"description", _description},
			{"priority", static_cast<int>(_priority)},
			{"startTime", _startTime.toJson()},
			{"endTime", _endTime.toJson()},
			{"completed", _completed}
		};
	}

	// Convert JSON to task
	static Task fromJson(const json& j) {
		Task task;
		task._name = j["name"];
		task._description = j["description"];
		task._priority = static_cast<Priority>(j["priority"].get<int>());
		task._startTime = DateTime::fromJson(j["startTime"]);
		task._endTime = DateTime::fromJson(j["endTime"]);
		task._completed = j["completed"];

		return task;
	}
};


class User {
private:
	string _username;
	string _password;
	string _hashpassword;
	vector<Task> _tasks;

	static string hashPassword(const string& password) {
		return to_string(hash<string>{}(password));
	}

public:
	User() {};

	User(const string& uname, const string& password)
		: _username(uname), _hashpassword(hashPassword(password)) {
	}

	string getUsername() const {
		return _username;
	}

	const vector<Task>& getTasks() const {
		return _tasks;
	}

	void addTask(const Task& task) {
		_tasks.push_back(task);
	}

	vector<Task>& getTasks() {
		return _tasks;
	}

	void removeTask(const string text) {
		auto it = find_if(_tasks.begin(), _tasks.end(), [&](const Task& task) {
			return task.getName() == text;
			});
		if (it != _tasks.end()) {
			_tasks.erase(it);
		}
		else {
			cout << "Task not found!" << endl;
		}
	}

	void removeTask(size_t index) {
		if (index < _tasks.size()) {
			_tasks.erase(_tasks.begin() + index);
		}
	}

	void updateTask(size_t index, const Task& task) {
		if (index < _tasks.size()) {
			_tasks[index] = task;
		}
	}

	bool verifyPassword(const string& password) const {
		string hashedInput = hashPassword(password);
		if (hashedInput == _hashpassword) {
			cout << "Password verified successfully for user: " << _username << endl;
			return true;
		}
		else {
			cout << "Password verification failed for user: " << _username << endl;
			return false;
		}
	}

	void displayTasks() {
		sort(_tasks.begin(), _tasks.end(), [](const Task& a, const Task& b) {
			if (a.getPriority() == b.getPriority()) {
				return a.getStartTime() < b.getStartTime();
			}
			return a.getPriority() > b.getPriority();
			});
		for (const auto& task : _tasks) {
			task.display();
		}
	}

	void filterByPriority(Priority p) {
		for (const auto& task : _tasks) {
			if (task.getPriority() == p) {
				task.display();
			}
		}
	}

	void filterTasksByDate(const DateTime& start, const DateTime& end) {
		for (const auto& task : _tasks) {
			if (task.getStartTime() > start && task.getStartTime() < end) {
				task.display();
			}
		}
	}

	json toJson() const {
		json tasksJson = json::array();
		for (const auto& task : _tasks) {
			tasksJson.push_back(task.toJson());
		}
		return json{ {"username", _username}, {"password", _password}, {"tasks", tasksJson} };

	}

	static User fromJson(const json& j) {
		User user(j["username"], j["password"]);
		for (const auto& taskJson : j["tasks"]) {
			user._tasks.push_back(Task::fromJson(taskJson));
		}
		return user;
	}
};



class JsonHandler {
private:
	string _filepath;

public:

	explicit JsonHandler(const string& filepath) : _filepath(filepath) {}

	std::vector<User> loadUsersFromFile() const {
		vector<User> users;
		ifstream file(_filepath);

		if (!file.is_open()) {
			cerr << "Error opening file: " << _filepath << endl;
			return users;
		}

		try {
			json jsonData;
			file >> jsonData;
			for (const auto& userJson : jsonData) {
				users.push_back(User::fromJson(userJson));
			}
		}
		catch (const std::exception& e) {
			cerr << "Error parsing JSON: " << e.what() << endl;
		}

		return users;
	}

	void saveUsersToFile(const std::vector<User>& users) {
		std::ofstream file(_filepath);

		if (!file.is_open()) {
			std::cerr << "Error opening file for writing: " << _filepath << std::endl;
			return;
		}

		json jsonData = json::array();
		for (const auto& user : users) {
			jsonData.push_back(user.toJson());
		}

		file << jsonData.dump(4);
	}
};



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
	cout << R"(
                                    _____         _          _____           _ 
                                   |_   _|       | |        |_   _|         | |
                                     | | __ _ ___| | __       | | ___   ___ | |
                                     | |/ _` / __| |/ /       | |/ _ \ / _ \| |
                                     | | (_| \__ \   <        | | (_) | (_) | |
                                     \_/\__,_|___/_|\_\       \_/\___/ \___/|_|
                                                                  
)" << endl;

	cout << "\033[1;36m" << "Welcome to TaskMaster Pro - Your Ultimate Task Management System!" << "\033[0m" << endl;
	cout << "\033[1;33m" << "Version 2.4.1 | Secure Login Enabled | Multi-User Support" << "\033[0m" << endl << endl;

	cout << "Features:" << endl;
	cout << static_cast<char>(250) << " Secure user authentication" << endl;
	cout << static_cast<char>(250) << " Priority-based task organization" << endl;
	cout << static_cast<char>(250) << " DateTime scheduling with validation" << endl;
	cout << static_cast<char>(250) << " JSON data persistence" << endl;
	cout << static_cast<char>(250) << " Beautiful console interface" << endl << endl;

	cout << "\033[3m" << "Please login to access your tasks or register as a new user." << "\033[0m" << endl;
	cout << "----------------------------------------------------" << endl;
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
		cout << "Use arrow keys to navigate and Enter to select." << endl;

		if (user->getTasks().empty()) {
			cout << "No tasks available." << endl;
			break;
		}

		int i = 0;
		for (const auto& task : user->getTasks()) {
			if (choice == i) {
				cout << COLOR_MAGENTA << i + 1 << ". " << task.getName() << COLOR_RESET << endl;
			}
			else {
				cout << i + 1 << ". " << task.getName() << endl;
			}
			++i;
		}

		int c = _getch();
		switch (c) {
		case KEY_UP:
			choice = (choice > 0) ? choice-- : user->getTasks().size() - 1;
			break;
		case KEY_DOWN:
			choice = (choice < user->getTasks().size() - 1) ? choice++ : 0;
			break;
		case ENTER_KEY:
			if (!user->getTasks().empty()) {
				Task& selectedTask = user->getTasks()[choice];
				system("cls||clear");
				cout << "Selected task: " << selectedTask.getName() << endl;

				string newName;
				cout << "Enter new task name (current: " << selectedTask.getName() << "): ";
				cin.ignore();
				getline(cin, newName);
				if (!newName.empty()) {
					selectedTask.setName(newName);
				}

				string newDescription;
				cout << "Enter new task description (current: " << selectedTask.getDescription() << "): ";
				getline(cin, newDescription);
				if (!newDescription.empty()) {
					selectedTask.setDescription(newDescription);
				}

				int newPriority;
				cout << "Enter new task priority (0-LOW, 1-MEDIUM, 2-HIGH, 3-CRITICAL, 4-URGENT): ";
				while (!(cin >> newPriority) || newPriority < 0 || newPriority > 4) {
					cout << "Invalid priority! Please enter a value between 0 and 4." << endl;
					cin.clear();
					cin.ignore(INT_MAX, '\n');
				}
				selectedTask.setPriority(static_cast<Priority>(newPriority));
				cin.ignore();

				DateTime newStartDate;
				cout << "Enter new task start date (YYYY MM DD HH MM SS): ";
				while (!(cin >> newStartDate.year >> newStartDate.month >> newStartDate.day
					>> newStartDate.hour >> newStartDate.minute >> newStartDate.second) ||
					!newStartDate.isValid()) {
					cout << "Invalid start date! Please enter in format YYYY MM DD HH MM SS." << endl;
					cin.clear();
					cin.ignore(INT_MAX, '\n');
				}
				selectedTask.setStartTime(newStartDate);

				DateTime newEndDate;
				cout << "Enter new task end date (YYYY MM DD HH MM SS): ";
				while (!(cin >> newEndDate.year >> newEndDate.month >> newEndDate.day
					>> newEndDate.hour >> newEndDate.minute >> newEndDate.second) ||
					!newEndDate.isValid() || newEndDate < newStartDate) {
					cout << "Invalid end date! End date must be valid and later than the start date." << endl;
					cin.clear();
					cin.ignore(INT_MAX, '\n');
				}
				selectedTask.setEndTime(newEndDate);

				int newStatus;
				cout << "Enter new task status (0-Not Completed, 1-Completed): ";
				while (!(cin >> newStatus) || (newStatus != 0 && newStatus != 1)) {
					cout << "Invalid status! Please enter 0 (Not Completed) or 1 (Completed)." << endl;
					cin.clear();
					cin.ignore(INT_MAX, '\n');
				}
				selectedTask.setStatus(newStatus);
				cin.ignore();

				cout << "Task updated successfully!" << endl;
				cout << "Press any key to continue..." << endl;
				(void)_getch();
			}
			break;
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
		cout << "Use arrow keys to navigate and Enter to remove a task." << endl;

		if (user->getTasks().empty()) {
			cout << "No tasks available." << endl;
			cout << "Press any key to return to the previous menu..." << endl;
			(void)_getch();
			break;
		}

		int i = 0;
		for (const auto& task : user->getTasks()) {
			if (choice == i) {
				cout << COLOR_MAGENTA << i + 1 << ". " << task.getName() << COLOR_RESET << endl;
			}
			else {
				cout << i + 1 << ". " << task.getName() << endl;
			}
			++i;
		}
		cout << COLOR_RESET << endl;

		int c = _getch();
		switch (c) {
		case KEY_UP:
			choice = (choice > 0) ? choice - 1 : user->getTasks().size() - 1;
			break;
		case KEY_DOWN:
			choice = (choice < user->getTasks().size() - 1) ? choice + 1 : 0;
			break;
		case ENTER_KEY:
			if (!user->getTasks().empty()) {
				Task removedTask = user->getTasks()[choice];
				user->removeTask(choice);
				cout << "Task \"" << removedTask.getName() << "\" removed successfully!" << endl;
				cout << "Press any key to continue..." << endl;
				(void)_getch();
			}
			break;
		case ESC:
			cout << "Exiting task removal..." << endl;
			return;
		default:
			cout << "Invalid key pressed!" << endl;
			cout << "Press any key to continue..." << endl;
			(void)_getch();
			break;
		}
	}
}

void displayTasks(User* user) {
	int choice = 0;
	while (true) {
		system("cls||clear");

		cout << "Use arrow keys to navigate and Enter to select." << endl;

		if (user->getTasks().empty()) {
			cout << "No tasks available." << endl;
			cout << "Press any key to return to the previous menu..." << endl;
			(void)_getch();
			break;
		}

		int i = 0;
		for (const auto& task : user->getTasks()) {
			if (choice == i) {
				cout << COLOR_MAGENTA << i + 1 << ". " << task.getName() << COLOR_RESET << endl;
			}
			else {
				cout << i + 1 << ". " << task.getName() << endl;
			}
			++i;
		}
		cout << COLOR_RESET << endl;

		int c = _getch();
		switch (c) {
		case KEY_UP:
			choice = (choice > 0) ? choice - 1 : user->getTasks().size() - 1;
			break;
		case KEY_DOWN:
			choice = (choice < user->getTasks().size() - 1) ? choice + 1 : 0;
			break;
		case ENTER_KEY:
			if (!user->getTasks().empty()) {
				Task& selectedTask = user->getTasks()[choice];
				cout << "Selected task: " << selectedTask.getName() << endl;
				selectedTask.display();
				cout << "Press any key to continue..." << endl;
				(void)_getch();
			}
			break;
		case ESC:
			cout << "Exiting task display..." << endl;
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
								user->displayTasks(); // Tapşırıqları göstər
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
				break;
			}
			break;
		default:
			break;
		}
	}
}