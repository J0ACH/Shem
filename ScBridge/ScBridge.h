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

namespace SupercolliderBridge
{

	class ScBridge : public QProcess
	{
		Q_OBJECT

	public:
		ScBridge(QObject *parent);
		~ScBridge();
		
	public slots:
		void startLang();
		void killLang();
		void startServer();
		void killServer();
		void evaluateCode(QString const & commandString, bool silent = false);

	signals:
		void bootedLang(bool);
		void bootedServer(bool);

		void scPost(QString const &);
		void statusMessage(QString const &);
		//void response(const QString & selector, const QString & data);


	private slots:
		void onReadyRead(void);
		//void testStart();

	private:
		QLocalServer *mIpcServer;
		QLocalSocket *mIpcSocket;
		QString mIpcServerName;
		QByteArray mIpcData;

		bool mTerminationRequested;
		QDateTime mTerminationRequestTime;
		bool mCompiled;

		
	};

}

#endif // SCBRIDGE


	