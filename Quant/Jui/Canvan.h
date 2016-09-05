#ifndef CANVAN_H
#define CANVAN_H

#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QDockWidget>
#include <QtWidgets/QMainWindow>
#include <QTimer>
#include <QFontDatabase>
#include <QMap>
#include <QDesktopWidget>

#include "Console.h"
#include "Button.h"
#include "Panel.h"

namespace Jui
{

  class CanvanNEW_MenuBar : public QMenuBar
  {
    Q_OBJECT
      Q_PROPERTY(QColor colorBackground READ getColorBackground WRITE setColorBackground)

  public:
    CanvanNEW_MenuBar(QWidget *parent = 0);
    ~CanvanNEW_MenuBar();

    void setColorBackground(QColor);
    QColor getColorBackground();

  protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

  private:
    QColor colorBackground;
  };

  ///////////////////////////////////////////////////////////////////////////

  class CanvanNEW_StatusBar : public QStatusBar
  {
    Q_OBJECT
      Q_PROPERTY(QColor colorBackground READ getColorBackground WRITE setColorBackground)

  public:
    CanvanNEW_StatusBar(QWidget *parent = 0);
    ~CanvanNEW_StatusBar();

    void setColorBackground(QColor);
    QColor getColorBackground();

  protected:
    void paintEvent(QPaintEvent *event);

  private:
    QColor colorBackground;
  };

  ///////////////////////////////////////////////////////////////////////////

  class CanvanNEW_ToolBar : public QToolBar
  {
    Q_OBJECT
      Q_PROPERTY(QColor colorBackground READ getColorBackground WRITE setColorBackground)

  public:
    CanvanNEW_ToolBar(QWidget *parent = 0);
    ~CanvanNEW_ToolBar();

    void addButton(QString name, QImage icon, const QObject* reciver, const char* member);
    Button* getButton(QString name);

    void setColorBackground(QColor);
    QColor getColorBackground();

  protected:
    void paintEvent(QPaintEvent *event);

  private:
    QColor colorBackground;
    QMap<QString, Button*> buttonsList;
  };

  ///////////////////////////////////////////////////////////////////////////

  class CanvanNEW : public QMainWindow
  {
    Q_OBJECT
      Q_PROPERTY(QColor colorBackground READ getColorBackground WRITE setColorBackground)

  public:
    CanvanNEW(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~CanvanNEW();

    void addPanel(Panel *panel, QString name, Qt::DockWidgetArea startPosition = Qt::DockWidgetArea::RightDockWidgetArea);
    QWidget* getPanel(QString name);
    void addButtonBar(CanvanNEW_ToolBar *buttonBar, QString name, Qt::ToolBarArea startPosition = Qt::ToolBarArea::BottomToolBarArea);
    CanvanNEW_ToolBar* getButtonBar(QString name);
    QWidget* getStaustBar();

    void setColorBackground(QColor);
    QColor getColorBackground();

    public slots:
    void onEmitClose();
    void onCanvanClose();
    void onCanvanMaximized();
    void onCanvanMinimized();

  signals:
    void actClose();

  protected:
    void resizeEvent(QResizeEvent *resizeEvent);
    void paintEvent(QPaintEvent *paintEvent);
    bool eventFilter(QObject *target, QEvent *event);

  private:
    void initControl();
    int headerSize, tailSize;
    QWidget *screen;
    CanvanNEW_MenuBar *menuBar;
    CanvanNEW_StatusBar *statusBar;
    QMenu *fileMenu;
    QMap<QString, Panel*> *mapPanels;
    QMap<QString, CanvanNEW_ToolBar*> *mapButtonBars;
    Button *closeButton, *maximizeButton, *minimizeButton;

    bool isMoveing;
    QPoint cursorScreen, cursorCanvan, originCanvanScreen;

    QColor colorBackground;
  };
  }

#endif // CANVAN_H
