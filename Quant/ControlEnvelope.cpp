#include "ControlEnvelope.h"

namespace QuantIDE
{
	ControlEnvelope::ControlEnvelope(QWidget *parent, ScBridge *bridge, QString controlName) :
		QWidget(parent),
		mBridge(bridge),
		name(controlName)
	{
		this->setObjectName("ControlEnvelope");
		objectPattern = QUuid::createUuid().toString();
		objectID = QUuid::createUuid();

		qDebug() << "objectPattern: " << objectPattern;

		
		
		this->initControl();
		//this->fitGeometry();
		
		connect(envelopeCode, SIGNAL(evaluateAct()), this, SLOT(onEnvelopeCodeEvaluate()));
		
		connect(this, SIGNAL(bridgeQuestionAct(QString, int, QString, bool)), mBridge, SLOT(question(QString, int, QString, bool)));
		connect(mBridge, SIGNAL(answerAct(QString, int, QStringList)), this, SLOT(onBridgeAnswer(QString, int, QStringList)));
	}

	QRect ControlEnvelope::bounds() { return QRect(0, 0, width() - 1, height() - 1); }

	void ControlEnvelope::initControl()
	{
		envelopeCode = new CodeEditor(this);
		envelopeCode->setGeometry(5, 5, 300, 25);
		envelopeCode->setText("Env([0,1,0], [0.15,0.85], ['lin', 'sin'])");			
	}

	void ControlEnvelope::onEnvelopeCodeEvaluate()
	{
		this->onBridgeQuestion(QuestionType::envLevels);
		this->onBridgeQuestion(QuestionType::envTimes);
		this->onBridgeQuestion(QuestionType::envCurves);
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
			//qDebug() << "Question levels" << questionCode;
			emit bridgeQuestionAct(objectPattern, selectorNum, questionCode, true);
			break;
			
		case envTimes:
			selectorNum = QuestionType::envTimes;
			questionCode = tr("%1.times").arg(envelopeCode->toPlainText());
			emit bridgeQuestionAct(objectPattern, selectorNum, questionCode, true);
			break;
		case envCurves:
			selectorNum = QuestionType::envCurves;
			questionCode = tr("%1.curves").arg(envelopeCode->toPlainText());
			emit bridgeQuestionAct(objectPattern, selectorNum, questionCode, true);
			break;
			
		}
	}

	void ControlEnvelope::onBridgeAnswer(QString pattern, int selectorNum, QStringList answer)
	{
		qDebug() << "Node::onBridgeAnswer: " << pattern << answer;
		if (pattern == objectPattern)
		{
			QuestionType selector = static_cast<QuestionType>(selectorNum);
			QString txt = "";

			switch (selector)
			{
			case envLevels:
				foreach(QString oneAnsw, answer)
				{
					txt += tr("%1; ").arg(oneAnsw);
					qDebug() << "ControlEnvelope::envLevels: " << selector << " || " << oneAnsw;
				}				
				break;

			case envTimes:
				foreach(QString oneAnsw, answer)
				{
					txt += tr("%1; ").arg(oneAnsw);
					qDebug() << "ControlEnvelope::envTimes: " << selector << " || " << oneAnsw;
				}
				break;

			case envCurves:
				foreach(QString oneAnsw, answer)
				{
					txt += tr("%1; ").arg(oneAnsw);
					qDebug() << "ControlEnvelope::envCurves: " << selector << " || " << oneAnsw;
				}
				break;


			}
		}
	}

	void ControlEnvelope::paintEvent(QPaintEvent *event)
	{
		QPainter painter(this);

		painter.setPen(QPen(Qt::white, 1));
		painter.fillRect(bounds(), QColor(120, 20, 20));

	}

	ControlEnvelope::~ControlEnvelope() { }
}