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
	void deleteGroup(const char*groupname);
	void deleteUser(const char*groupname,const char*username);
	int createFace(const char*user, const char* group, const char* filepath, const char* filename);
	int createGroup(const char*groupname);
	void expandItem(QTreeWidgetItem*);
	// ����Ӧ�ڵ�չ��
	bool groupExist(const QString& str);
	QMenu* menu() { return m_menu; }
	BaiduFaceApi* BaiduApi() { return baiduApi; }
	Ui::DBManager& UI() { return ui; }
	static std::string  parse(const std::string&fileaname);
	static std::string deParse(const std::string&filename);
	int getFileInfo(const QString &str, QString&filename, QString &filetoken);

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
private:
	void initBaiduFaceApi();//�԰ٶ�����ʶ��api��ʼ��
	void initDB();//��ʼ�����ݿ�
    //��ȡstart��ʼ��10��Ⱥ��
	int Read10Group(QStringList&, int start);
	// ��ȡstart��ʼ��100���û�
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