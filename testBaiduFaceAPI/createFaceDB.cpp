#include "createFaceDB.h"
#include<json/json.h>
#include<qdir.h>
#include<qmessagebox.h>
#include<QTime>
#include<qdebug.h>
#include<baidu_face_api.h>
#include<qmutex.h>
#include<qtimer.h>
#include"createFaceDB.h"
#include"DBTreeManager.h"
createFaceDB::createFaceDB(QString dn, QString g, DBTreeManager * a):ItemThread(a,g),dirname(dn),group(g),db(a),total(0),filenum(0)
{
	QThread::start();
	setCount(&filenum, &total);
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

}

void createFaceDB::run()
{
	QDir dir(dirname);
	QTime starttime = QTime::currentTime();
	qDebug() << u8"开始录入人脸数据库";
	int person = 0, successfile = 0,fileAlearyIn=0;
	int err =createGroup(group.toStdString().c_str());
	if (err) {
		sendMessage(u8"群组创建失败", QString("群组<font color=green>%1</font>创建失败，失败码为<font color=red>%2</font>").arg(group, err),u8"了解");
	}
	for (auto it : dir.entryList(QDir::Filter::AllDirs, QDir::Name)) {
		QDir persondir = dirname + '/' + it;
		QStringList personpics = persondir.entryList(QStringList() << "*.jpg" << "*.png");
		int picperson = 0;

		//开始读取一个人的人脸
		qDebug() << QString(u8"    正在录入%1的%2张人脸").arg(it).arg(personpics.count());
		for (auto pic : personpics) {
			qDebug() << QString(u8"        正在录入人脸%1...").arg(pic);
			int err = loadOneFace(it.toStdString().c_str(), group.toStdString().c_str(), (dirname + '/' + it + '/' + pic).toStdString().c_str(),pic.toStdString().c_str());
			switch (err ) {
			case 0:
				qDebug() << QString(u8"        人脸%1录入成功, 总共已经录入%2张人脸！").arg(pic).arg(++successfile);
				++picperson;
				break;
			case 1006:
				qDebug() << QString(u8"        人脸%1录入失败, 人脸已经在库中！").arg(pic);
				++fileAlearyIn;
				break;
			default:
				qDebug() << QString(u8"        人脸%1录入失败, 失败代码：%2").arg(pic).arg(err);
				break;
			}
			++filenum;
		}
		if (picperson) {
			qDebug() << QString(u8"    %2张人脸录入成功！目前总共录入了%1个人的人脸！").arg(++person).arg(picperson);
		}
		else {
			qDebug() << QString(u8"    人脸库里人脸读取失败！目前总共录入了%1个人的人脸！").arg(++person).arg(picperson);
		}

	}
	QTime endtime = QTime::currentTime();
	double sec = double(starttime.msecsTo(endtime)) / 1000;
	int min = sec / 60;
	qDebug() << QString(u8"人脸数据库录入完成,总共耗时%1分钟%2秒").arg(min).arg(sec - 60 * min);
	emit sendMessage(QString("人脸库%1录入到%2完成").arg(dirname,group), QString(u8"耗时：<font color=red> %1分钟%2秒 </font> 人数：  <font color=red> %3 </font> 总人脸数：  <font color=red> %4 </font> 已经在人脸库中的人脸数：  <font color=red> %7，成功读取人脸数：  <font color=red> %5 </font> 读取人脸成功率：  <font color=red> %6 %</font> ").arg(min).arg(sec - 60 * min).arg(person).arg(filenum).arg(successfile).arg(double(successfile+fileAlearyIn) / filenum * 100).arg(fileAlearyIn), u8"知道了!");
}

