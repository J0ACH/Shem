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
		enum QuestionType {nodeID, namedControls};

		void connectBridge(ScBridge*);
		void setName(QString);
		void setSourceCode(QString);

		QString name();

		QRect bounds();

	public slots:
		void fitGeometry();

		void onReciveText(QString);
		void onBridgeQuestion(QuestionType);
		void onBridgeAnswer(QString pattern, int selectorNum, QStringList answer);
		
		void changeNodePlay();

	signals:
		void evaluateAct(QString);
		void killAct(QString);
		void bridgeQuestionAct(QString pattern, int selectorNum, QString question, bool print);
		
	protected:
		void closeEvent(QCloseEvent *event);
		void paintEvent(QPaintEvent *event);

	private:
		void initControl();
		QString objectPattern;

		ScBridge *mBridge;

		QLabel *nameLabel;
		QLabel *labelNodeID, *labelNamedControls;
		CodeEditor *sourceCode;
		Button *closeButton, *playButton;

		
		
	};
}

#endif // QUANT

