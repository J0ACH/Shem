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

	class Quant : public Canvan
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
		void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
		void closeEvent(QCloseEvent *event);

	private:
		void initControl();
		

		ScBridge *bridge;
		//Canvan *canvan;
		Button *buttAddNode;

		void draw();

		
	};
}
#endif // QUANT

