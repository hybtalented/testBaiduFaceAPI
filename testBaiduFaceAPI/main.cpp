#include "testbaidufaceapi.h"
#include <QtWidgets/QApplication>
#include <QtDebug>
#include<qfile.h>
#include<qdatetime.h>
#include<qmutex.h>
QFile outputFile("customMessageLog.log");
QTextStream textStream(&outputFile);
void customMessageHandler(QtMsgType type,const QMessageLogContext&context, const QString& msg)
{
	static QMutex mutex;
	QString txtMessage;
	QDateTime time = QDateTime::currentDateTime();
	txtMessage=time.toString(u8"yyyy-MM-dd HH:mm:ss.zzz\t");
	txtMessage += QString("Thread id: %1 ").arg(__threadid());
	switch (type)
	{
	case QtDebugMsg:    //������Ϣ��ʾ
		txtMessage += QString("Debug: %1").arg(msg);
		break;

	case QtWarningMsg:    //һ���warning��ʾ
		txtMessage += QString("Warning: %1").arg(msg);
		break;

	case QtCriticalMsg:    //���ش�����ʾ
		txtMessage += QString("Critical: %1").arg(msg);
		break;

	case QtFatalMsg:    //����������ʾ
		txtMessage = QString("Fatal: %1").arg(msg);
		abort();
	}
	//������������Ϣ��ָ���ļ�
	
	mutex.lock();
	textStream << txtMessage.toUtf8().constData() << endl;
	textStream.flush();
	mutex.unlock();
}

int main(int argc, char *argv[])
{
	outputFile.open(QIODevice::WriteOnly |QIODevice::Text| QIODevice::Append);
	QApplication a(argc, argv);
	qInstallMessageHandler(&customMessageHandler); //ע��MsgHandler�ص�����
	a.setOrganizationName("China Unicom");
	a.setApplicationName("FaceRecognition");
	a.setApplicationVersion("1.0.0");
	testBaiduFaceAPI w;
	w.setWindowState(Qt::WindowMaximized);
	w.show();
	return a.exec();
}
