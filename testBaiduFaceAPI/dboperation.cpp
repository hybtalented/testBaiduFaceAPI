#include"testbaidufaceapi.h"
#include"createFaceDB.h"
#include"loadFaceDB.h"
#include"mydebug.h"
#include<qdir.h>
#include<qmessagebox.h>
#include<json/json.h>




int createFaceDB::loadOneFace(const char * user, const char * group, const char * filepath, const char * filename)
{
	return db->createFace(user, group, filepath, filename);
}
int createFaceDB::createGroup(const char * groupname)
{
	return db->createGroup(groupname);
}
void testBaiduFaceAPI::LoadFaceDB() {
	loadFaceDB *win = new loadFaceDB(this, ui.filetree->UI().lineEdit->text());
	win->setModal(false);
	win->show();
	connect(win, SIGNAL(sendResult(QString, QString)), this, SLOT(LoadFaceDBProcess(QString, QString)));
}
void testBaiduFaceAPI::LoadFaceDBProcess(QString dirname, QString group)
{
	QDir dir(dirname);
	if (!dir.exists()) {
		QMessageBox::warning(this, u8"目录不存在", QString("目录：<font color = blue> %1 </font> 不存在，人脸库读取失败！").arg(dirname), u8"确定");
		return;
	}
	switch (QMessageBox::information(this, u8"即将开始读取人脸库", QString(u8"现在要开始读取人脸库<a href= \"%1\"> %1 </a>到数据库的群/组<font color=red> %2!</font>").arg(dirname).arg(group)
		, u8"&继续?", u8"取消。")) {
	case 0:
		break;
	default:
		return;
	}
	createFaceDB * cdb = new createFaceDB(dirname, group, ui.filetree);
	tasksbar->addAction(cdb->getAction());
	connect(cdb, SIGNAL(sendMessage(QString, QString, QString)), this, SLOT(recvMessage(QString, QString, QString)));
	connect(this, SIGNAL(destroyed()), cdb, SLOT(terminate()));
	connect(cdb, SIGNAL(finished()), cdb, SLOT(deleteLater()));
}
void testBaiduFaceAPI::recvMessage(QString title, QString msg, QString button) {
	QMessageBox::information(this, title, msg, button);
}
