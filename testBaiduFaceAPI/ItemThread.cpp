#include "ItemThread.h"
#include<qtimer.h>
ItemThread::ItemThread(QWidget*parent, QString n) :  QThread(parent),sec(0), min(0), hour(0),name(n), isStop(false)
{
	act = new ItemThreadAction(this);
	timer = new QTimer(this);
	setActionText();
	timer->start(1000);
	connect(timer, SIGNAL(timeout()), this, SLOT(slotTimeout()));
}
ItemThread::~ItemThread() {
	timer->stop();
}
void ItemThread::stop()
{
	QMutexLocker locker(&mutex);
	isStop = true;
	requestInterruption();
}

void ItemThread::Start()
{
	QMutexLocker locker(&mutex);
	isStop = false;
	start();
}
void ItemThread::slotTimeout()
{
	if (++sec >= 60) {
		sec -= 60;
		if (++min >= 60) {
			min -= 60;
			++hour;
		}
	}
	setActionText();
}


ItemThreadAction::ItemThreadAction(ItemThread * parent, const QString & p):QAction(p,parent)
{
}
