#pragma once
#include<exception>
#include <qtreewidget.h>
#include<baidu_face_api.h>
#include<json/json.h>
#include"ui_DBTreeManager.h"
#define OTL_ORA11G_R2
#define OTL_BIGINT __int64
#include"otlv4.h"
#include"SQLite.h"
class testBaiduFaceAPI;
class DBTreeManager;
class DBTreeManagerItem :
	public QObject,public QTreeWidgetItem
{
	Q_OBJECT
public:
	enum ItemType { group, user, image,UnKnow };
	enum ExpandState { Expanded, UnExpanded, NoChild };
	enum ItemBelongState { file=0x1, DB=0x2,BOTH=file|DB};
	//创建一个群组节点
	DBTreeManagerItem(DBTreeManager*parent, const QString & name = QString());			
	//创建一个用户节点
	DBTreeManagerItem(DBTreeManagerItem*parent, const QString & name );					
	//创建一个文件节点
	DBTreeManagerItem(DBTreeManagerItem*parent, const QString & name,const QString&token); 

	~DBTreeManagerItem() {}
	ExpandState expandState() { return m_expandstate; }
	void setBelongState(const ItemBelongState& state) { belong = state; }
	ItemType type();
	DBTreeManager*parent() { return par; }
public slots:
	// 删除选择的项 未测试
	void Delete();		
private:
	void deleteChild();
	DBTreeManager *par;
	ExpandState  m_expandstate;
	ItemBelongState belong;
};

struct Setting {
	bool isCheckQuality;
	int illum_thr;
	float blur_thr;
	float occlu_thr;
	int eulur_angle_x, eulur_angle_y, eulur_angle_z;
	float notFace_thr;
	int min_face_size;
};

#define FORMAT_1(cstr,arg1) QString(cstr).arg(arg1).toStdString().c_str()
#define FORMAT_2(cstr,arg1,arg2) QString(cstr).arg(arg1).arg(arg2).toStdString().c_str()
#define FORMAT_3(cstr,arg1,arg2,arg3) QString(cstr).arg(arg1).arg(arg2).arg(arg3).toStdString().c_str()
#define FORMAT_4(cstr,arg1,arg2,arg3,arg4) QString(cstr).arg(arg1).arg(arg2).arg(arg3).arg(arg4).toStdString().c_str()
#define FORMAT_5(cstr,arg1,arg2,arg3,arg4,arg5) QString(cstr).arg(arg1).arg(arg2).arg(arg3).arg(arg4).arg(arg5).toStdString().c_str()
#define FORMAT_6(cstr,arg1,arg2,arg3,arg4,arg5,arg6) QString(cstr).arg(arg1).arg(arg2).arg(arg3).arg(arg4).arg(arg5).arg(arg6).toStdString().c_str()
#define DB_PREFIX "TEST_"
#define DB_TABLE(table)  ((QString(DB_PREFIX)+#table).toStdString().c_str())
#define TABLE(table) #table
#define CREATESEQUENCESTR "create sequence %1 %2"
#define CREATETABLESTR "create table %1 (%2) %3"
#define DELETESTR "delete from %1 %2"
#define SELECTSTR  "select %1 from %2 %3"
#define  INSERTSTR  "insert into %1 values (%2) %3"
#define UPDATESTR  "update %1 set %2 %3"
#define ALTERSTR  "alter table %1 %2 %3"
#define ALTADD  "add"
#define ALTDROP "drop"
#define ALTMODIFY "modify"
#define ALTRENAME "rename"
#define ALTRENAMETO "rename to"




