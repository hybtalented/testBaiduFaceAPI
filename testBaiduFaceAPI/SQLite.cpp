#include "SQLite.h"
#include"mydebug.h"
#include<qstring.h>
static int callback(void *data, int argc, char **argv, char **azColName) {
	Json::Value value;
	SQLITE_RET* result = (SQLITE_RET*) data;
	result->rows++;
	Debug() << QString("recieve %1 columns:").arg(argc);
	for (int i =0 ; i<argc; ++i) {
		value[azColName[i]] = argv[i] ? argv[i] : value.null;
		Debug() << QString("%1 = %2").arg(azColName[i]).arg(value[azColName[i]].asCString());
	}
	result->value.append(value);
	return 0;
}
SQLite::SQLite(const char* dbpath)
{
	result = sqlite3_open(dbpath, &db);
	if (result!=SQLITE_OK ) {
		Critical() << QString("Sqlite database %1 open Failed, Error No: %2 ,ERROR MSG: %3").arg(dbpath).arg(result).arg(sqlite3_errmsg(db));
	}
	else {
		Debug() << QString("Sqlite database %1 open Successfully").arg(dbpath);
	}
}

SQLITE_RET SQLite::exec(const char * cmd)
{
	char* errmsg1 = 0;
	SQLITE_RET ret;
	result = sqlite3_exec(db, cmd, callback, &ret,&errmsg1);
	if (result != SQLITE_OK) {
		errmsg = errmsg1;
		Warning() << QString("Execute \" %1 \" Failed, Error No: %2 ,ERROR MSG: %3").arg(cmd).arg(result).arg(errmsg1);
		sqlite3_free(errmsg1);
	}
	else {
		errmsg.clear();
		Debug() << QString("Query OK! %1 Rows recieve").arg( ret.rows);
	}
	return ret;
}


SQLite::~SQLite()
{
}

const char * SQLITE_RET::getCString(int row, const char * col,bool*ok)
{
	if (row >= 0 && row < rows) {
		if (value[row][col].isString()) {
			if (ok) {
				*ok = true;
			}
			return  value[row][col].asCString();
		}
	}
	if (ok) {
		*ok = false;
	}
	return nullptr;
}

int SQLITE_RET::getInt(int row, const char * col, bool*ok)
{
	if (row >= 0 && row < rows) {
		 QString s=value[row][col].asCString();
		 return s.toInt(ok);
	}
	Critical() << "the row " << row << " Out of range, total rows is " << rows;
	if (ok) {
		*ok = false;
	}
	return -1;
}

qint64 SQLITE_RET::getInt64(int row, const char * col, bool * ok)
{
	if (row >= 0 && row < rows) {
		QString s = value[row][col].asCString();
		return s.toLongLong();
	}
	Critical() << "the row " << row << " Out of range, total rows is " << rows;
	if (ok) {
		*ok = false;
	}
	return -1;
}

float SQLITE_RET::getFloat(int row, const char * col, bool*ok)
{
	if (row >= 0 && row < rows) {
		QString s = value[row][col].asCString();
		return s.toFloat(ok);
	}
	Critical() << "the row " << row << " Out of range, total rows is " << rows;
	if (ok) {
		*ok = false;
	}
	return 0.0f;
}
