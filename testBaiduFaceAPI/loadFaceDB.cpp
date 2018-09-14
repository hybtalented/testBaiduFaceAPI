#include "loadFaceDB.h"
#include <qmessagebox.h>
#include <qfiledialog.h>
#include "testbaidufaceapi.h"
#include"createFaceDB.h"

loadFaceDB::loadFaceDB(QWidget*parent,QString dir):QDialog(parent),dir_default(dir)
{
	ui.setupUi(this);
	ui.dirName->setText(dir);
	this->setStyleSheet("QLineEdit{ background - color:gray; border - top - left - radius:15px; border - top - right - radius:5px; }");
	setAttribute(Qt::WA_DeleteOnClose);
}


loadFaceDB::~loadFaceDB()
{
}
void loadFaceDB::openDir() {
	QString dirName = QFileDialog::getExistingDirectory(this, u8"打卡人脸库", dir_default);
	if (!dirName.isEmpty()) {
		dir_default = dirName;
		ui.dirName->setText(dirName);
	}
}
void loadFaceDB::accept() {
	if (ui.dirName->text().isEmpty() || ui.groupName->text().isEmpty()) {
		QMessageBox::warning(this, u8"设置失败", u8"文件夹或群组名为空，请检查！","确定");
		return;
	}
	emit sendResult(ui.dirName->text(), ui.groupName->text());
	close();
}


