#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_testbaidufaceapi.h"

class testBaiduFaceAPI : public QMainWindow
{
	Q_OBJECT

public:
	testBaiduFaceAPI(QWidget *parent = Q_NULLPTR);

private:
	Ui::testBaiduFaceAPIClass ui;
};
