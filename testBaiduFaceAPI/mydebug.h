#pragma once
#include<qdebug.h>
#define Debug() qDebug()<<QString("In FILE \"%1\", FUNCTION %2, LINE %3 :").arg(__FILE__).arg(__FUNCTION__).arg(__LINE__)
#define Info() qInfo()<<QString("In FILE \"%1\", FUNCTION %2, LINE %3 :").arg(__FILE__).arg(__FUNCTION__).arg(__LINE__)
#define Warning() qWarning()<<QString("In FILE \"%1\", FUNCTION %2, LINE %3 :").arg(__FILE__).arg(__FUNCTION__).arg(__LINE__)
#define Critical() qCritical()<<QString("In FILE \"%1\", FUNCTION %2, LINE %3 :").arg(__FILE__).arg(__FUNCTION__).arg(__LINE__)
#define Fatal(msg,...) qFatal(msg,__VA_ARGS__)
