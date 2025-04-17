#pragma once


#define ENTER_KEY 13

//#pragma warning(disable : 4996)

struct LoginAttempt {
	int failedAttempts = 0;
	chrono::system_clock::time_point blockUntill = chrono::system_clock::time_point::min();

	bool isBlocked()const {
		auto now = chrono::system_clock::now();
		return now < blockUntill;
	}

	void registerFailure() {
		failedAttempts++;
		if (failedAttempts >= 3) {
			blockUntill = chrono::system_clock::now() + chrono::minutes(5);
			failedAttempts = 0;
		}
	}

	void resetAttempts() {
		failedAttempts = 0;
		blockUntill = chrono::system_clock::time_point::min();
	}
};


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

	DateTime(const tm& tm) : year(tm.tm_year + 1900), month(tm.tm_mon + 1), day(tm.tm_mday), hour(tm.tm_hour), minute(tm.tm_min), second(tm.tm_sec) {}



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
		cout << '\t' << (day < 10 ? "0" : "") << day << "."
			<< (month < 10 ? "0" : "") << month << "."
			<< year << " "
			<< (hour < 10 ? "0" : "") << hour << ":"
			<< (minute < 10 ? "0" : "") << minute << ":"
			<< (second < 10 ? "0" : "") << second << endl;
	}

	// Convert task to JSON
	json toJson() const {
		return json{ {"year", year}, {"month", month}, {"day", day},
					{"hour", hour}, {"minute", minute}, {"second", second} };
	}


	// Convert JSON to task
	static DateTime fromJson(const json& j) {
		return DateTime(j["year"], j["month"], j["day"],
			j["hour"], j["minute"], j["second"]);
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

string getHiddenİnput() {
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

	static Task createUserInput() {
		string name, description;
		int priorityInput;
		DateTime start, end;
		bool validStart = false, validEnd = false;

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

		do
		{
			cout << "Enter start time (YYYY MM DD HH MM SS): ";
			cin >> start.year >> start.month >> start.day >> start.hour >> start.minute >> start.second;

			if (!start.isValid()) {
				cout << "Invalid start! please try again." << endl;
			}
			else {
				validStart = true;
			}
		} while (!validStart);

		do
		{
			cout << "Enter end time (YYYY MM DD HH MM SS): ";
			cin >> end.year >> end.month >> end.day >> end.hour >> end.minute >> end.second;

			if (!end.isValid()) {
				cout << "Invalid end timr! Please try again." << endl;
			}
			else if (end < start) {
				cout << "End time cannot be earlier than start time! Please try again." << endl;
			}
			else {
				validEnd = true;
			}
		} while (!validEnd);
		cin.ignore();

		return Task(name, description, static_cast<Priority>(priorityInput), start, end);
	}


	void display() const {
		cout << "\v~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\v" << endl;
		cout << "Name: " << _name << endl;
		cout << "Description: " << _description << endl;
		cout << "Start time: "; _startTime.display(); cout << endl;
		cout << "End time: "; _endTime.display(); cout << endl;
		cout << "Priority level: " << priorityToString(_priority) << endl;
		if (_completed) {
			cout << "Status: " << "\033[1;32m" << symbol << "\033[0m" << endl;
		}
		else {
			cout << "Status: " << "\033[1;31m" << symbol << "\033[0m" << endl;
		}
		cout << "\v~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\v" << endl;
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
	static const int Max = 100;
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

	string getUsername()const { return _username; }

	void addTask(const Task& task) {
		_tasks.push_back(task);
	}


	bool verifyPassword(const string& password) const {
		return _hashpassword == hashPassword(password);
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
		json j;
		j["username"] = _username;
		j["password"] = _hashpassword;
		j["tasks"] = json::array();
		for (const auto& task : _tasks) {
			j["tasks"].push_back(task.toJson());
		}
		return j;
	}

	static User fromJson(const json& j) {
		User user(j["username"], ""); // Şifrə JSON-dan yüklənəcək
		user._hashpassword = j["password"];
		for (const auto& taskJson : j["tasks"]) {
			user._tasks.push_back(Task::fromJson(taskJson));
		}
		return user;
	}

};


class UserManager {
private:
	vector<User> _users;
	vector<pair<string, LoginAttempt>> loginAttempts;
	string _datafile;  // json filename

	LoginAttempt& getLoginAttempt(const string& username) {
		for (auto& attempt : loginAttempts)
		{
			if (attempt.first == username) {
				return attempt.second;
			}
		}

		loginAttempts.emplace_back(username, LoginAttempt());
		return loginAttempts.back().second;
	}

public:
	UserManager(const string& filename) :_datafile(filename)
	{
		ifstream file(_datafile);
		if (!file.is_open() || file.peek() == ifstream::traits_type::eof()) {
			generateFakeData(_users);
			cout << "Generated initial fake data for the system." << endl;
		}
		else {
			file.close();
			loadUsersFromFile();
		}
	}

	void addUser(const User& user) {
		for (const auto& u : _users)
		{
			if (u.getUsername() == user.getUsername()) {
				cout << "User already exists!" << endl;
				return;
			}
		}
		_users.push_back(user);
		saveUsersToFile();
		cout << "User added successfully!" << endl;
	}

	User* authenticateUser(const string& username, const string& password) {
		LoginAttempt& attempt = getLoginAttempt(username);

		if (attempt.isBlocked()) {
			cout << "Your account is temporarily blocked. Please try again later." << endl;
			return nullptr;
		}

		for (auto& user : _users)
		{
			if (user.getUsername() == username) {
				if (user.verifyPassword(password)) {
					cout << "Login successfully!" << endl;
					attempt.resetAttempts();
					return &user;
				}
				else {
					attempt.registerFailure();
					if (attempt.isBlocked()) {
						cout << "Too many failed attempts. Your account is blocked for 5 minutes." << endl;
					}
					return nullptr;
				}
			}
		}

		cout << "Invalid username or password. Please try again!" << endl;
		attempt.registerFailure();
		if (attempt.isBlocked()) {
			cout << "Too many failed attempts. Your account is blocked for 5 minutes!" << endl;
		}
		return nullptr;
	}

	void saveUsersToFile() {
		json j = json::array();
		for (const auto& user : _users)
		{
			j.push_back(user.toJson());
		}
		ofstream file(_datafile);
		if (!file.is_open()) {
			cerr << "Error: Could open file for writing!" << endl;
			return;
		}
		file << j.dump(4);
		file.close();
	}

	void loadUsersFromFile() {
		ifstream file(_datafile);
		if (!file.is_open()) {
			cerr << "Error: Could not open file. Generating default data.\n";
			generateFakeData(_users);  // Generate data if file doesn't exist
			return;
		}

		try {
			json j;
			file >> j;
			if (j.empty()) {
				generateFakeData(_users);
			}
			else {
				for (const auto& userJson : j) {
					_users.push_back(User::fromJson(userJson));
				}
			}
		}
		catch (const json::exception& e) {
			cerr << "JSON Error: " << e.what() << "\n";
			generateFakeData(_users);  // Regenerate data if corrupted
		}
	}

	void generateFakeData(vector<User>& users) {
		User user1("nhu55uxn", "R3alFutb0l!@#"),
			user2("ako_ismahilov", "Gam3Over_420!"),
			user3("h_haciyefh", "N1vaPower++22"),
			user4("404nilfound", "RusAze_123!Code"),
			user5("axmedli_cs", "C$Strike4Life"),
			user6("gulchin_snoozer", "Sl33pAlarmFail@7"),
			user7("vistoria_vito", "V1t0Black_Pro99"),
			user8("tamerlan_trainMan", "QabalaFan4ever!");

		// Tasks for user1 - Huseyn
		user1.addTask(Task("Watch El Clasico Replay", "Re-watch Real Madrid's thrilling win and analyze their offensive and defensive transitions thoroughly.", HIGH, DateTime(2025, 4, 16, 21, 0, 0), DateTime(2025, 4, 16, 23, 0, 0)));
		user1.addTask(Task("Read Quran Peacefully", "Focus on reciting Surah Al-Baqarah and reflecting on the meaning of verses with tafsir.", HIGH, DateTime(2025, 4, 16, 23, 30, 0), DateTime(2025, 4, 17, 0, 30, 0)));
		user1.addTask(Task("Work on Dribbling", "Improve left foot control and acceleration with cone-based dribbling drills at the local stadium.", MEDIUM, DateTime(2025, 4, 17, 18, 0, 0), DateTime(2025, 4, 17, 18, 45, 0)));
		user1.addTask(Task("Football Tracker App", "Build a Python-based app to track matches using live football data APIs.", HIGH, DateTime(2025, 4, 18, 20, 0, 0), DateTime(2025, 4, 18, 22, 0, 0)));
		user1.addTask(Task("Family Dinner", "Sit down with family to eat and talk about the week’s positive moments and future plans.", LOW, DateTime(2025, 4, 19, 19, 0, 0), DateTime(2025, 4, 19, 21, 0, 0)));
		user1.addTask(Task("Backend Login Fix", "Review backend login code and solve token expiration bugs and redirect loops.", CRITICAL, DateTime(2025, 4, 20, 14, 0, 0), DateTime(2025, 4, 20, 15, 30, 0)));

		// Tasks for user2 - Akif
		user2.addTask(Task("Valorant Ranked Night", "Play competitive matches with friends and try to reach Platinum rank before reset.", HIGH, DateTime(2025, 4, 16, 22, 0, 0), DateTime(2025, 4, 17, 1, 0, 0)));
		user2.addTask(Task("Oversleep Plan", "Set an alarm but ignore it to extend sleep, skipping first lectures.", LOW, DateTime(2025, 4, 17, 10, 0, 0), DateTime(2025, 4, 17, 12, 0, 0)));
		user2.addTask(Task("Flu Recovery", "Stay in bed, drink herbal tea and monitor temperature and symptoms closely.", MEDIUM, DateTime(2025, 4, 18, 9, 0, 0), DateTime(2025, 4, 18, 9, 30, 0)));
		user2.addTask(Task("Fix RPG Inventory Bug", "Debug an item stacking issue in RPG code that breaks the player inventory.", HIGH, DateTime(2025, 4, 19, 20, 0, 0), DateTime(2025, 4, 19, 21, 30, 0)));
		user2.addTask(Task("Streamer Tactics Review", "Analyze high-rank streamer tactics for better positioning and map control.", LOW, DateTime(2025, 4, 20, 23, 0, 0), DateTime(2025, 4, 21, 1, 0, 0)));
		user2.addTask(Task("Optimize Game Engine", "Improve movement and hitbox collision handling for smoother in-game physics.", HIGH, DateTime(2025, 4, 21, 15, 0, 0), DateTime(2025, 4, 21, 17, 0, 0)));

		// Tasks for user3 - Abulfaz
		user3.addTask(Task("Niva Repair Guide", "Read forums and watch mechanic tutorials on engine maintenance for Niva cars.", MEDIUM, DateTime(2025, 4, 17, 13, 0, 0), DateTime(2025, 4, 17, 14, 0, 0)));
		user3.addTask(Task("Recharge Nap", "Take a power nap in the late afternoon to boost energy for coding sessions.", LOW, DateTime(2025, 4, 17, 17, 0, 0), DateTime(2025, 4, 17, 19, 0, 0)));
		user3.addTask(Task("Launch Portfolio Website", "Set up a basic portfolio with sections for resume, contact, and projects using HTML/CSS.", HIGH, DateTime(2025, 4, 18, 10, 0, 0), DateTime(2025, 4, 18, 13, 0, 0)));
		user3.addTask(Task("C++ Vector Mastery", "Revisit custom vector implementation and test various edge cases and memory usage.", HIGH, DateTime(2025, 4, 19, 15, 0, 0), DateTime(2025, 4, 19, 16, 30, 0)));
		user3.addTask(Task("Desk Cleanup", "Organize the desk, sort notes and clean cables for better focus.", LOW, DateTime(2025, 4, 20, 12, 0, 0), DateTime(2025, 4, 20, 12, 45, 0)));
		user3.addTask(Task("Fix LinkedList Error", "Debug and resolve segmentation fault caused by improper node insertion.", CRITICAL, DateTime(2025, 4, 21, 14, 0, 0), DateTime(2025, 4, 21, 16, 0, 0)));

		// Tasks for user4 - Nilufar
		user4.addTask(Task("Study Azerbaijani", "Learn grammar rules and practice pronunciation from the A1 language guide.", MEDIUM, DateTime(2025, 4, 17, 10, 0, 0), DateTime(2025, 4, 17, 11, 30, 0)));
		user4.addTask(Task("iPad Coding Challenge", "Code a to-do list app in Swift using UIKit and test it on your iPad.", HIGH, DateTime(2025, 4, 17, 14, 0, 0), DateTime(2025, 4, 17, 16, 0, 0)));
		user4.addTask(Task("Russian Speaking Practice", "Join a conversation club to improve fluency by speaking with native Russian speakers.", HIGH, DateTime(2025, 4, 18, 18, 0, 0), DateTime(2025, 4, 18, 18, 45, 0)));
		user4.addTask(Task("Language Homework", "Complete exercises on noun cases and verbs of motion for your language course.", MEDIUM, DateTime(2025, 4, 19, 10, 0, 0), DateTime(2025, 4, 19, 12, 0, 0)));
		user4.addTask(Task("Install IDE", "Download and configure a coding environment app on your iPad like Playgrounds or Juno.", LOW, DateTime(2025, 4, 20, 19, 0, 0), DateTime(2025, 4, 20, 19, 30, 0)));
		user4.addTask(Task("Flashcard Review", "Use Anki to revise vocabulary learned during the week and track spaced repetition progress.", MEDIUM, DateTime(2025, 4, 21, 22, 0, 0), DateTime(2025, 4, 21, 22, 30, 0)));

		// Tasks for user5 - Murad
		user5.addTask(Task("CS Map Practice", "Practice grenade throws and angles on Dust2 map.", MEDIUM, DateTime(2025, 4, 17, 13, 0, 0), DateTime(2025, 4, 17, 14, 30, 0)));
		user5.addTask(Task("Host LAN Party", "Organize a CS LAN party with friends and ensure all configs are synced.", HIGH, DateTime(2025, 4, 18, 19, 0, 0), DateTime(2025, 4, 18, 23, 30, 0)));
		user5.addTask(Task("Demo Review Session", "Review match demos to spot reaction time mistakes and missed headshots.", HIGH, DateTime(2025, 4, 19, 15, 0, 0), DateTime(2025, 4, 19, 17, 0, 0)));
		user5.addTask(Task("Crosshair Tuning", "Test multiple crosshair styles and dynamic/static settings for precision.", LOW, DateTime(2025, 4, 20, 20, 0, 0), DateTime(2025, 4, 20, 20, 30, 0)));
		user5.addTask(Task("Update Gaming Rig", "Install latest GPU drivers and remove dust from fans.", MEDIUM, DateTime(2025, 4, 21, 11, 0, 0), DateTime(2025, 4, 21, 12, 0, 0)));
		user5.addTask(Task("CS Plugin Debug", "Fix plugin bug affecting killfeed display in private servers.", CRITICAL, DateTime(2025, 4, 21, 18, 0, 0), DateTime(2025, 4, 21, 19, 30, 0)));

		// Tasks for user5 - Murad
		user5.addTask(Task("CS Practice Aim", "Complete 60 minutes of aim training using Aim Lab, focusing on reflex speed and precision.", HIGH, DateTime(2025, 4, 17, 10, 0, 0), DateTime(2025, 4, 17, 11, 0, 0)));
		user5.addTask(Task("Gym Strength Routine", "Follow a strength-building program with squats, bench press, and deadlifts for norm preparation.", MEDIUM, DateTime(2025, 4, 17, 17, 0, 0), DateTime(2025, 4, 17, 18, 30, 0)));
		user5.addTask(Task("Colloquium Notes Review", "Review past colloquium notes to prepare for upcoming evaluations.", HIGH, DateTime(2025, 4, 18, 14, 0, 0), DateTime(2025, 4, 18, 15, 30, 0)));
		user5.addTask(Task("Mock Test", "Take a timed practice test in exam format to identify weak points in subject comprehension.", HIGH, DateTime(2025, 4, 19, 9, 0, 0), DateTime(2025, 4, 19, 10, 30, 0)));
		user5.addTask(Task("CS Update Patch Read", "Read patch notes from Counter Strike’s latest update to understand gameplay changes.", LOW, DateTime(2025, 4, 20, 20, 0, 0), DateTime(2025, 4, 20, 20, 30, 0)));
		user5.addTask(Task("Nutrition Meal Prep", "Prepare meals for the week that enhance energy and focus during training.", MEDIUM, DateTime(2025, 4, 21, 12, 0, 0), DateTime(2025, 4, 21, 13, 0, 0)));

		// Tasks for user6 - Gulchin
		user6.addTask(Task("Set Alarm Challenge", "Try waking up to first alarm without snoozing; record experience in journal.", LOW, DateTime(2025, 4, 17, 7, 0, 0), DateTime(2025, 4, 17, 7, 30, 0)));
		user6.addTask(Task("Class Punctuality Goal", "Attempt to attend all classes on time for one week and reflect on performance.", MEDIUM, DateTime(2025, 4, 17, 9, 0, 0), DateTime(2025, 4, 17, 10, 0, 0)));
		user6.addTask(Task("Language Clarity Exercise", "Practice speaking slower and more clearly in Azerbaijani with a partner.", HIGH, DateTime(2025, 4, 18, 11, 0, 0), DateTime(2025, 4, 18, 11, 30, 0)));
		user6.addTask(Task("Motivational Video Session", "Watch 2 motivational videos on self-discipline and record key points.", LOW, DateTime(2025, 4, 19, 15, 0, 0), DateTime(2025, 4, 19, 15, 30, 0)));
		user6.addTask(Task("Wake-Up Routine Plan", "Design and implement a consistent morning routine to improve energy.", MEDIUM, DateTime(2025, 4, 20, 8, 0, 0), DateTime(2025, 4, 20, 8, 45, 0)));
		user6.addTask(Task("Voice Recorder Practice", "Use voice recording to evaluate speech clarity and pace.", MEDIUM, DateTime(2025, 4, 21, 17, 0, 0), DateTime(2025, 4, 21, 17, 45, 0)));

		// Tasks for user7 - Malik
		user7.addTask(Task("Vito Engine Check", "Inspect Mercedes Vito engine and fluid levels before trip to ensure safety.", MEDIUM, DateTime(2025, 4, 17, 10, 0, 0), DateTime(2025, 4, 17, 11, 0, 0)));
		user7.addTask(Task("Community Assistance", "Support neighbors with loading furniture and appliances into moving trucks.", HIGH, DateTime(2025, 4, 17, 13, 0, 0), DateTime(2025, 4, 17, 14, 0, 0)));
		user7.addTask(Task("Police Coordination Call", "Coordinate with traffic police for temporary road use approval.", MEDIUM, DateTime(2025, 4, 18, 12, 0, 0), DateTime(2025, 4, 18, 12, 30, 0)));
		user7.addTask(Task("Interior Van Clean", "Deep clean vehicle cabin to improve comfort and hygiene for clients.", LOW, DateTime(2025, 4, 19, 16, 0, 0), DateTime(2025, 4, 19, 17, 0, 0)));
		user7.addTask(Task("Transport Schedule Plan", "Create weekly transport schedule and delivery optimization for upcoming requests.", HIGH, DateTime(2025, 4, 20, 19, 0, 0), DateTime(2025, 4, 20, 20, 30, 0)));
		user7.addTask(Task("Review Customer Feedback", "Check feedback from clients and prepare improvement action points.", MEDIUM, DateTime(2025, 4, 21, 15, 0, 0), DateTime(2025, 4, 21, 15, 45, 0)));

		// Tasks for user8 - Tamerlan
		user8.addTask(Task("Train Ride Reflection", "Write a short piece about experience living briefly on a train during summer.", LOW, DateTime(2025, 4, 17, 20, 0, 0), DateTime(2025, 4, 17, 20, 45, 0)));
		user8.addTask(Task("Skip Class Strategy", "Re-evaluate academic priorities and plan how to improve attendance moving forward.", MEDIUM, DateTime(2025, 4, 18, 10, 0, 0), DateTime(2025, 4, 18, 11, 0, 0)));
		user8.addTask(Task("Qəbələ Game Night", "Attend Qəbələ FC match with friends and enjoy stadium atmosphere.", HIGH, DateTime(2025, 4, 19, 18, 0, 0), DateTime(2025, 4, 19, 20, 0, 0)));
		user8.addTask(Task("Football Analysis Thread", "Create social media thread analyzing Qəbələ's tactics in their last match.", MEDIUM, DateTime(2025, 4, 20, 21, 0, 0), DateTime(2025, 4, 20, 22, 0, 0)));
		user8.addTask(Task("Morning Jog", "Jog 2km in the park to improve stamina and start day with energy.", LOW, DateTime(2025, 4, 21, 7, 0, 0), DateTime(2025, 4, 21, 7, 30, 0)));
		user8.addTask(Task("Fix TrainCode Module", "Debug the seat reservation logic inside the custom-made railway system project.", CRITICAL, DateTime(2025, 4, 21, 18, 0, 0), DateTime(2025, 4, 21, 19, 30, 0)));



		_users.push_back(user1);
		_users.push_back(user2);
		_users.push_back(user3);
		_users.push_back(user4);
		_users.push_back(user5);
		_users.push_back(user6);
		_users.push_back(user7);
		_users.push_back(user8);


		saveUsersToFile();
	}

};