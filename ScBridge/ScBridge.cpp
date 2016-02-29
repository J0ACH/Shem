#include "ScBridge.h"
/*
#include "main.hpp"
#include "settings/manager.hpp"
#include "session_manager.hpp"
#include "util/standard_dirs.hpp"
#include "../widgets/main_window.hpp"
#include "../widgets/help_browser.hpp"
#include "../widgets/lookup_dialog.hpp"
#include "../widgets/code_editor/highlighter.hpp"
#include "../widgets/style/style.hpp"
#include "../../../QtCollider/hacks/hacks_mac.hpp"

#include "yaml-cpp/node.h"
#include "yaml-cpp/parser.h"

#include <QAction>
#include <QApplication>
#include <QBuffer>
#include <QDataStream>
#include <QDir>
#include <QFileOpenEvent>
#include <QLibraryInfo>
#include <QTranslator>
#include <QDebug>
*/

//#include <SC_LanguageClient.h>




ScBridge::ScBridge()
{
//SC_LanguageClient * client = createLanguageClient("sclang");

	//client->runMain();
	//client->setCmdLine("s.boot;");

	/*
	QStringList arguments(QApplication::arguments());
	arguments.pop_front(); // application path

	// Pass files to existing instance and quit
	ScIDE::SingleInstanceGuard guard;
	if (guard.tryConnect(arguments))
		//return 0;

	ScIDE::Main *myMain = ScIDE::Main::instance();
	ScIDE::MainWindow *win = new ScIDE::MainWindow(myMain);
	*/

	

	//ScIDE::Settings::Manager *settings = myMain->settings();
	//ScIDE::SessionManager *sessions = myMain->sessionManager();



	//client->setCmdLine("s.boot");

	//win.msgConsole(QString("ScBridge"));
}
/*
QString ScBridge::getClientName()
{
return client->getName();
}
*/


ScBridge::~ScBridge()
{

}

/*
void QntInstanceGuard::onNewIpcConnection()
{
	mIpcSocket = mIpcServer->nextPendingConnection();
	connect(mIpcSocket, SIGNAL(disconnected()), mIpcSocket, SLOT(deleteLater()));
	connect(mIpcSocket, SIGNAL(readyRead()), this, SLOT(onIpcData()));
}

void QntInstanceGuard::onIpcLog(const QString &message) {

}

bool QntInstanceGuard::tryConnect(QStringList const & arguments)
{
	if (!arguments.empty()) {
		QTcpSocket *socket = new QTcpSocket(this);
		socket->connectToHost(QHostAddress(QHostAddress::LocalHost), QntInstanceGuard::Port);

		QVariantList canonicalArguments;
		foreach(QString path, arguments) {
			QFileInfo info(path);
			canonicalArguments << info.canonicalFilePath();
		}

		if (socket->waitForConnected(200)) {
			ScIpcChannel *ipcChannel = new ScIpcChannel(socket, QString("SingleInstanceGuard"), this);
			ipcChannel->write(QString("open"), canonicalArguments);

			return true;
		}
	}

	mIpcServer = new QTcpServer(this);
	bool listening = mIpcServer->listen(QHostAddress(QHostAddress::LocalHost), QntInstanceGuard::Port);
	if (listening) {
		mIpcChannel = new ScIpcChannel(mIpcServer->nextPendingConnection(), QString("SingleInstanceGuard"), this);

		connect(mIpcServer, SIGNAL(newConnection()), this, SLOT(onNewIpcConnection()));
		return false;
	}
	return false;
}

void QntInstanceGuard::onIpcMessage(const QString &selector, const QVariantList &data) {
	//if (selector == QString("open"))
		//foreach(QVariant path, data)
		//Main::documentManager()->open(path.toString());
		//ScBridge::documentManager()->open(path.toString());
}

void QntInstanceGuard::onIpcData() {
	mIpcChannel->read();
}



static inline QString getSettingsFile()
{
	return standardDirectory(ScConfigUserDir) + "/sc_ide_conf.yaml";
}

*/
