#ifndef NODE_H
#define NODE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QPainter>

//#include "Quant.h"

namespace QuantIDE
{

	class Node : public QWidget
	{
		Q_OBJECT

	public:
		Node(QWidget *parent = 0);
		~Node();

		QRect bounds();

	public slots:		
		void fitGeometry();

	protected:
		void paintEvent(QPaintEvent *event);

	private:
		void initControl();

		QPushButton *testButton;
		QLabel *nameLabel;
	};
}

#endif // QUANT

