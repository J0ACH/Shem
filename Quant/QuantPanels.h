#ifndef QUANTPANELS_H
#define QUANTPANELS_H

#include "Panel.h"
#include "Text.h"
#include "QuantObjects.h"

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

    void setScrollOffset(int);

  protected:
    void resizeEvent(QResizeEvent *event);

  private:
    QScrollArea *scrollArea;
    int scrollOffset;
  };

  class NodePanelNEW : public QuantPanel
  {
    Q_OBJECT

  public:
    NodePanelNEW(QWidget *parent);
    ~NodePanelNEW();

    public slots:
    void onAddNodePressed();

  signals:
    void actAddNode(QString, bool);

  protected:
    void resizeEvent(QResizeEvent *event);

  private:
    Button *buttonAddNode;
  };
}

#endif // QUANTPANELS

