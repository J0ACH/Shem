#include "Data.h"

namespace SupercolliderBridge
{

  Data::Data()
  {
    library = new QMap<DataKey, QVariant>();
  }

  //Data::Data(DataType tableType)  {  }

  //void Data::setDataType(DataType tableType)  {  }

  void Data::setValue(DataKey key, char* value)  { library->insert(key, QVariant(value)); qDebug() << "Data::setValue asChar(" << key << "," << value << " ) ->" << library->value(key); }
  void Data::setValue(DataKey key, QString value)  { library->insert(key, QVariant(value)); qDebug() << "Data::setValue asString(" << key << "," << value << " ) ->" << library->value(key); }
  void Data::setValue(DataKey key, bool value)  { library->insert(key, value); qDebug() << "Data::setValue asBool(" << key << "," << value << " ) ->" << library->value(key); }
  void Data::setValue(DataKey key, int value)  { library->insert(key, value); qDebug() << "Data::setValue asInt(" << key << "," << value << " ) ->" << library->value(key); }
  void Data::setValue(DataKey key, double value) { library->insert(key, value); qDebug() << "Data::setValue asDouble(" << key << "," << value << " ) ->" << library->value(key); }
  void Data::setValue(DataKey key, QFont value) { library->insert(key, value); qDebug() << "Data::setValue asFont(" << key << "," << value << " ) ->" << library->value(key); }
  void Data::setValue(DataKey key, QColor value) { library->insert(key, value); qDebug() << "Data::setValue asColor(" << key << "," << value << " ) ->" << library->value(key); }

  QString Data::getValue_string(DataKey key) { return library->value(key).toString(); }
  bool Data::getValue_bool(DataKey key) { return library->value(key).toBool(); }
  int Data::getValue_int(DataKey key) { return library->value(key).toInt(); }
  float Data::getValue_float(DataKey key) { return library->value(key).toFloat(); }
  QFont Data::getValue_font(DataKey key) { return library->value(key).value<QFont>(); }
  QColor Data::getValue_color(DataKey key) { return library->value(key).value<QColor>(); }

  void Data::deleteValue(DataKey key)
  {
  }
  void Data::print(DataKey key)
  {
    switch (library->value(key).type())
    {
    case QVariant::String:
      qDebug() << "Data::print string (" << key << "," << library->value(key) << " )";
      break;
    case QVariant::Bool:
      qDebug() << "Data::print bool (" << key << "," << library->value(key) << " )";
      break;
    case QVariant::Int:
      qDebug() << "Data::print int (" << key << "," << library->value(key) << " )";
      break;
    case QVariant::Double:
      qDebug() << "Data::print double (" << key << "," << library->value(key) << " )";
      break;
    case QVariant::Font:
      qDebug() << "Data::print font (" << key << "," << library->value(key) << " )";
      break;
    case QVariant::Color:
      qDebug() << "Data::print color (" << key << "," << library->value(key) << " )";
      break;
    default:
      qDebug() << "Data::print NESPECIFIKOVANY TYP (" << key << "," << library->value(key) << " ) ->" << library->value(key).type();
      break;
    }
  }

  QString Data::toString(DataKey key)
  {
    QFont font;
    QColor color;

    switch (library->value(key).type())
    {
    case QVariant::String: return library->value(key).toString();  break;
    case QVariant::Bool:
      if (library->value(key).toBool()) { return "true"; }
      else { return "false"; }
      break;
    case QVariant::Int:
      return QString::number(library->value(key).toInt());
      break;
    case QVariant::Double:
      return QString::number(library->value(key).toDouble());
      break;
    case QVariant::Font:
      font = library->value(key).value<QFont>();
      return QString("%1, %2").arg(font.family(), QString::number(font.pointSize()));
      break;
    case QVariant::Color:
      color = library->value(key).value<QColor>();
      return QString("%1, %2, %3").arg(QString::number(color.red()), QString::number(color.green()), QString::number(color.blue()));
      break;
    default:
      qDebug() << "Data::toString() NESPECIFIKOVANY TYP (" << key << "," << library->value(key) << " ) ->" << library->value(key).type();
      return "NaN";
      break;
    }
  }
  /*
  void Data::toDataType()
  {

  }
  */

  Data::~Data() { }


}
