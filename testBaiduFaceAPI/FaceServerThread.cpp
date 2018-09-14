#include "FaceServerThread.h"


FaceServer::FaceServer(QObject*parent) :QTcpServer(parent){
	
}
void FaceServer::incomingConnection(qintptr socketDescriptor) {
	FaceSocket *socket = new FaceSocket(socketDescriptor);

	connect(socket, SIGNAL(readyRead()), socket, SLOT(RecvData())); // 会移进线程里
	connect(socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));

	QThread *thread = new QThread(socket); // 以socket为父类，当socket释放删除后也会删除线程，或者将线程的quit信号关联槽deleteLater()也可以达到效果
	connect(socket, SIGNAL(disconnected()), thread, SLOT(quit()));
	socket->moveToThread(thread);
	thread->start();
	emit newConnection();   //文档要求继承本函数需要发射此信号，此处没有与此信号连接的槽
}
FaceServerThread::FaceServerThread(QWidget*parent,const QString &name):ItemThread(parent,name)
{

}


FaceServerThread::~FaceServerThread()
{
}

void FaceServerThread::run()
{
	server = new FaceServer(this);
	if (!server->listen(QHostAddress::Any, ListenPort)) {
		qDebug() << "服务器启动错误：" << server->errorString();
		sendMessage("服务器启动失败", QString("失败代码为信息:<font color=red> %1</color>").arg(server->errorString()), "&确定");
		delete server;
		return;
	}
	connect(server, SIGNAL(disconnected()), this, SLOT(socketDisConn()));
	server->setMaxPendingConnections(10);
	while (1) { sleep(1000); }
}

void FaceServerThread::socketDisConn()
{
}

void FaceServerThread::socketEnded(QTcpSocket *socket)
{
	socketlist.removeOne(socket);
}

FaceSocket::FaceSocket(qintptr socketDescriptor, QObject*parent = nullptr) :QTcpSocket(parent){
	setSocketDescriptor(socketDescriptor);
	connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SocketErr(QAbstractSocket::SocketError)));
}

/*
   功能：接收数据
   状态：未完成
*/
void FaceSocket::RecvData() {

}
void FaceSocket::SocketErr(QAbstractSocket::SocketError socketError)
{
		qDebug()<< QString("socket[%1] ip[%2] port[%3] err[%4]").arg(socketDescriptor()).arg(
			peerAddress().toString().toLocal8Bit().data()).arg( peerPort())
			.arg( errorString().toLocal8Bit().data());
}
