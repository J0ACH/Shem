#ifndef CONTROLGRAPH_H
#define CONTROLGRAPH_H

#include "ScBridge.h"
#include "CodeEditor.h"
#include "Graph.h"

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

		enum QuestionType {
			envLevels,
			envTimes,
			envCurves,
			envAt
		};

		QRect bounds();

	public slots:
		void onEnvelopeCodeEvaluate();
		void onBridgeQuestion(QuestionType selector, QString args = QString::null);
		void onBridgeAnswer(QUuid id, int selectorNum, QStringList answer);

	signals:
		void bridgeQuestionAct(QUuid id, int selectorNum, QString question, bool print);

	protected:
		void paintEvent(QPaintEvent *event);
		void resizeEvent(QResizeEvent *event);
		//virtual bool eventFilter(QObject * watched, QEvent * event);


	private:
		ScBridge *mBridge;
		QUuid objectID;
		QString name;
		
		QList<double> levels;
		QList<double> times;
		QList<QString> curves;
		
		//QList<QPointF*> graphCurve;
		QList<double> graphCurveX;
		QList<double> graphCurveY;

		CodeEditor *envelopeCode;
		Graph *envGraph;
		//QLabel *envelopeLabel;


		void initControl();
	};
}

#endif // CONTROLGRAPH

