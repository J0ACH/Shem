#ifndef CANVAN_H
#define CANVAN_H

#include <QMainWindow>
#include <QMenuBar>
#include <QDockWidget>
#include <QtWidgets/QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QFontDatabase>
#include <QMap>
#include <QDesktopWidget>


#include "Console.h"
#include "Button.h"
#include "Panel.h"

namespace Jui
{

  class CanvanNEW : public QMainWindow
  {
    Q_OBJECT

  public:
    CanvanNEW(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~CanvanNEW();

    void addPanel(PanelNEW *panel, QString name);
    QWidget* getPanel(QString name);
    void setColorHeader(QColor);

    public slots:
    void onCanvanClosed();
    void onCanvanMaximized();
    void onCanvanMinimized();

  protected:
    void resizeEvent(QResizeEvent *resizeEvent);
    void paintEvent(QPaintEvent *paintEvent);
    bool eventFilter(QObject *target, QEvent *event);

  private:
    void initControl();
    int headerSize;
    QMenuBar *menuBar;
    QMenu *fileMenu;
    QWidget *screen;
    QMap<QString, PanelNEW*> *mapPanels;
    Button *closeButton, *maximizeButton, *minimizeButton;

    bool isMoveing;
    QPoint cursorScreen, cursorCanvan, originCanvanScreen;

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
