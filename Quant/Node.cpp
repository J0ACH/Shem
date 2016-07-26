#include "Node.h"

namespace QuantIDE
{
  Node::Node(QWidget *parent, ScBridge *bridge, Customize *customize, QString name, int nodeNum) :
    QWidget(parent),
    mBridge(bridge),
    mCustomize(customize),
    nodeName(name),
    nodeNumber(nodeNum)
  {
    setFocusPolicy(Qt::StrongFocus);

    this->initControl();

    nodeBusIndexReserve = 30;
    stateNodePlay = StateNodePlay::FREE;

    connect(mCustomize, SIGNAL(actCustomizeChanged()), this, SLOT(onCustomize()));

    connect(playButton, SIGNAL(pressAct()), this, SLOT(changeNodePlay()));
    connect(closeButton, SIGNAL(pressAct()), this, SLOT(close()));
    connect(sourceCode, SIGNAL(sendText(QString)), this, SLOT(sendSourceCode(QString)));
    connect(volumeBox, SIGNAL(actValueChanged(QString)), this, SLOT(setNodeVolume(QString)));
    connect(fTimeBox, SIGNAL(actValueChanged(QString)), this, SLOT(setNodeFadeTime(QString)));

    this->initNode();
    this->onCustomize();
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

    volumeBox = new ControlBox(this);
    volumeBox->setLabel("volume");
    volumeBox->setLabelSize(45);

    fTimeBox = new ControlBox(this);
    fTimeBox->setLabel("fTime");
    fTimeBox->setLabelSize(45);
  }
  void Node::onCustomize()
  {
    qDebug("Node::onCustomize");
    /*
    colorPanelBackground = mCustomize->getColor("color_shem_PanelBackground");
    colorNormal = mCustomize->getColor("color_shem_Normal");
    colorOver = mCustomize->getColor("color_shem_Over");
    colorActive = mCustomize->getColor("color_shem_Active");
    fontTextBig = mCustomize->getFont("font_shem_TextBig");
    fontTextSmall = mCustomize->getFont("font_shem_TextSmall");
    fontTextCode = mCustomize->getFont("font_shem_TextCode");
    colorText = mCustomize->getColor("color_shem_Text");
    */

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

    QPalette palete = this->palette();
    palete.setColor(this->foregroundRole(), colorText);
    nameLabel->setPalette(palete);
    labelNodeID->setPalette(palete);
    labelNamedControls->setPalette(palete);

    volumeBox->setFont(fontTextSmall);
//    volumeBox->setColorBackground(colorPanelBackground);
    volumeBox->setColorText(colorText);

    fTimeBox->setFont(fontTextSmall);
   // fTimeBox->setColorBackground(colorPanelBackground);
    fTimeBox->setColorText(colorText);

  }
  /*
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

    volumeBox->setFont(fontTextSmall);
    volumeBox->setColorBackground(colorPanelBackground);
    volumeBox->setColorText(colorNormal);

    fTimeBox->setFont(fontTextSmall);
    fTimeBox->setColorBackground(colorPanelBackground);
    fTimeBox->setColorText(colorNormal);

    configData = config;
    update();
  }
  */

  QRect Node::bounds() { return QRect(0, 0, width() - 1, height() - 1); }
  void Node::setName(QString name) { nameLabel->setText(name); }
  //QString Node::name() { return nameLabel->text(); }

  void Node::initNode()
  {
    volume = "0.2";
    fTime = "0";

    mBridge->evaluate(tr("~%1 = NodeProxy.audio(s, 2);").arg(nodeName), true);
    mBridge->evaluate(tr("~%1.play(vol:%2, fadeTime: %3).quant_(1);").arg(nodeName, volume, fTime), true);
    mBridge->evaluate(tr("~%1.pause()").arg(nodeName), true);
    stateNodePlay = StateNodePlay::STOP;

    labelNodeID->setText(tr("nodeID: %1").arg(this->getNodeID()));
    volumeBox->setValue("0.2");
    fTimeBox->setValue("0");
  }

  QString Node::getNodeID()
  {
    QString txtNodeID = tr("~%1.nodeID;").arg(nodeName);
    return mBridge->question(txtNodeID).toString();
  }

  void Node::setSourceCode(QString txt)
  {
    sourceCode->setText(txt);
    this->sendSourceCode(txt);
  }
  void Node::sendSourceCode(QString txt)
  {
    mBridge->evaluate(tr("(~%1[0] = { %2 })").arg(nodeName, txt), true);

    //qDebug() << "Node::sendSourceCode::controlKeys: " << controlKeys;
    labelNamedControls->setText(tr("controls: %1").arg(this->getControlKeys().join("; ")));
  }

