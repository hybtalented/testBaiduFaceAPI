#include "DBTreeManager.h"
#include<json/json.h>
#include<qdir.h>
#include"mydebug.h"
#include<qmessagebox.h>
#include<qdatetime.h>


using namespace Json;

DBTreeManager::DBTreeManager(QWidget*parent)throw(std::exception) :QWidget(parent), m_menu(nullptr), sqlite(face_db_addr), db_init(false)
{
	if (sqlite.Result()) {
		QMessageBox::critical(this, u8"打开本地人脸库失败", QString(u8"错误代码:<font color=red> %1 </font>. 错误信息: <font color=red> %2 </font>").arg(sqlite.Result()).arg(sqlite.DB_STATUS()),u8"&退出程序");
		exit(0);
	}
	ui.setupUi(this);
	initBaiduFaceApi();
	initDB();
	if (!db_init) {
		qCritical() << "DATABASE INITIALIZE FAILED!!";
	}
	connect(ui.filetree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(slotItemDoubleClicked(QTreeWidgetItem*, int)));
	connect(ui.filetree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slotItemClicked(QTreeWidgetItem*, int)));
	ui.filetree->setItemsExpandable(true);
	connect(ui.lineEdit, SIGNAL(editingFinished()), this, SIGNAL(PathChanged()));
	connect(ui.pushButton, SIGNAL(clicked()), this, SIGNAL(PathChangeActionClicked()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SIGNAL(SearchButtonClicked()));
	connect(this, SIGNAL(setPrograssValue(int)),ui.syncProgress, SLOT(setValue(int)));
	connect(this, SIGNAL(setPrograssRange(int ,int)), ui.syncProgress, SLOT(setRange(int,int)));
}


