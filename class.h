#pragma once


#define ENTER_KEY 13

#pragma warning(disable : 4996)

#pragma region Enum

struct DateTime {
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;

	DateTime()
	{
		year = 1900;
		month = 1;
		day = 1;
		hour = 0;
		minute = 0;
		second = 0;
	}

	DateTime(int y, int m, int d, int h, int min, int s) : year(y), month(m), day(d), hour(h), minute(min), second(s) {}

	DateTime(const tm& tm) : year(tm.tm_year), month(tm.tm_mon), day(tm.tm_mday), hour(tm.tm_hour), minute(tm.tm_min), second(tm.tm_sec) {}



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

	int getdaysinmonth(int year, int month) const {
		if (month == 2) {
			return isleapyear(year) ? 29 : 28;
		}
		if (month == 3 || month == 5 || month == 8 || month == 10) {
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
		cout << day << "." << month << "." << year << " " << hour << ":" << minute << ":" << second;
	}

	// Convert task to JSON
	json toJson() const {
		json j;
		j["year"] = year;
		j["month"] = month;
		j["day"] = day;
		j["hour"] = hour;
		j["minute"] = minute;
		j["second"] = second;

		return j;
	}

	// Convert JSON to task
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


string toLowerCase(string& text) {
	for (size_t i = 0; i < text.length(); ++i) {
		text[i] = tolower(text[i]);
	}
	return text;
}

string getHiddenÝnput() {
	string password;
	char ch;
	while (true) {
		ch = _getch();
		if (ch == ENTER_KEY) {
			break;
		}
		else if (ch == 8) { // Backspace key
			if (!password.empty()) {
				password.pop_back();
				cout << "\b \b"; // Move back, print space, move back again
			}
		}
		else {
			password += ch;
			cout << '*'; // Print symbol for each character
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

string priorityToString(Priority priority) {

	switch (priority) {
	case Priority::LOW: return "Low";
	case Priority::MEDIUM: return "Medium";
	case Priority::HIGH: return "High";
	case Priority::CRITICAL: return "Critical";
	case Priority::URGENT: return "Urgent";
	default: return "Unknown";
	}
}

class Task {
private:
	string _name;
	string _description;
	Priority _priority;
	DateTime _startTime;
	DateTime _endTime;
	bool _completed;
	char symbol = 253;

public:

	Task() : _name(""), _description(""), _priority(Priority::LOW),
		_startTime(), _endTime(), _completed(false) {
	}

	Task(const string& n, const string& desc, Priority prio,
		const DateTime& start, const DateTime& end, bool comp = false)
		: _name(n), _description(desc), _priority(prio),
		_startTime(start), _endTime(end), _completed(comp) {
	}


	void display() {
		cout << "\v~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\v" << endl;
		cout << "Name: " << _name << " [" << priorityToString(_priority) << "]." << endl;
		cout << "Description: " << _description << endl;
		cout << "Start time: "; _startTime.display(); cout << endl;
		cout << "End time: "; _endTime.display(); cout << endl;
		cout << "\v~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\v" << endl;
		if (_completed) {
			cout << "Status: " << "\033[1;32m" << symbol << "\033[0m" << endl;
		}
		else {
			cout << "Status: " << "\033[1;31m" << symbol << "\033[0m" << endl;
		}
	}

	// Convert task to JSON
	json toJson() const {
		json j;
		j["name"] = _name;
		j["description"] = _description;
		j["priority"] = static_cast<int>(_priority);
		j["startTime"] = _startTime.toJson();
		j["endTime"] = _endTime.toJson();
		j["completed"] = _completed;

		return j;
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
	static const int Max = 100;
	string _username;
	string _password;
	vector<Task> _tasks;

public:

	User() {};

	User(const string& name, const string& password) : _username(name), _password(password) {}

	static Task createUserInput() {
		string name, description;
		int priorityInput;
		DateTime start, end;

		cout << "Enter the task's name: ";
		getline(cin, name);
		cout << "Enter the task's description: ";
		getline(cin, description);

		do
		{
			cout << "Enter priority level (0:LOW, 1:MEDIUM, 2:HIGH, 3:CRITICAL, 4:URGENT)";
			cin >> priorityInput;
			if (priorityInput < 0 || priorityInput > 4)
			{
				cout << "Invalid priority! Please enter a value between 0 and 4." << endl;
			}
		} while (priorityInput < 0 || priorityInput > 4);
		cin.ignore();



	}

	string getUsername()const { return _username; }

	void addTask(const Task& task) {
		_tasks.push_back(task);
	}

	/*void listTasks() const {
	   for (size_t i = 0; i < _tasks.size(); i++) {
		   _tasks[i].display();
	   }
	}*/


	void saveToFile(const string& filename) const {
		ofstream file(filename);
		if (!file.is_open()) {
			cerr << "Error: Could not open file for writing." << endl;
			return;
		}
		json j;
		j["username"] = _username;
		j["password"] = _password;
		j["tasks"] = json::array();
		for (const auto& task : _tasks) {
			j["tasks"].push_back(task.toJson());
		}
		file << j.dump(4);
		file.close();
	}

	static User loadFromFile(const string& filename) {
		ifstream file(filename);
		json j;
		file >> j;
		User user(j["username"], "");
		user._password = j["password"];
		for (const auto& taskJson : j["tasks"]) {
			user._tasks.push_back(Task::fromJson(taskJson));
		}

		return user;
	}

};