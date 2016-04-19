#ifndef CUSTOMIZE_H
#define CUSTOMIZE_H

#include "ScBridge.h"

#include <QWidget>
#include <QDir>
#include <QFile>
#include <QMap>
#include <QDebug>


namespace SupercolliderBridge
{

	class Customize : public QObject
	{
		Q_OBJECT

		enum QuestionType { appExtensionDir };

	public:
		Customize(QObject *parent = 0);
		~Customize();

		void setTargetBridge(ScBridge*);

	public slots:
		void onInterpretStart();
		void onBridgeAnswer(QString, int, QStringList);

	signals:
		void actBridgeQuestion(QString, int, QString, bool);
		void actConfigData(QMap<QString, QVariant*> config);

	private:
		ScBridge *mBridge;
		QString objectPattern;
		QFile *configFile;

		void onBridgeQuestion(QuestionType selector, QString args = QString::null);
		void initConfigFile(QString systemExtensionDir);
		void mergeConfigData();

		QMap<QString, QVariant*> readConfigFile();
		QMap<QString, QVariant*> defaultConfig();
		QMap<QString, QVariant*> processingConfigData(QMap<QString, QVariant*>);

		void writeConfigFile(QMap<QString, QVariant*>);
		
	};
}

#endif // CUSTOMIZE

