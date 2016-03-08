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

class ScBridge : public QProcess
{
	Q_OBJECT

public:
	ScBridge(QObject *parent);
	~ScBridge();

	void startLang();
	void read();

public slots:
	void evaluateCode(QString const & commandString, bool silent = false);

signals:
	void scPost(QString const &);
	void statusMessage(const QString &);
	void response(const QString & selector, const QString & data);
	//void classLibraryRecompiled();
	//void introspectionAboutToSwap();

private slots:
	//void onNewIpcConnection();
	//void onIpcData();
	//void finalizeConnection();
	//void onProcessStateChanged(QProcess::ProcessState state);
	void onReadyRead(void);
	//void updateToggleRunningAction();

private:
	QLocalServer *mIpcServer;
	QLocalSocket *mIpcSocket;
	QString mIpcServerName;
	QByteArray mIpcData;

	bool mTerminationRequested;
	QDateTime mTerminationRequestTime;
	bool mCompiled;
};




#endif // SCBRIDGE


	