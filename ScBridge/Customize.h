#ifndef CUSTOMIZE_H
#define CUSTOMIZE_H

#include "ScBridge.h"

#include <QWidget>
#include <QDir>
#include <QFile>
#include <QMap>
#include <QDebug>
#include <QStandardPaths> 


namespace SupercolliderBridge
{

  class Customize : public QObject
  {
    Q_OBJECT

  public:
    Customize(QObject *parent);
    ~Customize();

    void initConfig();
    QString getString(QString key);
    bool getBool(QString key);
    QColor getColor(QString key);
    QFont getFont(QString key);

  signals:
    void actCustomizeChanged();

  private:
    QFile *configFile;

    void initConfigFile(QString systemExtensionDir);
    void mergeConfigData();

    QMap<QString, QVariant*> readConfigFile();
    QMap<QString, QVariant*> defaultConfig();
    QMap<QString, QVariant*> processingConfigData(QMap<QString, QVariant*>);

    void writeConfigFile(QMap<QString, QVariant*>);

  };


}

#endif // CUSTOMIZE

