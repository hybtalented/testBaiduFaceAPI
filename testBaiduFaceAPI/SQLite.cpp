#include "SQLite.h"
#include<qdebug.h>
static int callback(void *data, int argc, char **argv, char **azColName) {
	Json::Value* sqlite_ret = (Json::Value*) data;
	Json::Value value;
	for (int i =0 ; i<argc; ++i) {
		value[azColName[i]] = argv[i] ? argv[i] : value.null;
	}
	sqlite_ret->append(value);
	
}
SQLite::SQLite(const char* dbpath)
{
	result = sqlite3_open(dbpath, &db);
	if (result!=SQLITE_OK ) {
		qCritical() << QString("Sqlite database %1 open Failed, Error No: %2 ,ERROR MSG: %3").arg(dbpath).arg(result).arg(sqlite3_errmsg(db));
	}
	else {
		qDebug() << QString("Sqlite database %1 open Successfully").arg(dbpath);
	}
}

Json::Value SQLite::exec(const char * cmd)
{
	char* errmsg1 = 0;
	Json::Value sqlite_ret;
	result = sqlite3_exec(db, cmd, callback, (void*) &sqlite_ret,&errmsg1);
	if (result != SQLITE_OK) {
		errmsg = errmsg1;
		qWarning() << QString("Execute \" %1 \" Failed, Error No: %2 ,ERROR MSG: %3").arg(cmd).arg(result).arg(errmsg1);
	}
	else {
		errmsg.clear();
		qDebug() << QString("Query OK!");
	}
	return sqlite_ret;
}


SQLite::~SQLite()
{
}
