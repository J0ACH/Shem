#include "ControlEnvelope.h"

namespace QuantIDE
{
  ControlEnvelope::ControlEnvelope(QWidget *parent, ScBridge *bridge, QString inNodeName, QString cntName, int cBus) :
    QWidget(parent),
    mBridge(bridge),
    nodeName(inNodeName),
    controlName(cntName),
    busIndex(cBus)
  {
    this->setObjectName("ControlEnvelope");
    objectID = QUuid::createUuid();

    setFocusPolicy(Qt::StrongFocus);

    this->initControl();

    connect(envelopeCode, SIGNAL(sendText(QString)), this, SLOT(setEnv(QString)));
   // connect(this, SIGNAL(bridgeQuestionAct(QUuid, int, QString, bool)), mBridge, SLOT(question(QUuid, int, QString, bool)));
  //  connect(mBridge, SIGNAL(answerAct(QUuid, int, QStringList)), this, SLOT(onBridgeAnswer(QUuid, int, QStringList)));
   // connect(this, SIGNAL(actCodeEvaluated(QString, bool, bool)), mBridge, SLOT(evaluateCode(QString, bool, bool)));

    /*
    connect(
      this, SIGNAL(actGraphEnv(QList<double>, QList<double>, QList<double>)),
      envGraph, SLOT(onGraphEnv(QList<double>, QList<double>, QList<double>))
      );
    connect(
      envGraph, SIGNAL(actGraphEnv(QList<double>, QList<double>, QList<double>)),
      this, SLOT(onGraphEnv(QList<double>, QList<double>, QList<double>))
      );
    */

    // this->onEnvelopeCodeEvaluate();

    mBridge->evaluateNEW(tr("~%1.set(\\%2, BusPlug.for(%3));").arg(nodeName, controlName, QString::number(busIndex)), true);
  }

  QRect ControlEnvelope::bounds() { return QRect(0, 0, width() - 1, height() - 1); }

  void ControlEnvelope::initControl()
  {
    nameLabel = new QLabel(this);
    nameLabel->setText(controlName);

    busLabel = new QLabel(this);
    busLabel->setText(tr("busIndex : %1").arg(busIndex));

    envelopeCode = new CodeEditor(this);
    // envelopeCode->setText("Env([0,1,0], [0.15,0.85], ['lin', 'sin'])");

    envGraph = new Graph(this);
    envGraph->setDomainX(0, 1);
    envGraph->setDomainY(0, 1);
    //envGraph->drawLine(0.25, 0.25, 0.5, 0.5);
    //envGraph->drawLine(0.75, 0.25, 0.5, 0.5);

    graphCurveX = QList<double>();
    int noSeg = envGraph->boundsGraph().width();
    //int noSeg = 200;
    double min = envGraph->getDomainX()[0];
    double max = envGraph->getDomainX()[1];
    for (int i = 0; i <= noSeg; i++)
    {
      graphCurveX.append((max - min) / noSeg*i + min);
    }
  }

