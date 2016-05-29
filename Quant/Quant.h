#ifndef QUANT_H
#define QUANT_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QDebug>

#include "ScBridge.h"
#include "Customize.h"
#include "UDPServer.h"
#include "Canvan.h"
#include "Panel.h"
#include "Edges.h"
#include "CodeEditor.h"

#include "NodePanel.h"
#include "Node.h"

using namespace SupercolliderBridge;
using namespace Jui;

namespace QuantIDE
{

	class Quant : public QWidget
	{
		Q_OBJECT

	public:
		Quant(QWidget *parent = 0);
		~Quant();

	signals:
		void bootInterpretAct();
		void bootServerAct();
		void actConfigDone();
		void actConfigData(QMap<QString, QVariant*> config);
		void print(QString, QColor);
		void println(QString, QColor);

	public slots :
		void onMsgNormal(QString);
		void onMsgStatus(QString);
		void onMsgEvaluate(QString);
		void onMsgResult(QString);
		void onMsgError(QString);
		void onMsgWarning(QString);
		void onMsgBundle(QString);

		void onInterpretBootInit();
		void onInterpretBootDone();
		void onInterpretKillInit();
		void onInterpretKillDone();

		void onServerBootInit();
		void onServerBootDone();
		void onServerKillInit();
		void onServerKillDone();

		void onConfigDataDone();

		void fitGeometry();
		void onConfigData(QMap<QString, QVariant*> config);
		void onServerTask();

		void onRecivedGlobalCode(QString);
		
	protected:
		void closeEvent(QCloseEvent *event);
		void paintEvent(QPaintEvent *event);

	private:
		void initControl();
		
		ScBridge *bridge;
		Customize *customize;
  UDPServer *udpServer;
		Canvan *canvan;
		NodePanel *nodePanel;

		QColor colorAppBackground, colorPanelBackground, colorNormal, colorOver, colorActive, colorText;
		QColor colorMsgNormal, colorMsgStatus, colorMsgEvaluate, colorMsgResult, colorMsgError, colorMsgWarning, colorMsgBundle;
		QFont fontTextSmall, fontTextCode;

		Button *buttLang, *buttServer, *buttConsol, *buttNodes, *buttCustomize;
		CodeEditor *globalCode;

		QLabel *labelServerMeter, *labelServerSynths;
		QTimer serverTask;
	};
}
#endif // QUANT

