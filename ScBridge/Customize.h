#ifndef CUSTOMIZE_H
#define CUSTOMIZE_H

#include "Data.h"
//#include "ScBridge.h"

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

  signals:
    void actDataChanged(Data);
    void actCustomizeChanged(Customize*);

  private:
    QFile *configFile;
    Data library;
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