  void ControlEnvelope::setEnv(QString envCode)
  {
    qDebug() << "ControlEnvelope::setEnv: " << envCode;

    levels = QList<double>();
    times = QList<double>();
    curves = QList<QString>();
    double minLevel = 0;
    double maxLevel = 1;

    /*
    QStringList txtLevels = mBridge->questionNEW(tr("%1.levels").arg(envCode)).toStringList();
    foreach(QString oneLevel, txtLevels)
    {
    double dLev = oneLevel.toDouble();
    levels.append(dLev);
    if (dLev > maxLevel) { maxLevel = dLev; }
    if (dLev < minLevel) { minLevel = dLev; }
    }

    QStringList txtTimes = mBridge->questionNEW(tr("%1.times").arg(envCode)).toStringList();
    foreach(QString oneTime, txtTimes) { times.append(oneTime.toDouble()); }

    QStringList txtCurves = mBridge->questionNEW(tr("%1.curves").arg(envCode)).toStringList();
    foreach(QString oneCurve, txtCurves) { curves.append(oneCurve); }
    */


   // QList<double> dblList;
    QStringList answer = mBridge->questionNEW(tr("%1.asArray").arg(envCode), true).toStringList();
    /*
    foreach(QString oneAnsw, answer)
    {
      dblList.append(oneAnsw.toDouble());
      qDebug() << "oneArrayVal: " << oneAnsw;
    }
    */
    // qDebug() << "ControlEnvelope::envArray: " << answer;
    /*
    double currentPointTime = 0 - answer[1].toDouble();
    for (int i = 0; i < answer.size(); i += 4)
    {
      currentPointTime += answer[i + 1].toDouble();

      levels.append(answer[i].toDouble());
      times.append(currentPointTime);
      curves.append(answer[i + 3]);

      qDebug() << "level: " << answer[i];
      qDebug() << "time: " << answer[i + 1];
      qDebug() << "type: " << answer[i + 2];
      qDebug() << "curve: " << answer[i + 3];
      qDebug() << "///////////////////\n";
    }



    duration = mBridge->questionNEW(tr("%1.totalDuration").arg(this->getEnv()), true).toString().toDouble();
    //  qDebug() << "duration:" << duration;
    envGraph->setDomainX(0, duration);
    envGraph->setDomainY(minLevel, maxLevel);

    

    envGraph->drawPolyline(this->getEnvValues(200));
    update();
    */
    envelopeCode->setText(envCode);
  }
  void ControlEnvelope::setEnv(QList<double> listLevels, QList<double> listTimes, QList<QString> listCurves)
  {
    QStringList txtLevels;
    QStringList txtTime;
    QStringList txtCurves;

    foreach(double oneLevel, listLevels) { txtLevels.append(QString::number(oneLevel, 'f', 2)); }
    foreach(double oneTime, listTimes) { txtTime.append(QString::number(oneTime, 'f', 2)); }
    foreach(QString oneCurve, listCurves) { txtCurves.append(oneCurve); }

    QString codeEnv = tr("Env([%1], [%2], [%3])").arg(
      txtLevels.join(", "),
      txtTime.join(", "),
      txtCurves.join(", ")
      );

    this->setEnv(codeEnv);
  }

  QString ControlEnvelope::getEnv()
  {
    QStringList txtLevels;
    QStringList txtTime;
    QStringList txtCurves;

    foreach(double oneLevel, levels) { txtLevels.append(QString::number(oneLevel, 'f', 2)); }
    foreach(double oneTime, times) { txtTime.append(QString::number(oneTime, 'f', 2)); }
    foreach(QString oneCurve, curves) { txtCurves.append(oneCurve); }

    QString codeEnv = tr("Env([%1], [%2], [%3])").arg(
      txtLevels.join(", "),
      txtTime.join(", "),
      txtCurves.join(", ")
      );

    return codeEnv;
  }

  QVector<QPointF> ControlEnvelope::getEnvValues(int segments)
  {
    // QList<double> values;
    QVector<QPointF> points;

    for (int i = 0; i <= segments; i++)
    {
      double xVal = (duration / (double)segments * i);
      //qDebug() << "xVal:" << xVal;
      QString txtVal = mBridge->questionNEW(tr("%1.at(%2)").arg(this->getEnv(), QString::number(xVal))).toString();
      // qDebug() << "txtVal:" << txtVal;
      //values.append(txtVal.toDouble());
      points.append(QPointF(xVal, txtVal.toDouble()));
    }
    return points;
  }
  
  void ControlEnvelope::freeControlBusIndex()
  {
    qDebug() << "Bus FREE test";
    
    // unmap control and set bus to nil or 0
    
    //QString code = tr("Bus.new('control', %1, 1, s).free").arg(QString::number(busIndex)); // nefunguje
   // emit actCodeEvaluated(code);
  }
  
