#pragma once
#include"ItemThread.h"
#include<qdialog.h>
class DBTreeManager;
class QTimer;
class createFaceDB;

class createFaceDB :
	public ItemThread
{
	Q_OBJECT
public:
	createFaceDB(QString dn,QString g, DBTreeManager* a);
	const QString& DirName() { return dirname; }
	const QString& Group() { return group; }
	~createFaceDB(){}
	void run()override;
	int createGroup(const char*groupname);
private:
	int loadOneFace(const char*user, const char* group, const char* filepath, const char* filename);
	QString dirname, group;
	DBTreeManager * db;
	int total, filenum;
};

