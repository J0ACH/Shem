#ifndef QUANT_H
#define QUANT_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>

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

	signals:
		void bootInterpretrAct();
		void bootServerAct();
		void killInterpretrAct();
		void killServerAct();

	public slots:
		//void consoleAddMsg(QString);
		void switchInterpretr();
		void switchServer();

		void addNode();
		void fitGeometry();

		//void evaluateAct(QString);

	public slots:
		void evaluateCode();

	protected:
		void closeEvent(QCloseEvent *event);
		void paintEvent(QPaintEvent *event);

	private:
		void initControl();

		ScBridge *bridge;
		Canvan *canvan;
		NodePanel *nodePanel;

		QTextEdit *globalCode;
		Button *buttEvaluate;
		Button *buttLang, *buttServer;

		CodeEditor *testCode;
	};
}
#endif // QUANT