  void ControlEnvelope::sendTask()
  {
    /*
    qDebug() << "TASK TEST";

    QString code;
    code += "(\n";
    //code += tr("var bus = Bus.new('control', %1, 1, s);\n").arg(QString::number(busIndex));
    //code += "var cNode = NodeProxy.for (bus);\n";
    code += tr("var cNode = NodeProxy.for (%1);\n").arg(QString::number(busIndex));;
    code += "var pattern = Pseq([0, 0], inf);\n";
    code += tr("var envs = Pswitch([%1], pattern);\n").arg(envelopeCode->toPlainText());
    code += "cNode.quant_(1);\n";
    code += tr("~%1[%2] = Task({\n").arg(parentName, QString::number(busIndex));
    code += "\tvar cntLoops = envs.which.list.size * envs.which.repeats;\n";
    code += "\tcntLoops.do({ | noLoop |\n";
    code += "\t\tvar currentNum = envs.which.list[noLoop % envs.which.list.size];\n";
    code += "\t\tvar currentEnv = envs.list[currentNum];\n";
    code += tr("\t\t~%1.set('%2', cNode.source_({EnvGen.kr(currentEnv,doneAction:2)}));\n").arg(parentName, name);
    code += "\t\tcurrentEnv.totalDuration.wait;\n";
    code += "\t})\n";
    code += "});\n";
    code += ")";
    emit actCodeEvaluated(code, false, false);
    */
  }

  void ControlEnvelope::onEnvelopeCodeEvaluate()
  {
    qDebug() << "onEnvelopeCodeEvaluate TED";
    envGraph->deleteGraph();
    //this->setEnv(envelopeCode->toPlainText());

    //graphCurveY = QList<double>();
    //graphPolyline = QVector<QPointF>();

    // this->onBridgeQuestion(QuestionType::envArray);

    /*
    foreach(double oneX, graphCurveX)
    {
    //   this->onBridgeQuestion(QuestionType::graphAt, QString::number(oneX));
    }
    */
    //this->onBridgeQuestion(QuestionType::redrawEnvGraph);

    //this->sendTask();
    /*
    double currentPointTime;
    QList<double> dblList;
    QList<double> levels;
    QList<double> times;
    QList<double> type;
    QList<double> curves;

    QStringList answer = mBridge->questionNEW(tr("%1.asArray").arg(this->getEnv()), true).toStringList();

    foreach(QString oneAnsw, answer) { dblList.append(oneAnsw.toDouble()); }
    // qDebug() << "ControlEnvelope::envArray: " << answer;

    currentPointTime = 0 - answer[1].toDouble();
    for (int i = 0; i < answer.size(); i += 4)
    {
      currentPointTime += answer[i + 1].toDouble();

      levels.append(answer[i].toDouble());
      times.append(currentPointTime);
      curves.append(answer[i + 3].toDouble());

      qDebug() << "level: " << answer[i];
      qDebug() << "time: " << answer[i + 1];
      qDebug() << "type: " << answer[i + 2];
      qDebug() << "curve: " << answer[i + 3];
      qDebug() << "///////////////////\n";

    }
    */

    /*
    if (times.size() > 0)
    {
    midCurvePointX = QList<double>();
    midCurvePointY = QList<double>();

    for (int i = 1; i < times.size(); i++)
    {
    //qDebug() << "times[i] " << times[i];
    //qDebug() << "times[i-1] " << times[i-1];
    currentPointTime = ((times[i] - times[i - 1]) / 2) + times[i - 1];
    midCurvePointX.append(currentPointTime);
    //qDebug() << "currentTime " << currentPointTime;
    //this->onBridgeQuestion(QuestionType::envAt, QString::number(currentPointTime));
    QString txtVal = mBridge->questionNEW(tr("%1.at(%2)").arg(this->getEnv(), QString::number(currentPointTime))).toString();
    points.append(QPointF(currentPointTime, txtVal.toDouble()));
    //currentPointTime += times[i];
    }
    }
    */
   // envGraph->drawPolyline(this->getEnvValues(200));

    //☻ emit actGraphEnv(levels, times, curves);

    // update();
  }

