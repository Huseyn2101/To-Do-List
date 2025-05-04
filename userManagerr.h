#pragma once

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