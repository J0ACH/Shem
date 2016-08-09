#include "Library.h"

namespace QuantIDE
{
  Library::Library(QWidget *parent, QObject *core) :
    QWidget(parent),
    mCore(core)
  {
    this->setGeometry(0, 0, 150, 150);
  }

  QList<QString> Library::keys()  { return lib.keys(); }

  void Library::setColorBackground(QColor color) { colorBackground = color; }
  QColor Library::getColorBackground() { return colorBackground; }

  void Library::addObject(QuantObject *obj)
  {
    obj->setParent(this);
    obj->show();

    QString name;
    switch (obj->getType())
    {
    case QuantObject::ObjectType::USER:
      name = static_cast<QuantUser*>(obj)->getName();
      // qDebug() << "Library::addObject JSEM OBJECT USER :" << name;
      lib.insert(name, obj);
      break;
    case QuantObject::ObjectType::PROXY:
      if (!this->containObject("proxy"))
      {
        //     qDebug() << "Library::addObject JSEM OBJECT PROXY";
        lib.insert("proxy", obj);
      }
      break;
    case QuantObject::ObjectType::NODE:
      if (!this->containObject("testNode"))
      {
        //     qDebug() << "Library::addObject JSEM OBJECT PROXY";
        lib.insert("testNode", obj);
      }
      break;
    }

    this->updateObjectPosition();
  }
  void Library::addObject(QByteArray msg)
  {

    switch (Data::getType(msg))
    {
    case Data::DataType::USER:
      //DataUser
      DataUser userData(msg);
      if (!this->containObject(userData))
      {
        QString name(userData.getValue_string(DataUser::Key::NAME));
        // qDebug() << "Library::addObject JSEM OBJECT USER from DATA :" << name;

        QuantUser *newUser = new QuantUser(this, mCore);
        newUser->setName(name);
        newUser->show();
        lib.insert(name, newUser);
      }
      break;
    }
    this->updateObjectPosition();
  }

  void Library::removeObject(QString name)
  {
    lib.value(name)->close();
    lib.value(name)->deleteLater();
    lib.remove(name);

    this->updateObjectPosition();
  }
  void Library::removeObject(Data data)
  {
    switch (data.getType())
    {
    case Data::DataType::USER:
      if (this->containObject(data))
      {
        DataUser userData(data);
        QString name(userData.getValue_string(DataUser::Key::NAME));
        lib.value(name)->close();
        lib.value(name)->deleteLater();
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

  QuantProxy* Library::getProxy() { return static_cast<QuantProxy*>(lib.value("proxy", NULL)); }

  QuantNode* Library::getNode(QString name)
  {
    return static_cast<QuantNode*>(lib.value(name, NULL));
  }
  QuantNode* Library::getNode(DataUser data)
  {
    QString name(data.getValue_string(DataUser::Key::NAME));
    return static_cast<QuantNode*>(lib.value(name, NULL));
  }

  bool Library::containObject(QString name)  { return lib.contains(name); }
  bool Library::containObject(Data data)
  {
    switch (data.getType())
    {
    case Data::DataType::USER:
      //DataUser userData(data);
      QString name = DataUser(data).getValue_string(DataUser::Key::NAME);
      return lib.contains(name);
      break;
    }
    return false;
  }

  void Library::updateObjectPosition()
  {
    int gapSize = 10;
    int lastObjOriginY = gapSize;

    foreach(QuantObject *obj, lib)
    {

      obj->setGeometry(
        10,
        lastObjOriginY,
        width() - 30,
        obj->height()
        );
      lastObjOriginY += obj->height() + gapSize;
    }
    this->setFixedHeight(lastObjOriginY);
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
    painter.fillRect(0, 0, width(), height(), colorBackground);
  }

  Library::~Library() { }
}