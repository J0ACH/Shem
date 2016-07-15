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
    void refresh();

    enum Keys {
      userName,
      boolBootInterpretr, boolBootServer, boolTextAntialiasing,
      colorBackground, colorBars, colorHeaders, colorNormal, colorOver, colorActive, colorText,
      fontBig, fontSmall, fontConsole, fontCode,
      msgNormal, msgStatus, msgEvaluate, msgAnswer, msgError, msgWarninig, msgBundle
    };

    QString getString(QString key);
    bool getBool(QString key);
    QColor getColor(QString key);
    QFont getFont(QString key);    

    QString getString(Keys);
    QColor getColor(Keys);
    bool getBool(Keys);
    QFont getFont(Keys);

    void setColor(Keys, QColor);

    public slots:
    //void onModify();

  signals:
    void actCustomizeChanged(Customize*);

  private:
    QFile *configFile;

    void initConfigFile(QString systemExtensionDir);
    void mergeConfigData();

    QMap<QString, QVariant*> readConfigFile();
    QMap<QString, QVariant*> defaultConfig();
    QMap<QString, QVariant*> processingConfigData(QMap<QString, QVariant*>);
            
    void writeConfigFile(QMap<QString, QVariant*>);

    //void makePalette();
    //QPalette customPalette;

  };


}

#endif // CUSTOMIZE

