#pragma once

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