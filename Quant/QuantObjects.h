#ifndef QUANTOBJECTS_H
#define QUANTOBJECTS_H

#include <QWidget>
#include <QDebug>
#include <QPainter>

#include "Data.h"
#include "Button.h"
#include "ControlBox.h"
#include "CodeEditor.h"

using namespace SupercolliderBridge;
using namespace Jui;

namespace QuantIDE
{

  class QuantObject : public QWidget
  {
    Q_OBJECT

  public:
    enum ObjectType { USER, PROXY, NODE };

    QuantObject(QWidget *parent, QObject *core);
    ~QuantObject();

    int getType();

  signals:
    void actDataChanged(Data);
    void actEvaluate(QString code, bool print = false);
    void actPrint(QString, MessageType);

  protected: // protected je viditelna jen detmi, ne z venku
    void paintEvent(QPaintEvent *event);
    QWidget *mCanvan;
    QObject *mCore;
    QMetaEnum metaEnum_targetMethods;

    ObjectType objectType;

  private:

  };

  // QUANT USER ////////////////////////////////////////////////////////////////

  class QuantUser : public QuantObject
  {
    Q_OBJECT
      Q_ENUMS(TargetMethod)

  public:

    enum TargetMethod { UserJoin, UserExist, UserLeave, UserServerStatus };

    QuantUser(QWidget *parent, QObject *core);
    ~QuantUser();

    void setName(QString);
    void setServerMeter(QString);
    void setServerSynth(QString);
    void setServerGroup(QString);

    QString getName();
    QString getServerMeter();
    QString getServerSynth();
    QString getServerGroup();

    void sendData(TargetMethod targetMethod);

    public slots:
    void onNet_UserJoin(DataUser);
    void onNet_UserExist(DataUser);
    void onNet_UserLeave(DataUser);
    void onNet_UserServerStatus(DataUser);

    void onServerStatus(QStringList);

  protected: // protected je viditelna jen detmi, ne z venku
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

  private:
    DataUser userData;
    //QMetaEnum metaEnum_targetMethods;

    Text *textName;
    Text *textServerMeter, *textServerSynths, *textServerGroups;

    private slots:
    //void onTestChanged(QString);
  };

  // QUANT PROXYSPACE ////////////////////////////////////////////////////////////////

  class QuantProxy : public QuantObject
  {
    Q_OBJECT
      Q_ENUMS(TargetMethod)

  public:

    enum TargetMethod { ProxyExist, ProxySet };

    QuantProxy(QWidget *parent, QObject *core);
    ~QuantProxy();

    void initProxy();

    void setBPM(int);
    int getBPM();
    double getTempo();

    void sendData(TargetMethod targetMethod);

    public slots:
    void onNet_ProxyExist(DataProxy);
    void onNet_ProxySet(DataProxy);


  protected: // protected je viditelna jen detmi, ne z venku
    void resizeEvent(QResizeEvent *event);

  private:
    Button *testButton;
    ControlBox *tempoBox;
    DataProxy proxyData;

    private slots:
    void onTempoChanged(QString);
    void onBeep();
  };

  // QUANT NODEPROXY ////////////////////////////////////////////////////////////////

  class QuantNode : public QuantObject
  {
    Q_OBJECT
      Q_ENUMS(TargetMethod)
  public:

    enum TargetMethod {
      NodeCreated, NodeKilled, NodePlayingChanged,
      AddCodeEditor, RemoveCodeEditor,
      CodeChanged, CodeEvaluate, NodeDisplay
    };

    QuantNode(QWidget *parent, QObject *core);
    ~QuantNode();

    void setName(QString);
    QString getName();

    // void setCode(int index, QString code);
    void sendData(TargetMethod targetMethod);

  signals:
    void actKilled(QString, bool);
    void actSizeChanged();

    public slots:
    void onNet_NodeCreated(DataNode);
    void onNet_NodeKilled(DataNode);
    void onNet_NodePlayingChanged(DataNode);

    void onNet_AddCodeEditor(DataNode);
    void onNet_RemoveCodeEditor(DataNode);

    void onNet_CodeChanged(DataNode);
    void onNet_CodeEvaluate(DataNode);
    void onNet_NodeDisplay(DataNode);

  protected: // protected je viditelna jen detmi, ne z venku
    void resizeEvent(QResizeEvent *event);

  private:
    DataNode nodeData;

    Button *closeButton, *playButton;
    Text *textName;

    QList<CodeEditor*> codeEditors;
    QList<Text*> codeIndexs;
    QList<Button*> insertCodeButtons, removeCodeButtons;

    bool isPlaying;

    void initControl();
    void addCodeEditor(int index);
    void removeCodeEditor(int index);

    void fitEditorsPosition();

    private slots:
    void onClose();
    void onPlayingChanged();
    void onAddCodeEditor(QString insertButtonName);
    void onRemoveCodeEditor(QString insertButtonName);
    void onCodeChanged(QString indexCodeName, QString code);
    void onCodeEvaluate(QString indexCodeName, QString code);
    void onSourceCursorMoved(int);
  };

  // QUANT CONTROLS ////////////////////////////////////////////////////////////////

  class QuantControl : public QuantObject
  {
    Q_OBJECT
  public:
    QuantControl(QWidget *parent, QObject *core);
    ~QuantControl();
  };


  // QUANT BUS ////////////////////////////////////////////////////////////////

  class QuantBus : public QuantObject
  {
    Q_OBJECT
  public:
    QuantBus(QWidget *parent, QObject *core);
    ~QuantBus();
  };
}
#endif // QUANTOBJECTS_H

