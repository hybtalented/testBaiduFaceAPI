#pragma once
#include<qdebug.h>
#define Debug() qDebug()<<qPrintable(QString("In FILE \"%1\", FUNCTION %2, LINE %3 :").arg(__FILE__).arg(__FUNCTION__).arg(__LINE__))
#define Info() qInfo()<<qPrintable(QString("In FILE \"%1\", FUNCTION %2, LINE %3 :").arg(__FILE__).arg(__FUNCTION__).arg(__LINE__))
#define Warning() qWarning()<<qPrintable(QString("In FILE \"%1\", FUNCTION %2, LINE %3 :").arg(__FILE__).arg(__FUNCTION__).arg(__LINE__))
#define Critical() qCritical()<<qPrintable(QString("In FILE \"%1\", FUNCTION %2, LINE %3 :").arg(__FILE__).arg(__FUNCTION__).arg(__LINE__))
#define Fatal(msg,...) qFatal(msg,__VA_ARGS__)
