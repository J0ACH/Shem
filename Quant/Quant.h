#ifndef QUANT_H
#define QUANT_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QDebug>

#include "ScBridge.h"

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

	public slots :
		void onInterpretBootInit();
		void onInterpretBootDone();
		void onInterpretKillInit();
		void onInterpretKillDone();

		void onServerBootInit();
		void onServerBootDone();
		void onServerKillInit();
		void onServerKillDone();

		void fitGeometry();
		void addNode();

	protected:
		void closeEvent(QCloseEvent *event);
		void paintEvent(QPaintEvent *event);

	private:
		void initControl();

		ScBridge *bridge;
		Canvan *canvan;
		NodePanel *nodePanel;

		Button *buttLang, *buttServer;
		CodeEditor *globalCode;
	};
}
#endif // QUANT

