#pragma once
#include "ItemThread.h"
#include<qtcpserver.h>
#include<qtcpsocket.h>
class QTcpSocket;
class FaceServer;
class DBTreeManager;
class FaceSocket :
	public QTcpSocket
{
	Q_OBJECT
public:
	FaceSocket(qintptr socketDescriptor, DBTreeManager*dbmgr, QObject*parent= nullptr);
	~FaceSocket(){}
public slots:
	void RecvData();
	void SocketErr(QAbstractSocket::SocketError socketError);
private:
	DBTreeManager*mgr;
	qint64 m_recvsize;
};

class FaceServer :
	public QTcpServer
{
	Q_OBJECT
public:
	FaceServer(QObject*parent,DBTreeManager*dbmgr);
	~FaceServer(){
		qDebug() << __FILE__ << __LINE__ << __FUNCTION__ ;
		for (auto socket : socketlist) {

			socketlist.removeAll(socket);
			delete socket;
		}
		socketlist.clear();
		qDebug() << __FILE__ << __LINE__ << __FUNCTION__;
	}
	const QList< FaceSocket*>& socketList() { return socketlist; }
public slots:
	void incomingConnection(qintptr socketDescriptor)override;
	void socketDisConn();
private:
	DBTreeManager*mgr;
	QList< FaceSocket*> socketlist;
};
class FaceServerThread :
	public ItemThread
{
	Q_OBJECT
public:
	enum {max_connection = 10, ListenPort = 8888};
	FaceServerThread(QWidget*parent,const QString& name,DBTreeManager*);
	~FaceServerThread();
	void run()override { while (1)sleep(1000); }
private:
	void setActionText() override {
		act->setText((name + u8"连接数：%1").arg(server->socketList().count()));
		act->setToolTip(act->text()+ QString(u8" 运行时间:(%1h:%2m:%3s)").arg(hour).arg(min).arg(sec));
	}

	FaceServer *server;

};

QTcpSocket* operator<<(QTcpSocket*,const QString&);