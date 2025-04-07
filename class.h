#pragma once



#pragma warning(disable : 4996)

#pragma region Enum

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

#pragma endregion

#pragma region Task

class Task {
private:
	string _name;
	string _description;
	Priority _priority;
	tm _startdatetime;
	tm _enddatetime;

	string getPriorityString(Priority priority) const {
		switch (priority) {
		case LOW: return "Low";
		case MEDIUM: return "Medium";
		case HIGH: return "High";
		case CRITICAL: return "Critical";
		case URGENT: return "Urgent";
		default: return "Unknown";
		}
	}

public:
	Task() :_priority(LOW), _startdatetime({ 0 }), _enddatetime({ 0 }) {}

	Task(const string& name, const string& description, Priority priority_level, const tm& startdatetime, const tm& enddatetime) :
		_name(name), _description(description), _priority(priority_level), _startdatetime(startdatetime), _enddatetime(enddatetime) {
	}

	void display() const {
		cout << "\v~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << "Task Details:" << endl;
		cout << "Task Name: " << _name << endl;
		cout << "Description: " << _description << endl;
		cout << "Priority: " << _priority << endl;
		cout << "Start Date and Time: " << asctime(&_startdatetime);
		cout << "End Date and Time: " << asctime(&_enddatetime);
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	}

	void setName(const string& name) { _name = name; }
	void setDescription(const string& description) { _description = description; }
	void setPriority(Priority priority) { _priority = priority; }
	void setStartDateTime(const tm& startdatetime) { _startdatetime = startdatetime; }
	void setEndDateTime(const tm& enddatetime) { _enddatetime = enddatetime; }
	string getName() const { return _name; }
	string getDescription() const { return _description; }
	Priority getPriority() const { return _priority; }
	tm getStartDateTime() const { return _startdatetime; }
	tm getEndDateTime() const { return _enddatetime; }
};

#pragma endregion


#pragma region ToDoList

class ToDoList {
private:
	static const int MAX_TASKS = 100;
	Task _tasks[MAX_TASKS];
	int _taskCount;

	bool isleapyear(int year) const {
		return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
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

	tm getDateTime(const string& type) const {
		tm dateTime = {};
		int year, month, date;

		while (true)
		{
			cout << "Enter" << type << "year: ";
			cin >> year;
			if (cin.fail() || year < 1900 || year > 2100) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cerr << "Invalid year. Please enter a year between 1900 and 2100." << endl;
				continue;
			}
			dateTime.tm_year = year;
			break;
		}
		while (true)
		{
			cout << "Enter" << type << " month: ";
			cin >> month;
			if (cin.fail() || month < 1 || month > 12) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cerr << "Invalid month. Please enter a month between 1 and 12." << endl;
				continue;
			}
			dateTime.tm_mon = month;
			break;
		}
		while (true)
		{
			int max_days = getdaysinmonth(year, month);
			cout << "Enter" << type << " day (1 - " << max_days << "): ";
			cin >> date;
			if (cin.fail() || date < 0 || date > max_days)
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cerr << "Invalid day. Please enter day between 1 and " << max_days << "." << endl;
				continue;
			}
			break;
		}
		dateTime.tm_mday = date;

		while (true)
		{
			cout << "Enter" << type << " the hour: ";
			cin >> dateTime.tm_hour;
			if (cin.fail() || dateTime.tm_hour < 0 || dateTime.tm_hour > 23)
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cerr << "Invalid hour. Please enter a hour between 0 and 59." << endl;
				continue;
			}
			break;
		}

		while (true) {
			cout << "Enter" << type << " minute: ";
			cin >> dateTime.tm_min;
			if (cin.fail() || dateTime.tm_min < 0 || dateTime.tm_min > 59) {
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				cerr << "invalid minute. please enter a minute between 0 and 59.\n";
				continue;
			}
			break;
		}

		while (true) {
			cout << "enter " << type << " second: ";
			cin >> dateTime.tm_sec;
			if (cin.fail() || dateTime.tm_sec < 0 || dateTime.tm_sec > 59) {
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				cerr << "invalid second. please enter a second between 0 and 59.\n";
				continue;
			}
			break;
		}

		cin.ignore();
		return dateTime;
	}


