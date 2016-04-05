#ifndef NODE_H
#define NODE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QTextEdit>
#include <QDebug>

//#include "Quant.h"

namespace QuantIDE
{

	class Node : public QWidget
	{
		Q_OBJECT

	public:
		Node(QWidget *parent = 0);
		~Node();

		void setName(QString);
		void setSourceCode(QString);

		QRect bounds();
		

	signals:
		void evaluateAct(QString);

	public slots:		
		void fitGeometry();
		void evaluateCode();

	protected:
		void paintEvent(QPaintEvent *event);

	private:
		void initControl();

		QLabel *nameLabel;
		QPushButton *buttEvaluate;
		QTextEdit *sourceCode;
	};
}

#endif // QUANT

