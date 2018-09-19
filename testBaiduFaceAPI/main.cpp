#include "testbaidufaceapi.h"
#include <QtWidgets/QApplication>
#include <QtDebug>
#include<qfile.h>
#include<qdatetime.h>
QFile outputFile("customMessageLog.txt");
QTextStream textStream(&outputFile);
void customMessageHandler(QtMsgType type,const QMessageLogContext&context, const QString& msg)
{
	QString txtMessage;
	QDateTime time = QDateTime::currentDateTime();
	txtMessage=time.toString(u8"yyyy-mm-dd hh:mi:ss\t");
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
	txtMessage += QString(". In FILE\"%1\", FUNCTION %3, LINE%2").arg(context.file).arg(context.line).arg(context.function);
	textStream << txtMessage << endl;
}

int main(int argc, char *argv[])
{
	outputFile.open(QIODevice::WriteOnly | QIODevice::Append);
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