  void ControlEnvelope::onGraphEnv(QList<double> envLevels, QList<double> envTimes, QList<double> envCurves)
  {

    qDebug() << "ControlEnvelope::onControlPointsChange";
    qDebug() << "levels: " << envLevels;
    qDebug() << "times: " << envTimes;
    qDebug() << "curves: " << envCurves;

    QStringList txtLevels;
    QStringList txtTime;
    QStringList txtCurves;

    foreach(double oneLevel, envLevels) { txtLevels.append(QString::number(oneLevel, 'f', 2)); }
    foreach(double oneTime, envTimes) { txtTime.append(QString::number(oneTime, 'f', 2)); }
    foreach(double oneCurve, envCurves) { txtCurves.append(QString::number(oneCurve)); }

    QString codeEnv = tr("Env([%1], [%2], [%3])").arg(
      txtLevels.join(", "),
      txtTime.join(", "),
      txtCurves.join(", ")
      );

    envelopeCode->setText(codeEnv);
    envGraph->deleteGraph();
    envelopeCode->evaluateAct();

    qDebug() << "ENV: " << codeEnv;
  }


  void ControlEnvelope::mouseReleaseEvent(QMouseEvent *mouseEvent)
  {

    mBridge->questionNEW(tr("~%1.nodeID").arg(nodeName), true);
    mBridge->questionNEW(tr("~%1.source.def.sourceCode").arg(nodeName), true);
    mBridge->questionNEW(tr("~%1.isPlaying").arg(nodeName), true);

    for (int i = 0; i <= 10; i++)
    {
      double oneX = 1 / 10.0*i;

      QString code1 = tr("%1.at(%2)").arg(envelopeCode->toPlainText(), QString::number(oneX));
      QString answer1 = tr("QA: %1 = %2").arg(code1, mBridge->questionNEW(code1, true).toString());
      qDebug() << answer1;
    }
  }

  void ControlEnvelope::resizeEvent(QResizeEvent *event)
  {
    nameLabel->setGeometry(5, 5, 95, 25);
    busLabel->setGeometry(100, 5, 100, 25);
    envelopeCode->setGeometry(5, 30, width() - 10, 25);
    envGraph->setGeometry(5, 60, width() - 10, height() - 65);

    /*
    foreach(double oneX, graphCurveX) {
      this->onBridgeQuestion(QuestionType::graphAt, QString::number(oneX));
    }
    */
  }

  void ControlEnvelope::paintEvent(QPaintEvent *event)
  {
    QPainter painter(this);

    //painter.setPen(QPen(Qt::white, 1));
    //painter.fillRect(bounds(), QColor(120, 20, 20));

    if (this->hasFocus()) { painter.setPen(QColor(120, 20, 20)); }
    else { painter.setPen(QColor(60, 60, 60)); }

    //painter.setPen(colorOver);
    painter.drawLine(0, 0, width(), 0);
    painter.drawLine(0, height() - 1, width(), height() - 1);

  }

  ControlEnvelope::~ControlEnvelope()
  {

  }
 
