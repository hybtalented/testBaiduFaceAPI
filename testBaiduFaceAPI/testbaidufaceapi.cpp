#include "testbaidufaceapi.h"
#include<baidu_face_api.h>
#include<qmessagebox.h>
#include<qgraphicsitem.h>
#include<qsettings.h>
#include"mydebug.h"
#include<qdir.h>
#include"FaceServerThread.h"
QPen testBaiduFaceAPI::penlist[testBaiduFaceAPI::maxDetectFace] = {
	QPen(QColor(0,0,0),3,Qt::SolidLine),
	QPen(QColor(200,0,0),3,Qt::SolidLine) ,
	QPen(QColor(0,200,0),3,Qt::SolidLine),
	QPen(QColor(0,0,200),3,Qt::SolidLine),
	QPen(QColor(200,0,200),3,Qt::SolidLine) ,
	QPen(QColor(200,100,0),3,Qt::SolidLine),
	QPen(QColor(0,100,200),3,Qt::SolidLine),
	QPen(QColor(100,100,100),3,Qt::SolidLine),
	QPen(QColor(100,50,200),3,Qt::SolidLine),
	QPen(QColor(100,200,50),3,Qt::SolidLine) };
testBaiduFaceAPI::testBaiduFaceAPI(QWidget *parent)
	: QMainWindow(parent),setDB(true)// setDB(true)，switchToManageMode(false);一起用保证一次状态切换
{
	ui.setupUi(this);
	QSettings setting("setting.ini", QSettings::Format::IniFormat, this);
	ui.filetree->UI().lineEdit->setText(setting.value("current path", QVariant(".")).toString());
	setTools();
	readSetting();
	ui.filetree->setSetting(settings);
	bool authed = ui.filetree->BaiduApi()->is_auth();
	if (authed) {
		ui.statusBar->showMessage(u8"百度离线人脸识别库，鉴权成功！",3000);
	}
	else {
		QMessageBox::critical(this, u8"百度离线鉴权失败", u8"请检查是否存在鉴权文件<font color=red> license.ini</font>,<font color=red> license.key</font> 或者鉴权是否过期？\n详见 <a href=\"http://ai.baidu.com/docs#/Face-Offline-SDK-Activation/top\"> 百度人脸识别 </a> 文档。");
		exit(0);
	}
	scene = new QGraphicsScene(ui.pictureView);
	ui.pictureView->setScene(scene);
	connect(ui.actionexit, SIGNAL(triggered()), qApp, SLOT(quit()));
	tasksbar = new QToolBar(u8"正在进行的任务", statusBar());
	statusBar()->addWidget(tasksbar);
	tasksbar->show();
	switchToManageMode(false);
	sthread = new FaceServerThread(this, u8"服务器", ui.filetree);
	sthread->Start();
	tasksbar->addAction(sthread->getAction());
	connect(sthread, SIGNAL(sendMessage(QString, QString, QString)), this, SLOT(recvMessage(QString, QString, QString)));
	syncthread = new SyncDataBase(ui.filetree);
	syncthread->Start();
	tasksbar->addAction(syncthread->getAction());
	connect(syncthread, SIGNAL(sendMessage(QString, QString, QString)), this, SLOT(recvMessage(QString, QString, QString)));
}
testBaiduFaceAPI::~testBaiduFaceAPI() {
	QSettings setting("setting.ini", QSettings::Format::IniFormat, this);
	setting.setValue("current path", QVariant(ui.filetree->UI().lineEdit->text())) ;
	sthread->requestInterruption();
	Debug() << " Interrupte the face server thread";
	Debug()<<"Wait for face server thread end:"<<sthread->wait();
	delete sthread;
	syncthread->requestInterruption();
	Debug() << " Interrupte the sync thread";
	Debug() << "Wait for face server thread end:" << syncthread->wait();
	delete syncthread;
}
void testBaiduFaceAPI::setTools() {
	QActionGroup *lockgroup=new QActionGroup(this);
	lockgroup->setExclusive(true);
	lockgroup->addAction(ui.actionlock);
	lockgroup->addAction(ui.actionunlock);
	QToolBar * toolbar = ui.mainToolBar;
	toolbar->addAction(ui.actionOpen_File);
	toolbar->addAction(ui.actionOpenDir);
	toolbar->addAction(ui.actionsaveAllImages);
	toolbar->addAction(ui.actionsaveAllText);
	toolbar->addSeparator();
	toolbar->addAction(ui.actionFaceSetting);
	toolbar->addAction(ui.actiondefault_setting);
	toolbar->addAction(ui.actionsave_setting);
	toolbar->addSeparator();
	toolbar->addAction(ui.actionnew_repo);
	toolbar->addAction(ui.actionopen_test);
	toolbar->addAction(ui.actionmanage);
	toolbar->addAction(ui.actionlock);
	toolbar->addAction(ui.actionunlock);
	toolbar->addAction(ui.actioncloseDB);
	toolbar->addSeparator();
	toolbar->addAction(ui.actiongroupadd);
	toolbar->addAction(ui.actiongroupdel);
	toolbar->addSeparator();
	toolbar->addAction(ui.actionuseradd);
	toolbar->addAction(ui.actionuserdel);
}

