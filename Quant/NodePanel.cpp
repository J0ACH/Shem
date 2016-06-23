#include "NodePanel.h"

namespace QuantIDE
{
  NodePanel::NodePanel(QWidget *parent, ScBridge *bridge) :
    Panel(parent),
    mBridge(bridge)
  {
    setObjectName("NodePanel");

    this->initControl();
    numOfNode = 0;
    
    connect(buttAddNode, SIGNAL(pressAct()), this, SLOT(addNode()));
    connect(tempoBox, SIGNAL(actValueChanged(QString)), mBridge, SLOT(onChangeTempo(QString)));
  }

  QRect NodePanel::bounds() { return QRect(0, 0, width() - 1, height() - 1); }

  void NodePanel::initControl()
  {
    buttAddNode = new Button(this);
    buttAddNode->setText("AddNode");
    buttAddNode->setGeometry(200, 10, 50, 20);

    tempoBox = new ControlBox(this);
    tempoBox->setLabel("BPM");
    tempoBox->setValue("60");
    tempoBox->setLabelSize(20);
    tempoBox->setGeometry(260, 10, 80, 20);

    scrollArea = new QScrollArea(this);
    scrollArea->setFrameStyle(QFrame::NoFrame);
    scrollArea->move(5, 50);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setStyleSheet("background-color: rgba(0,0,0,0)");

    scrollWidget = new QWidget(this);
    scrollWidget->setAutoFillBackground(true);
    scrollWidget->setStyleSheet("background-color: rgba(0,0,0,0)");
    scrollWidget->setGeometry(0, 0, 300, 100);

    scrollArea->setWidget(scrollWidget);
  }

  void NodePanel::onConfigData(QMap<QString, QVariant*> config)
  {

    colorNormal = config.value("color_shem_Normal")->value<QColor>();
    colorOver = config.value("color_shem_Over")->value<QColor>();
    colorActive = config.value("color_shem_Active")->value<QColor>();
    fontTextSmall = config.value("font_shem_TextSmall")->value<QFont>();

    Panel::onConfigData(config);

    buttAddNode->setColorNormal(colorNormal);
    buttAddNode->setColorOver(colorOver);
    buttAddNode->setColorActive(colorActive);
    buttAddNode->setFont(fontTextSmall);

    configData = config;
    emit actConfigData(config);

    update();
  }

  void NodePanel::addNode()
  {
    QString newName = this->nextNodeName("temp");
    Node *newNode = new Node(scrollWidget, mBridge, newName, numOfNode);

    newNode->setSourceCode("SinOsc.ar(140!2, mul:\\amp.kr())");
    newNode->setFixedWidth(scrollArea->width() - 10);
    newNode->show();
    
    connect(newNode, SIGNAL(killAct(QString)), this, SLOT(deleteNode(QString)));
    connect(newNode, SIGNAL(actChangedHeight()), this, SLOT(fitNodesPosition()));
    connect(this, SIGNAL(actConfigData(QMap<QString, QVariant*>)),
      newNode, SLOT(onConfigData(QMap<QString, QVariant*>)));

    dictNode.insert(newNode->nodeName, newNode);

    this->fitNodesPosition();
    emit actConfigData(configData);

    scrollArea->ensureWidgetVisible(newNode, 0, newNode->height());

    numOfNode++;
  }

  void NodePanel::deleteNode(QString nodeName)
  {
    dictNode.remove(nodeName);
    this->fitNodesPosition();
  }

  QString NodePanel::nextNodeName(QString baseName)
  {
    int cntOfNames = 0;
    foreach(QString oneKey, dictNode.keys())
    {
      if (oneKey.startsWith(baseName))
      {
        QString num = oneKey.remove(tr("%1").arg(baseName));
        if (num.toInt() > cntOfNames) { cntOfNames = num.toInt() + 1; }
        else { cntOfNames++; }
      }
    };
    if (cntOfNames == 0) { return baseName; }
    else { return tr("%1%2").arg(baseName, QString::number(cntOfNames)); };
  }

  void NodePanel::fitNodesPosition()
  {
    int nextNodeOriginY = 0;
    foreach(Node *oneNode, dictNode.values())
    {
      oneNode->setGeometry(5, nextNodeOriginY, oneNode->width(), oneNode->height());
      nextNodeOriginY += oneNode->height() + 10;
    }
    scrollWidget->setFixedHeight(nextNodeOriginY);
  }

  void NodePanel::resizeEvent(QResizeEvent *event)
  {
    Panel::resizeEvent(event); // send event to superclass

    scrollArea->resize(width() - 10, height() - 100);
    scrollWidget->setFixedWidth(scrollArea->width());

    foreach(Node *oneNode, dictNode.values())
    {
      oneNode->setFixedWidth(scrollArea->width() - 10);
    }

    //qDebug("NodePanel::resizeEvent");

  }
  void NodePanel::paintEvent(QPaintEvent *event)
  {
    Panel::paintEvent(event); // send event to superclass

    QPainter painter(this);

    //painter.fillRect(scrollWidget->geometry(), QColor(120, 30, 30, 60));

  }

  NodePanel::~NodePanel()
  {

  }
}