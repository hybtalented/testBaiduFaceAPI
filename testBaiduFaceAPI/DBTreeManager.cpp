#include "DBTreeManager.h"
#include<json/json.h>
#include<qdir.h>
using namespace Json;

DBTreeManager::DBTreeManager(QWidget*parent):QWidget(this),m_menu(0)
{
	ui.setupUi(this);
	initBaiduFaceApi();
	connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(slotItemDoubleClocked(QTreeWidgetItem*, int)));
	connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slotItemClocked(QTreeWidgetItem*, int)));
	ui.filetree->setItemsExpandable(true);
}


DBTreeManager::~DBTreeManager()
{
	delete baiduApi;
}

void DBTreeManager::setMenu(QMenu *menu)
{
	m_menu = menu;
	setMenu(menu);
}
void DBTreeManager::setSetting(Setting & setting)
{
	// 设置是否执行质量检测，默认为false
	baiduApi->set_isCheckQuality(setting.isCheckQuality);
	// 光照阈值，取值范围0~255，默认40，越大代表脸部越亮
	baiduApi->set_illum_thr(setting.illum_thr);
	// 模糊阈值，取值范围0~1，默认0.7，越小代表图像越清晰
	// 当设置为1时，模糊检测不进行且结果不通过；当设置为0时，模糊检测不进行且结果通过
	baiduApi->set_blur_thr(setting.blur_thr);
	// 右眼、左眼、鼻子、嘴巴、左脸颊、右脸颊、下巴的遮挡阈值，取值范围0~1，默认0.5，值越小代表遮挡程序越小
	// 当设置为1时，遮挡检测不进行且结果不通过；当设置为0时，遮挡检测不进行且结果通过
	baiduApi->set_occlu_thr(setting.occlu_thr);
	// 设置pitch、yaw、roll等角度的阈值，默认都为15 & deg; ，越大代表可采集的人脸角度越大，
	// 但是角度越大识别效果会越差
	baiduApi->set_eulur_angle_thr(setting.eulur_angle_x, setting.eulur_angle_y, setting.eulur_angle_z);
	// 非人脸的置信度阈值，取值范围0~1，取0则认为所有检测出来的结果都是人脸，默认0.5
	baiduApi->set_notFace_thr(setting.notFace_thr);
	// 最小人脸尺寸：需要检测到的最小人脸尺寸，比如需要检测到30*30的人脸则设置为30，
	// 该尺寸占图像比例越小则检测速度越慢，具体请参考性能指标章节。默认值30
	baiduApi->set_min_face_size(setting.min_face_size);
	// 跟踪到目标后执行检测的时间间隔，单位毫秒，默认300，值越小越会更快发现新目标，
	// 但是cpu占用率会提高、处理速度变慢
		//baiduApi->set_track_by_detection_interval(300);
	// 未跟踪到目标时的检测间隔，单位毫秒，默认300，值越小越快发现新目标，
	// 但是cpu占用率会提高、处理速度变慢
		//baiduApi->set_detect_in_video_interval(300);
}
/* DBTreeManager::itemClicked
  功能： 对应节点被点击后的动作
  完成情况； 未完成
*/
void DBTreeManager::itemClicked(QTreeWidgetItem * item, int column)
{
}
/* DBTreeManager::ReadGroup
  功能： 读取群组列表
  完成情况； 未测试
*/
QStringList DBTreeManager::ReadGroup()
{
	QStringList groups;
	for (int start = 0; Read10Group(groups, start) == 10; start += 10);
	return groups;
}
/* DBTreeManager::ReadPerson
  功能： 读取用户列表
  完成情况； 未测试
*/
QStringList DBTreeManager::ReadPerson(const char * groupname)
{
	QStringList users;
	for (int start = 0; Read100Person(groupname, start,users) == 100; start += 100);
	return users;
}
void DBTreeManager::deleteGroup(const char * groupname)
{
	std::string res = baiduApi->group_delete(groupname);
}
void DBTreeManager::deleteUser(const char * groupname, const char * username)
{
	std::string res = baiduApi->user_delete(username, groupname);
}
int DBTreeManager::createFace(const char * user, const char * group, const char * filepath, const char * filename)
{
	Value value;
	std::string res = baiduApi->user_add(user, group, filepath, 2, parse(filename).c_str());
	if (reader.parse(res.c_str(), value)) {
		if (value["errno"].isInt()) {
			return value["errno"].asInt();
		}
		else {
			return -1;
		}
	}
	return 1;
}
int DBTreeManager::createGroup(const char * groupname)
{
	Value value;
	std::string s = baiduApi->group_add(groupname);
	/* 返回结果实例
	errno:1006  群组已经存在
	{
		"data" :
		{
				"log_id" : "1536302772"
		},
		"errno" : 0,
		"msg" : "success"
}
	*/
	reader.parse(s, value);
	int ret = value["errno"].asInt();
	switch (ret) {
	case 0:case 1006:
		return 0;
	default:
		return ret;
	}
}
/* DBTreeManager::expandItem
  功能： 展开对应的节点
  完成情况； 未完成
*/
void DBTreeManager::expandItem(QTreeWidgetItem * item)
{
	DBTreeManagerItem * titem = dynamic_cast<DBTreeManagerItem*> (item);
	if (titem) {
		if (titem->expandState() == DBTreeManagerItem::UnExpanded) {
			
		}
	}
}
bool DBTreeManager::groupExist(const QString& str) {
	std::string res = baiduApi->get_group_list();
	for (auto group : getBaiduList(res.c_str(), "group")) {
		if (group == str) {
			return true;
		}
	}
	return false;
}
std::string  DBTreeManager::parse(const std::string&filename) {
	std::string s;
	s.reserve(2 * filename.size());
	static char temp[2] = { '\0','\0' };
	for (auto it = filename.begin(); it != filename.end(); ++it) {
		switch (*it) {
		case '_':
			s.append("__"); break;
		case '.':
			s.append("_"); break;
		default:
			temp[0] = *it;
			s.append(temp); break;
		}
	}
	return s;
}
std::string DBTreeManager::deParse(const std::string&filename) {
	char temp[2] = { '\0','\0' };
	std::string s;
	s.reserve(filename.size());
	for (auto it = filename.begin(); it != filename.end(); ++it) {

		switch (*it) {
		case '_':
			if (((it + 1) != filename.end()) && *(it + 1) == '_') {
				s.append("_");
				++it;
			}
			else {
				s.append(".");
			}
			break;
		default:
			temp[0] = *it;
			s.append(temp);
			break;
		}
	}
	return s;
}
int DBTreeManager::getFileInfo(const QString &str, QString&filename, QString &filetoken) {
	Value value;
	if (reader.parse(str.toStdString(), value)) {
		filename = value["name"].asCString();
		filetoken = value["token"].asCString();
	}
	else {
		return 1;
	}
}
/* DBTreeManager::addItem
  功能： 在对应下增加子节点
  完成情况； 未完成
*/
void DBTreeManager::addItem()
{
}
/* DBTreeManager::changeSelection
  功能： 选择状态改变后的相关操作，包括删除掉为选择节点的子节点
  完成情况； 未完成
*/
void DBTreeManager::changeSelection(QTreeWidgetItem *)
{
}
/* DBTreeManager::renameItem
  功能： 重命名节点
  完成情况； 未完成
*/
void DBTreeManager::renameItem()
{
}
/* DBTreeManager::slotItemDoubleClicked
  功能： 双击后的操作
  完成情况； 未完成
*/
void DBTreeManager::slotItemDoubleClicked(QTreeWidgetItem *, int)
{
  
}
/* DBTreeManager::slotItemDoubleClicked
  功能： 单击后的操作
  完成情况； 未完成
*/
void DBTreeManager::slotItemClicked(QTreeWidgetItem *, int)
{
}
int DBTreeManager::Read10Group(QStringList & ret, int start)
{
	int groupnum = 0;
	std::string  res = baiduApi->get_group_list(start, 10);
	for (auto it : getBaiduList(res.c_str(), "group")) {
		ret.append(it);
		groupnum++;
	}
	return groupnum;
}
int DBTreeManager::Read100Person(const char* groupname, int start, QStringList &persons)
{
	int personnum = 0;
	std::string res = baiduApi->get_user_list(groupname, start, 100);
	for (auto person : getBaiduList(res.c_str(), "user")) {
		personnum++;
		persons.append(person);
		/*  get_user_info 返回实例
		{
			"data" :
			{
				"log_id" : "1536281756",
				"result" :
				[
					{
						"create_time" : "1536142910",
						"face_token" : "2174eaa168b611fa2b8ef5bc274ec8b1",
						"group_id" : "main",
						"user_info" : ""
					}
				]
			},
			"errno" : 0,
			"msg" : "success"
		}
		*/
	}
	return personnum;
}
QStringList DBTreeManager::getBaiduList(const char*res, const char* name) {
	QStringList ret;
	Value value;
	bool ok = reader.parse(std::string(res), value);
	if (ok) {
		if (value["errno"].asInt() == 0) {
			value = value["data"][(std::string(name) + "_id_list")];
			if (value.isArray()) {
				for (auto i = 0; i < value.size(); ++i) {
					Value id = value[i][(std::string(name) + "_id")];
					if (id.isString())
						ret << id.asCString();
				}
			}
		}
	}
	return ret;
}