void testBaiduFaceAPI::fileItemClicked(QTreeWidgetItem *item,int column) {
	BaiduFaceApi *baiduApi =ui.filetree->BaiduApi();
	QString filename = item->text(0);
	if (item->childCount() != 0)return;
	scene->clear();
	QString imagepath = ui.filetree->UI().lineEdit->text() + "/" + filename;
	QString res_attr = baiduApi->face_attr(imagepath.toStdString().c_str(), 2);
	QString res_qual = baiduApi->face_quality(imagepath.toStdString().c_str(), 2);
	std::vector<TrackFaceInfo> *faceinfos = new std::vector<TrackFaceInfo>();
	QPixmap image(imagepath);
	scene->addPixmap(image);  //显示图片
/*	人脸信息
struct FaceInfo
	{
		float mWidth;     // rectangle width
		float mAngle;     // rectangle tilt angle [-45 45] in degrees
		float mCenter_y;  // rectangle center y
		float mCenter_x;  // rectangle center x
		float mConf;
	};
	struct TrackFaceInfo
	{
		TrackFaceInfo() {}

		FaceInfo box;
		std::vector<int> landmarks;
		float score;
		unsigned long face_id;
		std::vector<float> headPose;
	};
*/  
	int facenum=0;
	facenum = baiduApi->track_max_face(faceinfos, imagepath.toStdString().c_str(), 2);
	QString message;
	message = QString(u8"人脸性质") + res_attr + '\n' + QString(u8"人脸质量") + res_qual;
	message += QString(u8"总共有%1个人脸：").arg(facenum);
	for (int i = 0; i < faceinfos->size(); ++i) {
		TrackFaceInfo faceinfo = faceinfos->at(i);
		FaceInfo box = faceinfo.box;
		message += QString("{\n\t width:%1\n\tAngle:%2\n\tCenter_y:%3\n\tCenter_x:%4\n\tConf:%5\n}").arg(box.mWidth).arg(box.mAngle).arg(box.mCenter_y).arg(box.mCenter_x).arg(box.mConf);
		QGraphicsRectItem* rect = scene->addRect(QRectF(box.mCenter_x - box.mWidth / 2, box.mCenter_y - box.mWidth / 2, box.mWidth, box.mWidth), penlist[i]);
		rect->setRotation(faceinfo.headPose[2]);
	}
	delete faceinfos;
	ui.FaceDetail->setText(message);
	scene->setSceneRect(image.rect());
	ui.pictureView->adjustSize();
	ui.pictureView->show();
}

void testBaiduFaceAPI::fileItemClickedDB(QTreeWidgetItem * item, int column)
{

}

