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

		////////////////
		// question/answer test

		QUuid mId;
		QString mCommand;
		QString mData;
		void send(const QString & command, const QString & data);
		void sendRequest(const QString &id, const QString &command, const QString &data);

		/////////////////

	public slots:
		void killBridge();
		void changeInterpretState();
		void changeServerState();

		void evaluateCode(QString const & commandString, bool silent = false);
		////////////////
		// question/answer test
		//void question(QString const & commandString);
		////////////////

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

		void killBridgeDoneAct();

		void response(const QString & selector, const QString & data);

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
	};

	////////////////
	// question/answer test

	class ScRequest : public QObject
	{
		Q_OBJECT
	public:
		ScRequest(ScBridge *sc, QObject * parent = 0) :
			QObject(parent),
			mSc(sc)
		{
			connect(mSc, SIGNAL(response(QString, QString)),
				this, SLOT(onResponse(QString, QString)));

			connect(mSc, SIGNAL(classLibraryRecompiled()),
				this, SLOT(cancel()));
		}

		void send(const QString & command, const QString & data)
		{
			mId = QUuid::createUuid();
			mCommand = command;
			mSc->sendRequest(mId.toString(), command, data);
		}

		public slots:
		void cancel()
		{
			mId = QUuid();
			emit cancelled();
		}

	signals:
		void response(const QString & command, const QString & data);
		void cancelled();

		private slots:
		void onResponse(const QString & responseId, const QString & responseData)
		{
			if (responseId == mId.toString())
				emit response(mCommand, responseData);
		}

	private:
		QString mCommand;
		QUuid mId;
		ScBridge *mSc;
	};

	///////////
}

#endif // SCBRIDGE


