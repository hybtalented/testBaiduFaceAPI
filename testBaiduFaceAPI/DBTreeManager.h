#pragma once
#include<exception>
#include <qtreewidget.h>
#include<baidu_face_api.h>
#include<json/json.h>
#include"ui_DBTreeManager.h"
#define OTL_ORA10G
#include"otlv4.h"

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
	void deleteGroup(const char*groupname);
	void deleteUser(const char*groupname,const char*username);
	int createFace(const char*user, const char* group, const char* filepath, const char* filename);
	int createGroup(const char*groupname);
	void expandItem(QTreeWidgetItem*);
	// 将对应节点展开
	bool groupExist(const QString& str);
	QMenu* menu() { return m_menu; }
	BaiduFaceApi* BaiduApi() { return baiduApi; }
	Ui::DBManager& UI() { return ui; }
	static std::string  parse(const std::string&fileaname);
	static std::string deParse(const std::string&filename);
	int getFileInfo(const QString &str, QString&filename, QString &filetoken);

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
private:
	void initBaiduFaceApi();//对百度人脸识别api初始化
	void initDB();//初始化数据库
    //读取start开始的10个群组
	int Read10Group(QStringList&, int start);
	// 读取start开始的100个用户
	int Read100Person(const char* groupname, int start, QStringList&);
	QStringList getBaiduList(const char*, const char*);
	QStringList getFlist(const char* group,const char* user);
	bool change;
	QMenu* m_menu;
	Json::Reader reader;
	BaiduFaceApi* baiduApi;
	QLineEdit* current_path;
	Ui::DBManager ui;
	otl_connect db;
};
void deleteDir(const QString& dirname);
void deleteFile(const QString& filename);