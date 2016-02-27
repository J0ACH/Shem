#ifndef SCBRIDGE_H
#define SCBRIDGE_H

/*
#include <QAbstractNativeEventFilter>
#include <QAction>
#include <QObject>
*/

//#include "C:/Supercollider/Supercollider_src/editors/sc-ide/core/sc_process.hpp"
/*
#include "sc_process.hpp"
#include "sc_server.hpp"
#include "doc_manager.hpp"
#include "settings/manager.hpp"
*/


//using namespace ScIDE

//class SessionManager;

class ScBridge
{


public:
	ScBridge();
	~ScBridge();
private:

};

/*
class QntInstanceGuard :
public QObject, public IIpcHandler
{
Q_OBJECT

public:
static const int Port = 6770;

bool tryConnect(QStringList const & arguments);
void onIpcLog(const QString & message) override;
void onIpcMessage(const QString &selector, const QVariantList &data) override;

public Q_SLOTS:
void onNewIpcConnection();
void onIpcData();


private:
IIpcHandler * mHandler;
ScIpcChannel * mIpcChannel;
QTcpServer * mIpcServer;
QTcpSocket * mIpcSocket;
};
*/


#endif // SCBRIDGE
