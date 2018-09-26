#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_testbaidufaceapi.h"
#include <qlist.h>
#include<qmap.h>
#include<json/json.h>

class createFaceDB;
class ItemThreadAction;
class SyncDataBase;
class FaceServerThread;
class testBaiduFaceAPI : public QMainWindow
{
	Q_OBJECT

public:
	testBaiduFaceAPI(QWidget *parent = Q_NULLPTR);
	~testBaiduFaceAPI();
	enum { maxDetectFace = 10 };
	static QPen penlist[maxDetectFace];
	void setCurrentDir(QString dir) {
		ui.filetree->UI().lineEdit->setText(dir);
	}
public slots:
	void recvMessage(QString,QString,QString);
	void FileOpen(); 
	void DirOpen();
	void ImageSave();
	void TextSave();
	void openFaceSetting();
	void saveSetting();
	void readSetting();
	void fileItemClicked(QTreeWidgetItem *item,int column);
	void fileItemClickedDB(QTreeWidgetItem *item, int column);
	void LoadFaceDB();//创建人脸库读取窗口
	void LoadFaceDBProcess(QString dirname, QString group);
	void openFaceRepoDir();
	void groupDelete();
	void updateCurrentPathDB();
	void switchToManageMode(bool manage = true, bool update=true);
	void PathChanged();
private:
	void setBaiduFaceApi();//对百度人脸识别的设置
	void addItem(QTreeWidgetItem*,QString file);
	void addItem(QTreeWidget * filetree, QString file);
	void updateTreeDB(QTreeWidgetItem*,QStringList dbgroup, QStringList person=QStringList());
	void updateCurrentPath();
	SyncDataBase* syncthread;
	FaceServerThread* sthread;
	Setting settings;
	QToolBar* tasksbar;
	void setTools();
	Ui::testBaiduFaceAPIClass ui;
	QGraphicsScene * scene;
	bool setDB;
};