QStringList DBTreeManager::getFlist(const char * group,const char* user)
{
	Value value;
	std::string res = baiduApi->get_user_info(user, group);
	QStringList flist;
	reader.parse(res, value);
	Value root, temp;
	if (value["errno"] == 0) {
		value = value["data"]["result"];
		if (value.isArray()) {
			for (auto i = 0; i < value.size(); ++i) {
				root.clear(); temp.clear();
				if (value[i]["user_info"].isString()) {
					temp["token"] = value[i]["face_token"].asCString();
					temp["name"] = deParse(value[i]["user_info"].asString()).c_str();
					root.append(temp);
					flist << root.toStyledString().c_str();
				}

			}
		}
	}
	return flist;
}
void DBTreeManager::delelteSelection() {
	QList< DBTreeManagerItem*> itemlist;
	for (auto item : children()) {
		dynamic_cast<DBTreeManagerItem*>(item)->Delete();
		delete item;
	}
}
// 对百度人脸识别接口的初始化函数
void DBTreeManager::initBaiduFaceApi() {
	baiduApi = new BaiduFaceApi();
	//初始化sdk
 // 若采用证件照模式，请把id_card设为true，否则为false，证件照模式和非证件照模式提取的人脸特征值不同，
 // 不能混用
	bool id_card = false;
	//	try {
	baiduApi->sdk_init(id_card);
	//	}
	//	catch (std::length_error ex) {
	//		std::cout << ex.what()<<std::endl;
	//		std::vector<int> a;
	//		std::cout << a.max_size() << std::endl;
	//	}

		// 提前加载人脸库到内存
	// api->load_db_face();
}
//对百度人脸识别的相关设置

