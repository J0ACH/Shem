#ifndef SCBRIDGE_H
#define SCBRIDGE_H

#include <QtWidgets/QApplication>
#include <QAction>
#include <QByteArray>
#include <QDateTime>
#include <QDebug>
#include <QProcess>
#include <QThread>
#include <QtNetwork/QLocalSocket>
#include <QtNetwork/QLocalServer>
#include <QBuffer>
#include <QDebug>
#include <QQueue>
#include <QTimer>

#include "Data.h"

namespace SupercolliderBridge
{
  class ScBridge : public QProcess
  {
    Q_OBJECT
      Q_ENUMS(FlagType)

  public:

    enum FlagType { ServerStatus, NecoJineho };

    ScBridge(QObject *parent);
    ~ScBridge();

    QString nextID();
    float tempo;
    int beat;
    bool evaluate(QString code, bool print = false, bool silent = false);
    QVariant question(QString code, bool print = false);

    bool isInterpretRunning();
    bool isServerRunning();

    void initOSC(); // bude private

    private slots:
    void onReadyRead(void);
    void onNewIpcConnection();
    void finalizeConnection();
    void onIpcData();

    public slots:
    void killBridge();
    void changeInterpretState();
    void changeServerState();

    void onChangeTempo(QString BPM);

  signals:
    void actInterpretInit();
    void actInterpretInitDone();
    void actInterpretKill();
    void actInterpretKillDone();

    void actServerInit();
    void actServerInitDone();
    void actServerKill();
    void actServerKillDone();

    void actPrint(QString, MessageType type = MessageType::NORMAL);

    void actSynced();
    void actAnswered();
    void actServerStatus(QStringList); // serverPeak, cntSynths, cntGroups
    void actNextBeat(int beat);
    void actTempoChanged();

    void actGroupAdd(int ID);
    void actGroupFree(int ID);
    void actSynthAdd(int ID);
    void actSynthFree(int ID);



    //void response(const QString & selector, const QString & data);

  private:
    enum class StateInterpret { OFF, RUN };
    enum class StateServer{ OFF, RUN };
    enum class BridgeProcess{ NaN, INTERPRET_BOOTING, INTERPRET_KILLING, SERVER_BOOTING, SERVER_KILLING };
    

    QLocalServer *mIpcServer;
    QLocalSocket *mIpcSocket;
    QString mIpcServerName;
    QByteArray mIpcData;

    StateInterpret mInterpretState;
    StateServer mServerState;
    BridgeProcess mBridgeProcess;


    int lateFlagBreakTime;
    bool mTerminationRequested;
    QDateTime mTerminationRequestTime;
    bool mCompiled;

    void startInterpretr();
    void killInterpreter();

    QMetaEnum metaEnum_flagType;
    void msgFilter(QString msg);
    void msgFilterNEW(QString msg);

    void onResponse(const QString & selector, const QString & data);

    //QStringList answer;
    QVariant answer;
  };

}

#endif // SCBRIDGE


