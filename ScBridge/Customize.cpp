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
		//qDebug() << "Customize start";
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
		//qDebug() << "Customize::onBridgeAnswer: " << pattern << answer;
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
	//	qDebug() << "Customize::initConfigFile: " << systemExtensionDir;
		QDir configDir;
		configDir.setPath(systemExtensionDir);
		configDir.mkdir("Shem");
		configDir.setPath(QDir::fromNativeSeparators(tr("%1/Shem").arg(systemExtensionDir)));
	//	qDebug() << "Customize::path: " << configDir.path();

		configFile = new QFile(tr("%1/ShemConfig.txt").arg(configDir.path()));

		if (configFile->exists()) { this->readConfigFile(); }
		else { this->writeConfigFile(); }
	}

	void Customize::writeConfigFile()
	{
	//	qDebug() << "Customize::writeConfigFile " << configFile->fileName();

		configFile->open(QIODevice::WriteOnly | QIODevice::Text);

		QTextStream out(configFile);
		out << "shem_colorAppBackground = " << 20 << "," << 20 << "," << 20 << "\n";
		out << "shem_colorAppHeaderBackground = " << 40 << "," << 40 << "," << 40 << "\n";
		out << "shem_colorPanelBackground = " << 30 << "," << 30 << "," << 30 << "\n";
		out << "shem_colorNormal = " << 120 << "," << 120 << "," << 120 << "\n";
		out << "shem_colorFrozen = " << 70 << "," << 70 << "," << 70 << "\n";
		out << "shem_colorOver = " << 255 << "," << 255 << "," << 255 << "\n";
		out << "shem_colorActive = " << 70 << "," << 140 << "," << 210 << "\n";
		out << "shem_colorText = " << 230 << "," << 230 << "," << 230 << "\n";

		out << "shem_colorMsgNormal = " << 70 << "," << 70 << "," << 70 << "\n";
		out << "shem_colorMsgStatus = " << 230 << "," << 230 << "," << 230 << "\n";
		out << "shem_colorMsgEvaluate = " << 170 << "," << 230 << "," << 230 << "\n";
		out << "shem_colorMsgResult = " << 170 << "," << 200 << "," << 160 << "\n";
		out << "shem_colorMsgError = " << 230 << "," << 30 << "," << 30 << "\n";
		out << "shem_colorMsgWarning = " << 230 << "," << 130 << "," << 30 << "\n";
		out << "shem_colorMsgBundle = " << 170 << "," << 160 << "," << 20 << "\n";

		out << "shem_fontTextBig = " << "Univers Condensed" << ", " << 13 << "\n";
		out << "shem_fontTextSmall =  " << "Univers Condensed" << ", " << 10 << "\n";
		out << "shem_fontCode = " << "Univers 57 Condensed" << ", " << 9 << "\n";

		configFile->close();

		this->readConfigFile();
	}

	void Customize::readConfigFile()
	{
		//qDebug() << "Customize::readConfigFile " << configFile->fileName();

		QMap<QString, QVariant*> configData;

		configFile->open(QIODevice::ReadOnly | QIODevice::Text);
		QTextStream in(configFile);
		while (!in.atEnd()) {

			QString line = in.readLine();
			QStringList lineParts = line.remove("\n").split("=");

			//qDebug() << "Customize::readConfigFile line: " << line;
			//qDebug() << "Customize::key: " << lineParts[0];
			//qDebug() << "Customize::args: " << lineParts[1];

			QString key = lineParts[0].remove(" ");
			QStringList args = lineParts[1].split(",");
			QVariant *value;

			if (args.size() == 3) // COLOR
			{
				bool isColor = true;
				int rgb[3];
				for (int i = 0; i < 3; i++)
				{
					bool isNumber;
					args[i] = args[i].remove(" ");
					rgb[i] = args[i].toInt(&isNumber, 10);
					if (!isNumber) { isColor = false; break; }
				}

				if (isColor) {
					value = new QVariant(QColor(rgb[0], rgb[1], rgb[2]));
					configData.insert(key, value);
					continue;
				}
			}
			else if (args.size() == 2) // FONT
			{
				bool isFont = true;
				bool isNumber;
				int size = args[1].remove(" ").toInt(&isNumber, 10);
				if (!isNumber) { isFont = false; break; }
				if (isFont)
				{
					QFont font(args[0], size);
					font.setStretch(QFont::Unstretched);
					value = new QVariant(font);
					configData.insert(key, value);
					continue;
				}
			}
			else
			{
				qWarning() << "COnfigData: nespecifikovany typ";
				qDebug() << "key: " << key;
				qDebug() << "value: " << args.join(" || ");

				//configData.insert(key, value);
			}
		}

		foreach(QString key, configData.keys())
		{
			qDebug() << "configKey [" << key << "] ->" << configData[key]->toString();
		}

		emit actConfigData(configData);
		mBridge->msgNormalAct(tr("\ncustomization config file: %1\r").arg(configFile->fileName()));
		configFile->close();
	}


	Customize::~Customize() { }
}