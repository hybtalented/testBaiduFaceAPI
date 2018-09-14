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
	void setCount(const int* fn, const int* t) { filenum = fn, total = t; }
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
		int fn, t;
		if(filenum){
			fn = *filenum;
		}
		if (total) {
			t = *total;
		}
		act->setText((name + "(%4/%5)(%1h:%2m:%3s)").arg(hour).arg(min).arg(sec).arg(fn).arg(t));
	}
	short sec, min, hour;
	QTimer* timer;
	QString name;
	const int *filenum,  *total;
	ItemThreadAction * act;
};

