#ifndef NODEPANEL_H
#define NODEPANEL_H

#include "Button.h"
#include "Panel.h"
#include "Node.h"
#include "Canvan.h"
#include "ScBridge.h"
#include "ControlBox.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QMap>
#include <QDebug>
#include <QScrollArea>


using namespace Jui;
using namespace SupercolliderBridge;

namespace QuantIDE
{

  class NodePanel : public Panel
  {
    Q_OBJECT

  public:
    NodePanel(QWidget *parent, ScBridge *bridge, Customize *customize);
    ~NodePanel();

    QRect bounds();

    public slots:
    void fitNodesPosition();
       void onCustomize();

    void addNode();
    void deleteNode(QString name);

  signals:
       void actCustomizeChanged();

  protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

  private:
    void initControl();
    QString nextNodeName(QString);

    int numOfNode;    
     
    Button *buttAddNode;
    ControlBox *tempoBox;
    QScrollArea *scrollArea;
    QWidget *scrollWidget;
    QMap<QString, Node*> dictNode;
    ScBridge *mBridge;
    Customize *mCustomize;

  };
}

#endif // QUANT

