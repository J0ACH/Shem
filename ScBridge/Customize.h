#ifndef CUSTOMIZE_H
#define CUSTOMIZE_H

#include "Data.h"

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

    public slots:
    void onModify(Data);
    void onModify2(DataCustomize);
    void onSave(Data);
    void onSave2(DataCustomize);

    // void onModify(DataNEW);
    //  void onSave(DataNEW);

  signals:
    void actDataChanged(Data);
    void actDataChanged2(DataCustomize);
    void actCustomizeChanged(Customize*);

  private:
    QFile *configFile, *configFile2;
    Data library;
    DataCustomize dataCustomize;
    QMap<DataKey, QString> configFileKeys;

    void configFileAssocciation();
    void initConfigFile(QString systemExtensionDir);
    void defaultLibrary();
    void readConfigFileNEW();
    void readConfigFile2();
    void postprocessingLibrary();
    void writeConfigFileNEW();
    void writeConfigFile2();
  };
}

#endif // CUSTOMIZE

