#pragma execution_character_set("utf-8")
#include "myMainWindow.h"
#include "ui_myMainWindow.h"

#include <pqApplicationCore.h>
#include <pqCategoryToolbarsBehavior.h>
#include <pqColorToolbar.h>
#include <pqVCRToolbar.h>
#include <pqCameraToolbar.h>
#include <pqLightToolbar.h>
#include <pqMainControlsToolbar.h>
#include <pqAnimationTimeToolbar.h>
#include <pqAxesToolbar.h>
#include <pqSetName.h>
#include <pqDeleteReaction.h>
#include <pqHelpReaction.h>
#include <pqLoadDataReaction.h>
#include <pqParaViewBehaviors.h>
#include <pqParaViewMenuBuilders.h>
#include <pqRepresentationToolbar.h>

//-----------------------------------------------------------------------------
class myMainWindow::pqInternals : public Ui::pqClientMainWindow
{
};

//-----------------------------------------------------------------------------
myMainWindow::myMainWindow()
  : Internals(new pqInternals())
{
  // Setup default GUI layout.
  this->Internals->setupUi(this);

  // Dock布局. Setup the dock window corners to give the vertical docks more room.
  this->setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
  this->setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

  // Dock：颜色面板. Setup color editor
  // Provide access to the color-editor panel for the application and hide it
  pqApplicationCore::instance()->registerManager(
    "COLOR_EDITOR_PANEL", this->Internals->colorMapEditorDock);
  this->Internals->colorMapEditorDock->hide();

  // 目录：：件. Create a custom file menu with only Open and close
  QList<QAction*> actionList = this->Internals->menu_File->actions();
  QAction* action = actionList.at(0);
  new pqLoadDataReaction(action);
  QObject::connect(
    actionList.at(1), SIGNAL(triggered()), QApplication::instance(), SLOT(closeAllWindows()));

  // 目录：：滤器. Build the filters menu
  pqParaViewMenuBuilders::buildFiltersMenu(*this->Internals->menuFilters, this);

  // Dock：：染列表浏览器. Setup the context menu for the pipeline browser.
  pqParaViewMenuBuilders::buildPipelineBrowserContextMenu(
    *this->Internals->pipelineBrowser->contextMenu());

  // Populate toolbars
#if 0
  pqParaViewMenuBuilders::buildToolbars(*this);
#else

  QToolBar* mainToolBar = new pqMainControlsToolbar(this);
  mainToolBar->layout()->setSpacing(0);
  this->addToolBar(Qt::TopToolBarArea, mainToolBar);

  // 工具栏：：olorTool.
  QToolBar* colorToolbar = new pqColorToolbar(this);
  colorToolbar->layout()->setSpacing(0);
  this->addToolBar(Qt::TopToolBarArea, colorToolbar);

  // 工具栏：：xesTool.
  QToolBar* axesToolbar = new pqAxesToolbar(this);
  axesToolbar->layout()->setSpacing(0);
  this->addToolBar(Qt::TopToolBarArea, axesToolbar);

  // 工具栏：：CRTool.
  QToolBar* vcrToolbar = new pqVCRToolbar(this);
  vcrToolbar->layout()->setSpacing(0);
  this->addToolBar(Qt::TopToolBarArea, vcrToolbar);

  // 工具栏：：nimationTimeToolbar. Current Time
  QToolBar* animationTimeToolbar = new pqAnimationTimeToolbar(this);
  animationTimeToolbar->layout()->setSpacing(0);
  this->addToolBar(Qt::TopToolBarArea, animationTimeToolbar);

  // 工具栏：：ameraToolbar.
  QToolBar* cameraToolbar = new pqCameraToolbar(this);
  cameraToolbar->layout()->setSpacing(0);
  this->addToolBar(Qt::TopToolBarArea, cameraToolbar);

  // 工具栏：：ightToolbar.
  QToolBar* lightToolbar = new pqLightToolbar(this);
  lightToolbar->layout()->setSpacing(0);
  this->addToolBar(Qt::TopToolBarArea, lightToolbar);

#endif

  // 目录：：iew. This must be setup after all toolbars and dockwidgets
  // have been created.
  pqParaViewMenuBuilders::buildViewMenu(*this->Internals->menuView, *this);

  // Dock：：性面板. Enable help from the properties panel.
  // This is not really working as the documentation is not built in this app
  QObject::connect(this->Internals->proxyTabWidget, &pqPropertiesPanel::helpRequested,
    &pqHelpReaction::showProxyHelp);

  // hook delete to pqDeleteReaction.
  QAction* tempDeleteAction = new QAction(this);
  pqDeleteReaction* handler = new pqDeleteReaction(tempDeleteAction);
  handler->connect(this->Internals->proxyTabWidget, SIGNAL(deleteRequested(pqProxy*)),
    SLOT(deleteSource(pqProxy*)));

  // Final step, define application behaviors. Since we want all ParaView
  // behaviors, we use this convenience method.
  new pqParaViewBehaviors(this, this);
}

//-----------------------------------------------------------------------------
myMainWindow::~myMainWindow() = default;
