#include "ControlEnvelope.h"

namespace QuantIDE
{
	ControlEnvelope::ControlEnvelope(QWidget *parent, ScBridge *bridge, QString controlName) :
		QWidget(parent),
		mBridge(bridge),
		name(controlName)
	{
		this->setObjectName("ControlEnvelope");
		objectID = QUuid::createUuid();

		setFocusPolicy(Qt::StrongFocus);

		this->initControl();
		//this->fitGeometry();

		connect(envelopeCode, SIGNAL(evaluateAct()), this, SLOT(onEnvelopeCodeEvaluate()));

		connect(this, SIGNAL(bridgeQuestionAct(QUuid, int, QString, bool)), mBridge, SLOT(question(QUuid, int, QString, bool)));
		connect(mBridge, SIGNAL(answerAct(QUuid, int, QStringList)), this, SLOT(onBridgeAnswer(QUuid, int, QStringList)));
	}

	QRect ControlEnvelope::bounds() { return QRect(0, 0, width() - 1, height() - 1); }

	void ControlEnvelope::initControl()
	{
		envelopeCode = new CodeEditor(this);
		envelopeCode->setGeometry(5, 5, 300, 25);
		envelopeCode->setText("Env([0,1,0], [0.15,0.85], ['lin', 'sin'])");

		envGraph = new Graph(this);
		envGraph->setGeometry(5, 35, 600, 160);
		envGraph->setDomainX(0, 1);
		envGraph->setDomainY(0, 1);

		int noSeg = envGraph->boundsGraph().width();
		double min = envGraph->getDomainX()[0];
		double max = envGraph->getDomainX()[1];
		for (int i = 0; i <= noSeg; i++)
		{
			graphCurveX.append((max - min) / noSeg*i + min);
		}

		
	}

	void ControlEnvelope::onEnvelopeCodeEvaluate()
	{
		this->onBridgeQuestion(QuestionType::envLevels);
		this->onBridgeQuestion(QuestionType::envTimes);
		this->onBridgeQuestion(QuestionType::envCurves);

		graphCurveY = QList<double>();
		envGraph->deleteGraph();
		foreach(double oneX, graphCurveX) {
			this->onBridgeQuestion(QuestionType::envAt, QString::number(oneX));
		}
	}

	void ControlEnvelope::onBridgeQuestion(QuestionType selector, QString args)
	{
		QString questionCode;
		int selectorNum;

		switch (selector)
		{
		case envLevels:
			selectorNum = QuestionType::envLevels;
			questionCode = tr("%1.levels").arg(envelopeCode->toPlainText());
			emit bridgeQuestionAct(objectID, selectorNum, questionCode, false);
			break;
		case envTimes:
			selectorNum = QuestionType::envTimes;
			questionCode = tr("%1.times").arg(envelopeCode->toPlainText());
			emit bridgeQuestionAct(objectID, selectorNum, questionCode, false);
			break;
		case envCurves:
			selectorNum = QuestionType::envCurves;
			questionCode = tr("%1.curves").arg(envelopeCode->toPlainText());
			emit bridgeQuestionAct(objectID, selectorNum, questionCode, false);
			break;
		case envAt:
			selectorNum = QuestionType::envAt;
			questionCode = tr("%1.at(%2)").arg(envelopeCode->toPlainText(), args);
			emit bridgeQuestionAct(objectID, selectorNum, questionCode, false);
			break;
		}
	}

	void ControlEnvelope::onBridgeAnswer(QUuid id, int selectorNum, QStringList answer)
	{

		if (id == objectID)
		{
			QuestionType selector = static_cast<QuestionType>(selectorNum);

			switch (selector)
			{
			case envLevels:
				foreach(QString oneAnsw, answer) { levels.append(oneAnsw.toDouble()); }
				qDebug() << "ControlEnvelope::envLevels: " << levels;
				break;

			case envTimes:
				foreach(QString oneAnsw, answer) { times.append(oneAnsw.toDouble()); }
				qDebug() << "ControlEnvelope::envTimes: " << times;
				break;

			case envCurves:
				foreach(QString oneAnsw, answer) { curves.append(oneAnsw); }
				qDebug() << "ControlEnvelope::envCurves: " << curves;
				break;

			case envAt:
				graphCurveY.append(answer[0].toDouble());

				int prepareSize = graphCurveY.size() - 1;

				if (prepareSize > 1)
				{
					envGraph->addLine(
						graphCurveX[prepareSize - 1],
						graphCurveY[prepareSize - 1],
						graphCurveX[prepareSize],
						graphCurveY[prepareSize]
						);
				}

				break;
			}
		}
	}

	void ControlEnvelope::paintEvent(QPaintEvent *event)
	{
		QPainter painter(this);

		//painter.setPen(QPen(Qt::white, 1));
		//painter.fillRect(bounds(), QColor(120, 20, 20));

		if (this->hasFocus()) { painter.setPen(QColor(120, 20, 20)); }
		else { painter.setPen(QColor(60, 60, 60)); }

		//painter.setPen(colorOver);
		painter.drawLine(0, 0, width(), 0);
		painter.drawLine(0, height() - 1, width(), height() - 1);

	}

	ControlEnvelope::~ControlEnvelope() { }
}