#pragma once

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