#pragma once

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