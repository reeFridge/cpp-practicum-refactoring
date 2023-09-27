struct DBParams {
	string_view name;
	int connection_timeout;
	bool allow_exceptions;
	DBLogLevel log_level;
};

struct IntRange {
	int min;
	int max;
};

struct QueryParams {
	IntRange age;
	string_view name_filter;
};

DBHandler ConnectDB(const DBParams& params) {
	DBConnector connector(params.allow_exceptions, params.log_level);
	DBHandler db;
	if (params.name.starts_with("tmp."s)) {
		db = connector.ConnectTmp(params.name, params.connection_timeout);
	} else {
		db = connector.Connect(params.name, params.connection_timeout);
	}

	return db;
}

string CreateQueryString(const DBHandler& db, QueryParams params) {
	ostringstream query_str;
	query_str << "from Persons "s
		<< "select Name, Age "s
		<< "where Age between "s << params.age.min << " and "s << params.age.max << " "s
		<< "and Name like '%"s << db.Quote(params.name_filter) << "%'"s;

	return query_str.str();
}

vector<Person> LoadPersons(const DBParams& db_params, QueryParams query_params) {
	DBHandler db = ConnectDB(db_params);
	if (!db_params.allow_exceptions && !db.IsOK()) {
		return {};
	}

	DBQuery query(CreateQueryString(db, query_params));

	vector<Person> persons;
	for (auto [name, age] : db.LoadRows<string, int>(query)) {
		persons.push_back({move(name), age});
	}

	return persons;
}
