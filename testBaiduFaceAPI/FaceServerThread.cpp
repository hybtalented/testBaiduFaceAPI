#include <windows.h>
#include "FaceServerThread.h"
#include<qlist.h>
#include<algorithm>
#include"DBTreeManager.h"
#include<json/json.h>
#include"mydebug.h"
#include<qtimer.h>
FaceServer::FaceServer(QObject*parent, DBTreeManager*dbmgr) :QTcpServer(parent),mgr(dbmgr){

}
void FaceServer::incomingConnection(qintptr socketDescriptor) {
	FaceSocket *socket = new FaceSocket(socketDescriptor,mgr);
	connect(socket, SIGNAL(readyRead()), socket, SLOT(RecvData())); // ���ƽ��߳���
	connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisConn()));
	QThread *thread = new QThread(socket); // ��socketΪ���࣬��socket�ͷ�ɾ����Ҳ��ɾ���̣߳����߽��̵߳�quit�źŹ�����deleteLater()Ҳ���ԴﵽЧ��
	connect(socket, SIGNAL(disconnected()), thread, SLOT(quit()));
	socket->moveToThread(thread);
	thread->start();
	socketlist.append(socket);
	emit newConnection();   //�ĵ�Ҫ��̳б�������Ҫ������źţ��˴�û������ź����ӵĲ�
}
FaceServerThread::FaceServerThread(QWidget*parent,const QString &name,DBTreeManager*manager):ItemThread(parent,name)
{
	server = new FaceServer(this,manager);
	if (!server->listen(QHostAddress::Any, ListenPort)) {
		Info() << "��������������" << server->errorString();
		sendMessage("����������ʧ��", QString("ʧ�ܴ���Ϊ��Ϣ:<font color=red> %1</color>").arg(server->errorString()), "&ȷ��");
		delete server;
		return;
	}
}


FaceServerThread::~FaceServerThread()
{
	server->close();
}


void FaceServer::socketDisConn()
{
	auto socket =(FaceSocket*)(sender());
	auto it = std::find(socketlist.begin(), socketlist.end(), socket);
	if (it != socketlist.end()) {
		socketlist.erase(it);
		(*it)->deleteLater();
	}
}


FaceSocket::FaceSocket(qintptr socketDescriptor, DBTreeManager*dgmgr,QObject*parent) :QTcpSocket(parent),mgr(dgmgr), m_recvsize(0){
	setSocketDescriptor(socketDescriptor);
	connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SocketErr(QAbstractSocket::SocketError)));
}

/*
   ���ܣ���������
   ״̬��δ���
*/
void FaceSocket::RecvData() {
	QDataStream recv(this);
	recv.setVersion(recv.Qt_5_9);
	Json::Value value;
	if (m_recvsize == 0) {
		if (bytesAvailable() < sizeof(m_recvsize)) {
			Info() << u8"�޷���ȡ�ļ���С����������С��";
			return;
		}
		recv >> m_recvsize;
	}
	if (m_recvsize > bytesAvailable()) {
		Info() << u8"�޷���ȡ�ļ����ļ�δ��ȫ���뻺������";
		return;
	}
	QString recvstr;
	recv >> recvstr;
	m_recvsize = 0;
	Json::Reader reader;
	reader.parse(recvstr.toStdString(), value);
	if (value.isObject()) {
		if (value["cmd"].isString()) {
			if (value["cmd"].asString() == "compare") {
				if (value["pic1"].isString() && value["pic2"].isString()) {
					auto pic1 = value["pic1"].asString();
					auto pic2 = value["pic2"].asString();
					QString ret=mgr->CompareBase64(pic1.c_str(), pic2.c_str());
					this << ret;
				}
			}
			else if (value["cmd"].asString() == "findindb") {
				if (value["pic1"].isString() && value["group"].isString()) {
					auto pic1 = value["pic1"].asString();
					auto group = value["group"].asString();
					auto user = value["user"].asString();
					QString ret= mgr->CompareWithGroup(pic1.c_str(), group.c_str(), user.c_str());
					this << ret;
				}
			}
			else if (value["cmd"].asString() == "getGroupList") {
				Json::Value root,subroot;
				QStringList grouplist = mgr->ReadGroup();
				subroot.resize(grouplist.count());
				for (auto i = 0; i < grouplist.count();i++) {
					subroot[i] = grouplist[i].toStdString();
				}
				root["grouplist"] = subroot;
				this <<QString(root.toStyledString().c_str());
			}
			else if (value["cmd"].asString() == "getUserList") {
				if (value["group"].isString()) {
					Json::Value root, subroot;
					root["group"] = value["group"].asString();
					QStringList userlist = mgr->ReadPerson(value["group"].asCString());
					subroot.resize(userlist.count());
					for (auto i = 0; i < userlist.count(); i++) {
						subroot[i] = userlist[i].toStdString();
					}
					root["userlist"] = subroot;
					this << QString(root.toStyledString().c_str());
				}
			}
			else if (value["cmd"].asString() == "getUserInfo") {
				if (value["group"].isString()&& value["user"].isString()) {
					std::string userinfo = mgr->ReadUserInfo(value["group"].asCString(), value["user"].asCString());
					this << QString(userinfo.c_str());
				}
			}
			else if (value["cmd"].asString() == "GroupAdd") {
				if (value["group"].isString()) {
					QString userinfo = mgr->AddGroup(value["group"].asCString());
					this << userinfo;
				}
			}
			else if (value["cmd"].asString() == "UserAdd") {
				if (value["group"].isString()) {
					QString userinfo = mgr->AddUser(value["group"].asCString(),value["user"].asCString(),value["pic1"].asCString(),0);
					this << userinfo;
				}
			}
			else if (value["cmd"].asString() == "GroupDel") {
				if (value["group"].isString()) {
					QString userinfo = mgr->deleteGroup(value["group"].asCString());
					this << userinfo;
				}
			}
		}
	}
}

void FaceSocket::SocketErr(QAbstractSocket::SocketError socketError)
{
	Info()<< QString("socket[%1] ip[%2] port[%3] err[%4]").arg(socketDescriptor()).arg(
			peerAddress().toString().toLocal8Bit().data()).arg( peerPort())
			.arg( errorString().toLocal8Bit().data());
}

QTcpSocket* operator<<(QTcpSocket*tcpsocket, const QString&s) {
	QByteArray byte;
	QDataStream out(&byte, QIODevice::WriteOnly);
	out.setVersion(out.Qt_5_9);
	qint64 length = 0;
	out << length << s;
	length = byte.size() - sizeof(length);
	out.device()->seek(0);
	out << length;
    tcpsocket->write(byte);
	return tcpsocket;
}

SyncDataBase::SyncDataBase(DBTreeManager*parent) :ItemThread(parent, u8"���ݿ�ͬ��")
{
}



void SyncDataBase::run()
{
	DBTreeManager * manager = qobject_cast<DBTreeManager *>(parent());
	while (!isInterruptionRequested()) {
		if (!manager||!manager->DBInitialized()) {
			Critical() << "parent is not a DBTreeManager or Database not initialized!";
			return;
		}
		isSync = true;
		manager->synchronization(manager->getLocalDBTimeStamp(), manager->getRemoteDBtimeStamp());
		{
			if (haveStop()) return;
		}
		isSync = false;
		Sleep(15000);
	}
}
