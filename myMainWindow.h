/**
 * A main window class copied from SimpleParaView example
 * Make sure to propagate any changes mades
 */
#pragma execution_character_set("utf-8")

#ifndef myMainWindow_h
#define myMainWindow_h

#include <QMainWindow>
#include <QScopedPointer>

class myMainWindow : public QMainWindow
{
  Q_OBJECT
  typedef QMainWindow Superclass;

public:
  myMainWindow();
  ~myMainWindow() override;

private:
  Q_DISABLE_COPY(myMainWindow)

  class pqInternals;
  QScopedPointer<pqInternals> Internals;
};

#endif
