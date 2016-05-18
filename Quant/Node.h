#ifndef NODE_H
#define NODE_H

#include "CodeEditor.h"
#include "ControlEnvelope.h"
#include "Graph.h"
#include "ScBridge.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QTextEdit>
#include <QDebug>
#include <QMap>


using namespace Jui;
using namespace SupercolliderBridge;

namespace QuantIDE
{
  enum class StateNodePlay{ PLAY, STOP, FREE };

  class Node : public QWidget
  {
    Q_OBJECT


  public:
    Node(QWidget *parent, ScBridge *bridge, QString name, int nodeNum);
    ~Node();

    StateNodePlay stateNodePlay;
    enum QuestionType { /*nodeID,*/ namedControls, namedValues };

    void setName(QString);
    void setSourceCode(QString);

    QString nodeName;
    QString getNodeID();

    //QString name();
    QRect bounds();

    public slots:
    void fitControlsPosition();
    void onConfigData(QMap<QString, QVariant*> config);

    //void onEvaluateNode();
    //void onReciveText(QString);
    void onBridgeQuestion(QuestionType selector, QString args = QString::null);
    void onBridgeAnswer(QUuid id, int selectorNum, QStringList answer);

    void changeNodePlay();

    void sendFreeNode();
    void sendSourceCode(QString txt);
    void sendSetNode(QString nameControl, QString txt);

  signals:
    void actCodeEvaluated(QString, bool silent = false, bool print = false);
    void killAct(QString);
    void bridgeQuestionAct(QUuid id, int selectorNum, QString question, bool print);
    void actChangedHeight();

  protected:
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    virtual bool eventFilter(QObject * watched, QEvent * event);

  private:
    void initControl();
    void initControlsEditor(QStringList namedControls);
    void initNode();

    void addControl(QString name);
    void removeControl(QString name);

    QUuid objectID;
    
    int nodeNumber;
    int nodeBusIndexReserve;
    QMap<QString, QVariant*> configData;
    QColor colorAppHeaderBackground, colorPanelBackground, colorNormal, colorOver, colorActive, colorText;
    QFont fontTextBig, fontTextSmall, fontTextCode;
    ScBridge *mBridge;

    QLabel *nameLabel;
    QLabel *labelNodeID, *labelNamedControls;
    CodeEditor *sourceCode;
    Button *closeButton, *playButton;

    QMap<QString, ControlEnvelope*> conteinerControlsGraph;

  };
}

#endif // QUANT

