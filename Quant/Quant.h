#ifndef QUANT_H
#define QUANT_H

#include <QWidget>
#include <QDebug>

#include "ScBridge.h"
#include "Customize.h"
#include "UDPServer.h"
#include "QuantCore.h"
#include "Canvan.h"
#include "Panel.h"
#include "Edges.h"
#include "CodeEditor.h"

#include "NodePanel.h"
#include "Node.h"

using namespace SupercolliderBridge;
using namespace Jui;

namespace QuantIDE
{

  class QuantNEW : public QObject
  {
    Q_OBJECT

  public:
    QuantNEW(QObject *parent);
    ~QuantNEW();

    public slots:
    //void onCustomize(Customize*);
    void onCustomize(Data);
    //void onCloseQuant();


  private:
    Customize *customize;
    CanvanNEW *canvanNEW;
    PanelNEW *networkPanel, *customizePanel;
    Console *console;

    QuantCore *core;

   // void initStyleSheet();
  };


  class Quant : public QWidget
  {
    Q_OBJECT

  public:
    Quant(QWidget *parent = 0);
    ~Quant();

    public slots :
    void onMsgNormal(QString);
    void onMsgStatus(QString);
    void onMsgEvaluate(QString);
    void onMsgResult(QString);
    void onMsgError(QString);
    void onMsgWarning(QString);
    void onMsgBundle(QString);

    void onInterpretBootInit();
    void onInterpretBootDone();
    void onInterpretKillInit();
    void onInterpretKillDone();

    void onServerBootInit();
    void onServerBootDone();
    void onServerKillInit();
    void onServerKillDone();

    //void onConfigDataDone();
    void onCustomize();

    void fitGeometry();
    //void onConfigData(QMap<QString, QVariant*> config);

    void onServerStatus(QStringList);

    void onRecivedGlobalCode(QString);

    void onCloseQuant();

  signals:
    void bootInterpretAct();
    void bootServerAct();
    void actConfigDone();
    void actConfigData(QMap<QString, QVariant*> config);
    void print(QString, QColor);
    void println(QString, QColor);

  protected:
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *event);

  private:
    void initControl();
    void initStyleSheet(); // ???? snad bude odstraneno

    ScBridge *bridge;
    Customize *customize;
    UDPServer *udpServer;

    QuantCore *core;
    //QuantObject *proxyTest1, *proxyTest2;

    Canvan *canvan;
    NodePanel *nodePanel;
    Panel *customizePanel, *networkPanel;

    QString userName;
    QColor colorAppBackground, colorPanelBackground, colorNormal, colorOver, colorActive, colorText;
    QColor colorMsgNormal, colorMsgStatus, colorMsgEvaluate, colorMsgResult, colorMsgError, colorMsgWarning, colorMsgBundle;
    QFont fontTextBig, fontTextSmall, fontTextCode;

    Button *buttLang, *buttServer, *buttConsole, *buttCustomize, *buttNetwork;
    CodeEditor *globalCode;

    QLabel *labelServerMeter, *labelServerSynths, *labelServerGroups;
  };
}
#endif // QUANT

