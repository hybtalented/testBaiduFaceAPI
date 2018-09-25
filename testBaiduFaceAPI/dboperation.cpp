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
		QMessageBox::warning(this, u8"Ŀ¼������", QString("Ŀ¼��<font color = blue> %1 </font> �����ڣ��������ȡʧ�ܣ�").arg(dirname), u8"ȷ��");
		return;
	}
	switch (QMessageBox::information(this, u8"������ʼ��ȡ������", QString(u8"����Ҫ��ʼ��ȡ������<a href= \"%1\"> %1 </a>�����ݿ��Ⱥ/��<font color=red> %2!</font>").arg(dirname).arg(group)
		, u8"&����?", u8"ȡ����")) {
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
