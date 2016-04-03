#ifndef QUANT_H
#define QUANT_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>

#include "ScBridge.h"

#include "Canvan.h"
#include "Panel.h"
#include "Edges.h"

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

	public slots:
		void consoleAddMsg(QString);
		void addNode();
		//void startLang();
		void beep();

	protected:
		void resizeEvent(QResizeEvent *event);
		void closeEvent(QCloseEvent *event);
		void paintEvent(QPaintEvent *event);

	private:
		void initControl();

		ScBridge *bridge;
		Canvan *canvan;
		Panel *nodePanel;

		Button *buttLang, *buttServer, *buttAddNode;
		QPushButton *testButton;
		Panel *testPanel, *testPanel2;

		Node *testNode;
	};
}
#endif // QUANT

