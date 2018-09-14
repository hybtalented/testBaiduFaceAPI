#pragma once
#include "ItemThread.h"
#include<qtcpserver.h>
#include<qtcpsocket.h>
class QTcpSocket;
class FaceServer;
class FaceSocket :
	public QTcpSocket
{
	Q_OBJECT
public:
	FaceSocket(qintptr socketDescriptor, QObject*parent = nullptr);
	~FaceSocket(){}
public slots:
	void RecvData();
	void SocketErr(QAbstractSocket::SocketError socketError);
};

class FaceServer :
	public QTcpServer
{
	Q_OBJECT
public:
	FaceServer(QObject*parent);
	~FaceServer(){}
public slots:
	void incomingConnection(qintptr socketDescriptor)override;
signals:
	void disconnected();
};
class FaceServerThread :
	public ItemThread
{
	Q_OBJECT
public:
	enum {max_connection = 10, ListenPort = 8888};
	FaceServerThread(QWidget*parent,const QString& name);
	~FaceServerThread();
	void run()override;
public slots:
	void newConnection();
	void socketDisConn();
     void socketEnded(QTcpSocket*);
private:
	FaceServer *server;
	QList< QTcpSocket*> socketlist;
};

