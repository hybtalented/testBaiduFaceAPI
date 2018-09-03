#include "testbaidufaceapi.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	testBaiduFaceAPI w;
	w.show();
	return a.exec();
}
