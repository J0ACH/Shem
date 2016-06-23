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
   // GraphVertex getEnvVertex(int ID);
    // QPointF getEnvVertex(int ID);
   // QPointF getEnvMidCurve(int ID);
    QVector<QPointF> getEnvPoints(int segments);

    void freeControlBusIndex();
    

    public slots:
    void setEnv(QString envCode);
    void setEnv(QList<double> listLevels, QList<double> listTimes, QList<QString> listCurves);
    void setDuration(QString);

    void onNextBeat();

  signals:
    void actEnvChanged(QList<double> levels, QList<double> times, QList<QString> curves);

  protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mouseReleaseEvent(QMouseEvent *mouseEvent);

  private:
    ScBridge *mBridge;
    QString nodeName, controlName;

    float duration;
    int currentBeat;
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

    void initControl();
    void makeTask(QString env, float startTime);
    float timeToNextQuant();
  };
}

#endif // CONTROLGRAPH

