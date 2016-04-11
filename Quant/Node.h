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
	enum class StateNodePlay{ PLAY, STOP, FREE };

	class Node : public QWidget
	{
		Q_OBJECT

	public:
		Node(QWidget *parent = 0);
		~Node();

		StateNodePlay stateNodePlay;

		void setName(QString);
		void setSourceCode(QString);

		QString name();

		QRect bounds();

	public slots:
		void fitGeometry();
		void reciveText(QString);

		void changeNodePlay();

	signals:
		void evaluateAct(QString);
		void killAct(QString);

	protected:
		void closeEvent(QCloseEvent *event);
		void paintEvent(QPaintEvent *event);

	private:
		void initControl();

		QLabel *nameLabel;
		CodeEditor *sourceCode;
		Button *closeButton;
		Button *buttNodePlay;
		
	};
}

#endif // QUANT

