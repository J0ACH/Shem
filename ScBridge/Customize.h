#ifndef CUSTOMIZE_H
#define CUSTOMIZE_H

#include "Data.h"

#include <QWidget>
#include <QDir>
#include <QFile>
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
    void onModify(DataCustomize);
    void onSave(DataCustomize);

  signals:
    void actDataChanged(DataCustomize);
  
  private:
    QFile *configFile;
    DataCustomize dataCustomize;
  
    void initConfigFile(QString systemExtensionDir);
    void defaultLibrary();
    void readConfigFile();
    void postprocessingData();
    void writeConfigFile();
  };
}

#endif // CUSTOMIZE

