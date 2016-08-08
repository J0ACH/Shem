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
    void onSave(Data);

   // void onModify(DataNEW);
  //  void onSave(DataNEW);

  signals:
    void actDataChanged(Data);
  //  void actDataChanged(DataNEW);
    void actCustomizeChanged(Customize*);

  private:
    QFile *configFile;
    Data library;
    DataCustomize dataCustomize;
    QMap<DataKey, QString> configFileKeys;

    void configFileAssocciation();
    void initConfigFile(QString systemExtensionDir);
    void defaultLibrary();
    void readConfigFileNEW();
    void postprocessingLibrary();
    void writeConfigFileNEW();

      };
}

#endif // CUSTOMIZE

