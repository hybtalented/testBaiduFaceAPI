#include "ItemThread.h"
#include<qtimer.h>
ItemThread::ItemThread(QWidget*parent, QString n) :  QThread(parent),sec(0), min(0), hour(0),name(n),filenum(nullptr),total(nullptr)
{

	act = new ItemThreadAction(this);
	timer = new QTimer(this);
	setActionText();
	timer->start(1000);
	connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
	connect(this, SIGNAL(finished()), this, SLOT(processEnd()));
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
