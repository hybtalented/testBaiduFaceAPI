#pragma once
#include<sqlite3.h>
#include<json/json.h>
class SQLite
{
public:
	SQLite(const char* dbpath);
	Json::Value exec(const char* cmd);
	int Result() { return result; }
	const char* DB_STATUS() { return sqlite3_errmsg(db); }
	const char* ErrMsg() { return errmsg.c_str(); }
	~SQLite();
private:
	int result;
	std::string errmsg;
	sqlite3 * db;
};

