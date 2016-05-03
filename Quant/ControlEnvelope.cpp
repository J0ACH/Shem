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

		connect(envelopeCode, SIGNAL(evaluateAct()), this, SLOT(onEnvelopeCodeEvaluate()));
		connect(this, SIGNAL(bridgeQuestionAct(QUuid, int, QString, bool)), mBridge, SLOT(question(QUuid, int, QString, bool)));
		connect(mBridge, SIGNAL(answerAct(QUuid, int, QStringList)), this, SLOT(onBridgeAnswer(QUuid, int, QStringList)));
		
		connect(
			this, SIGNAL(actGraphEnv(QList<double>, QList<double>, QList<double>)),
			envGraph, SLOT(onGraphEnv(QList<double>, QList<double>, QList<double>))
			);
		connect(
			envGraph, SIGNAL(actGraphEnv(QList<double>, QList<double>, QList<double>)),
			this, SLOT(onGraphEnv(QList<double>, QList<double>, QList<double>))
			);

		this->onEnvelopeCodeEvaluate();
	}

	QRect ControlEnvelope::bounds() { return QRect(0, 0, width() - 1, height() - 1); }

	void ControlEnvelope::initControl()
	{
		nameLabel = new QLabel(this);
		nameLabel->setText(name);

		levelLabel = new QLabel(this);
		timeLabel = new QLabel(this);
		curveLabel = new QLabel(this);

		envelopeCode = new CodeEditor(this);
		envelopeCode->setText("Env([0,1,0], [0.15,0.85], ['lin', 'sin'])");

		envGraph = new Graph(this);
		envGraph->setDomainX(0, 1);
		envGraph->setDomainY(0, 1);
		envGraph->drawLine(0.25, 0.25, 0.5, 0.5);
		envGraph->drawLine(0.75, 0.25, 0.5, 0.5);

		graphCurveX = QList<double>();
		//int noSeg = envGraph->boundsGraph().width();
		int noSeg = 200;
		double min = envGraph->getDomainX()[0];
		double max = envGraph->getDomainX()[1];
		for (int i = 0; i <= noSeg; i++)
		{
			graphCurveX.append((max - min) / noSeg*i + min);
		}

	}

	void ControlEnvelope::onEnvelopeCodeEvaluate()
	{
		envGraph->deleteGraph();

		graphCurveY = QList<double>();
		graphPolyline = QVector<QPointF>();

		this->onBridgeQuestion(QuestionType::envArray);
		//this->onBridgeQuestion(QuestionType::envTimes);
		//this->onBridgeQuestion(QuestionType::envLevels);
		//this->onBridgeQuestion(QuestionType::envCurves);
		//this->onBridgeQuestion(QuestionType::redrawEnvPoints);

		foreach(double oneX, graphCurveX) {
			this->onBridgeQuestion(QuestionType::envAt, QString::number(oneX));
		}
		this->onBridgeQuestion(QuestionType::redrawEnvGraph);
	}

	void ControlEnvelope::onGraphEnv(QList<double> envLevels, QList<double> envTimes, QList<double> envCurves)
	{
		qDebug() << "ControlEnvelope::onControlPointsChange";
		qDebug() << "levels: " << envLevels;
		qDebug() << "times: " << envTimes;
		qDebug() << "curves: " << envCurves;

		QStringList txtLevels;
		QStringList txtTime;
		QStringList txtCurves;

		for each (double oneLevel in envLevels) { txtLevels.append(QString::number(oneLevel, 'f', 2)); }
		for each (double oneTime in envTimes) { txtTime.append(QString::number(oneTime, 'f', 2)); }
		for each (double oneCurve in envCurves) { txtCurves.append(QString::number(oneCurve)); }

		QString codeEnv = tr("Env([%1], [%2], [%3])").arg(
			txtLevels.join(", "),
			txtTime.join(", "),
			txtCurves.join(", ")
			);

		envelopeCode->setText(codeEnv);
		envGraph->deleteGraph();
		envelopeCode->evaluateAct();

		qDebug() << "ENV: " << codeEnv;
	}

	void ControlEnvelope::onBridgeQuestion(QuestionType selector, QString args)
	{
		QString questionCode;
		int selectorNum;

		switch (selector)
		{
		case envArray:
			selectorNum = QuestionType::envArray;
			questionCode = tr("%1.asArray").arg(envelopeCode->toPlainText());
			emit bridgeQuestionAct(objectID, selectorNum, questionCode, false);
			break;
		case envAt:
			selectorNum = QuestionType::envAt;
			questionCode = tr("%1.at(%2)").arg(envelopeCode->toPlainText(), args);
			emit bridgeQuestionAct(objectID, selectorNum, questionCode, false);
			break;
		case redrawEnvGraph:
			selectorNum = QuestionType::redrawEnvGraph;
			questionCode = tr("\" \"").arg(envelopeCode->toPlainText());
			emit bridgeQuestionAct(objectID, selectorNum, questionCode, false);
			break;
		}
	}
	void ControlEnvelope::onBridgeAnswer(QUuid id, int selectorNum, QStringList answer)
	{

		if (id == objectID)
		{
			QuestionType selector = static_cast<QuestionType>(selectorNum);

			double currentPointTime;
			QList<double> dblList;
			QList<double> levels;
			QList<double> times;
			QList<double> type;
			QList<double> curves;

			switch (selector)
			{
			case envArray:
				foreach(QString oneAnsw, answer) { dblList.append(oneAnsw.toDouble()); }
				qDebug() << "ControlEnvelope::envArray: " << dblList;
				
				currentPointTime = 0 - answer[1].toDouble();
				for (int i = 0; i < answer.size(); i += 4)
				{
					currentPointTime += answer[i + 1].toDouble();

					levels.append(answer[i].toDouble());
					times.append(currentPointTime);
					curves.append(answer[i + 3].toDouble());

					qDebug() << "level: " << answer[i];
					qDebug() << "time: " << answer[i + 1];
					qDebug() << "type: " << answer[i + 2];
					qDebug() << "curve: " << answer[i + 3];
					qDebug() << "///////////////////\n";
				}
				emit actGraphEnv(levels, times, curves);
				break;

			case envAt:
				//qDebug() << "polyline size: " << graphPolyline.size();
				//qDebug() << "ControlEnvelope::envAt " << answer[0];
				//currentGraphPtID = graphPolyline.size();
				graphCurveY.append(answer[0].toDouble());

				break;

			case redrawEnvGraph:
				qDebug() << "ControlEnvelope::refreshGraph NOW";
				qDebug() << "ControlEnvelope::graphCurveX " << graphCurveX.size();
				qDebug() << "ControlEnvelope::graphCurveY " << graphCurveY.size();
				//qDebug() << "ControlEnvelope::graphPolyline " << graphPolyline.size();

				if (graphCurveX.size() == graphCurveY.size())
				{
					for (int i = 0; i < graphCurveX.size(); i++)
					{
						graphPolyline.append(QPointF(graphCurveX[i], graphCurveY[i]));
					}
					envGraph->drawPolyline(graphPolyline);
				}
				break;
			}
		}
	}

	void ControlEnvelope::resizeEvent(QResizeEvent *event)
	{
		nameLabel->setGeometry(5, 5, 95, 25);
		envelopeCode->setGeometry(5, 30, width() - 10, 25);
		envGraph->setGeometry(5, 60, width() - 10, height() - 65);

		foreach(double oneX, graphCurveX) {
			this->onBridgeQuestion(QuestionType::envAt, QString::number(oneX));
		}
		//this->onBridgeQuestion(QuestionType::redrawEnvGraph);
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