#ifndef QUANT_H
#define QUANT_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QDebug>

#include "ScBridge.h"
#include "Customize.h"
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
		void evaulateAct(QString);
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

		void fitGeometry();
		void onConfigData(QMap<QString, QVariant*> config);
		
	protected:
		void closeEvent(QCloseEvent *event);
		void paintEvent(QPaintEvent *event);

	private:
		void initControl();

		ScBridge *bridge;
		Customize *customize;
		Canvan *canvan;
		NodePanel *nodePanel;

		QColor colorAppBackground, colorPanelBackground, colorNormal, colorOver, colorActive;

		Button *buttLang, *buttServer, *buttConsol, *buttNodes, *buttCustomize;
		CodeEditor *globalCode;
	};
}
#endif // QUANT

