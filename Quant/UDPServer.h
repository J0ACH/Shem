#ifndef UDPServer_H
#define UDPServer_H

#include "CodeEditor.h"
#include "ScBridge.h"

#include <QDebug>
#include <QLabel>
#include <QMap>
#include <QPainter>
#include <QPushButton>
#include <QTextEdit>
#include <QWidget>

using namespace Jui;
using namespace SupercolliderBridge;

namespace QuantIDE {
class UDPServer : public QWidget {
Q_OBJECT

public:
UDPServer(QWidget *parent = 0);
~UDPServer();
}

private: QUdpSocket socket;
}

#endif // end UDPServer
