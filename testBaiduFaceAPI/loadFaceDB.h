#pragma once
#include <qdialog.h>
#include"ui_loadFaceDB.h"
class loadFaceDB :
	public QDialog
{
	Q_OBJECT
public:
	loadFaceDB(QWidget* parent,QString dir);
	~loadFaceDB();
private slots:
	void openDir();
	void accept()override;
signals:
	void sendResult(QString, QString);
private:
	QString dir_default;
	Ui::Dialog ui;
};