DBTreeManager::~DBTreeManager()noexcept(true)
{
	db.logoff();
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
/* DBTreeManager::ReadUserInfo
  功能： 读取用户信息
  完成情况； 未测试
*/
const char* DBTreeManager::ReadUserInfo(const char* group, const char* user) {
	return baiduApi->get_user_info(user, group);
	
}
const char * DBTreeManager::AddGroup(const char * group)
{
	qint64 time = getCurrentTimeStamp();
	sqlite.lock();
	std::string ret = baiduApi->group_add(group);
	sqlite.unLock();
	Json::Value value;
	reader.parse(ret, value);
	if (value["errno"].asInt() == 0) {
		qint64 gid = getSequenceVal(DB_TABLE(USER_GROUP_SEQUENCE));
		addRemoteDBUSER_GROUP(gid, group, time);
		updateLocalDB(TABLE(USER_GROUP), FORMAT_2("ID=%1,CREATE_TIME=%2", gid, time), FORMAT_1("where GROUP_ID='%1'", group));
	}
	return ret.c_str();
}
const char * DBTreeManager::AddUser(const char * group, const char * user,const char* img,  const char* user_info)
{
	qint64 time=getCurrentTimeStamp();
	bool groupexist = groupExist(group), userexist = userExist(group, user);
	sqlite.lock();
	std::string ret= baiduApi->user_add(user, group, img, 1, user_info);
	sqlite.unLock();
	Json::Value value;
	reader.parse(ret, value);
	if (value["errno"].asInt() == 0 ) {
		if (!groupexist) {
			qint64 gid = getSequenceVal(DB_TABLE(USER_GROUP_SEQUENCE));
			addRemoteDBUSER_GROUP(gid, group, time);
			updateLocalDB(TABLE(USER_GROUP), FORMAT_2("ID=%1,CREATE_TIME=%2", gid,time),FORMAT_1("where GROUP_ID='%1'",group));
		}
		if (!userexist) {
			qint64 uid = getSequenceVal(DB_TABLE(USER_SEQUENCE));
			addRemoteDBUSER(uid, user, group, user_info, time);
			updateLocalDB(TABLE(USER), FORMAT_2("ID=%1,CREATE_TIME=%2", uid, time), FORMAT_2("where GROUP_ID='%1' and USER_ID='%2'", group,user));
		}
		std::string face_token = value["data"]["face_token"].asString();
		SQLITE_RET ret = selectLocalDB("FEATURE", TABLE(FEATURE), FORMAT_3("where GROUP_ID='%1' and USER_ID='%2' and face_token='%3'", group, user, face_token.c_str()));
		if (ret.rows >= 1) {
			qint64 fid = getSequenceVal(DB_TABLE(FEATURE_SEQUENCE));
			addRemoteDBFEATURE(fid, face_token.c_str(), group, user, ret.getCString(0,"FEATURE"), time);
			updateLocalDB(TABLE(FEATURE), FORMAT_2("ID=%1,CREATE_TIME=%2", fid, time), FORMAT_3("where GROUP_ID='%1' and USER_ID='%2' and face_token='%3'", group, user, face_token.c_str()));
			updateLocalDB(TABLE(TIMESTAMP), FORMAT_1("TIME=%1", time), "where IDX=1");
			if(ret.rows>1)
			    Warning() << QString("Too many rows(%1) getted when add picture to user %2 in group %3").arg(ret.rows).arg(user).arg(group);	
		}
	}
	else {
		Warning() << "Failed add user " << user << "to group" << group<<"with result "<< ret.c_str();
	}
	return ret.c_str();
}
/* DBTreeManager::ReadPerson
  功能： 读取用户列表
  完成情况； 完成
  返回值： 匹配结果
*/
QString DBTreeManager::CompareBase64(const char *img1, const char *img2)
{
	return  baiduApi->match(img1, 1, img2, 1);
}
QString DBTreeManager::CompareWithGroup(const char * imgbase64, const char * group, const char * user)
{
	if (strlen(group)) {
		return baiduApi->identify(imgbase64, 1,group,user);
	}
	else {
		return baiduApi->identify(imgbase64, 1);
	}
}
const char* DBTreeManager::deleteGroup(const char * groupname)
{
	qint64 time = getCurrentTimeStamp();
	deleteLocalDB(TABLE(USER_GROUP), FORMAT_1("where GROUP_ID='%1'", groupname));
	deleteLocalDB(TABLE(USER), FORMAT_1("where GROUP_ID='%1'", groupname));
	deleteLocalDB(TABLE(FEATURE), FORMAT_1("where GROUP_ID='%1'", groupname));
	Json::Value value;
	int uc=0,gc=0,fc=0;
	try {
			qint64 gid;
			otl_stream os(1, FORMAT_3(SELECTSTR, "ID", DB_TABLE(USER_GROUP), FORMAT_1("where GROUP_ID='%1'", groupname)), db);
			deleteRemoteDB(DB_TABLE(USER_GROUP), FORMAT_1("where GROUP_ID='%1'", groupname));
			
			while (!os.eof()) {
				os >> gid;
				deleteRemoteDB(DB_TABLE(RECODE), RECODE(gid, group));
				gc++;
				addRemoteDBRECODE(gid, time, group | del);
			}
			os.close();
			os.open(1, FORMAT_3(SELECTSTR, "ID", DB_TABLE(USER), FORMAT_1("where GROUP_ID='%1'", groupname)), db);
			deleteRemoteDB(DB_TABLE(USER), FORMAT_1("where GROUP_ID='%1'", groupname));
			while (!os.eof()) {
				os >> gid;
				deleteRemoteDB(DB_TABLE(RECODE), RECODE(gid, user));
				uc++;
				addRemoteDBRECODE(gid, time, user | del);
			}
			os.close();
			os.open(1, FORMAT_3(SELECTSTR, "ID", DB_TABLE(FEATURE), FORMAT_1("where GROUP_ID='%1'", groupname)), db);
			deleteRemoteDB(DB_TABLE(FEATURE), FORMAT_1("where GROUP_ID='%1'", groupname));
			while (!os.eof()) {
				os >> gid;
				deleteRemoteDB(DB_TABLE(RECODE), RECODE(gid, feature));
				fc++;
				addRemoteDBRECODE(gid, time, feature | del);
			}
			
	}
	catch (otl_exception& p) {
		Critical() << QString::fromUtf8((char*)p.msg) << "On query: " << p.stm_text << "Cause by " << p.var_info;  // print out error message
	}
	value["groupDel"] = gc;
	value["userDel"] = uc;
	value["faceFeatureDel"] = fc;
	return value.toStyledString().c_str();
}
const char* DBTreeManager::deleteUser(const char * groupname, const char * username)
{
	qint64 time = getCurrentTimeStamp();
	deleteLocalDB(TABLE(USER), FORMAT_1("where USER_ID='%1'", groupname));
	deleteLocalDB(TABLE(FEATURE), FORMAT_1("where USER_ID='%1'", groupname));
	Json::Value value;
	int uc = 0, fc = 0;
	try {
		qint64 gid;
		otl_stream os(1, FORMAT_3(SELECTSTR, "ID", DB_TABLE(USER_GROUP), FORMAT_1("where USER_ID='%1'", groupname)), db);
		deleteRemoteDB(DB_TABLE(USER_GROUP), FORMAT_1("where USER_ID='%1'", groupname));
		while (!os.eof()) {
			os >> gid;
			deleteRemoteDB(DB_TABLE(RECODE), RECODE(gid, user));
			uc++;
			addRemoteDBRECODE(gid, time, user | del);
		}
		os.close();
		os.open(1, FORMAT_3(SELECTSTR, "ID", DB_TABLE(FEATURE), FORMAT_1("where USER_ID='%1'", groupname)), db);
		deleteRemoteDB(DB_TABLE(FEATURE), FORMAT_1("where USER_ID='%1'", groupname));
		while (!os.eof()) {
			os >> gid;
			deleteRemoteDB(DB_TABLE(RECODE), RECODE(gid, feature));
			fc++;
			addRemoteDBRECODE(gid, time, feature | del);
		}
	}
	catch (otl_exception& p) {
		Critical() << QString::fromUtf8((char*)p.msg) << "On query: " << p.stm_text << "Cause by " << p.var_info;  // print out error message
	}
	value["userDel"] = uc;
	value["faceFeatureDel"] = fc;
	return value.toStyledString().c_str();
}
int DBTreeManager::createFace(const char * user, const char * group, const char * filepath, const char * filename)
{
	Value value;
	QFile file(filepath);
	qDebug() << filepath;
	std::string res = AddUser(group, user, file.readAll().toBase64().toStdString().c_str(),parse(filename).c_str());
	qDebug() << res.c_str();
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
	std::string s = AddGroup(groupname);
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
	SQLITE_RET ret = selectLocalDB("count(0) c", TABLE(USER_GROUP), FORMAT_1("where group_id='%1'", str));
	if (ret.getInt(0,"c") > 0) {
		return true;
	}
	return false;
}
bool DBTreeManager::userExist(const QString & group, const QString & user)
{
	SQLITE_RET ret = selectLocalDB("count(0) c", TABLE(USER), FORMAT_2("where group_id='%1' and user_id='%2'", group,user));
	if (ret.getInt(0,"c") > 0) {
		return true;
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
		return 0;
	}
	else {
		return 1;
	}
}
qint64 DBTreeManager::getCurrentTimeStamp()
{
	Debug();
	try {
		otl_stream o(1, FORMAT_3(SELECTSTR, "TO_CHAR(LOCALTIMESTAMP(3),'yyyy-mm-dd HH24:mi:ss.ff3')", "dual", ""),db);
		char s[100];
		o >> s;
		int64 a = QDateTime::fromTime_t(0).msecsTo(QDateTime::fromString(s, "yyyy-MM-dd hh:mm:ss.zzz"));
		return a;
	}
	catch (otl_exception p) {
		Critical() << QString::fromUtf8((char*)p.msg) << "On query: " << p.stm_text << "Cause by " << p.var_info;  // print out error message
		return -1;
	}
}
qint64 DBTreeManager::getLocalDBTimeStamp()
{
	Debug();
	SQLITE_RET ret = selectLocalDB("TIME", TABLE(TIMESTAMP), "where IDX=1");
	if (sqlite.Result() != SQLITE_OK) {
		Warning() << sqlite.ErrMsg();
		db_init = false;
		return -1;
	}
	if (ret.rows == 0) {
		insertLocalDB(FORMAT_2("%1 (%2)", DB_TABLE(TIMESTAMP), "IDX,TIME"), "1,0", "");
		return 0;
	}
	return  ret.getInt64(0, "TIME");
}
qint64 DBTreeManager::getRemoteDBtimeStamp()
{
	Debug();
	try {
		otl_stream o;
		o.open(1, FORMAT_3(SELECTSTR, "TIME", DB_TABLE(TIMESTAMP), "where IDX=1"), db);
		if (o.eof()) {
			insertRemoteDB(FORMAT_2("%1 (%2)",DB_TABLE(TIMESTAMP),"IDX,TIME"), "1,0","");
			return 0;
		}
		OTL_BIGINT time;
		o >> time;
		o.close();
		return time;
	}
	catch (otl_exception&p) {
		Warning()<< QString::fromUtf8((char*)p.msg) << "On query: " << p.stm_text << "Cause by " << p.var_info;
		db_init = false;
		return -1;
	}
	
}
qint64 DBTreeManager::getSequenceVal(const char * sequence)
{
	try {
		otl_stream o;
		o.open(50, FORMAT_3(SELECTSTR, FORMAT_1("%1.nextval",sequence), "dual",""), db);
		OTL_BIGINT a;
		o >> a;
		return a;
	}
	catch (otl_exception&p) {
		Critical() << QString::fromUtf8((char*)p.msg)  << "On query: " << p.stm_text << "Cause by " << p.var_info;
		return -1;
	}
}
bool DBTreeManager::setLocalTimeStamp(qint64 timestamp)
{
	Debug() << "timestamp is " << timestamp;
	switch (updateLocalDB(TABLE(TIMESTAMP), FORMAT_1("TIME=%1",QVariant::fromValue(timestamp).toString()), "where IDX = 1")) {
	case SQLITE_OK:
		return true;
	default:
		return false;
	}
}
bool DBTreeManager::setRemoteDBtimeStamp(qint64 timestamp)
{
	Debug()<< "timestamp is " << timestamp;
	switch (updateRemoteDB(DB_TABLE(TIMESTAMP), FORMAT_1("TIME=%1", QVariant::fromValue(timestamp).toString()), "where IDX = 1")) {
	case 0:
		return true;
	default:
		return false;
	}
}
bool DBTreeManager::addLocalDBUSER(qint64 ID, const char * USER_ID, const char * GROUP_ID, const char * USER_INFO, qint64 CREATE_TIME)
{
	Debug() << FORMAT_4("Adding (ID:%1,USERID:%4,GROUP_ID:%2,CREATE_TIME:%3)", ID, GROUP_ID, CREATE_TIME, USER_ID);
	switch (insertLocalDB(FORMAT_2("%1 (%2)",TABLE(USER),"ID,USER_ID,GROUP_ID,USER_INFO,CREATE_TIME"), FORMAT_5("%1,'%2','%3','%4',%5 ", QVariant::fromValue(ID).toString(), USER_ID,GROUP_ID, USER_INFO,CREATE_TIME), "where IDX = 1")) {
	case SQLITE_OK:
		return true;
	default:
		return false;
	}
}
bool DBTreeManager::addRemoteDBUSER(qint64 ID, const char * USER_ID, const char * GROUP_ID, const char * USER_INFO, qint64 CREATE_TIME)
{
	Debug() << FORMAT_4("Adding (ID:%1,USERID:%4,GROUP_ID:%2,CREATE_TIME:%3)", ID, GROUP_ID, CREATE_TIME, USER_ID);
	switch (insertRemoteDB(FORMAT_2("%1 (%2)", DB_TABLE(USER), "ID,USER_ID,GROUP_ID,USER_INFO,CREATE_TIME,UPDATE_TIME"), FORMAT_6("%1,'%2','%3','%4',%5,%6", ID, USER_ID, GROUP_ID, USER_INFO, CREATE_TIME, CREATE_TIME), "")) {
	case 0:
		addRemoteDBRECODE(ID, CREATE_TIME, user | ins);
		return true;
	default:
		return false;
	}
}

bool DBTreeManager::addLocalDBUSER_GROUP(qint64 ID, const char * GROUP_ID, qint64 CREATE_TIME)
{
	Debug() << FORMAT_3("Adding group (ID:%1,GROUP_ID:%2,CREATE_TIME:%3", ID, GROUP_ID, CREATE_TIME);
	switch (insertLocalDB(FORMAT_2("%1 (%2)", TABLE(USER_GROUP), "ID,GROUP_ID,CREATE_TIME"), FORMAT_3("%1,'%2',%3 ", ID,  GROUP_ID,  CREATE_TIME), "")) {
	case SQLITE_OK:
		return true;
	default:
		return false;
	}
}
bool DBTreeManager::addRemoteDBUSER_GROUP(qint64 ID, const char * GROUP_ID, qint64 CREATE_TIME)
{
	Debug() << FORMAT_3("Adding group (ID:%1,GROUP_ID:%2,CREATE_TIME:%3", ID, GROUP_ID, CREATE_TIME);
	switch (insertRemoteDB(FORMAT_2("%1 (%2)", DB_TABLE(USER_GROUP), "ID,GROUP_ID,CREATE_TIME,UPDATE_TIME"), FORMAT_3("%1,'%2',%3,%3", ID, GROUP_ID, CREATE_TIME), "")) {
	case 0:
		addRemoteDBRECODE(ID, CREATE_TIME, group | ins);
		return true;
	default:
		return false;
	}
}

bool DBTreeManager::addLocalDBFEATURE(qint64 ID, const char * FACE_TOKEN, const char * GROUP_ID, const char * USER_ID, const char * FEATURE, qint64 CREATE_TIME)
{
	Debug() << FORMAT_5("Adding group (ID:%1,GROUP_ID:%2,CREATE_TIME:%3,FACE_TOKEN:%4,USER_ID:%5", ID, GROUP_ID, CREATE_TIME, FACE_TOKEN, USER_ID);
	switch (insertLocalDB(FORMAT_2("%1 (%2)", TABLE(FEATURE), "ID,FACE_TOKEN,GROUP_ID,USER_ID,FEATURE,CREATE_TIME"), FORMAT_6("%1,'%2','%3','%4','%5',%6", QVariant::fromValue(ID).toString(), FACE_TOKEN,GROUP_ID, USER_ID, FEATURE, CREATE_TIME), "")) {
	case SQLITE_OK:
		return true;
	default:
		return false;
	}
}
bool DBTreeManager::addRemoteDBFEATURE(qint64 ID, const char * FACE_TOKEN, const char * GROUP_ID, const char * USER_ID, const char * FEATURE, qint64 CREATE_TIME)
{
	Debug() << FORMAT_3(INSERTSTR, FORMAT_2("%1 (%2)", DB_TABLE(FEATURE), "ID,FACE_TOKEN,GROUP_ID,USER_ID,FEATURE,CREATE_TIME,UPDATE_TIME"), ":id<bigint>,:face_token<char[128]>,:group_id<char[128]>,:user_id<char[128]>,empty_clob(),:create_time<bigint>,:create_time", "returning FEATURE into :FEATURE<clob>");
	try {
		int len = strlen(FEATURE);
		Debug() << len;
		otl_stream o;
		otl_long_string str(FEATURE, len + 1,len+1);
		o.open(1, FORMAT_3(INSERTSTR, FORMAT_2("%1 (%2)", DB_TABLE(FEATURE), "ID,FACE_TOKEN,GROUP_ID,USER_ID,FEATURE,CREATE_TIME,UPDATE_TIME"), ":id<bigint>,:face_token<char[128]>,:group_id<char[128]>,:user_id<char[128]>,empty_clob(),:create_time<bigint>,:create_time ", "returning FEATURE into :FEATURE<clob>"), db);
		o << ID << FACE_TOKEN << GROUP_ID << USER_ID << CREATE_TIME << str;
		addRemoteDBRECODE(ID, CREATE_TIME, feature | ins);
		return true;
	}
	catch (otl_exception& p) {
		Warning() << "Error Code:" << p.code << " " << QString::fromUtf8((char*)p.msg) << "On query: " << p.stm_text << "Cause by " << p.var_info;
		return false;
	}	
}
bool DBTreeManager::addRemoteDBRECODE(qint64 id, qint64 time, int oper)
{
	switch (insertRemoteDB(FORMAT_2("%1 (%2)", DB_TABLE(RECODE), "ID,TIME,OPER"), FORMAT_3("%1,%2,%3", id, time, oper), "")) {
	case 0:
		updateRemoteDB(DB_TABLE(TIMESTAMP), FORMAT_1("TIME=%1", time), "where IDX=1");
		return true;
	default:
		return false;
	}
}
bool DBTreeManager::findRECODE(qint64 id, RECODE_OPER table)
{
	try {
		int count;
		otl_stream o(1, FORMAT_3(SELECTSTR, "count(0)", DB_TABLE(RECODE), RECODE(id, table)), db);
		o >> count;
		if (count > 0)return true;
	}
	catch (otl_exception& p) {
		Critical() << QString::fromUtf8((char*)p.msg) << "On query: " << p.stm_text << "Cause by " << p.var_info;  // print out error message
	}
	return false;
}
SQLITE_RET DBTreeManager::selectLocalDB(const char * content, const char * table, const char * addtion_option)
{
	Debug() << FORMAT_3(SELECTSTR, content, table, addtion_option);
	SQLITE_RET ret=sqlite.exec(FORMAT_3(SELECTSTR, content, table, addtion_option));
	if (sqlite.Result() != SQLITE_OK) {
		Info() << sqlite.ErrMsg();
		ret.rows = 0;
		ret.value.clear();
	}
	return ret;
}


int DBTreeManager::insertLocalDB(const char * table, const char * content, const char * addtion_option)
{
	Debug() << FORMAT_3(SELECTSTR, content, table, addtion_option);
	sqlite.lock();
	SQLITE_RET ret = sqlite.exec(FORMAT_3(INSERTSTR, table, content, addtion_option));
	if (sqlite.Result() != SQLITE_OK) {
		Info() << sqlite.ErrMsg();
	}
	sqlite.unLock();
	return sqlite.Result();
}

int DBTreeManager::insertRemoteDB(const char * table, const char * content, const char * addtion_option)
{
	QMutexLocker locker(&dbmutex);
	Debug() << FORMAT_3(INSERTSTR, table, content, addtion_option);
	try {
		otl_stream o(1, FORMAT_3(INSERTSTR, table, content, addtion_option), db);
		return 0;
	}
	catch (otl_exception& p) {
		Warning()<<"Error Code:"<<p.code<<" "<< QString::fromUtf8((char*)p.msg) << "On query: " << p.stm_text << "Cause by " << p.var_info;
		return p.code;
	}
}

int DBTreeManager::updateLocalDB(const char * table, const char * columns, const char * addtion_option)
{
	sqlite.lock();
	Debug() << FORMAT_3(UPDATESTR, table, columns, addtion_option);
	SQLITE_RET ret = sqlite.exec(FORMAT_3(UPDATESTR, table, columns, addtion_option));
	if (sqlite.Result() != SQLITE_OK) {
		Warning() << sqlite.ErrMsg();
	}
	sqlite.unLock();
	return sqlite.Result();
}

int DBTreeManager::updateRemoteDB(const char * table, const char * columns, const char * addtion_option)
{
	QMutexLocker locker(&dbmutex);
	Debug() << FORMAT_3(UPDATESTR, table, columns, addtion_option);
	try {
		otl_stream o(1, FORMAT_3(UPDATESTR, table, columns, addtion_option), db);
		return 0;
	}
	catch (otl_exception& p) {
		Warning() << "Error Code:" << p.code << " " << QString::fromUtf8((char*)p.msg) << "On query: " << p.stm_text << "Cause by " << p.var_info;
		return p.code;
	}
}

int DBTreeManager::deleteLocalDB(const char * table, const char * addtion_option)
{
	sqlite.lock();
	Debug() << FORMAT_2(DELETESTR, table, addtion_option);
	SQLITE_RET ret = sqlite.exec(FORMAT_2(DELETESTR, table, addtion_option));
	if (sqlite.Result() != SQLITE_OK) {
		Warning() << sqlite.ErrMsg();
	}
	sqlite.unLock();
	return sqlite.Result();
}

int DBTreeManager::deleteRemoteDB(const char * table, const char * addtion_option)
{
	QMutexLocker locker(&dbmutex);
	Debug() << FORMAT_2(DELETESTR, table, addtion_option);
	try {
		otl_stream o(1, FORMAT_2(DELETESTR, table, addtion_option), db);
		return 0;
	}
	catch (otl_exception& p) {
		Warning() << "Error Code:" << p.code << " " << QString::fromUtf8((char*)p.msg) << "On query: " << p.stm_text << "Cause by " << p.var_info;
		return p.code;
	}
}

int DBTreeManager::alterLocalDB(const char * table, const char * option, const char * content)
{
	sqlite.lock();
	Debug() << FORMAT_3(ALTERSTR, table, option, content);
	SQLITE_RET ret = sqlite.exec(FORMAT_3(ALTERSTR, table, option, content));
	if (sqlite.Result() != SQLITE_OK) {
		Warning() << sqlite.ErrMsg();
	}
	sqlite.unLock();
	return sqlite.Result();
}

int DBTreeManager::alterRemoteDB(const char * table, const char * option, const char * content)
{
	QMutexLocker locker(&dbmutex);
	Debug() << FORMAT_3(ALTERSTR, table, option, content);
	try {
		otl_stream o(1, FORMAT_3(ALTERSTR, table, option, content), db);
		o.close();
		return 0;
	}
	catch (otl_exception& p) {
		Warning() << "Error Code:" << p.code << " " << QString::fromUtf8((char*)p.msg) << "On query: " << p.stm_text << "Cause by " << p.var_info;
		return p.code;
	}
}

int DBTreeManager::createLocalTable(const char * table, const char * coldefs, const char * addtion_option)
{
	sqlite.lock();
	Debug() << FORMAT_3(CREATETABLESTR, table, coldefs, addtion_option);
	SQLITE_RET ret = sqlite.exec(FORMAT_3(CREATETABLESTR, table, coldefs, addtion_option));
	if (sqlite.Result() != SQLITE_OK) {
		Warning() << sqlite.ErrMsg();
	}
	sqlite.unLock();
	return sqlite.Result();
}

int DBTreeManager::createRemoteTable(const char * table, const char * coldefs, const char * addtion_option)
{
	QMutexLocker locker(&dbmutex);
	try {
		otl_stream o(1, FORMAT_3(CREATETABLESTR, table, coldefs, addtion_option), db);
		return 0;
	}
	catch (otl_exception& p) {
		Warning() << "Error Code:" << p.code << " " << QString::fromUtf8((char*)p.msg) << "On query: " << p.stm_text << "Cause by " << p.var_info;
		return p.code;
	}
}

int DBTreeManager::createRemoteSequence(const char * sequence)
{
	QMutexLocker locker(&dbmutex);
	try {
		otl_stream o(1, FORMAT_2(CREATESEQUENCESTR, sequence, " increment by 1 start with 1"),db);
		return 0;
	}
	catch (otl_exception& p) {
		Warning() << "Error Code:" << p.code << " " << QString::fromUtf8((char*)p.msg) << "On query: " << p.stm_text << "Cause by " << p.var_info;
		return p.code;
	}
}
int DBTreeManager::LocalTableExist(const char * table)
{
	SQLITE_RET ret = selectLocalDB("count(*) num", "sqlite_master", FORMAT_1("where TYPE='table' and name='%1'",table));
	if (sqlite.Result() == SQLITE_OK && ret.rows == 1) {
		return ret.getInt(0, "num");
	}
	return -1;
}

int DBTreeManager::RemoteTableExist(const char * table)
{
	try {
		int count;
		otl_stream o(1, FORMAT_3(SELECTSTR, "count(*)", "USER_ALL_TABLES", FORMAT_1("where TABLE_NAME='%1'", table)), db);
		o >> count;
		return count;
	}
	catch(otl_exception&p){
		Warning() << "Error Code:" << p.code << " " << QString::fromUtf8((char*)p.msg) << "On query: " << p.stm_text << "Cause by " << p.var_info;
		return -1;
	}
}

int DBTreeManager::RemoteSequenceExist(const char * sequence)
{
	try {
		int count;
		otl_stream o(1, FORMAT_3(SELECTSTR, "count(*)", "user_sequences", FORMAT_1("where SEQUENCE_NAME='%1'", sequence)), db);
		o >> count;
		return count;
	}
	catch (otl_exception&p) {
		Warning() << "Error Code:" << p.code << " " << QString::fromUtf8((char*)p.msg) << "On query: " << p.stm_text << "Cause by " << p.var_info;
		return -1;
	}
	return 0;
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
		ret << it;
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
				for (uint i = 0; i < value.size(); ++i) {
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
			for (uint i = 0; i < value.size(); ++i) {
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
void DBTreeManager::synchronization(qint64 local, qint64 remote)
{
    if (local < remote) {
		emit setPrograssRange(0, 0);
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
	Debug()<<"on sdk_init result:"<<baiduApi->sdk_init(id_card);
	//	}
	//	catch (std::length_error ex) {
	//		std::cout << ex.what()<<std::endl;
	//		std::vector<int> a;
	//		std::cout << a.max_size() << std::endl;
	//	}
	Debug() << "on load_db_face" << baiduApi->load_db_face();
}
void DBTreeManager::initDB()
{
	qint64 timestamp, sqlitetimestamp;
	try {
		db.otl_initialize(1);
		db.rlogon(oracle_login_data);
		db.set_max_long_size(5000);
		// 查询修改时间戳
		if (RemoteTableExist(DB_TABLE(TIMESTAMP)) ==0) {
			Info() << "On create table:"<<DB_TABLE(TIMESTAMP);
			if (createRemoteTable(DB_TABLE(TIMESTAMP), "IDX integer default 1 primary key,TIME integer default 0 not null")) {
				db_init = false;
				return;
			}
			
		}
		timestamp = getRemoteDBtimeStamp();
		if (RemoteSequenceExist(DB_TABLE(USER_SEQUENCE)) == 0) {
			Info() << "On create sequence:" << DB_TABLE(USER_SEQUENCE);
			if (createRemoteSequence(DB_TABLE(USER_SEQUENCE))) {
				db_init = false;
				return;
			}
		}
		if (RemoteSequenceExist(DB_TABLE(USER_GROUP_SEQUENCE)) == 0) {
			Info() << "On create sequence:" << DB_TABLE(USER_GROUP_SEQUENCE);
			if (createRemoteSequence(DB_TABLE(USER_GROUP_SEQUENCE))) {
				db_init = false;
				return;
			}
		}
		if (RemoteSequenceExist(DB_TABLE(FEATURE_SEQUENCE)) == 0) {
			Info() << "On create sequence:" << DB_TABLE(FEATURE_SEQUENCE);
			if (createRemoteSequence(DB_TABLE(FEATURE_SEQUENCE))) {
				db_init = false;
				return;
			}
		}
		if (RemoteTableExist(DB_TABLE(RECODE))==0) {
			Info() << "On create table:" << DB_TABLE(RECODE);
			if (createRemoteTable(DB_TABLE(RECODE), "ID INTEGER default 0 not null,TIME integer default 0 not null, OPER integer default 0 not null")) {
				db_init = false;
				return;
			}
			
		}
		if (RemoteTableExist(DB_TABLE(FEATURE)) == 0) {
			Info() << "On create table:" << DB_TABLE(FEATURE);
			if (createRemoteTable(DB_TABLE(FEATURE), "ID INTEGER default 0 primary key,FACE_TOKEN VARCHAR(128) DEFAULT '' NOT NULL,GROUP_ID VARCHAR(128) DEFAULT '' NOT NULL,USER_ID VARCHAR(128) DEFAULT '' NOT NULL, FEATURE CLOB DEFAULT '' NOT NULL,FACE_ID INTEGER DEFAULT 0 NOT NULL,CREATE_TIME INTEGER DEFAULT 0 NOT NULL,UPDATE_TIME INTEGER DEFAULT 0 NOT NULL")) {
				db_init = false;
				return;
			}
		}
		
		if (RemoteTableExist(DB_TABLE(USER))==0) {
			Info() << "On create table:" << DB_TABLE(USER);
			if (createRemoteTable(DB_TABLE(USER), "ID INTEGER default 0 primary key,USER_ID VARCHAR(128) DEFAULT '' NOT NULL,	GROUP_ID VARCHAR(128) DEFAULT '' NOT NULL,USER_INFO VARCHAR(256) DEFAULT '', CREATE_TIME INTEGER DEFAULT 0 NOT NULL,UPDATE_TIME INTEGER DEFAULT 0 NOT NULL")) {
				db_init = false;
				return;
			}
		}
		if (RemoteTableExist(DB_TABLE(USER_GROUP)) == 0) {
			Info() << "On create table:" << DB_TABLE(USER_GROUP);
			if (createRemoteTable(DB_TABLE(USER_GROUP), "ID INTEGER default 0 primary key,GROUP_ID VARCHAR(128) DEFAULT '' NOT NULL, CREATE_TIME INTEGER DEFAULT 0 NOT NULL,UPDATE_TIME INTEGER DEFAULT 0 NOT NULL")) {
				db_init = false;
				return;
			}
		}
	}
	catch (otl_exception &p) {
		Critical()<< QString::fromUtf8((char*)p.msg) << "On query: "<<p.stm_text<< "Cause by "<<p.var_info;  // print out error message
		db_init = false;
		return;
	}
	switch (LocalTableExist(TABLE(TIMESTAMP))) {
		case 0:
			Debug() << "on create " << TABLE(TIMESTAMP);
			if (createLocalTable(TABLE(TIMESTAMP), "IDX integer default 1 not null primary key,TIME integer default 0 not null") != SQLITE_OK) {
				Critical() << sqlite.ErrMsg();	
				db_init = false;
				return;
			}
			else {
				if (insertLocalDB(FORMAT_2("%1 (%2)", TABLE(TIMESTAMP), "IDX,TIME"), "1,0", "") != SQLITE_OK) {
					Critical() << sqlite.ErrMsg();
					db_init = false;
					return;
				}
			}
			break;
		case 1:
			break;
		default:
			Critical() << "TOO much table TIMESTAMP in sqlite";
			db_init = false;
			return;
		}
	switch (LocalTableExist(TABLE(USER))) {
	case 0:
		Debug() << "on create " << TABLE(USER);
		if (createLocalTable(TABLE(USER), "ID INTEGER auto_increment PRIMARY KEY,USER_ID VARCHAR(128) DEFAULT '' NOT NULL,	GROUP_ID VARCHAR(128) DEFAULT '' NOT NULL,USER_INFO VARCHAR(256) DEFAULT '' NOT NULL, CREATE_TIME INTEGER DEFAULT 0 NOT NULL,UPDATE_TIME INTEGER DEFAULT 0 NOT NULL") != SQLITE_OK) {
			Critical() << sqlite.ErrMsg();
			db_init = false;
			return;
		}
		break;
	case 1:
		break;
	default:
		Critical() << "TOO much table USER in sqlite";
		db_init = false;
		return;
	}
	switch (LocalTableExist(TABLE(USER_GROUP))) {
	case 0:
		Debug() << "on create " << TABLE(USER_GROUP);
		if (createLocalTable(TABLE(USER_GROUP), "ID INTEGER auto_increment PRIMARY KEY, GROUP_ID VARCHAR(128) DEFAULT '' NOT NULL, CREATE_TIME INTEGER DEFAULT 0 NOT NULL, UPDATE_TIME INTEGER DEFAULT 0 NOT NULL") != SQLITE_OK) {
			Critical() << sqlite.ErrMsg();
			db_init = false;
			return;
		}
		break;
	case 1:
		break;
	default:
		Critical() << "TOO much table USER_GROUP in sqlite";
		db_init = false;
		return;
	}
	switch (LocalTableExist(TABLE(FEATURE))) {
	case 0:
		Debug() << "on create " << TABLE(FEATURE);
		if (createLocalTable(TABLE(FEATURE), "ID INTEGER auto_increment PRIMARY KEY,FACE_TOKEN VARCHAR(128) DEFAULT '' NOT NULL,GROUP_ID VARCHAR(128) DEFAULT '' NOT NULL,USER_ID VARCHAR(128) DEFAULT '' NOT NULL, FEATURE CLOB DEFAULT '' NOT NULL,FACE_ID INTEGER DEFAULT 0 NOT NULL,CREATE_TIME INTEGER DEFAULT 0 NOT NULL,UPDATE_TIME INTEGER DEFAULT 0 NOT NULL") != SQLITE_OK) {
			Critical() << sqlite.ErrMsg();
			db_init = false;
			return;
		}
		break;
	case 1:
		break;
	default:
		Critical() << "TOO much table FEATURE in sqlite";
		db_init = false;
		return;
	}
		sqlitetimestamp =getLocalDBTimeStamp();
		db_init = true;
		Info() << "DB Initialize successfully";
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