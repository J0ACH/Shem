#ifndef NETWORKPANEL_H
#define NETWORKPANEL_H

#include "Panel.h"
#include "Text.h"
//#include "Library.h"

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QScrollArea>

using namespace Jui;

namespace QuantIDE
{
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
}

#endif // NETWORKPANEL_H

