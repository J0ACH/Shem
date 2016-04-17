#include "Customize.h"

namespace SupercolliderBridge
{
	Customize::Customize(QObject *parent) :
		QObject(parent),
		mBridge(NULL)
	{
		setObjectName("Customize");
		objectPattern = "ShemConfig";
	}

	void Customize::setTargetBridge(ScBridge *target)
	{
		mBridge = target;
		connect(mBridge, SIGNAL(interpretBootDoneAct()), this, SLOT(onInterpretStart()));
		connect(this, SIGNAL(actBridgeQuestion(QString, int, QString, bool)), mBridge, SLOT(question(QString, int, QString, bool)));
		connect(mBridge, SIGNAL(answerAct(QString, int, QStringList)), this, SLOT(onBridgeAnswer(QString, int, QStringList)));
	}

	void Customize::onInterpretStart()
	{
		qDebug() << "Customize start";
		this->onBridgeQuestion(QuestionType::appExtensionDir);
	}

	void Customize::onBridgeQuestion(QuestionType selector, QString args)
	{
		QString questionCode;
		int selectorNum;

		switch (selector)
		{
		case appExtensionDir:
			selectorNum = QuestionType::appExtensionDir;
			//questionCode = "Platform.userConfigDir";
			questionCode = "Platform.systemExtensionDir";
			emit actBridgeQuestion(objectPattern, selectorNum, questionCode, true);
			break;
		}
	}

	void Customize::onBridgeAnswer(QString pattern, int selectorNum, QStringList answer)
	{
		qDebug() << "Customize::onBridgeAnswer: " << pattern << answer;
		if (pattern == objectPattern)
		{
			QuestionType selector = static_cast<QuestionType>(selectorNum);
			QString txt = "";

			switch (selector)
			{
			case appExtensionDir:
				qDebug() << "Node::onBridgeAnswer::target: " << selector;
				configDir = answer[0];
				break;

			default:
				qDebug() << "Customize::onBridgeAnswer::DEFAULT";
				break;
			}
		}
	}

	Customize::~Customize() { }
}