void testBaiduFaceAPI::updateCurrentPath() {
	disconnect(ui.filetree->UI().filetree, SIGNAL(itemPressed(QTreeWidgetItem *, int)), this, SLOT(fileItemClicked(QTreeWidgetItem *, int)));
	ui.filetree->UI().filetree->clear();
	for (auto file : QDir(ui.filetree->UI().lineEdit->text()).entryList(QStringList() << "*.jpg" << "*.png", QDir::Filters::enum_type::Files, QDir::SortFlags::enum_type::Name)) {
		addItem(ui.filetree->UI().filetree, ui.filetree->UI().lineEdit->text() + '/' + file);
	}
	connect(ui.filetree->UI().filetree, SIGNAL(itemPressed(QTreeWidgetItem *, int)), this, SLOT(fileItemClicked(QTreeWidgetItem *, int)));
}

void testBaiduFaceAPI::switchToManageMode(bool manage,bool updatewindow)
{
	Debug() << "setDB is " << setDB<<"manage is "<<manage;
	if (manage) {
		if (!setDB) {
			setDB = true;
			disconnect(ui.filetree->UI().filetree, SIGNAL(itemPressed(QTreeWidgetItem *, int)), this, SLOT(fileItemClickedDB(QTreeWidgetItem *, int)));
			ui.actionFaceSetting->setEnabled(false);
			ui.actiondefault_setting->setEnabled(false);
			ui.actionsave_setting->setEnabled(false);
			ui.actioncloseDB->setEnabled(true);
			ui.actionlock->setEnabled(true);
			ui.actionunlock->setEnabled(true);
			ui.actionuseradd->setEnabled(true);
			ui.actionuserdel->setEnabled(true);
			ui.actiongroupadd->setEnabled(true);
			ui.actiongroupdel->setEnabled(true);   
			ui.filetree->UI().label_2->show();
			ui.filetree->UI().label_3->show();
			ui.filetree->UI().lineEdit_2->show();
			ui.filetree->UI().lineEdit_3->show();
			ui.filetree->UI().pushButton_2->show();
			connect(ui.filetree->UI().filetree, SIGNAL(itemPressed(QTreeWidgetItem *, int)), this, SLOT(fileItemClicked(QTreeWidgetItem *, int)));
		}
	    if(updatewindow)
		    updateCurrentPathDB();
	}
	else {
		if (setDB) {
			disconnect(ui.filetree->UI().filetree, SIGNAL(itemPressed(QTreeWidgetItem *, int)), this, SLOT(fileItemClicked(QTreeWidgetItem *, int)));
			setDB = false;
			ui.actionFaceSetting->setEnabled(true);
			ui.actiondefault_setting->setEnabled(true);
			ui.actionsave_setting->setEnabled(true);
			ui.actioncloseDB->setEnabled(false);
			ui.actionlock->setEnabled(false);
			ui.actionunlock->setEnabled(false);
			ui.actionuseradd->setEnabled(false);
			ui.actionuserdel->setEnabled(false);
			ui.actiongroupadd->setEnabled(false);
			ui.actiongroupdel->setEnabled(false);
			ui.filetree->UI().label_2->hide();
			ui.filetree->UI().label_3->hide();
			ui.filetree->UI().lineEdit_2->hide();
			ui.filetree->UI().lineEdit_3->hide();
			ui.filetree->UI().pushButton_2->hide();
			connect(ui.filetree->UI().filetree, SIGNAL(itemPressed(QTreeWidgetItem *, int)), this, SLOT(fileItemClickedDB(QTreeWidgetItem *, int)));
		}
		if(updatewindow)
		updateCurrentPath();
	}
}
void testBaiduFaceAPI::PathChanged()
{
	switchToManageMode(setDB);
}
/*  testBaiduFaceAPI::updateCurrentPathDB
  功能： 更新当前路径和文件列表
  完成情况； 未测试
*/
void testBaiduFaceAPI::updateCurrentPathDB()
{
	
}

void testBaiduFaceAPI::updateTreeDB(QTreeWidgetItem*,QStringList dbgroup, QStringList person ) {

}
void testBaiduFaceAPI::groupDelete() {

}