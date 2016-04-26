#ifndef CONTROLGRAPH_H
#define CONTROLGRAPH_H

#include "ScBridge.h"
//#include "Node.h"
#include "CodeEditor.h"

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QDebug>


using namespace Jui;
using namespace SupercolliderBridge;

namespace QuantIDE
{

	class ControlEnvelope : public QWidget
	{
		Q_OBJECT

	public:
		ControlEnvelope(QWidget *parent, ScBridge *bridge, QString controlName);
		~ControlEnvelope();

		enum QuestionType { envLevels, envTimes, envCurves };

		QRect bounds();
				
	public slots:
		void onEnvelopeCodeEvaluate();
		void onBridgeQuestion(QuestionType selector, QString args = QString::null);
		void onBridgeAnswer(QString pattern, int selectorNum, QStringList answer);
		
	signals:
		void bridgeQuestionAct(QString pattern, int selectorNum, QString question, bool print);

	protected:
		void paintEvent(QPaintEvent *event);


	private:
		ScBridge *mBridge;
		QString name;
		QString objectPattern;
		QUuid objectID;

		CodeEditor *envelopeCode;
		//QLabel *envelopeLabel;

		void initControl();
	};
}

#endif // CONTROLGRAPH

