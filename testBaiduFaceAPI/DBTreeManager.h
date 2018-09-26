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
	//����һ��Ⱥ��ڵ�
	DBTreeManagerItem(DBTreeManager*parent, const QString & name = QString());			
	//����һ���û��ڵ�
	DBTreeManagerItem(DBTreeManagerItem*parent, const QString & name );					
	//����һ���ļ��ڵ�
	DBTreeManagerItem(DBTreeManagerItem*parent, const QString & name,const QString&token); 

	~DBTreeManagerItem() {}
	ExpandState expandState() { return m_expandstate; }
	void setBelongState(const ItemBelongState& state) { belong = state; }
	ItemType type();
	DBTreeManager*parent() { return par; }
public slots:
	// ɾ��ѡ����� δ����
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
	// �ٶ�����ʶ��ģ�������
	void setSetting(Setting&setting);
	// ��Ӧ�ڵ㱻�����Ķ���
	void itemClicked(QTreeWidgetItem *item, int column);
	/*  
	�ٶ����������ز���
	*/
	// ��ȡȺ���б�
	QStringList ReadGroup();
	// ��ȡ�û��б� 
	QStringList ReadPerson(const char* groupname);
	// ��ȡ�û���Ϣ
	const char* ReadUserInfo(const char* group, const char* user);
	// ����Ⱥ��
	const char* AddGroup(const char* group);
	// �����û�
	const char* AddUser(const char* group,const char* user, const char* img, const char* user_info);

	// �ȶ�����Base64��ͼƬ
	QString CompareBase64(const char*, const char*);
	// ͼƬ�������Ƚ�
	QString CompareWithGroup(const char*imgbase64, const char*group,const char* user=nullptr);
	const char* deleteGroup(const char*groupname);
	const char* deleteUser(const char*groupname,const char*username);
	int createFace(const char*user, const char* group, const char* filepath, const char* filename);
	int createGroup(const char*groupname);
	void expandItem(QTreeWidgetItem*);
	// ����Ӧ�ڵ�չ��
	bool groupExist(const QString& str);
	bool userExist(const QString&group,const QString&user);
	QMenu* menu() { return m_menu; }
	BaiduFaceApi* BaiduApi() { return baiduApi; }
	Ui::DBManager& UI() { return ui; }
	static std::string  parse(const std::string&fileaname);
	static std::string deParse(const std::string&filename);
	int getFileInfo(const QString &str, QString&filename, QString &filetoken);
/*
*  ���ݿ����ز���
*  remote Ϊoracle���ݿ�
*  local  Ϊsqlite3���ݿ�
*/
    //�ж����ݿ��Ƿ��ʼ���ɹ�
	bool DBInitialized() { return db_init; }
	//��ʼ�����ݿ�
	void initDB();
	// ��oracle ���ݿ��ȡ��ǰʱ���
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
	// ͬ��Զ�̺ͱ������ݿ�
	void synchronization(qint64 local, qint64 remote);
public slots:
	// ɾ����ѡ�еĽڵ�
	void delelteSelection();
	// ��ָ���ڵ��������ӽڵ�
	void addItem();
	// ѡ��״̬�ı�����ز���������ɾ����Ϊѡ��ڵ���ӽڵ�
	void changeSelection(QTreeWidgetItem*);
	// �������ڵ�
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
	void initBaiduFaceApi();//�԰ٶ�����ʶ��api��ʼ��
	
    //��ȡstart��ʼ��10��Ⱥ��
	int Read10Group(QStringList&, int start);
	// ��ȡstart��ʼ��100���û�
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