class DBTreeManager :
	public QWidget
{
	Q_OBJECT
public:
	DBTreeManager(QWidget * parent)throw(std::exception);
	~DBTreeManager()noexcept(true);
	const char* const oracle_login_data = "face/face@sale";
	const char* const face_table_name = "FACEWAREHOUSE_TEST";
	const char* const face_db_addr = "db/face.db";
	void setMenu(QMenu*);
	// 百度人脸识别模块的配置
	void setSetting(Setting&setting);
	// 对应节点被点击后的动作
	void itemClicked(QTreeWidgetItem *item, int column);
	/*  
	百度人脸库的相关操作
	*/
	// 读取群组列表
	QStringList ReadGroup();
	// 读取用户列表 
	QStringList ReadPerson(const char* groupname);
	// 读取用户信息
	const char* ReadUserInfo(const char* group, const char* user);
	// 增加群组
	const char* AddGroup(const char* group);
	// 增加用户
	const char* AddUser(const char* group,const char* user, const char* img, const char* user_info);

	// 比对两张Base64的图片
	QString CompareBase64(const char*, const char*);
	// 图片与人类库比较
	QString CompareWithGroup(const char*imgbase64, const char*group,const char* user=nullptr);
	const char* deleteGroup(const char*groupname);
	const char* deleteUser(const char*groupname,const char*username);
	int createFace(const char*user, const char* group, const char* filepath, const char* filename);
	int createGroup(const char*groupname);
	void expandItem(QTreeWidgetItem*);
	// 将对应节点展开
	bool groupExist(const QString& str);
	bool userExist(const QString&group,const QString&user);
	QMenu* menu() { return m_menu; }
	BaiduFaceApi* BaiduApi() { return baiduApi; }
	Ui::DBManager& UI() { return ui; }
	static std::string  parse(const std::string&fileaname);
	static std::string deParse(const std::string&filename);
	int getFileInfo(const QString &str, QString&filename, QString &filetoken);
/*
*  数据库的相关操作
*  remote 为oracle数据库
*  local  为sqlite3数据库
*/
    //判断数据库是否初始化成功
	bool DBInitialized() { return db_init; }
	//初始化数据库
	void initDB();
	// 从oracle 数据库获取当前时间戳
	qint64 getCurrentTimeStamp();
	qint64 getLocalDBTimeStamp();
	qint64 getRemoteDBtimeStamp();
	qint64 getSequenceVal(const char* sequence);
	bool setLocalTimeStamp(qint64);
	bool setRemoteDBtimeStamp(qint64);
	bool addLocalDBUSER(qint64 ID,const char* USER_ID,const char*GROUP_ID,const char* USER_INFO, qint64 CREATE_TIME);
	bool addRemoteDBUSER(qint64 ID, const char* USER_ID, const char*GROUP_ID, const char* USER_INFO, qint64 CREATE_TIME);
	bool addLocalDBUSER_GROUP(qint64 ID, const char*GROUP_ID, qint64 CREATE_TIME);
	bool addRemoteDBUSER_GROUP(qint64 ID, const char*GROUP_ID, qint64 CREATE_TIME);
	bool addLocalDBFEATURE(qint64 ID, const char* FACE_TOKEN, const char*GROUP_ID, const char* USER_ID, const char* FAUTURE, qint64 CREATE_TIME);
	bool addRemoteDBFEATURE(qint64 ID, const char* FACE_TOKEN, const char*GROUP_ID, const char* USER_ID, const char* FAUTURE, qint64 CREATE_TIME);
	enum RECODE_OPER
	{
		user=0x1,group=0x2,feature=0x4,del=0x8,ins=0x10,upd=0x20,table=user|group|feature,oper=del|ins|upd
	};
	inline const char* RECODE(qint64 id, RECODE_OPER table) { return FORMAT_2("where ID=%1 and BITAND(OPER,%2)=%2 ", id, table); }
	bool addRemoteDBRECODE(qint64 id, qint64 time, int oper);
	bool findRECODE(qint64 id, RECODE_OPER table);
	SQLITE_RET selectLocalDB(const char* content, const char* table, const char* addtion_option);
	int insertLocalDB(const char* table, const char* content, const char*addtion_option);
	int insertRemoteDB( const char* table,  const char* content, const char*addtion_option);
	int updateLocalDB(const char* table, const char* columns, const char*addtion_option);
	int updateRemoteDB( const char* table, const char* columns, const char*addtion_option);
	int deleteLocalDB(const char* table, const char*addtion_option);
	int deleteRemoteDB(const char* table, const char*addtion_option);
	int alterLocalDB( const char* table, const char* option, const char*content);
	int alterRemoteDB( const char* table, const char* option, const char*content);
	int createLocalTable(const char* table, const char*coldefs, const char*addtion_option=0);
	int createRemoteTable(const char* table, const char*coldefs, const char*addtion_option=0);
	int createRemoteSequence(const char* sequence);
	int LocalTableExist(const char* table);
	int RemoteTableExist(const char* table);
	int RemoteSequenceExist(const char * sequence);
	// 同步远程和本地数据库
	void synchronization(qint64 local, qint64 remote);
public slots:
	// 删除掉选中的节点
	void delelteSelection();
	// 在指定节点下增加子节点
	void addItem();
	// 选择状态改变后的相关操作，包括删除掉为选择节点的子节点
	void changeSelection(QTreeWidgetItem*);
	// 重命名节点
	void renameItem();
	void slotItemDoubleClicked(QTreeWidgetItem*, int);
	void slotItemClicked(QTreeWidgetItem*, int);
signals:
	void PathChanged();
	void PathChangeActionClicked();
	void SearchButtonClicked();
	void setPrograssValue(int value);
	void setPrograssRange(int min, int max);
private:
	void initBaiduFaceApi();//对百度人脸识别api初始化
	
    //读取start开始的10个群组
	int Read10Group(QStringList&, int start);
	// 读取start开始的100个用户
	int Read100Person(const char* groupname, int start, QStringList&);
	QStringList getBaiduList(const char*, const char*);
	QStringList getFlist(const char* group,const char* user);
	bool db_init;
	bool change;
	QMenu* m_menu;
	Json::Reader reader;
	BaiduFaceApi* baiduApi;
	QLineEdit* current_path;
	Ui::DBManager ui;
	otl_connect db;
	SQLite sqlite;
	QMutex dbmutex;
};
void deleteDir(const QString& dirname);
void deleteFile(const QString& filename);