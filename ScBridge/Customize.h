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
    //Customize(QObject *parent, ScBridge *bridge);
    Customize(QObject *parent);
    ~Customize();

    void initConfig();
    void copyProperty(QObject*);
    void copyProperty(QObject*, QObject*);

  signals:
    void actConfigData(QMap<QString, QVariant*> config);
    void actCustomizeChanged();

  private:
   // ScBridge *mBridge;
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

