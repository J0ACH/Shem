#ifndef QUANT_H
#define QUANT_H


#include <QWidget>
#include <QPushButton>
#include <QTextEdit>

#include "ScBridge.h"
#include "Canvan.h"
#include "Panel.h"
#include "Edges.h"

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

	protected:
		void resizeEvent(QResizeEvent *event);
		void closeEvent(QCloseEvent *event);
		void paintEvent(QPaintEvent *event);

	private:
		void initControl();

		ScBridge *bridge;
		Canvan *canvan;
		
		Button *buttLang, *buttServer, *buttAddNode;

		Panel *testPanel, *testPanel2;

	};
}
#endif // QUANT

