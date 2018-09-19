#pragma once
#include <qthread.h>
#include<qaction.h>
#include<qdialog.h>
class QTimer;
class ItemThread;
class ItemThreadAction :public QAction {
public:
	ItemThreadAction(ItemThread*parent, const QString& p = QString());
	ItemThread* Parent() {
		return cdb;
	}
private:
	ItemThread* cdb;
};

class ItemThread :
	public QThread
{
	Q_OBJECT
public:;
	ItemThread(QWidget*parent, QString n);
	QAction* getAction() { return act; }
	~ItemThread() {}
public slots:
	void processEnd() {
		emit processEnded(act);
	}
	void slotTimeout();
signals:
	void processEnded(ItemThreadAction*);
	void sendMessage(QString, QString, QString);
protected:
	virtual void setActionText() {
		act->setText((name + "(%1h:%2m:%3s)").arg(hour).arg(min).arg(sec));
	}
	short sec, min, hour;
	QTimer* timer;
	QString name;
	ItemThreadAction * act;
};

