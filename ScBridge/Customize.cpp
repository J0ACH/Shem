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
			emit actBridgeQuestion(objectPattern, selectorNum, questionCode, false);
			break;
		}
	}

	void Customize::onBridgeAnswer(QString pattern, int selectorNum, QStringList answer)
	{
		qDebug() << "Customize::onBridgeAnswer: " << pattern << answer;
		if (pattern == objectPattern)
		{
			QuestionType selector = static_cast<QuestionType>(selectorNum);

			switch (selector)
			{
			case appExtensionDir:
				this->initConfigFile(answer[0]);
				break;

			default:
				qDebug() << "Customize::onBridgeAnswer::DEFAULT";
				break;
			}
		}
	}
	void Customize::initConfigFile(QString systemExtensionDir)
	{
		qDebug() << "Customize::initConfigFile: " << systemExtensionDir;
		QDir configDir;
		configDir.setPath(systemExtensionDir);
		configDir.mkdir("Shem");
		configDir.setPath(QDir::fromNativeSeparators(tr("%1/Shem").arg(systemExtensionDir)));
		qDebug() << "Customize::path: " << configDir.path();

		configFile = new QFile(tr("%1/ShemConfig.txt").arg(configDir.path()));

		if (configFile->exists()) { this->readConfigFile(); }
		else { this->writeConfigFile(); }
	}

	void Customize::writeConfigFile()
	{
		qDebug() << "Customize::writeConfigFile " << configFile->fileName();

		configFile->open(QIODevice::WriteOnly | QIODevice::Text);

		QTextStream out(configFile);
		out << "shem_backgroundColor = " << 230 << "," << 30 << "," << 30 << "\n";
		out << "shem_textColor = " << 230 << "," << "L" << "," << 230 << "\n";

		configFile->close();

		this->readConfigFile();
	}

	void Customize::readConfigFile()
	{
		qDebug() << "Customize::readConfigFile " << configFile->fileName();

		QMap<QString, QVariant*> configData;

		configFile->open(QIODevice::ReadOnly | QIODevice::Text);
		QTextStream in(configFile);
		while (!in.atEnd()) {

			QString line = in.readLine();
			QStringList lineParts = line.remove("\n").split("=");

			qDebug() << "Customize::readConfigFile line: " << line;

			QString key = lineParts[0].remove(" ");
			QStringList args = lineParts[1].remove(" ").split(",");
			QVariant *value;

			if (args.size() == 3)
			{
				bool isColor = true;
				int rgb[3];

				for (int i = 0; i < 3; i++)
				{
					bool isNumber;
					rgb[i] = args[i].toInt(&isNumber, 10);
					if (!isNumber) { isColor = false; break; }
				}

				if (isColor) { value = new QVariant(QColor(rgb[0], rgb[1], rgb[2])); continue; }
				else
				{
					qDebug() << "neni to barva";
				}
			}
			else
			{

				foreach(QString oneArg, args)
				{
					bool isNumber;
					int dec = oneArg.toInt(&isNumber, 10);
					qDebug() << "isNumber: " << isNumber;
				}
			}

			qDebug() << "key: " << key;
			qDebug() << "value: " << args.join(" || ");

			configData.insert(key, value);
		}

		foreach(QString key, configData.keys())
		{
			qDebug() << "configKey [" << key << "] ->" << configData[key]->toString();
		}

		configFile->close();
	}


	Customize::~Customize() { }
}