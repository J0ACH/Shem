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

    setFocusPolicy(Qt::StrongFocus);

    this->initControl();

    quant = 1;
    //currentBeat = 0;
    currentTime = 0;
    currentTime_step = 10;
    durationTimer = new QTimer(this);
    durationTimer->setInterval(currentTime_step);

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
    connect(
      this, SIGNAL(actEnvChanged(QList<double>, QList<double>, QList<QString>)),
      envGraph, SLOT(onEnvChanged(QList<double>, QList<double>, QList<QString>))
      );
    connect(mBridge, SIGNAL(actNextBeat(int)), this, SLOT(onNextBeat(int)));
    connect(mBridge, SIGNAL(actTempoChanged()), this, SLOT(onTempoChanged()));

    connect(durationBox, SIGNAL(actValueChanged(QString)), this, SLOT(setDuration(QString)));
    connect(durationTimer, SIGNAL(timeout()), this, SLOT(onDurationTimerTick()));

    mBridge->evaluate(tr("~%1.set(\\%2, BusPlug.for(%3));").arg(nodeName, controlName, QString::number(busIndex)), true);
    durationTimer->start();
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
          qDebug() << "pocet levelu: " << answer[i + 1].toInt();
          qDebug() << "level: " << answer[i].toDouble();
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

          qDebug() << "level: " << answer[i];
          qDebug() << "time: " << answer[i + 1].toDouble();
          qDebug() << "txtSymbol: " << answer[i + 2];
          qDebug() << "txtCurve: " << answer[i + 3];

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
          qDebug() << "symbol: " << symbol;

          if (symbol != "nil") { curves.append(tr("'%1'").arg(symbol)); }
          else { curves.append(answer[i + 3]); }
        }
        qDebug() << "///////////////////\n";
      }

      // set duration by quant
      duration = mBridge->question(tr("%1.totalDuration").arg(envCode)).toString().toDouble();

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
    //qDebug() << "ControlEnvelope::setEnv: " << envCode;

    //stane se jen pri prvnim nastaveni env

    if (env.isEmpty()) { envelopeCode->setText(envCode); }

    //qDebug() << "Graph::setEnv ENV TEST -> " << env;

    changedCntVertex = false;
    this->getEnvArray(envCode);

    /*
    qDebug() << "Graph::setEnv [string] -> "
    << "levels: " << levels
    << "times: " << times
    << "curves: " << curves;
    */


    previousEnv = env;
    env = this->getEnv();
    qDebug() << "Graph::setEnv [getEnv] -> " << env;

    emit actEnvChanged(levels, times, curves);

    this->makeTask(env);
    envelopeCode->setText(env);
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
    qDebug() << "setEnv [arrays]: " << codeEnv;
    this->getEnvArray(codeEnv);
    env = this->getEnv();
    previousEnv = env;

    envelopeCode->setText(env);
    this->makeTask(env);
  }

  QString ControlEnvelope::getEnv()
  {
    QStringList txtLevels;
    QStringList txtTime;
    QStringList txtCurves;

    qDebug() << "Graph::getEnv -> "
      << "levels: " << levels
      << "times: " << times
      << "curves: " << curves;

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

  void ControlEnvelope::setDuration(QString val)
  {
    quant = val.toInt();
    double newDuration = val.toDouble();
    //qDebug() << "newDuration set to " << newDuration;
    double restTime = newDuration - duration;
    if (restTime > 0)
    {
      times[times.size() - 1] += restTime;
      envGraph->setDomainX(0, newDuration);

      //qDebug() << "restTime set to " << restTime;
    }
    else
    {
      if (times[times.size() - 1] + restTime >= 0)
      {
        times[times.size() - 1] += restTime;
        envGraph->setDomainX(0, newDuration);
        //  qDebug() << "restTime reduce by " << restTime;
      }
      else
      {
        durationBox->setValue(QString::number(duration));
        mBridge->msgWarningAct(tr("Set of duration is lower than the penultimate vertex time. Quant is set back to %1").arg(QString::number(duration)));
      }
    }

    QString newEnv = this->getEnv();
    previousEnv = env;
    env = newEnv;
    this->getEnvArray(env);
    envelopeCode->setText(env);

    emit actEnvChanged(levels, times, curves);
    this->makeTask(env);
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
    QString taskCode;
    taskCode = tr("~%1[%2] = Task { p.clock.timeToNextBeat(%3).wait;	loop { Synth(\\envControl, [\\bus: %2, \\tempo: p.clock.tempo, \\env: [%4]]); %3.wait; }}").arg(
      nodeName,
      QString::number(busIndex),
      QString::number(duration / mBridge->tempo),
      env
      );
    mBridge->evaluate(taskCode);
    
  }

  void ControlEnvelope::mouseReleaseEvent(QMouseEvent *mouseEvent)
  {
    mBridge->question(tr("~%1.nodeID").arg(nodeName), true);
    mBridge->question(tr("~%1.source.def.sourceCode").arg(nodeName), true);
    mBridge->question(tr("~%1.isPlaying").arg(nodeName), true);

    //this->setEnv(this->getEnv());
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

  void ControlEnvelope::onNextBeat(int beat)
  {
    // problem s quantem a tempem
    // spravne if (beat % quant/mBridge->tempo == 0) ale neni int
     qDebug() << "beat:" << beat;    
    currentTime = beat % quant;
    //if (beat % quant == 0)
    //{
      // qDebug() << "beat:" << beat;    
      //this->resetTime();
    //}

    /*
    qDebug() << "\nControlEnvelope::onNextBeat beat" << beat;

    float divide = beat / (float)duration;
    qDebug() << "divide" << divide;

    int floor = qFloor(divide); 
    qDebug() << "floor" << floor;

    qDebug() << "beat - floor*duration" << beat - floor*duration ;
    if (beat - floor*duration >= duration-1)
    {
      this->resetTime();
    }
    */
    
  }

  void ControlEnvelope::onDurationTimerTick()
  {
    currentTime += currentTime_step * mBridge->tempo / (float)1000;
    envGraph->setTime(currentTime);
    this->update();
    // if (currentTime >= domainX.second) { currentTime = 0.0; }
  }

  void ControlEnvelope::onTempoChanged()  { this->makeTask(env); durationTimer->start(); }
  void ControlEnvelope::resetTime()  { currentTime = 0.0; }

  ControlEnvelope::~ControlEnvelope()  {  }
}


