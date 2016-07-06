#ifndef SCBRIDGE_H
#define SCBRIDGE_H

#include <QAction>
#include <QByteArray>
#include <QDateTime>
#include <QDebug>
#include <QProcess>
#include <QThread>
#include <QUuid>
#include <QtNetwork/QLocalSocket>
#include <QtNetwork/QLocalServer>
#include <QBuffer>
#include <QDebug>
#include <QQueue>
#include <QTimer>
//#include <QStringList>

namespace SupercolliderBridge
{
  enum class StateInterpret{ OFF, RUNNING };
  enum class StateServer{ OFF, RUNNING };
  enum class MessageType{ ERROR, WARNING, STATUS, EVAULATE, RESULT, ANSWER };

  class ScBridge : public QProcess
  {
    Q_OBJECT

  public:
    ScBridge(QObject *parent);
    ~ScBridge();

    StateInterpret stateInterpret;
    StateServer stateServer;

    QString nextID();
    float tempo;
    int beat;
    bool evaluate(QString code, bool print = false, bool silent = false);
    QVariant question(QString code, bool print = false);

    void OSCtest();

    private slots:
    void onReadyRead(void);
    void onNewIpcConnection();
    void finalizeConnection();
    void onIpcData();

    void killBridge();
    void changeInterpretState();
    void changeServerState();

    void onChangeTempo(QString BPM);

  signals:
    void interpretBootInitAct();
    void interpretBootDoneAct();
    void interpretKillInitAct();
    void interpretKillDoneAct();

    void serverBootInitAct();
    void serverBootDoneAct();
    void serverKillInitAct();
    void serverKillDoneAct();

    void msgErrorAct(QString const &);
    void msgWarningAct(QString const &);
    void msgNormalAct(QString const &);
    void msgStatusAct(QString const &);
    void msgEvaluateAct(QString const &);
    void msgResultAct(QString const &);
    void msgBundleAct(QString const &);

    void actSynced();
    void actAnswered();
    void actServerStatus(QStringList); // serverPeak, cntSynths, cntGroups
    void actNextBeat(int beat);
    void actTempoChanged();

    void actGroupAdd(int ID);
    void actGroupFree(int ID);
    void actSynthAdd(int ID);
    void actSynthFree(int ID);

    void killBridgeDoneAct();

    //void response(const QString & selector, const QString & data);

  private:
    QLocalServer *mIpcServer;
    QLocalSocket *mIpcSocket;
    QString mIpcServerName;
    QByteArray mIpcData;

    int lateFlagBreakTime;
    bool mTerminationRequested;
    QDateTime mTerminationRequestTime;
    bool mCompiled;

    void startInterpretr();
    void killInterpreter();

    void msgFilter(QString msg);

    void onResponse(const QString & selector, const QString & data);

        //QStringList answer;
    QVariant answer;
  };

}

#endif // SCBRIDGE


