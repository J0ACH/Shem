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
  class QuantPanel : public PanelNEW
  {
    Q_OBJECT

  public:
    QuantPanel(QWidget *parent);
    ~QuantPanel();

    void setScrollWidget(QWidget*);
    QWidget* getScrollWidget();

  protected:
    void resizeEvent(QResizeEvent *event);

  private:
    QScrollArea *scrollArea;
  };
}

#endif // QUANTPANELS

