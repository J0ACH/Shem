#ifndef CONTROLGRAPH_H
#define CONTROLGRAPH_H

#include "ScBridge.h"
#include "CodeEditor.h"
#include "Graph.h"

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

    enum QuestionType {
      envArray,
      envAt,
      graphAt,
      redrawEnvGraph,
      initBusIndex
    };

    QRect bounds();
    int busIndex;
    
    void setEnv(QList<double> listLevels, QList<double> listTimes, QList<QString> listCurves);
    QString getEnv();
    QVector<QPointF> getEnvValues(int segments);

    void freeControlBusIndex();
   
   public slots:
    void setEnv(QString envCode);
    void sendTask();

    void onEnvelopeCodeEvaluate();
    void onGraphEnv(QList<double> levels, QList<double> times, QList<double> curves);
   // void onBridgeQuestion(QuestionType selector, QString args = QString::null);
   // void onBridgeAnswer(QUuid id, int selectorNum, QStringList answer);

  signals:
   // void actCodeEvaluated(QString, bool silent = false, bool print = false);
   // void bridgeQuestionAct(QUuid id, int selectorNum, QString question, bool print);
    void actGraphEnv(QList<double> levels, QList<double> times, QList<double> curves);
    void actChangeEnvCode(QString txt);

  protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mouseReleaseEvent(QMouseEvent *mouseEvent);
    //virtual bool eventFilter(QObject * watched, QEvent * event);


  private:
    ScBridge *mBridge;
    QUuid objectID;
    QString nodeName, controlName;

    double duration;
    int numLevelPoints;
    QList<double> levels;
    QList<double> times;
    QList<QString> symbols;
    QList<int> curves;

    QVector<QPointF> graphPolyline;
    QList<double> graphCurveX;
    QList<double> graphCurveY;
    QList<double> midCurvePointX;
    QList<double> midCurvePointY;

    QLabel *nameLabel, *busLabel;

    //QLabel *levelLabel, *timeLabel, *curveLabel;
    CodeEditor *envelopeCode;
    Graph *envGraph;

    void initControl();
  };
}

#endif // CONTROLGRAPH

