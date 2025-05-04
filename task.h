#pragma once

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
		auto data = j["priority"].get<int>();
		task._priority = static_cast<Priority>(data);
		task._startTime = DateTime::fromJson(j["startTime"]);
		task._endTime = DateTime::fromJson(j["endTime"]);
		task._completed = j["completed"];

		return task;
	}
};
