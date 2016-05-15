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

		QString nextID();
		bool evaluateNEW(QString code, bool print = false, bool silent = false);
		QVariant questionNEW(QString code, bool print = false);

	private slots:
		void onReadyRead(void);
		void onNewIpcConnection();
		void finalizeConnection();
		void onIpcData();

    private slots:
    void onReadyRead(void);
    void onNewIpcConnection();
    void finalizeConnection();
    void onIpcData();

    void killBridge();
    void changeInterpretState();
    void changeServerState();

    void evaluateCode(QString const & commandString, bool silent = false, bool printAnswer = false);
    void question(QUuid id, int selector, QString commandString, bool printAnswer = false);

  signals:
    void interpretBootInitAct();
    void interpretBootDoneAct();
    void interpretKillInitAct();
    void interpretKillDoneAct();

    void serverBootInitAct();
    void serverBootDoneAct();
    void serverKillInitAct();
    void serverKillDoneAct();

		void answerAct(QUuid id, int selector, QStringList answer);
		void actSynced();
		void actAnswered();

    void answerAct(QUuid id, int selector, QStringList answer);

    void killBridgeDoneAct();

    //void response(const QString & selector, const QString & data);

		int lateFlagBreakTime;
		bool mTerminationRequested;
		QDateTime mTerminationRequestTime;
		bool mCompiled;

    bool mTerminationRequested;
    QDateTime mTerminationRequestTime;
    bool mCompiled;

    void startInterpretr();
    void killInterpreter();

		void onResponse(const QString & selector, const QString & data);
				
		//QStringList answer;
		QVariant answer;
	};

    void onResponse(const QString & selector, const QString & data);
  };

}

#endif // SCBRIDGE


