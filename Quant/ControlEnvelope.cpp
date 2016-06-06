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
    // objectID = QUuid::createUuid();

    setFocusPolicy(Qt::StrongFocus);

    this->initControl();
    cntVertex = 0;
    levels = QList<double>();
    times = QList<double>();
    curves = QList<QString>();
    env = "";
    previousEnv = "";

    connect(envelopeCode, SIGNAL(sendText(QString)), this, SLOT(setEnv(QString)));
    connect(
      envGraph, SIGNAL(actEnvGraphChanged(QList<double>, QList<double>, QList<QString>)),
      this, SLOT(setEnv(QList<double>, QList<double>, QList<QString>))
      );
    connect(durationBox, SIGNAL(actValueChanged(QString)), this, SLOT(setDuration(QString)));

    mBridge->evaluate(tr("~%1.set(\\%2, BusPlug.for(%3));").arg(nodeName, controlName, QString::number(busIndex)), true);

    // this->setEnv(this->getEnv());
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

    durationBox = new ControlBox(this);
    durationBox->setLabel("quant");
    durationBox->setValue("1");
    durationBox->setLabelSize(45);
    durationBox->setColorBackground(QColor(60, 30, 30));
    durationBox->setColorText(QColor(120, 120, 120));
  }

  void ControlEnvelope::getEnvArray(QString envCode)
  {
    float minLevel = 0;
    float maxLevel = 1;

    levels = QList<double>();
    times = QList<double>();
    curves = QList<QString>();

    QStringList answer;
    while (true)
    {
      answer = mBridge->question(tr("%1.asArray").arg(envCode)).toStringList();
      // odchytava spatnou odpoved ze servru, proc prichazi single cislo????
      if (answer.size() % 4 == 0) { break; }
    }
    qDebug() << "ControlEnvelope:: answer: " << answer;

    if (answer.isEmpty())
    {
      qDebug() << "ControlEnvelope answer: CHYBA V ENV KODU";
    }
    else
    {
      for (int i = 0; i < answer.size(); i += 4)
      {
        if (answer[i].toFloat() > maxLevel) { maxLevel = answer[i].toFloat(); }
        if (answer[i].toFloat() < minLevel) { minLevel = answer[i].toFloat(); }

        if (i == 0)
        {
          // qDebug() << "pocet levelu: " << answer[i + 1].toInt();
          // qDebug() << "level: " << answer[i].toDouble();
          if (answer[i + 1].toInt() != cntVertex)
          {
            cntVertex = answer[i + 1].toInt();
            changedCntVertex = true;
          }
          levels.append(answer[i].toDouble());
        }
        else
        {
          levels.append(answer[i].toDouble());
          times.append(answer[i + 1].toDouble());

          //qDebug() << "level: " << answer[i];
          //qDebug() << "time: " << answer[i + 1];
          //qDebug() << "txtSymbol: " << answer[i + 2];
          //qDebug() << "txtCurve: " << answer[i + 3];

          QString symbol;
          switch (answer[i + 2].toInt())
          {
          case 0: symbol = "step"; break;
          case 1: symbol = "lin"; break;
          case 2: symbol = "exp"; break;
          case 3: symbol = "sin"; break;
          case 4: symbol = "welch"; break;
          case 5: symbol = "nil"; break;
          case 6: symbol = "sqr"; break;
          case 7: symbol = "cub"; break;
          case 8: symbol = "hold"; break;
          default: symbol = "lin"; break;
          }
          //qDebug() << "symbol: " << symbol;

          if (symbol != "nil") { curves.append(tr("'%1'").arg(symbol)); }
          else { curves.append(answer[i + 3]); }
        }
        //qDebug() << "///////////////////\n";
      }

      // set duration by quant
      duration = mBridge->question(tr("%1.totalDuration").arg(envCode)).toString().toFloat();

      float restTime = durationBox->getValue() - duration;
      // qDebug() << "duration quant reduce by " << restTime;
      if (times[times.size() - 1] + restTime >= 0)
      {
        times[times.size() - 1] += restTime;
        duration = durationBox->getValue();
      }
      else
      {
        mBridge->msgWarningAct(tr("Set duration is too high. Env sum of duration is set back to %1").arg(QString::number(durationBox->getValue())));
        this->setEnv(previousEnv);
      }

      envGraph->setDomainX(0, duration);
      envGraph->setDomainY(minLevel, maxLevel);
    }
  }

  void ControlEnvelope::setEnv(QString envCode)
  {
    qDebug() << "ControlEnvelope::setEnv: " << envCode;

    //stane se jen pri prvnim nastaveni env
    if (env.isEmpty()) { envelopeCode->setText(envCode); }
    previousEnv = env;
    env = envCode;

    changedCntVertex = false;
    this->getEnvArray(env);

    // nastaveni vertexu
    if (!changedCntVertex)
    {
      qDebug() << "pocet controlnich bodu nezmenen";
      for (int i = 0; i < levels.size(); i++)
      {
        QPointF vertex = this->getEnvVertex(i);
        // qDebug() << vertex << "no:" << i << "levels.size(): " << levels.size();
        envGraph->setVertexPoint(i, vertex);
       // envGraph->setVertexType(i, GraphPoint::PointType::vertex);
       // if (i == 0) { envGraph->setVertexType(i, GraphPoint::PointType::startPoint); }
       // if (i == levels.size() - 1) { envGraph->setVertexType(i, GraphPoint::PointType::endPoint); }

        if (i < levels.size() - 1) // times je o cntVertex - 1
        {
          QPointF midPoint = this->getEnvMidCurve(i);
          envGraph->setCurvePoint(i, midPoint);
          envGraph->setCurveCurvature(i, curves[i]);
        }
      }
    }
    else
    {
      envGraph->deleteGraph();
      qDebug() << "pocet controlnich bodu ZMENEN";
      for (int i = 0; i < levels.size(); i++)
      {
        QPointF vertex = this->getEnvVertex(i);
        // qDebug() << vertex << "no:" << i << "levels.size(): " << levels.size();
        // GraphPoint *pt = envGraph->addVertexPoint(vertex);
        if (i == 0) { qDebug() << "startVertex";  envGraph->addStartPoint(vertex); }
        else if (i == levels.size() - 1) { qDebug() << "endVertex"; envGraph->addEndPoint(vertex); }
        else { qDebug() << "vertex"; envGraph->addVertexPoint(vertex); }

        if (i < levels.size() - 1) // times je o cntVertex - 1
        {
          QPointF midPoint = this->getEnvMidCurve(i);
          GraphPoint *mid = envGraph->addCurvePoint(midPoint);
          mid->setCurvature(curves[i]);
        }
      }
    }
    // QString env = this->getEnv();
    //qDebug() << "ControlEnvelope::levels: " << levels;
    //qDebug() << "ControlEnvelope::timse: " << times;
    //qDebug() << "ControlEnvelope::curve: " << curves;
    qDebug() << "ControlEnvelope::getEnv(): " << env;

    envelopeCode->setText(env);
    envGraph->drawPolyline(this->getEnvPoints(50));

    /*
    for (int i = 0; i < curves.size(); i++)
    {
    qDebug() << "curves: " << curves[i];
    QVector<QPointF> pts;
    GraphPoint *from = envGraph->getVertex(i);
    GraphPoint *to = envGraph->getVertex(i + 1);
    pts.append(QPointF(from->pixelX, from->pixelY));



    //switch (curves[i])
    //{
    //case "'lin'":
    // qDebug() << "curves LIN found at " << curves[i];
    //break;
    //default:
    // break;
    //}

    pts.append(QPointF(to->pixelX, to->pixelY));

    if (changedCntVertex)
    {
    GraphCurve *crv = new GraphCurve(this, from, to);
    crv->show();

    envGraph->addPolyline(pts);
    }
    else
    {

    }
    }
    */



    update();
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
    //envelopeCode->setText(codeEnv);
    this->setEnv(codeEnv);
  }

  void ControlEnvelope::setDuration(QString val)
  {
    double newDuration = val.toDouble();
    // qDebug() << "newDuration set to " << newDuration;
    double restTime = newDuration - duration;
    if (restTime > 0)
    {
      times[times.size() - 1] += restTime;
      envGraph->setDomainX(0, newDuration);
      // qDebug() << "restTime set to " << restTime;
      this->setEnv(this->getEnv());
    }
    else
    {
      if (times[times.size() - 1] + restTime >= 0)
      {
        times[times.size() - 1] += restTime;
        envGraph->setDomainX(0, newDuration);
        //  qDebug() << "restTime reduce by " << restTime;
        this->setEnv(this->getEnv());
      }
      else
      {
        durationBox->setValue(QString::number(duration));
        mBridge->msgWarningAct(tr("Set of duration is lower than the penultimate vertex time. Quant is set back to %1").arg(QString::number(duration)));
      }
    }
  }

  QString ControlEnvelope::getEnv()
  {
    QStringList txtLevels;
    QStringList txtTime;
    QStringList txtCurves;

    foreach(double oneLevel, levels) {
      QString txt = QString::number(oneLevel, 'f', 2);
      QStringList parts = txt.split(".");
      QString decVal = parts[1];
      if (!decVal.isEmpty())
      {
        if (decVal.endsWith("00")) { decVal.chop(2); }
        if (decVal.endsWith("0")) { decVal.chop(1); }
        if (!decVal.isEmpty()) { decVal.prepend("."); }
      }
      txtLevels.append(tr("%1%2").arg(parts[0], decVal));
    }
    foreach(double oneTime, times) {

      QString txt = QString::number(oneTime, 'f', 2);
      QStringList parts = txt.split(".");
      QString decVal = parts[1];
      if (!decVal.isEmpty())
      {
        if (decVal.endsWith("00")) { decVal.chop(2); }
        if (decVal.endsWith("0")) { decVal.chop(1); }
        if (!decVal.isEmpty()) { decVal.prepend("."); }
      }
      txtTime.append(tr("%1%2").arg(parts[0], decVal));
    }
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
    for (int i = 0; i < levels.size(); i++)
    {
      //qDebug() << "getEnvVertex ID: " << ID << "i: " << i << "times.size(): " << times.size();
      if (ID == i) { break; }
      else { vertexTime += times[i]; }
    }
    return QPointF(vertexTime, levels[ID]);
  }

  QPointF ControlEnvelope::getEnvMidCurve(int ID)
  {
    double midPtTime = 0;
    for (int i = 0; i < times.size(); i++)
    {
      if (ID == i)
      {
        midPtTime += times[i] / 2;
        break;
      }
      else { midPtTime += times[i]; }
    }
    QString env = this->getEnv();
    QString txtVal = mBridge->question(tr("%1.at(%2)").arg(env, QString::number(midPtTime))).toString();
    return QPointF(midPtTime, txtVal.toDouble());
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
      QString txtVal = mBridge->question(tr("%1.at(%2)").arg(env, QString::number(xVal))).toString();
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
    // pridat s.makeBundle(time2Quant, code) pro synchronizaci
    mBridge->evaluate(code);
  }

  void ControlEnvelope::mouseReleaseEvent(QMouseEvent *mouseEvent)
  {
    mBridge->question(tr("~%1.nodeID").arg(nodeName), true);
    mBridge->question(tr("~%1.source.def.sourceCode").arg(nodeName), true);
    mBridge->question(tr("~%1.isPlaying").arg(nodeName), true);

    envGraph->drawPolyline(this->getEnvPoints(200));

    this->setEnv(this->getEnv());
  }

  void ControlEnvelope::resizeEvent(QResizeEvent *event)
  {
    nameLabel->setGeometry(5, 5, 95, 25);
    busLabel->setGeometry(100, 5, 100, 25);
    envelopeCode->setGeometry(5, 30, width() - 90, 25);
    envGraph->setGeometry(5, 60, width() - 10, height() - 65);
    durationBox->setGeometry(width() - 80, 30, 80, 25);
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