DBTreeManagerItem::DBTreeManagerItem(DBTreeManager*parent, const QString & name): QTreeWidgetItem(parent->UI().filetree,QStringList()<<name<<"group") ,m_expandstate(UnExpanded),par(parent) {
	setCheckState(0, Qt::Unchecked);
}

DBTreeManagerItem::DBTreeManagerItem(DBTreeManagerItem * parent, const QString & name ): QTreeWidgetItem(parent, QStringList() << name << "user"),m_expandstate(UnExpanded),par(parent->par)
{
	setCheckState(0, Qt::Unchecked);
}
DBTreeManagerItem::DBTreeManagerItem(DBTreeManagerItem * parent, const QString & name, const QString & token):QTreeWidgetItem (parent,QStringList()<<name<<"image"<<token), m_expandstate(NoChild),par(parent->par)
{
	setCheckState(0, Qt::Unchecked);
}
DBTreeManagerItem::ItemType DBTreeManagerItem::type() {
	QString type = text(1);
	if (type == "group") {
		return group;
	}
	if (type == "user") {
		return user;
	}
	if (type == "image") {
		return image;
	}
	return UnKnow;
}
void DBTreeManagerItem::deleteChild()
{
	for (auto child : takeChildren()) {
		DBTreeManagerItem* childitem = dynamic_cast<DBTreeManagerItem*>(child);
		childitem->Delete();
		if(checkState(0)==Qt::Checked||(checkState(0)==Qt::PartiallyChecked&&m_expandstate==UnExpanded))
	        delete childitem;
	}
}
void DBTreeManagerItem::Delete() { 
	QString rootdir(parent()->UI().lineEdit->text());
	DBTreeManager* dbtree = dynamic_cast<DBTreeManager*>(parent());
	if (!dbtree) return;
	if (checkState(0) == Qt::Unchecked)return;
	if (type() != image) {
		if (m_expandstate == UnExpanded && checkState(0) == Qt::Checked) {
			dbtree->expandItem(this);
		}
		deleteChild();
	}
	switch (type()) {
	case group:
		if (belong&DB) {
			dbtree->deleteGroup(text(0).toStdString().c_str());
		}
		if (belong&file) {
			deleteDir(rootdir + "/" + text(0));
		}
		break;
	case user:
		if (belong&DB) {
			dbtree->deleteUser(text(0).toStdString().c_str(), text(0).toStdString().c_str());
		}
		if (belong&file) {
			QTreeWidgetItem *groupitem = QTreeWidgetItem::parent();
			if (!groupitem) return;
			deleteDir(rootdir + "/" + groupitem->text(0) + text(0));
		}
		break;
	case image:
		if (checkState(0) == Qt::Checked) {
			QTreeWidgetItem *useritem = QTreeWidgetItem::parent();
			if (useritem == 0)return;
			QTreeWidgetItem *groupitem = useritem->parent();
			if (!groupitem) return;
			if (belong&file) {
				deleteDir(rootdir + "/" + groupitem->text(0) + "/" + useritem->text(0) + "/" + text(0));
			}
		}
		break;
	default:
		break;
	}
}

void deleteDir(const QString& dirname)
{
	QDir dir;
	if (dir.exists()) {
		for (auto sub : dir.entryInfoList()) {
			if (sub.isDir()) {
				deleteDir(sub.absolutePath());
			}
			if (sub.isFile()) {
				deleteFile(sub.absolutePath());
			}
		}
	}

}
void deleteFile(const QString & filename)
{
	QFile file;
	if (file.exists()) {
		file.remove();
	}
}