#pragma once
#include<sqlite3.h>
#include<json/json.h>
#include<qobject.h>
#include<qmutex.h>
struct SQLITE_RET {
	SQLITE_RET():rows(0),value(){}
	const char* getCString(int row, const char*col,bool*ok=nullptr);
	int getInt(int row, const char*col, bool*ok = nullptr);
	qint64 getInt64(int row, const char*col, bool*ok = nullptr);
	float getFloat(int row, const char* col, bool*ok = nullptr);
	int rows;
	Json::Value value;
};
class SQLite
{
public:
	SQLite(const char* dbpath);
	SQLITE_RET exec(const char* cmd);
	void lock() {
		mutex.lock();
	}
// return SQLITE_OK when lock success and return SQLITE_BUSY when aleady been locked
	bool tryLock(int timeout) {
		return mutex.tryLock(timeout);
	}
	void unLock() {
		mutex.unlock();
	}
	int Result() { return result; }
	const char* DB_STATUS() { return sqlite3_errmsg(db); }
	const char* ErrMsg() { return errmsg.c_str(); }
	~SQLite();
private:
	QMutex mutex;
	int result;
	std::string errmsg;
	sqlite3 * db;
};
