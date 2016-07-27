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

    void addPanel(PanelNEW *panel, QString name, Qt::DockWidgetArea startPosition = Qt::DockWidgetArea::RightDockWidgetArea);
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
    QMap<QString, PanelNEW*> *mapPanels;
    QMap<QString, CanvanNEW_ToolBar*> *mapButtonBars;
    Button *closeButton, *maximizeButton, *minimizeButton;

    bool isMoveing;
    QPoint cursorScreen, cursorCanvan, originCanvanScreen;

    QColor colorBackground;
  };



  class Canvan : public QWidget
  {
    Q_OBJECT

  public:

    Canvan(QWidget *parent = 0);
    ~Canvan();

    void initControl();
    void setEdgeControler(EdgeControler::Direction, bool);

    void connectWindow(QWidget*);

    void addScreen(QWidget *inScreen);

    QWidget *win;
    QWidget *header, *screen, *tail;

    Console *mConsole;

    void setHeaderHeight(int);
    void setTailHeight(int);
    void setTitle(QString titleName);
    void setLogo(QImage);
    void setVersion(QString);

    void setColorHeader(QColor);
    void setColorNormal(QColor);
    void setColorOver(QColor);
    void setColorActive(QColor);
    void setColorText(QColor);

  signals:
    void resizeAct();
    void resizeScreenAct();
    void closeAct();

    void actConfigData(QMap<QString, QVariant*> config);

    void consolePrintAct(QString, QColor, bool);

    public slots:
    void print(QString, QColor);
    void println(QString, QColor);

    void closeCanvan();
    void minimizeCanvan();
    void maximizeCanvan();

    void fitScreen();
    //void onConfigData(QMap<QString, QVariant*> config);
    //void onCustomize();

  protected:
    void resizeEvent(QResizeEvent *resizeEvent);
    void mousePressEvent(QMouseEvent *mouseEvent);
    void mouseReleaseEvent(QMouseEvent *mouseEvent);
    void mouseMoveEvent(QMouseEvent *mouseEvent);
    void paintEvent(QPaintEvent *paintEvent);
    //void customizeEvent(QDynamicPropertyChangeEvent *event);

  private:
    bool isPressed;
    bool showScreen;

    int headerSize, tailSize;

    Button *closeButton, *maximizeButton, *minimizeButton;
    Edges *edges;
    QMap<QString, QVariant*> config;

    QImage logo;
    //QLabel *title;
    QString version;

    QColor colorAppHeaderBackground, colorPanelBackground, colorNormal, colorOver, colorActive, colorText;
    QFont fontTextSmall, fontCode;

    QPoint *mCursorGlobal;
    QPoint *mCursorLocal;
    QPoint *mFrameOriginGlobal;

    QPoint canvanOrigin;
    QSize canvanSize;

  };

}

#endif // CANVAN_H
