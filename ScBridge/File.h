#ifndef FILE_H
#define FILE_H

//#include "Data.h"
//#include <QWidget>

#include <QDir>
#include <QFile>
#include <QDebug>
#include <QStandardPaths> 
#include <QTimer>
#include <QEventLoop>

namespace SupercolliderBridge
{
  class File : public QObject
  {
    Q_OBJECT

  public:
    File();
    ~File();

    void setFileName(QString);
    void setDirectory(QString);
    void setFileText(QString);
    void setFileText(QByteArray);

    void onWrite();
    QByteArray onRead();

    //public slots:
    //void onModify(DataCustomize);
    //void onSave(DataCustomize);

  signals:
    //void actFileRead(QString);

  private:
    QString fileName, dirName;
    QFile *file;
    QDir directory;

    QString fileText;
  };
}

#endif // FILE_H
