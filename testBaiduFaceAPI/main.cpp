#include "testbaidufaceapi.h"
#include <QtWidgets/QApplication>




int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setOrganizationName("China Unicom");
	a.setApplicationName("FaceRecognition");
	a.setApplicationVersion("1.0.0");
	testBaiduFaceAPI w;
	w.setWindowState(Qt::WindowMaximized);
	w.show();
	return a.exec();
}
