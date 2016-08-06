#ifndef QUANTPANELS_H
#define QUANTPANELS_H

#include "Panel.h"
#include "Text.h"

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QScrollArea>

using namespace Jui;

namespace QuantIDE
{

  // NETWORK PANEL //////////////////////////////////////////////////////////////////////////////////////

  class NetworkPanel : public PanelNEW
  {
    Q_OBJECT

  public:
    NetworkPanel(QWidget *parent);
    ~NetworkPanel();

    void setScrollWidget(QWidget*);
    QWidget* getScrollWidget();

  protected:
    void resizeEvent(QResizeEvent *event);

  private:
    QScrollArea *scrollArea;
  };

  // TIME PANEL //////////////////////////////////////////////////////////////////////////////////////

  class TimePanel : public PanelNEW
  {
    Q_OBJECT

  public:
    TimePanel(QWidget *parent);
    ~TimePanel();

    //void insertProxy(QuantProxy*);

    void setScrollWidget(QWidget*);


  protected:
    void resizeEvent(QResizeEvent *event);

  private:
    //    void initControl();

    QScrollArea *scrollArea;
    //  QWidget *scrollWidget;

    //QuantProxy *mProxy;
  };

}

#endif // QUANTPANELS

