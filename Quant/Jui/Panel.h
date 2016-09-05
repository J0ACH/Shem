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
  class Panel : public QDockWidget
  {
    Q_OBJECT
      Q_PROPERTY(QColor colorHeader READ getColorHeader WRITE setColorHeader)
      Q_PROPERTY(QColor colorBackground READ getColorBackground WRITE setColorBackground)
      Q_PROPERTY(QColor colorTitle READ getColorTitle WRITE setColorTitle)
      Q_PROPERTY(QFont fontTitle READ getFontTitle WRITE setFontTitle)
      Q_PROPERTY(QFont fontBig READ getFontBig WRITE setFontBig)
      Q_PROPERTY(QFont fontSmall READ getFontSmall WRITE setFontSmall)

  public:
    Panel(QWidget *parent = 0);
    ~Panel();

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

    QFont getFontTitle();
    QFont getFontBig();
    QFont getFontSmall();

    void setPanelAllowedSides(Qt::DockWidgetAreas);

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
    QFont fontTitle, fontBig, fontSmall;
  };
}

#endif // PANEL
