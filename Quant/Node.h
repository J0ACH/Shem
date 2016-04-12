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

		void onReciveText(QString);
		void onRecivedControls(QStringList);
		
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
		QLabel *labelControls;
		CodeEditor *sourceCode;
		Button *closeButton, *playButton;
		
	};
}

#endif // QUANT

