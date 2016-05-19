#include "Node.h"

namespace QuantIDE
{
  Node::Node(QWidget *parent, ScBridge *bridge, QString name, int nodeNum) :
    QWidget(parent),
    mBridge(bridge),
    nodeName(name),
    nodeNumber(nodeNum)
  {
    setFocusPolicy(Qt::StrongFocus);

    this->initControl();

    nodeBusIndexReserve = 30;
    stateNodePlay = StateNodePlay::FREE;

    connect(playButton, SIGNAL(pressAct()), this, SLOT(changeNodePlay()));
    connect(closeButton, SIGNAL(pressAct()), this, SLOT(close()));
    connect(sourceCode, SIGNAL(sendText(QString)), this, SLOT(sendSourceCode(QString)));

    this->initNode();
  }

  void Node::initControl()
  {
    nameLabel = new QLabel(this);
    nameLabel->setText(nodeName);
    nameLabel->setGeometry(10, 5, 80, 30);

    closeButton = new Button(this);
    closeButton->setIcon(QImage(":/smallClose16.png"), 0);
    closeButton->setText("X");

    playButton = new Button(this);
    playButton->setText("play");
    playButton->setStateKeeping(Jui::Button::StateKeeping::HOLD);
    playButton->setGeometry(90, 10, 40, 20);

    sourceCode = new CodeEditor(this);

    labelNodeID = new QLabel(this);
    labelNamedControls = new QLabel(this);
  }
  void Node::onConfigData(QMap<QString, QVariant*> config)
  {
    colorPanelBackground = config.value("color_shem_PanelBackground")->value<QColor>();
    colorNormal = config.value("color_shem_Normal")->value<QColor>();
    colorOver = config.value("color_shem_Over")->value<QColor>();
    colorActive = config.value("color_shem_Active")->value<QColor>();
    fontTextBig = config.value("font_shem_TextBig")->value<QFont>();
    fontTextSmall = config.value("font_shem_TextSmall")->value<QFont>();
    fontTextCode = config.value("font_shem_TextCode")->value<QFont>();

    sourceCode->setFontCode(fontTextCode);

    closeButton->setColorNormal(colorNormal);
    closeButton->setColorOver(colorOver);
    closeButton->setColorActive(colorActive);

    playButton->setColorNormal(colorNormal);
    playButton->setColorOver(colorOver);
    playButton->setColorActive(colorActive);
    playButton->setFont(fontTextSmall);

    nameLabel->setFont(fontTextBig);
    labelNodeID->setFont(fontTextSmall);
    labelNamedControls->setFont(fontTextSmall);

    configData = config;
    //emit actConfigData(config);

    update();
  }

  QRect Node::bounds() { return QRect(0, 0, width() - 1, height() - 1); }
  void Node::setName(QString name) { nameLabel->setText(name); }
  //QString Node::name() { return nameLabel->text(); }

  void Node::initNode()
  {
    //QString groupID = mBridge->questionNEW("Group.new().nodeID", true).toString();

    mBridge->evaluateNEW(tr("~%1 = NodeProxy.audio(s, 2);").arg(nodeName), true);
    mBridge->evaluateNEW(tr("~%1.play(vol:0.5).quant_(2);").arg(nodeName), true);
    //mBridge->evaluateNEW(tr("~%1.play(vol:0.5);").arg(nodeName), true);
    //mBridge->evaluateNEW(tr("~%1.play(out:0, group:~%1.source.nodeID);").arg(nodeName), true);
    // mBridge->evaluateNEW(tr("~%1.fadeTime = 0.5;").arg(nodeName), true);
    // mBridge->evaluateNEW(tr("~%1.quant_(1);").arg(nodeName), true);

    //labelGroupID->setText(tr("groupID: %1").arg(groupID));

    labelNodeID->setText(tr("nodeID: %1").arg(this->getNodeID()));
  }

  QString Node::getNodeID()
  {
    QString txtNodeID = tr("~%1.nodeID;").arg(nodeName);
    return mBridge->questionNEW(txtNodeID, true).toString();
  }
  
  void Node::setSourceCode(QString txt)
  {
    sourceCode->setText(txt);
    this->sendSourceCode(txt);
  }
  void Node::sendSourceCode(QString txt)
  {
    mBridge->evaluateNEW(tr("(~%1[0] = { %2 })").arg(nodeName, txt), true);
    QStringList controlKeys = mBridge->questionNEW(tr("~%1.controlKeys").arg(nodeName), true).toStringList();

    //qDebug() << "Node::sendSourceCode::controlKeys: " << controlKeys;
    labelNamedControls->setText(tr("controls: %1").arg(controlKeys.join("; ")));

    this->initControlsEditor(controlKeys);
  }

