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

namespace SupercolliderBridge
{
	enum class StateInterpret{ OFF, RUNNING };
	enum class StateServer{ OFF, RUNNING };

	class ScBridge : public QProcess
	{
		Q_OBJECT

	public:
		ScBridge(QObject *parent);
		~ScBridge();

		StateInterpret stateInterpret;
		StateServer stateServer;

	public slots:
		void killBridge();
		void changeInterpretState();
		void changeServerState();

		void evaluateCode(QString const & commandString, bool silent = false);

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
		void msgAnswerAct(QString const &);
		void msgBundleAct(QString const &);

		void killBridgeDoneAct();

		//void response(const QString & selector, const QString & data);

	private slots:
		void onReadyRead(void);
		void onNewIpcConnection();
		void finalizeConnection();
		void onIpcData();

	private:
		QLocalServer *mIpcServer;
		QLocalSocket *mIpcSocket;
		QString mIpcServerName;
		QByteArray mIpcData;

		bool mTerminationRequested;
		QDateTime mTerminationRequestTime;
		bool mCompiled;

		void startInterpretr();
		void killInterpreter();

		void onResponse(const QString & selector, const QString & data);

		QString oscFullMsg;
		bool oscMsgProcess;
	};

}

#endif // SCBRIDGE


