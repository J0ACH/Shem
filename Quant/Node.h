#ifndef NODE_H
#define NODE_H

#include "ControlBox.h"
#include "CodeEditor.h"
#include "ControlEnvelope.h"
#include "Graph.h"
#include "ScBridge.h"
#include "Customize.h"

#include <QWidget>
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
    Node(QWidget *parent, ScBridge *bridge, Customize *customize, QString name, int nodeNum);
    ~Node();

    StateNodePlay stateNodePlay;

    void setName(QString);
    void setSourceCode(QString);

    QString nodeName;
    QString getNodeID();

    //QString name();
    QRect bounds();

    public slots:
    void fitControlsPosition();
   // void onConfigData(QMap<QString, QVariant*> config);
    void onCustomize();

    void changeNodePlay();  
    void setNodeVolume(QString);
    void setNodeFadeTime(QString);
    void sendSourceCode(QString txt);

  signals:
    void killAct(QString);
    void actChangedHeight();

  protected:
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    virtual bool eventFilter(QObject * watched, QEvent * event);

  private:
    void initControl();
    void initNode();

    void addControl(QString name);
    void removeControl(QString name);

    int nodeNumber;
    int nodeBusIndexReserve;
    int nextEmptyBusIndex();

    QMap<QString, QVariant*> configData;
    QColor colorAppHeaderBackground, colorPanelBackground, colorNormal, colorOver, colorActive, colorText;
    QFont fontTextBig, fontTextSmall, fontTextCode;
    ScBridge *mBridge;
    Customize *mCustomize;

    QLabel *nameLabel;
    
    ControlBox *volumeBox, *fTimeBox;
    QString volume, fTime;

    QLabel *labelNodeID, *labelNamedControls;
    CodeEditor *sourceCode;
    Button *closeButton, *playButton;

    QMap<QString, ControlEnvelope*> conteinerControlsGraph;
    QStringList getControlKeys();
    /*
    int quant;
    float timeToNextQuant();
    */
  };
}

#endif // QUANT

