#ifndef QUANT_H
#define QUANT_H


#include <QWidget>
#include <QPushButton>
#include <QTextEdit>

#include "ScBridge.h"
#include "Canvan.h"
#include "Edges.h"
#include "Rect.h"

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

	protected:
		void resizeEvent(QResizeEvent *event);
		void closeEvent(QCloseEvent *event);
	//	void paintEvent(QPaintEvent *event);

	private:
		void initControl();

		ScBridge *bridge;
		Canvan *canvan;
		Button *buttAddNode;

	};
}
#endif // QUANT

