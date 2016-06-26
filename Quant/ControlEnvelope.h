#ifndef CONTROLGRAPH_H
#define CONTROLGRAPH_H

#include "ScBridge.h"
#include "CodeEditor.h"
#include "Graph.h"
#include "ControlBox.h"

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QDebug>


using namespace Jui;
using namespace SupercolliderBridge;

namespace QuantIDE
{

  class ControlEnvelope : public QWidget
  {
    Q_OBJECT

  public:
    ControlEnvelope(QWidget *parent, ScBridge *bridge, QString nodeName, QString controlName, int cBus);
    ~ControlEnvelope();

    QRect bounds();
    int busIndex;

    void getEnvArray(QString env);
    QString getEnv();

    void freeControlBusIndex();
    void resetTime();

    public slots:
    void setEnv(QString envCode);
    void setEnv(QList<double> listLevels, QList<double> listTimes, QList<QString> listCurves);
    void setDuration(QString);

    void onTempoChanged();
    void onNextBeat(int beat);
    void onDurationTimerTick();

  signals:
    void actEnvChanged(QList<double> levels, QList<double> times, QList<QString> curves);

  protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mouseReleaseEvent(QMouseEvent *mouseEvent);

  private:
    ScBridge *mBridge;
    QString nodeName, controlName;

    int quant;
    float duration;
    int cntVertex;
    bool changedCntVertex;
    QString env, previousEnv;
    QList<double> levels;
    QList<double> times;
    QList<QString> curves;

    QLabel *nameLabel, *busLabel;
    ControlBox *durationBox;

    CodeEditor *envelopeCode;
    Graph *envGraph;

    QTimer *durationTimer;
    float currentTime, currentTime_step;

    void initControl();
    void makeTask(QString env);
  };
}

#endif // CONTROLGRAPH

