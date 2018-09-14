#include "FaceServerThread.h"


FaceServer::FaceServer(QObject*parent) :QTcpServer(parent){
	
}
void FaceServer::incomingConnection(qintptr socketDescriptor) {
	FaceSocket *socket = new FaceSocket(socketDescriptor);

	connect(socket, SIGNAL(readyRead()), socket, SLOT(RecvData())); // ���ƽ��߳���
	connect(socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));

	QThread *thread = new QThread(socket); // ��socketΪ���࣬��socket�ͷ�ɾ����Ҳ��ɾ���̣߳����߽��̵߳�quit�źŹ�����deleteLater()Ҳ���ԴﵽЧ��
	connect(socket, SIGNAL(disconnected()), thread, SLOT(quit()));
	socket->moveToThread(thread);
	thread->start();
	emit newConnection();   //�ĵ�Ҫ��̳б�������Ҫ������źţ��˴�û������ź����ӵĲ�
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
		qDebug() << "��������������" << server->errorString();
		sendMessage("����������ʧ��", QString("ʧ�ܴ���Ϊ��Ϣ:<font color=red> %1</color>").arg(server->errorString()), "&ȷ��");
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
   ���ܣ���������
   ״̬��δ���
*/
void FaceSocket::RecvData() {

}
void FaceSocket::SocketErr(QAbstractSocket::SocketError socketError)
{
		qDebug()<< QString("socket[%1] ip[%2] port[%3] err[%4]").arg(socketDescriptor()).arg(
			peerAddress().toString().toLocal8Bit().data()).arg( peerPort())
			.arg( errorString().toLocal8Bit().data());
}
