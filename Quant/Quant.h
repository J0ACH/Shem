#ifndef QUANT_H
#define QUANT_H


#include <QWidget>
#include <QPushButton>
#include <QTextEdit>

#include "ScBridge.h"
#include "Canvan.h"

using namespace SupercolliderBridge;
using namespace Jui;

namespace QuantIDE
{

	class Quant : public QWidget
	{
		Q_OBJECT

	public:
		explicit Quant(QWidget *parent = 0);
		~Quant();

	public slots:
		void beep();
		void pdefPlay();
		void consoleAddMsg(QString);
		void addNode();


	protected:
		void resizeEvent(QResizeEvent *event);
		void closeEvent(QCloseEvent *event);
		void paintEvent(QPaintEvent *event);

	private:
		void initControl();


		ScBridge *bridge;
		Canvan *canvan;
		Button *buttAddNode;

		QWidget *resizeTest;

	};
}
#endif // QUANT

