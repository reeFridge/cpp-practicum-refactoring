template <typename T>
struct Range {
	T min;
	T max;
};

enum class CheckResult {
	SMALL,
	BIG,
	PASSED
};

template <typename T>
CheckResult CheckValueIsInRange(Range<T> range, T value) {
	if (value < range.min) {
		return CheckResult::SMALL;
	}

	if (value > range.max) {
		return CheckResult::BIG;
	}

	return CheckResult::PASSED;
}

template <typename T>
void CheckDateTimeField(Range<T> range, T value, const string& field_name) {
	if (
		const auto res = CheckValueIsInRange(range, value);
		res != CheckResult::PASSED
	) {
		throw domain_error(
			field_name + " is too "s +
			(res == CheckResult::SMALL ? "small"s : "big"s)
		);
	}
}

void CheckYear(const DateTime& dt) {
	CheckDateTimeField({1, 9999}, dt.year, "year"s);
}

void CheckMonth(const DateTime& dt) {
	CheckDateTimeField({1, 12}, dt.month, "month"s);
}

void CheckDay(const DateTime& dt) {
	const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
	const array month_lengths = {31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	CheckDateTimeField({1, month_lengths[dt.month - 1]}, dt.day, "day"s);
}

void CheckHour(const DateTime& dt) {
	CheckDateTimeField({0, 23}, dt.hour, "hour"s);
}

void CheckMinute(const DateTime& dt) {
	CheckDateTimeField({0, 59}, dt.minute, "minute"s);
}

void CheckSecond(const DateTime& dt) {
	CheckDateTimeField({0, 59}, dt.second, "second"s);
}

void CheckDateTimeValidity(const DateTime& dt) {
	CheckYear(dt);
	CheckMonth(dt);
	CheckDay(dt);
	CheckHour(dt);
	CheckMinute(dt);
	CheckSecond(dt);
}
