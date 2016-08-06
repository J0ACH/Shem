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
      Q_PROPERTY(QColor colorHeader READ getColorHeader WRITE setColorHeader)
      Q_PROPERTY(QColor colorBackground READ getColorBackground WRITE setColorBackground)
      Q_PROPERTY(QColor colorTitle READ getColorTitle WRITE setColorTitle)
      Q_PROPERTY(QFont fontTitle READ getFontTitle WRITE setFontTitle)
      Q_PROPERTY(QFont fontBig READ getFontBig WRITE setFontBig)
      Q_PROPERTY(QFont fontSmall READ getFontSmall WRITE setFontSmall)
     // Q_PROPERTY(QFont fontConsole READ getFontConsole WRITE setFontConsole)

  public:
    PanelNEW(QWidget *parent = 0);
    ~PanelNEW();

    void setTitle(QString);

    void setColorBackground(QColor);
    void setColorHeader(QColor);
    void setColorTitle(QColor);

    QColor getColorHeader();
    QColor getColorBackground();
    QColor getColorTitle();

    void setFontTitle(QFont);
    void setFontBig(QFont);
    void setFontSmall(QFont);
   // void setFontConsole(QFont);
    
    QFont getFontTitle();
    QFont getFontBig();
    QFont getFontSmall();
   // QFont getFontConsole();

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

    QColor colorHeader, colorBackground, colorTitle;
    QFont fontTitle, fontBig, fontSmall; // , fontConsole;
  };

  ///////////////////////////////////////////////////////////////////////////

  // nize bude odstraneno

  
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
