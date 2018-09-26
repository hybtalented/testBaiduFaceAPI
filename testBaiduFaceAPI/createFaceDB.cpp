#include "createFaceDB.h"
#include"DBTreeManager.h"
#include<json/json.h>
#include<qdir.h>
#include<qmessagebox.h>
#include<QTime>
#include"mydebug.h"
#include<baidu_face_api.h>
#include<qmutex.h>
#include<qtimer.h>
#include"createFaceDB.h"
createFaceDB::createFaceDB(QString dn, QString g, DBTreeManager * a):ItemThread(a,g),dirname(dn),group(g),db(a),total(0),filenum(0)
{
	setActionText();
	act->setToolTip(QString("Directory:%1 \nGroup:%2").arg(dirname).arg(g));
	QDir dir(dirname);
	for (auto it : dir.entryList(QDir::Filter::AllDirs, QDir::Name)) {
		QString subdirname = dirname + '/' + it;
		QDir subdir(subdirname);
		for (auto it : subdir.entryList(QStringList() << "*.jpg" << "*.png")) {
			++total;
		}
	}
	QThread::start();
}

void createFaceDB::run()
{
	QDir dir(dirname);
	QTime starttime = QTime::currentTime();
	Debug() << u8"��ʼ¼���������ݿ�";
	int person = 0, successfile = 0,fileAlearyIn=0;
	int err =createGroup(group.toStdString().c_str());
	if (err) {
		sendMessage(u8"Ⱥ�鴴��ʧ��", QString("Ⱥ��<font color=green>%1</font>����ʧ�ܣ�ʧ����Ϊ<font color=red>%2</font>").arg(group, err),u8"�˽�");
		return;
	}
	for (auto it : dir.entryList(QDir::Filter::AllDirs, QDir::Name)) {
		QDir persondir = dirname + '/' + it;
		QStringList personpics = persondir.entryList(QStringList() << "*.jpg" << "*.png");
		int picperson = 0;

		//��ʼ��ȡһ���˵�����
		Debug() << QString(u8"    ����¼��%1��%2������").arg(it).arg(personpics.count());
		for (auto pic : personpics) {
			Debug() << QString(u8"        ����¼������%1...").arg(pic);
			int err = loadOneFace(it.toStdString().c_str(), group.toStdString().c_str(), (dirname + '/' + it + '/' + pic).toStdString().c_str(),pic.toStdString().c_str());
			switch (err ) {
			case 0:
				Debug() << QString(u8"        ����%1¼��ɹ�, �ܹ��Ѿ�¼��%2��������").arg(pic).arg(++successfile);
				++picperson;
				break;
			case 1006:
				Debug() << QString(u8"        ����%1¼��ʧ��, �����Ѿ��ڿ��У�").arg(pic);
				++fileAlearyIn;
				break;
			default:
				Debug() << QString(u8"        ����%1¼��ʧ��, ʧ�ܴ��룺%2").arg(pic).arg(err);
				break;
			}
			++filenum;
		}
		if (picperson) {
			Debug() << QString(u8"    %2������¼��ɹ���Ŀǰ�ܹ�¼����%1���˵�������").arg(++person).arg(picperson);
		}
		else {
			Debug() << QString(u8"    ��������������ȡʧ�ܣ�Ŀǰ�ܹ�¼����%1���˵�������").arg(++person).arg(picperson);
		}

	}
	QTime endtime = QTime::currentTime();
	double sec = double(starttime.msecsTo(endtime)) / 1000;
	int min = sec / 60;
	Info() << QString(u8"�������ݿ�¼�����,�ܹ���ʱ%1����%2��").arg(min).arg(sec - 60 * min);
	emit sendMessage(QString("������%1¼�뵽%2���").arg(dirname).arg(group), QString(u8"��ʱ��<font color=red> %1����%2�� </font> ������  <font color=red> %3 </font> ����������  <font color=red> %4 </font> �Ѿ����������е���������  <font color=red> %7���ɹ���ȡ��������  <font color=red> %5 </font> ��ȡ�����ɹ��ʣ�  <font color=red> %6 %</font> ").arg(min).arg(sec - 60 * min).arg(person).arg(filenum).arg(successfile).arg(double(successfile+fileAlearyIn) / filenum * 100).arg(fileAlearyIn), u8"֪����!");
}

