#pragma execution_character_set("utf-8")
#ifndef SPHDOCKWIDGT_H
#define SPHDOCKWIDGT_H

#include "SphParameters.h"
#include "PathConfig.h"
#include "SphThread.h"
#include "InoutSetting.h"
#include "TaskOperation.h"
#include <QWidget>
#include <QFileDialog>

namespace Ui {
class SphDockWidget;
}

class SphDockWidget : public QWidget
{
    Q_OBJECT
    typedef QWidget Superclass;

public:
    explicit SphDockWidget(QWidget *parent = nullptr);
    ~SphDockWidget() override;
    QList<QStringList> getLoadMultiDataPath(QString basePath, QString preFileName, QString suffix);

private slots:
    void showText(QString);
    void showState(QString);
    void showProgress(int);
    void showEndtime(QString);
    void sphStateChange(StateType);
    void sphOperation(int);
    void moveToEnd();

signals:
    void showTaskOperation();

public:
    Ui::SphDockWidget *ui; 
    QString getpath();

private:
  Q_DISABLE_COPY(SphDockWidget)
  class pqInternals;
  pqInternals* Internals;
  friend class pqInternals;
  SphParameters *sphParam;
  PathConfig *pathConfig;
  SphThread *sphThread;
  QPointer<InoutSetting> inoutSetting;
  QPointer<TaskOperation> taskOperation;

  void showParam();
  void saveParam();
  void widgetConstraint();
  void widgetRegExpValidat();
  void btnEvent();
  QString on_OpenModelFilePushButton_clicked();
};

#endif // SPHDOCKWIDGT_H
