#include"testbaidufaceapi.h"
//    百度人脸识别api
#include<qsettings.h>


void testBaiduFaceAPI::setBaiduFaceApi() {
	ui.filetree->setSetting(settings);
}

void testBaiduFaceAPI::readSetting() {
	QSettings setting(QSettings::UserScope, qApp->organizationName(), qApp->applicationName(), this);
	settings.isCheckQuality = setting.value("isCheckQuality", QVariant(true)).toBool();
	settings.illum_thr = setting.value("illum_thr", QVariant(40)).toInt();
	settings.blur_thr = setting.value("blur_thr", QVariant(0.7f)).toFloat();
	settings.occlu_thr = setting.value("occlu_thr", QVariant(0.5f)).toFloat();
	settings.eulur_angle_x = setting.value("eulur_angle_x", QVariant(15)).toInt();
	settings.eulur_angle_y = setting.value("eulur_angle_y", QVariant(15)).toInt();
	settings.eulur_angle_z = setting.value("eulur_angle_z", QVariant(15)).toInt();
	settings.notFace_thr = setting.value("notFace", QVariant(0.5f)).toFloat();
	settings.min_face_size = setting.value("min_face_size", QVariant(30)).toInt();
}

void  testBaiduFaceAPI::saveSetting(void) {
	QSettings setting(QSettings::UserScope, qApp->organizationName(), qApp->applicationName(), this);
	setting.setValue("isCheckQuality", settings.isCheckQuality);
	setting.setValue("illum_thr", settings.illum_thr);
	setting.setValue("blur_thr", settings.blur_thr);
	setting.setValue("occlu_thr", settings.occlu_thr);
	setting.setValue("eulur_angle_x", settings.eulur_angle_x);
	setting.setValue("eulur_angle_y", settings.eulur_angle_y);
	setting.setValue("eulur_angle_z", settings.eulur_angle_z);
	 setting.setValue("notFace", settings.notFace_thr);
	 setting.setValue("min_face_size", settings.min_face_size);
}

void testBaiduFaceAPI::openFaceSetting(void) {

}