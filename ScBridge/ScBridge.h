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

class ScBridge : public QProcess
{
	Q_OBJECT

public:
	ScBridge(QObject *parent);
	~ScBridge();

	void startLang();
	void read();

private:
	QLocalServer *mIpcServer;
	QLocalSocket *mIpcSocket;
	QString mIpcServerName;

	bool mTerminationRequested;
	QDateTime mTerminationRequestTime;
	bool mCompiled;
};




#endif // SCBRIDGE


	