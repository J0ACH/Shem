#ifndef CUSTOMIZE_H
#define CUSTOMIZE_H

#include "Data.h"
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
    void refresh();

    // bude odstraneno
    enum Keys {
      userName,
      boolBootInterpretr, boolBootServer, boolTextAntialiasing,
      colorBackground, colorBars, colorHeaders, colorNormal, colorOver, colorActive, colorText,
      fontBig, fontSmall, fontConsole, fontCode,
      msgNormal, msgStatus, msgEvaluate, msgAnswer, msgError, msgWarninig, msgBundle
    };

    // bude odstraneno
    QString getString(QString key);
    bool getBool(QString key);
    QColor getColor(QString key);
    QFont getFont(QString key);

    // bude odstraneno
    QString getString(Keys);
    QColor getColor(Keys);
    bool getBool(Keys);
    QFont getFont(Keys);

    // bude odstraneno
    void setColor(Keys, QColor);

    public slots:
    void onModify(Data);
    void onSave(Data);

  signals:
    void actDataChanged(Data);
    void actCustomizeChanged(Customize*);

  private:
    QFile *configFile;

    void configFileAssocciation();

    void initConfigFile(QString systemExtensionDir);
    void mergeConfigData();
    
    QMap<QString, QVariant*> readConfigFile();
    QMap<QString, QVariant*> defaultConfig();
    QMap<QString, QVariant*> processingConfigData(QMap<QString, QVariant*>);

    Data library;
    QMap<DataKey, QString> configFileKeys;

    void defaultLibrary();
    void readConfigFileNEW();
    void postprocessingLibrary();

    void writeConfigFile(QMap<QString, QVariant*>);
    void writeConfigFileNEW();

    //void makePalette();
    //QPalette customPalette;

  };


}

#endif // CUSTOMIZE