  /*
  void ControlEnvelope::onBridgeQuestion(QuestionType selector, QString args)
  {
    QString questionCode;
    int selectorNum;

    switch (selector)
    {
    case envArray:
      selectorNum = QuestionType::envArray;
      questionCode = tr("%1.asArray").arg(envelopeCode->toPlainText());
     // emit bridgeQuestionAct(objectID, selectorNum, questionCode, false);
      break;
    case envAt:
      selectorNum = QuestionType::envAt;
      questionCode = tr("%1.at(%2)").arg(envelopeCode->toPlainText(), args);
    //  emit bridgeQuestionAct(objectID, selectorNum, questionCode, false);
      break;
    case graphAt:
      selectorNum = QuestionType::graphAt;
      questionCode = tr("%1.at(%2)").arg(envelopeCode->toPlainText(), args);
    //  emit bridgeQuestionAct(objectID, selectorNum, questionCode, false);
      break;
    case redrawEnvGraph:
      selectorNum = QuestionType::redrawEnvGraph;
      questionCode = tr("\" \"").arg(envelopeCode->toPlainText());
     // emit bridgeQuestionAct(objectID, selectorNum, questionCode, false);
      break;
    case initBusIndex:
      selectorNum = QuestionType::initBusIndex;
      questionCode = "Bus.control(s,1).index";
     // emit bridgeQuestionAct(objectID, selectorNum, questionCode, false);
      break;
    }
  }
  void ControlEnvelope::onBridgeAnswer(QUuid id, int selectorNum, QStringList answer)
  {

    if (id == objectID)
    {
      QuestionType selector = static_cast<QuestionType>(selectorNum);

      double currentPointTime;
      QList<double> dblList;
      QList<double> levels;
      QList<double> times;
      QList<double> type;
      QList<double> curves;

      switch (selector)
      {
      case envArray:
        foreach(QString oneAnsw, answer) { dblList.append(oneAnsw.toDouble()); }
        //qDebug() << "ControlEnvelope::envArray: " << dblList;

        currentPointTime = 0 - answer[1].toDouble();
        for (int i = 0; i < answer.size(); i += 4)
        {
          currentPointTime += answer[i + 1].toDouble();

          levels.append(answer[i].toDouble());
          times.append(currentPointTime);
          curves.append(answer[i + 3].toDouble());
          
          qDebug() << "level: " << answer[i];
          qDebug() << "time: " << answer[i + 1];
          qDebug() << "type: " << answer[i + 2];
          qDebug() << "curve: " << answer[i + 3];
          qDebug() << "///////////////////\n";
          
        }

        if (times.size() > 0)
        {
          midCurvePointX = QList<double>();
          midCurvePointY = QList<double>();

          for (int i = 1; i < times.size(); i++)
          {
            //qDebug() << "times[i] " << times[i];
            //qDebug() << "times[i-1] " << times[i-1];
            currentPointTime = ((times[i] - times[i - 1]) / 2) + times[i - 1];
            midCurvePointX.append(currentPointTime);
            //qDebug() << "currentTime " << currentPointTime;
            this->onBridgeQuestion(QuestionType::envAt, QString::number(currentPointTime));
            //currentPointTime += times[i];
          }
        }

        emit actGraphEnv(levels, times, curves);
        break;

      case envAt:
        qDebug() << "ControlEnvelope::envAt " << answer;
        midCurvePointY.append(answer[0].toDouble());

        for (int i = 0; i < midCurvePointY.size(); i++)
        {
          //envGraph->drawPoint(midCurvePointX[i], midCurvePointY[i]);
          envGraph->addCurvePoint(midCurvePointX[i], midCurvePointY[i], 0);

          envGraph->drawLine(
            midCurvePointX[i],
            0,
            midCurvePointX[i],
            midCurvePointY[i]
            );
        }


        break;

      case graphAt:
        //qDebug() << "polyline size: " << graphPolyline.size();
        //qDebug() << "ControlEnvelope::envAt " << answer[0];
        //currentGraphPtID = graphPolyline.size();
        graphCurveY.append(answer[0].toDouble());


        break;

      case redrawEnvGraph:
        //qDebug() << "ControlEnvelope::refreshGraph NOW";
        //qDebug() << "ControlEnvelope::graphCurveX " << graphCurveX.size();
        //qDebug() << "ControlEnvelope::graphCurveY " << graphCurveY.size();
        //qDebug() << "ControlEnvelope::graphPolyline " << graphPolyline.size();

        if (graphCurveX.size() == graphCurveY.size())
        {
          for (int i = 0; i < graphCurveX.size(); i++)
          {
            graphPolyline.append(QPointF(graphCurveX[i], graphCurveY[i]));
          }
          envGraph->drawPolyline(graphPolyline);
        }
        break;

      case initBusIndex:
        busIndex = answer[0].toInt();
        busLabel->setText(tr("busIndex : %1").arg(busIndex));
        //qDebug() << "Control " << name << " busIndex : " << busIndex;
        break;
      }
    }
  }
  */
}