  void Node::initControlsEditor(QStringList namedControls)
  {
    QStringList existKeys = conteinerControlsGraph.keys();
    foreach(QString key, existKeys)
    {
      if (!namedControls.contains(key)) { this->removeControl(key); }
    }
    foreach(QString key, namedControls)
    {
      if (!conteinerControlsGraph.contains(key)) { this->addControl(key); }
    }
  }

  void Node::addControl(QString controlName)
  {
    int busIndex = this->nextEmptyBusIndex();
    ControlEnvelope *newGraph = new ControlEnvelope(
      this,
      mBridge,
      nameLabel->text(),
      controlName,
      busIndex
      );
    newGraph->setFixedHeight(250);
    newGraph->show();

    conteinerControlsGraph.insert(controlName, newGraph);

    //chyba, doresit pro nastaveni zakladu envelopy
    QString defaultValue = mBridge->questionNEW(tr("~%1.controlKeysValues (['%2'])").arg(nodeName, controlName)).toString();
    qDebug() << controlName << "control default value is " << defaultValue;
    //

    this->fitControlsPosition();
  }
  void Node::removeControl(QString controlName)
  {
    conteinerControlsGraph.value(controlName)->sendFreeBusIndex();
    conteinerControlsGraph.value(controlName)->close();
    conteinerControlsGraph.remove(controlName);

    this->fitControlsPosition();
  }

  int Node::nextEmptyBusIndex()
  {
    int startIndex = nodeNumber * nodeBusIndexReserve;
    for (int i = startIndex; i < startIndex + nodeBusIndexReserve; i++)
    {
      bool indexFound = false;
      foreach(ControlEnvelope *oneEnv, conteinerControlsGraph.values())
      {
        if (i == oneEnv->busIndex)
        {
          indexFound = true;
          break;
        }
      }
      if (!indexFound) {
        qDebug() << "Node::nextEmptyBusIndex(): " << i;
        return i;
      };
    }
    mBridge->msgWarningAct(tr("Array of reserved bus index for node %1 is full").arg(nodeName));
    return startIndex;
  }

  void Node::changeNodePlay()
  {
    QString code;
    switch (stateNodePlay)
    {
    case QuantIDE::StateNodePlay::PLAY:
      mBridge->evaluateNEW(tr("~%1.stop(4)").arg(nodeName), true);
      break;
    case QuantIDE::StateNodePlay::STOP:
      mBridge->evaluateNEW(tr("~%1.play(vol: 1, fadeTime: 4)").arg(nodeName), true);
      break;
    case QuantIDE::StateNodePlay::FREE:
      break;
    }

    labelNodeID->setText(tr("nodeID: %1").arg(this->getNodeID()));
  }

  void Node::fitControlsPosition()
  {
    int nextControlOriginY = 110;
    foreach(ControlEnvelope *oneEnv, conteinerControlsGraph.values())
    {
      oneEnv->setGeometry(10, nextControlOriginY, oneEnv->width(), oneEnv->height());
      nextControlOriginY += oneEnv->bounds().height() + 10;
    }
    this->setFixedHeight(nextControlOriginY);
  }

  bool Node::eventFilter(QObject* target, QEvent* event)
  {
    if (event->type() == QEvent::FocusIn)
    {
      qDebug() << "QEvent::FocusIn";
      installEventFilter(this);
      update();
      return true;
    }
    if (event->type() == QEvent::FocusOut)
    {
      qDebug() << "QEvent::FocusOut";
      update();
      return true;
    }

    QWidget::eventFilter(target, event);
    return false;
  }

  void Node::resizeEvent(QResizeEvent *resizeEvent)
  {
    closeButton->setGeometry(this->width() - 30, 10, 16, 16);
    sourceCode->setGeometry(10, 45, width() - 20, 60);
    labelNodeID->setGeometry(this->width() - 300, 5, 250, 20);
    labelNamedControls->setGeometry(this->width() - 300, 20, 250, 20);

    foreach(ControlEnvelope *oneEnv, conteinerControlsGraph.values())
    {
      oneEnv->setFixedWidth(width() - 20);
    }

    emit actChangedHeight();
    //qDebug("Node::resizeEvent");
  }

  void Node::paintEvent(QPaintEvent *paintEvent)
  {
    QPainter painter(this);
    painter.fillRect(this->bounds(), colorPanelBackground);
    //painter.fillRect(this->bounds(), QColor(120, 30, 30));

    if (this->hasFocus()) { painter.setPen(colorActive); }
    else { painter.setPen(colorNormal); }

    //painter.setPen(colorOver);
    painter.drawLine(0, 0, width(), 0);
    painter.drawLine(0, height() - 1, width(), height() - 1);
  }

  void Node::closeEvent(QCloseEvent *event)
  {
    mBridge->evaluateNEW(tr("~%1.free").arg(nodeName), true);
    stateNodePlay = StateNodePlay::FREE;
    emit killAct(nodeName);
    qDebug("Node::closeEvent()");
  }

  Node::~Node()
  {

  }
}