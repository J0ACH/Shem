#include "File.h"

namespace SupercolliderBridge
{
  File::File() : QObject()
  {
    fileName = "fileTest.txt";
    dirName = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);

    directory.setPath(dirName);
    directory.mkdir("Shem");
    directory.setPath(QDir::fromNativeSeparators(tr("%1/Shem").arg(dirName)));

    file = new QFile(QString("%1/%2").arg(directory.path(), fileName));

    fileText = "";
  }

  void File::setDirectory(QString dir)
  {
    dirName = dir;

    directory.setPath(dirName);
    directory.mkdir("Shem");
    directory.setPath(QDir::fromNativeSeparators(tr("%1/Shem").arg(dirName)));

    file->setFileName(QString("%1/%2").arg(directory.path(), fileName));
  }

  void File::setFileName(QString name)
  {
    fileName = name;
    file->setFileName(QString("%1/%2").arg(directory.path(), fileName));
  }

  void File::setFileText(QString text)
  {
    fileText = text;
  }

  void File::onWrite()
  {
    file->open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(file);

    out << fileText;

    file->close();
  }

  void File::onRead()
  {
    file->open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(file);
    fileText = "";
    while (!in.atEnd())
    {
      QString line = in.readLine();
      fileText += line;
    }

    //if (!txt.isEmpty()) { dataCustomize = DataCustomize(txt.toUtf8()); }
    qDebug() << "File::onRead() -- " << fileName << " -- " << fileText;
    file->close();
    emit actFileRead(fileText);
  }

  File::~File() { }
}