public:
	ToDoList() :_taskCount(0) {}

	void addTask(const Task& task) {
		if (_taskCount < MAX_TASKS) {
			_tasks[_taskCount++] = task;
		}
		else {
			cout << "Task list is full!" << endl;
		}
	}

	void addTask(const string& name, const string& description, Priority priority_level, const tm& startdatetime, const tm& enddatetime) {
		if (_taskCount < MAX_TASKS) {
			_tasks[_taskCount++] = Task(name, description, priority_level, startdatetime, enddatetime);
		}
		else {
			cout << "Task list is full!" << endl;
		}
	}

	void addTask() {
		if (_taskCount < MAX_TASKS) {
			Task task;
			string name, description;
			int priority;

			cout << "Enter task name: ";
			cin.ignore();
			getline(cin, name);
			task.setName(name);

			cout << "Enter task description: ";
			getline(cin, description);
			task.setDescription(description);

			cout << "Enter task priority (0: LOW, 1: MEDIUM, 2: HIGH, 3: CRITICAL, 4: URGENT): ";
			cin >> priority;
			task.setPriority(static_cast<Priority>(priority));

			task.setStartDateTime(getDateTime(" start "));
			task.setEndDateTime(getDateTime(" end "));

			_tasks[_taskCount++] = task;
		}
		else {
			cerr << "Task list is full!" << endl;
		}
	}

	void removeTask(size_t index) {
		if (index < _taskCount) {
			for (size_t i = index; i < _taskCount - 1; ++i) {
				_tasks[i] = _tasks[i + 1];
			}
			--_taskCount;
		}
		else {
			cout << "Invalid task index!" << endl;
		}
	}

	void removeTask(const string& name) {
		for (size_t i = 0; i < _taskCount; ++i) {
			if (_tasks[i].getName() == name) {
				removeTask(i);
				return;
			}
		}
		cout << "Task not found!" << endl;
	}

	void removeTask(const Task& task) {
		for (size_t i = 0; i < _taskCount; ++i) {
			if (_tasks[i].getName() == task.getName()) {
				removeTask(i);
				return;
			}
		}
		cout << "Task not found!" << endl;
	}

	void allTasks() const {
		if (_taskCount == 0) {
			cout << "No tasks available!" << endl;
			return;
		}
		for (size_t i = 0; i < _taskCount; ++i) {
			_tasks[i].display();
		}
	}

	void findTaskByName(const string& name) const {
		for (size_t i = 0; i < _taskCount; ++i) {
			if (_tasks[i].getName() == name) {
				cout << "\vTask found!" << endl;
				_tasks[i].display();
				return;
			}
		}
		cout << "Task not found!" << endl;
	}

	void findTaskByPriority(Priority priority) const {
		bool found = false;
		for (size_t i = 0; i < _taskCount; ++i) {
			if (_tasks[i].getPriority() == priority) {
				cout << "\vTask found!" << endl;
				_tasks[i].display();
				found = true;
			}
		}
		if (!found) {
			cout << "No tasks found with the specified priority!" << endl;
		}
	}

	void findByDescription(const string& description) const {
		bool found = false;
		for (size_t i = 0; i < _taskCount; ++i) {
			if (_tasks[i].getDescription() == description) {
				cout << "\vTask found!" << endl;
				_tasks[i].display();
				found = true;
			}
		}
		if (!found) {
			cout << "No tasks found with the specified description!" << endl;
		}
	}

	void findTaskByDate(const tm& date) const {
		bool found = false;
		for (size_t i = 0; i < _taskCount; ++i) {
			if (_tasks[i].getStartDateTime().tm_year == date.tm_year &&
				_tasks[i].getStartDateTime().tm_mon == date.tm_mon &&
				_tasks[i].getStartDateTime().tm_mday == date.tm_mday) {
				cout << "\vTask found!" << endl;
				_tasks[i].display();
				found = true;
			}
		}
		if (!found) {
			cout << "No tasks found on the specified date!" << endl;
		}
	}

	void tasksForDay()const {
		for (size_t i = 0; i < _taskCount; ++i) {
			if (_tasks[i].getEndDateTime().tm_mday - _tasks[i].getStartDateTime().tm_mday == 1) {
				cout << "\vTask found!" << endl;
				_tasks[i].display();
			}
			else {
				cout << "No tasks found in the specified day!" << endl;
			}
		}
	}

	void tasksForWeek(const tm& date) const {
		bool found = false;
		for (size_t i = 0; i < _taskCount; ++i) {
			if (_tasks[i].getEndDateTime().tm_mday - _tasks[i].getStartDateTime().tm_mday <= 7) {
				cout << "\vTask found!" << endl;
				_tasks[i].display();
				found = true;
			}
		}
		if (!found) {
			cout << "No tasks found in the specified week!" << endl;
		}
	}

	void tasksForMonth() const {
		for (size_t i = 0; i < _taskCount; i++)
		{
			if (_tasks[i].getEndDateTime().tm_mday - _tasks[i].getStartDateTime().tm_mday <= 31)
			{
				cout << "\vTask found!" << endl;
				_tasks[i].display();
			}
			else
			{
				cout << "No tasks found in the specified month!" << endl;
			}
		}
	}

	void sortByPriority() {
		for (size_t i = 0; i < _taskCount - 1; ++i) {
			for (size_t j = 0; j < _taskCount - i - 1; ++j) {
				if (_tasks[j].getPriority() > _tasks[j + 1].getPriority()) {
					swap(_tasks[j], _tasks[j + 1]);
				}
			}
		}
	}

	void sortByDate() {
		for (size_t i = 0; i < _taskCount - 1; ++i) {
			for (size_t j = 0; j < _taskCount - i - 1; ++j) {
				if (difftime(mktime(&_tasks[j].getStartDateTime()), mktime(&_tasks[j + 1].getStartDateTime())) > 0) {
					swap(_tasks[j], _tasks[j + 1]);
				}
			}
		}
	}

	void updateTaskByIndex(size_t index) {
		for (size_t i = 0; i < _taskCount; i++)
		{
			if (i = index)
			{
				string name, description;
				int priority;
				cout << "Enter new task name (current: " << _tasks[i].getName() << ')';
				cin.ignore();
				getline(cin, name);
				_tasks[i].setName(name);
				cout << "Enter new task description (current: " << _tasks[i].getDescription() << ')';
				getline(cin, description);
				_tasks[i].setDescription(description);
				cout << "Enter new task priority (0: LOW, 1: MEDIUM, 2: HIGH, 3: CRITICAL, 4: URGENT) (current: " << _tasks[i].getPriority() << ')';
				cin >> priority;
				_tasks[i].setPriority(static_cast<Priority>(priority));
				_tasks[i].setStartDateTime(getDateTime(" start "));
				_tasks[i].setEndDateTime(getDateTime(" end "));
			}
			else
			{
				cout << "Task not found!" << endl;
			}
		}
	}

	void updateTaskByName(string& name) {
		for (size_t i = 0; i < _taskCount; i++)
		{
			string _name = _tasks[i].getName();
			toLowerCase(_name);
			toLowerCase(name);
			if (_name == name)
			{
				string name, description;
				int priority;
				cout << "Enter new task name (current: " << _tasks[i].getName() << ')';
				cin.ignore();
				getline(cin, name);
				_tasks[i].setName(name);
				cout << "Enter new task description (current: " << _tasks[i].getDescription() << ')';
				getline(cin, description);
				_tasks[i].setDescription(description);
				cout << "Enter new task priority (0: LOW, 1: MEDIUM, 2: HIGH, 3: CRITICAL, 4: URGENT) (current: " << _tasks[i].getPriority() << ')';
				cin >> priority;
				_tasks[i].setPriority(static_cast<Priority>(priority));
				_tasks[i].setStartDateTime(getDateTime(" start "));
				_tasks[i].setEndDateTime(getDateTime(" end "));
			}
			else
			{
				cout << "Task not found!" << endl;
			}
		}
	}

	void updateTaskByDescription(string& description) {
		for (size_t i = 0; i < _taskCount; i++)
		{
			string _description = _tasks[i].getDescription();
			toLowerCase(_description);
			toLowerCase(description);
			if (_description == description)
			{
				string name, description;
				int priority;
				cout << "Enter new task name (current: " << _tasks[i].getName() << ')';
				cin.ignore();
				getline(cin, name);
				_tasks[i].setName(name);
				cout << "Enter new task description (current: " << _tasks[i].getDescription() << ')';
				getline(cin, description);
				_tasks[i].setDescription(description);
				cout << "Enter new task priority (0: LOW, 1: MEDIUM, 2: HIGH, 3: CRITICAL, 4: URGENT) (current: " << _tasks[i].getPriority() << ')';
				cin >> priority;
				_tasks[i].setPriority(static_cast<Priority>(priority));
				_tasks[i].setStartDateTime(getDateTime(" start "));
				_tasks[i].setEndDateTime(getDateTime(" end "));
			}
			else
			{
				cout << "Task not found!" << endl;
			}
		}
	}











};


#pragma endregion