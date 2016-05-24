﻿#include "ControlEnvelope.h"

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
    cntVertex = 0;

    connect(envelopeCode, SIGNAL(sendText(QString)), this, SLOT(setEnv(QString)));
    connect(
      envGraph, SIGNAL(actEnvGraphChanged(QList<double>, QList<double>, QList<QString>)),
      this, SLOT(setEnv(QList<double>, QList<double>, QList<QString>))
      );

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

    envGraph = new Graph(this);
    envGraph->setDomainX(0, 1);
    envGraph->setDomainY(0, 1);
  }

  void ControlEnvelope::setEnv(QString envCode)
  {
    // qDebug() << "ControlEnvelope::setEnv: " << envCode;

    //stane se jen pri prvnim nastaveni env
    if (cntVertex == 0) { envelopeCode->setText(envCode); };

    levels = QList<double>();
    times = QList<double>();
    curves = QList<QString>();
    double minLevel = 0;
    double maxLevel = 1;

    QStringList answer = mBridge->questionNEW(tr("%1.asArray").arg(envCode)).toStringList();

    int newNumLevelPoints;
    for (int i = 0; i < answer.size(); i += 4)
    {
      if (answer[i].toDouble() > maxLevel) { maxLevel = answer[i].toDouble(); }
      if (answer[i].toDouble() < minLevel) { minLevel = answer[i].toDouble(); }

      if (i == 0)
      {
        // qDebug() << "pocet levelu: " << answer[i + 1].toInt();
        // qDebug() << "level: " << answer[i].toDouble();
        newNumLevelPoints = answer[i + 1].toInt();
        levels.append(answer[i].toDouble());
      }
      else
      {
        levels.append(answer[i].toDouble());
        times.append(answer[i + 1].toDouble());

        // qDebug() << "level: " << answer[i];
        // qDebug() << "time: " << answer[i + 1];

        QString symbol = mBridge->questionNEW(tr("Env.shapeNames.findKeyForValue(%1)").arg(answer[i + 2])).toString();

        if (symbol != "nil") { curves.append(tr("'%1'").arg(symbol)); }
        else { curves.append(answer[i + 3]); }
      }
      // qDebug() << "///////////////////\n";
    }

    duration = mBridge->questionNEW(tr("%1.totalDuration").arg(envCode)).toString().toDouble();
    envGraph->setDomainX(0, duration);
    envGraph->setDomainY(minLevel, maxLevel);

    if (cntVertex == newNumLevelPoints)
    {
      qDebug() << "pocet controlnich bodu nezmenen";
      for (int i = 0; i <= cntVertex; i++)
      {
        QPointF vertex = this->getEnvVertex(i);
        qDebug() << vertex;
        envGraph->setVertexPoint(i, vertex);
      }
    }
    else
    {
      envGraph->deleteGraph();
      cntVertex = newNumLevelPoints;
      qDebug() << "pocet controlnich bodu ZMENEN";
      for (int i = 0; i <= newNumLevelPoints; i++)
      {
        QPointF vertex = this->getEnvVertex(i);
        qDebug() << vertex;
        envGraph->addVertexPoint(vertex);
      }
    }
    QString env = this->getEnv();

    //qDebug() << "ControlEnvelope::levels: " << levels;
    //qDebug() << "ControlEnvelope::timse: " << times;
    //qDebug() << "ControlEnvelope::curve: " << curves;
    qDebug() << "ControlEnvelope::getEnv(): " << env;

    envGraph->drawPolyline(this->getEnvPoints(200));
    this->makeTask(env);
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
    qDebug() << "setEnv: " << codeEnv;
    envelopeCode->setText(codeEnv);
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

  QPointF ControlEnvelope::getEnvVertex(int ID)
  {
    double vertexTime = 0;
    for (int i = 0; i < times.size(); i++)
    {
      if (ID == i) { break; }
      else { vertexTime += times[i]; }
    }
    return QPointF(vertexTime, levels[ID]);
  }

  QVector<QPointF> ControlEnvelope::getEnvPoints(int segments)
  {
    QVector<QPointF> points;
    QString env = this->getEnv();
    // qDebug() << "graph pixelX: " << envGraph->boundsGraph().width();
    // segments = envGraph->boundsGraph().width()/4;
    for (int i = 0; i <= segments; i++)
    {
      double xVal = (duration / (double)segments * i);
      QString txtVal = mBridge->questionNEW(tr("%1.at(%2)").arg(env, QString::number(xVal))).toString();
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

  void ControlEnvelope::makeTask(QString env)
  {
    // pozor na index, bude treba doresit
    QString code = tr("~%1[%2] = Pbind(\\instrument, \\envControl, \\bus, %2, \\dur, %3, \\env, [%4])").arg(
      nodeName,
      QString::number(busIndex),
      QString::number(duration),
      env
      );
    mBridge->evaluateNEW(code);
  }

  void ControlEnvelope::mouseReleaseEvent(QMouseEvent *mouseEvent)
  {
    mBridge->questionNEW(tr("~%1.nodeID").arg(nodeName), true);
    mBridge->questionNEW(tr("~%1.source.def.sourceCode").arg(nodeName), true);
    mBridge->questionNEW(tr("~%1.isPlaying").arg(nodeName), true);

    envGraph->drawPolyline(this->getEnvPoints(200));
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

  ControlEnvelope::~ControlEnvelope()  {  }
}
