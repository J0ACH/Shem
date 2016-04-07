#ifndef NODE_H
#define NODE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QTextEdit>
#include <QDebug>

#include "CodeEditor.h"
using namespace Jui;

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

	public slots:
		void fitGeometry();
		void reciveText(QString);

	signals:
		void evaluateAct(QString);

	protected:
		void paintEvent(QPaintEvent *event);

	private:
		void initControl();

		QLabel *nameLabel;
		CodeEditor *sourceCode;

		//QPushButton *buttEvaluate;
		//QTextEdit *sourceCode;
	};
}

#endif // QUANT

