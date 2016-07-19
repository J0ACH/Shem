#ifndef PANEL_H
#define PANEL_H

#include <QDockWidget>
#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QEvent>

#include "Button.h"
#include "Edges.h"


namespace Jui
{
  // const enum PanelSide{ LEFT, RIGHT };

  class PanelNEW : public QDockWidget
  {
    Q_OBJECT


  public:
    PanelNEW(QWidget *parent = 0);
    ~PanelNEW();

    void setTitle(QString);
    void setFontTitle(QFont);
    void setColorTitle(QColor);
    void setColorBackground(QColor);
    void setColorHeader(QColor);
    void setColorNormal(QColor);
    void setColorOver(QColor);
    void setColorActive(QColor);

    void setPanelAllowedSides(Qt::DockWidgetAreas);
    //void setPanelSide(Qt::DockWidgetArea side);

    public slots:
    void onSwitchVisible();
    void onClose();

  signals:
    void actClosed();

  protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

  private:
    QString title;
    Button *closeButton;

    bool isMoveing;
    QPoint cursorScreen, cursorCanvan, originCanvanScreen;

    QColor colorHeader, colorBackground, colorText;
    QFont fontTitle;

    //Qt::DockWidgetArea allowedSide;
    //PanelSide allowedSide;
  };

  class Panel : public QWidget
  {
    Q_OBJECT

  public:
    Panel(QWidget *parent = 0);
    ~Panel();

    QRect bounds();

    void initControl();
    void setTitle(QString);
    void setFontTitle(QFont);
    void setColorBackground(QColor);
    void setColorTitle(QColor);

    void setEdgeControler(EdgeControler::Direction, bool);

    public slots:
    //void onConfigData(QMap<QString, QVariant*> config);
    //void onCustomize();

  signals :
    void resizeAct();

  protected:
    void mousePressEvent(QMouseEvent *mouseEvent);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

  private:
    QString title;

    QColor colorPanelBackground, colorNormal, colorOver, colorActive, colorText;
    QFont fontTitle;

    Button *closeButton;
    Edges *edges;


  };



}

#endif // PANEL