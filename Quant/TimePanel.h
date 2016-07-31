#ifndef TIMEPANEL_H
#define TIMEPANEL_H

#include "Customize.h"
#include "Button.h"
#include "Panel.h"
#include "ControlBox.h"
#include "Text.h"
#include "QuantObjects.h"

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QScrollArea>


using namespace Jui;
using namespace SupercolliderBridge;

namespace QuantIDE
{
  class TimePanel : public PanelNEW
  {
    Q_OBJECT

  public:
    TimePanel(QWidget *parent);
    ~TimePanel();

    void insertProxy(QuantProxy*);

  protected:
    void resizeEvent(QResizeEvent *event);

  private:
    void initControl();

    QScrollArea *scrollArea;
    QWidget *scrollWidget;

    QuantProxy *mProxy;
  };
}

#endif // TIMEPANEL_H

