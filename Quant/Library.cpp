#include "Library.h"

namespace QuantIDE
{
  Library::Library(QWidget *parent, QObject *core) :
    QWidget(parent),
    mCore(core)
  {

  }

  void Library::addObject(QuantObject *obj)
  {
    obj->setParent(this);
    obj->show();
    obj->setGeometry(10, 10, width() - 20, 100);

    QString name;
    switch (obj->getType())
    {
    case QuantObject::ObjectType::USER:
      name = static_cast<QuantUser*>(obj)->getName();
      // qDebug() << "JSEM OBJECT USER :" << name;
      lib.insert(name, obj);
      this->updateObjectPosition();
      break;
    case QuantObject::ObjectType::PROXY:
      break;
    }

    this->updateObjectPosition();
  }

  void Library::addObject(DataNEW data)
  {
    switch (data.getType())
    {
    case DataNEW::DataType::USER:
      if (!this->containObject(data))
      {
        DataUser userData(data);
        QString name(userData.getValue_string(DataUser::Key::NAME));
        qDebug() << "JSEM OBJECT USER from DATA :" << name;

        QuantUser *newUser = new QuantUser(this, mCore);
        newUser->setName(name);
        newUser->show();
        lib.insert(name, newUser);
      }
      break;
    }
    this->updateObjectPosition();
  }

  void Library::removeObject(DataNEW data)
  {
    switch (data.getType())
    {
    case DataNEW::DataType::USER:
      if (!this->containObject(data))
      {
        DataUser userData(data);
        QString name(userData.getValue_string(DataUser::Key::NAME));
        lib.value(name)->close();
        lib.remove(name);
      }
    }
    this->updateObjectPosition();
  }

  QuantUser* Library::getUser(QString name)
  {
    return static_cast<QuantUser*>(lib.value(name, NULL));
  }

  QuantUser* Library::getUser(DataUser data)
  {
    QString name(data.getValue_string(DataUser::Key::NAME));
    return static_cast<QuantUser*>(lib.value(name, NULL));
  }

  bool Library::containObject(DataNEW data)
  {
    switch (data.getType())
    {
    case DataNEW::DataType::USER:
      //DataUser userData(data);
      QString name = DataUser(data).getValue_string(DataUser::Key::NAME);
      return lib.contains(name);
      break;
    }
    return false;
  }

  void Library::display(QWidget *parent)
  {
    this->setParent(parent);
    this->setGeometry(50, 50, 300, 300);
    this->show();
  }

  void Library::updateObjectPosition()
  {
    // int noLoop = 0;
    // int endY = 0;
    int gapSize = 10;
    int lastObjOriginY = gapSize;

    foreach(QuantObject *obj, lib)
    {
      // int originY = noLoop * (obj->height() + 10) + 10;
      // int heightY = obj->height();
      obj->setGeometry(
        10,
        lastObjOriginY,
        width() - 30,
        obj->height()
        );
      // endY = originY + heightY;
      lastObjOriginY += obj->height() + gapSize;
      // noLoop++;
    }
    this->setFixedHeight(lastObjOriginY + gapSize);
  }

  void Library::resizeEvent(QResizeEvent *event)
  {
    foreach(QuantObject *obj, lib)
    {
      obj->setFixedWidth(width() - 20);
      // obj->setGeometry(10, 10, width() - 20, 100);
    }
  }

  void Library::paintEvent(QPaintEvent *event)
  {
    QPainter painter(this);
    painter.fillRect(0, 0, width(), height(), QColor(60, 60, 60));
  }

  Library::~Library() { }
}