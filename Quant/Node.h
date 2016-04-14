#ifndef NODE_H
#define NODE_H

#include "CodeEditor.h"
#include "ScBridge.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QTextEdit>
#include <QDebug>


using namespace Jui;
using namespace SupercolliderBridge;

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

		void connectBridge(ScBridge*);
		void setName(QString);
		void setSourceCode(QString);

		QString name();

		QRect bounds();

	public slots:
		void fitGeometry();

		void onReciveText(QString);
		void onRecivedControls(QStringList);

		void onBridgeAnswer(QString, QString);
		
		void changeNodePlay();

	signals:
		void evaluateAct(QString);
		void killAct(QString);
		void idNodeAct(QString, QString, bool);

	protected:
		void closeEvent(QCloseEvent *event);
		void paintEvent(QPaintEvent *event);

	private:
		void initControl();
		QString objectPattern;

		ScBridge *mBridge;

		QLabel *nameLabel;
		QLabel *labelControls;
		CodeEditor *sourceCode;
		Button *closeButton, *playButton;
		
	};
}

#endif // QUANT