  QStringList Node::getControlKeys()
  {
    QStringList controlKeys = mBridge->question(tr("~%1.controlKeys").arg(nodeName)).toStringList();

    //check pouze pro source (index[0])
    //controlKeys obsahuje i namapovane klice, ktere je treba jeste ~node.unmap(\key)
    QStringList constants = mBridge->question(tr("~%1.source.def.constants").arg(nodeName)).toStringList();

    foreach(QString key, controlKeys)
    {
      if (!constants.contains(key))
      {
        mBridge->evaluate(tr("~%1.unmap(\\%2)").arg(nodeName, key), true);
        controlKeys.removeAt(controlKeys.indexOf(key, 0));
      }
    }

    QStringList existKeys = conteinerControlsGraph.keys();
    foreach(QString key, existKeys)
    {
      if (!controlKeys.contains(key)) { this->removeControl(key); }
    }
    foreach(QString key, controlKeys)
    {
      if (!conteinerControlsGraph.contains(key)) { this->addControl(key); }
    }

    return controlKeys;
  }

  void Node::addControl(QString controlName)
  {
    QString defaultValue = mBridge->question(tr("~%1.controlKeysValues ([\\%2])[1]").arg(nodeName, controlName)).toString();
    // qDebug() << controlName << "control default value is " << defaultValue;

    //int busIndex = this->nextEmptyBusIndex();
    ControlEnvelope *newGraph = new ControlEnvelope(
      this,
      mBridge,
      mCustomize,
      nameLabel->text(),
      controlName,
      this->nextEmptyBusIndex()
      );
    newGraph->setFixedHeight(250);
    newGraph->setEnv(tr("Env([%1,%1], 1, \\sin)").arg(defaultValue));
    newGraph->show();

    conteinerControlsGraph.insert(controlName, newGraph);
    this->fitControlsPosition();
  }
  void Node::removeControl(QString controlName)
  {
    conteinerControlsGraph.value(controlName)->freeControlBusIndex();
    conteinerControlsGraph.value(controlName)->close();
    conteinerControlsGraph.remove(controlName);

    this->fitControlsPosition();
  }

  int Node::nextEmptyBusIndex()
  {
    QString fisrtPrivateBus = mBridge->question("s.options.numOutputBusChannels + s.options.numInputBusChannels").toString();
    int startIndex = nodeNumber * nodeBusIndexReserve + fisrtPrivateBus.toInt();
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
        // qDebug() << "Node::nextEmptyBusIndex(): " << i;
        return i;
      };
    }
    mBridge->actPrint(tr("Array of reserved bus index for node %1 is full").arg(nodeName), MessageType::WARNING);
    return startIndex;
  }

  void Node::changeNodePlay()
  {
    QString code;
    switch (stateNodePlay)
    {
    case QuantIDE::StateNodePlay::PLAY:
      mBridge->evaluate(tr("~%1.stop(%2)").arg(nodeName, fTime), true);
      mBridge->evaluate(tr("~%1.pause()").arg(nodeName), true);
      // mBridge->evaluate(tr("s.makeBundle(%1, ~%2.pause();)").arg(fTime,nodeName), true);
      stateNodePlay = StateNodePlay::STOP;
      break;
    case QuantIDE::StateNodePlay::STOP:
      mBridge->evaluate(tr("~%1.resume()").arg(nodeName), true);
      mBridge->evaluate(tr("~%1.play(vol: %2, fadeTime: %3)").arg(nodeName, volume, fTime), true);
      stateNodePlay = StateNodePlay::PLAY;
      break;
    case QuantIDE::StateNodePlay::FREE:
      mBridge->evaluate(tr("~%1.free(%2)").arg(nodeName, fTime), true);
      break;
    }

    labelNodeID->setText(tr("nodeID: %1").arg(this->getNodeID()));
  }

  void Node::setNodeVolume(QString vol)
  {
    volume = vol;
    mBridge->evaluate(tr("~%1.play(vol: %2, fadeTime: %3)").arg(nodeName, volume, fTime), true);
  }
  void Node::setNodeFadeTime(QString time)
  {
    fTime = time;
    mBridge->evaluate(tr("~%1.play(vol: %2, fadeTime: %3)").arg(nodeName, volume, fTime), true);
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
    sourceCode->setGeometry(10, 45, width() - 105, 60);
    labelNodeID->setGeometry(this->width() - 300, 5, 250, 20);
    labelNamedControls->setGeometry(this->width() - 300, 20, 250, 20);
    volumeBox->setGeometry(width() - 90, 45, 80, 27);
    fTimeBox->setGeometry(width() - 90, 78, 80, 27);

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
    mBridge->evaluate(tr("~%1.free").arg(nodeName), true);
    stateNodePlay = StateNodePlay::FREE;
    emit killAct(nodeName);
    qDebug("Node::closeEvent()");
  }

  Node::~Node()
  {

  }
  /*
  float Node::timeToNextQuant()
  {
  float time = mBridge->question(tr("p[\\tempo].clock.timeToNextBeat(%1)").arg(
  QString::number(quant)
  ), true).toString().toFloat();

  return time;
  }
  */
}