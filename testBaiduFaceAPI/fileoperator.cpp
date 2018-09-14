#include"testbaidufaceapi.h"
#include<baidu_face_api.h>
#include<qfiledialog.h>
#include<qmessagebox.h>
#include<qdebug.h>
#include<qmutex.h>
void testBaiduFaceAPI::FileOpen() {
	QStringList files = QFileDialog::getOpenFileNames(this, u8"打开文件", ui.filetree->UI().lineEdit->text(), u8"图像文件(*.jpg *.png)");
	QStringList flist;
	if (files.isEmpty()) {
		return;
	}
	QFileInfo fileinfo(files[0]);
	setCurrentDir( fileinfo.dir().absolutePath());
	disconnect(ui.filetree, SIGNAL(itemPressed(QTreeWidgetItem *, int)), this, SLOT(fileItemClicked(QTreeWidgetItem *, int)));
	ui.filetree->UI().filetree->clear();
	for (auto it = files.begin(); it != files.end(); ++it) {
		addItem(ui.filetree->UI().filetree,*it);
	}
	connect(ui.filetree, SIGNAL(itemPressed(QTreeWidgetItem *,int)), this, SLOT(fileItemClicked(QTreeWidgetItem *,int)));
	switchToManageMode(false, false);
}
void testBaiduFaceAPI::DirOpen() {
	QString path = QFileDialog::getExistingDirectory(this, u8"读取文件夹内的图片", ui.filetree->UI().lineEdit->text());
	QDir dir(path);
	if (dir.exists()) {
		setCurrentDir( path);
		switchToManageMode(false);
	}
}
void testBaiduFaceAPI::addItem(QTreeWidgetItem * filetree, QString file)
{
	QFileInfo fileinfo(file);
	QString filename = fileinfo.fileName();
	QTreeWidgetItem* filelist = new QTreeWidgetItem(filetree, QStringList(filename));
	filelist->setIcon(0, QIcon(file));
	filelist->setToolTip(0, QString(file));
}
void testBaiduFaceAPI::addItem(QTreeWidget * filetree, QString file)
{
	QFileInfo fileinfo(file);
	QString filename = fileinfo.fileName();
	QTreeWidgetItem* filelist = new QTreeWidgetItem(filetree, QStringList(filename));
	filelist->setIcon(0, QIcon(file));
	filelist->setToolTip(0, QString(file));
}
void testBaiduFaceAPI::openFaceRepoDir() {
	QString path = QFileDialog::getExistingDirectory(this, u8"打开人脸库文件夹", ui.filetree->UI().lineEdit->text());
	if (path.isEmpty()) return;
	setCurrentDir(path);
	switchToManageMode(setDB);
}

void testBaiduFaceAPI::ImageSave() {

}
void testBaiduFaceAPI::TextSave() {

}


