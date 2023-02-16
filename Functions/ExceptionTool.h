#pragma execution_character_set("utf-8")
#include <iostream>
using namespace std;
#include "SphException.h"

#include <QObject>

class ExceptionTool : public QObject
{
    Q_OBJECT
public:
    explicit ExceptionTool(QObject *parent = nullptr);
    static QString ReadCSV(QString exceptionCode);
    static string getExceptionLine(QString exceptionCode);
    static SphException* getSphException(QString exceptionCode);

signals:

};
