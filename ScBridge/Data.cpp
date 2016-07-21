#include "Data.h"

namespace SupercolliderBridge
{

  Data::Data()
  {
    library = new QMap<DataKey, QVariant>();
    libraryNEW = new QMap<QString, QVariant>();
  }

  Data::Data(QByteArray wrapedData)
  {
    library = new QMap<DataKey, QVariant>();

    QString dataMsg = QString::fromUtf8(wrapedData);
    QStringList dataList = dataMsg.split("||");

    foreach(QString oneLine, dataList)
    {
      QStringList args = oneLine.split("|");



      qDebug() << "\t - line:" << oneLine;
      qDebug() << "\t - key:" << args[0] << ", type:" << args[1] << ", value:" << args[2];
    }

    //qDebug() << "List2:" << list;
  }

  QString Data::enum2str(Key key)
  {
    /*
    switch (key)
    {
    case Data::USERNAME: return "USERNAME"; break;
    case Data::BOOL_BOOT_INTERPRETR: return "BOOL_BOOT_INTERPRETR"; break;
    case Data::BOOL_BOOT_SERVER: return "BOOL_BOOT_SERVER"; break;
    case Data::BOOL_TEXT_ANTIALIASING: return "BOOL_TEXT_ANTIALIASING"; break;
    case Data::COLOR_APP_HEADER: return "COLOR_APP_HEADER"; break;
    case Data::COLOR_APP_BACKGROUND: return "COLOR_APP_BACKGROUND"; break;
    case Data::COLOR_PANEL_HEADER: return "COLOR_PANEL_HEADER"; break;
    case Data::COLOR_PANEL_BACKGROUND: return "COLOR_PANEL_BACKGROUND"; break;
    case Data::COLOR_NORMAL: return "COLOR_NORMAL"; break;
    case Data::COLOR_OVER: return "COLOR_OVER"; break;
    case Data::COLOR_ACTIVE: return "COLOR_ACTIVE"; break;
    case Data::COLOR_TEXT: return "COLOR_TEXT"; break;
    case Data::COLOR_MSG_NORMAL: return "COLOR_MSG_NORMAL"; break;
    case Data::COLOR_MSG_STATUS: return "COLOR_MSG_STATUS"; break;
    case Data::COLOR_MSG_EVALUATE: return "COLOR_MSG_EVALUATE"; break;
    case Data::COLOR_MSG_ANSWER: return "COLOR_MSG_ANSWER"; break;
    case Data::COLOR_MSG_ERROR: return "COLOR_MSG_ERROR"; break;
    case Data::COLOR_MSG_WARNINIG: return "COLOR_MSG_WARNINIG"; break;
    case Data::COLOR_MSG_BUNDLE: return "COLOR_MSG_BUNDLE"; break;
    case Data::FONT_BIG: return "FONT_BIG"; break;
    case Data::FONT_SMALL: return "FONT_SMALL"; break;
    case Data::FONT_CONSOLE: return "FONT_CONSOLE"; break;
    case Data::FONT_CODE: return "FONT_CODE"; break;
    default: return "NaN"; break;
    }
    */
    return "";
  }

  void Data::setValue(DataKey key, char* value)  { library->insert(key, QVariant(value)); }
  void Data::setValue(DataKey key, QString value)  { library->insert(key, QVariant(value)); }
  void Data::setValue(DataKey key, bool value)  { library->insert(key, value); }
  void Data::setValue(DataKey key, int value)  { library->insert(key, value); }
  void Data::setValue(DataKey key, double value) { library->insert(key, value); }
  void Data::setValue(DataKey key, QFont value) { library->insert(key, value); }
  void Data::setValue(DataKey key, QColor value) { library->insert(key, value); }

  // test
  void Data::setValue(QString key, char* value)
  {
    qDebug() << "Data::setValue [key:" << key << ", value:" << value << "]";
    libraryNEW->insert(key, value);
  }
  QString Data::getValue_stringNEW(QString key) { return libraryNEW->value(key).toString(); }
  //void Data::setValue(DataKey key, QMap<DataKey, QVariant> *value) { library->insert(key, *value); }
  // test

  QString Data::getValue_string(DataKey key) { return library->value(key).toString(); }
  bool Data::getValue_bool(DataKey key) { return library->value(key).toBool(); }
  int Data::getValue_int(DataKey key) { return library->value(key).toInt(); }
  float Data::getValue_float(DataKey key) { return library->value(key).toFloat(); }
  QFont Data::getValue_font(DataKey key) { return library->value(key).value<QFont>(); }
  QColor Data::getValue_color(DataKey key) { return library->value(key).value<QColor>(); }

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

  QString Data::toStyleSheet(DataKey key)
  {
    QString text;
    QFont font, font2;
    switch (library->value(key).type())
    {
    case QVariant::Int:
    case QVariant::Double:
    case QVariant::String: return library->value(key).toString();  break;
    case QVariant::Bool:
      if (library->value(key).toBool()) { return "true"; }
      else { return "false"; }
      break;
    case QVariant::Font:
      font = library->value(key).value<QFont>();
      return QString("\"%1\"").arg(font.toString());
      break;
    case QVariant::Color:
      return library->value(key).value<QColor>().name();
      break;
    default:
      qDebug() << "Data::toString() NESPECIFIKOVANY TYP (" << key << "," << library->value(key) << " ) ->" << library->value(key).type();
      return "NaN";
      break;
    }
  }

  QByteArray Data::wrap()
  {
    QStringList list;
    QByteArray bArray;

    qDebug() << "Data::wrap() library keys:" << library->keys();
    qDebug() << "Data::wrap() library values:" << library->values();

    foreach(DataKey oneKey, library->keys())
    {
      qDebug() << "\t - key:" << oneKey << ", type:" << library->value(oneKey).typeName() << ", value:" << library->value(oneKey).toString();
      list.append(QString("%1|%2|%3").arg(QString::number(oneKey), library->value(oneKey).typeName(), library->value(oneKey).toString()));

      bArray.append(QString("%1|%2|%3").arg(
        QString::number(oneKey),
        library->value(oneKey).typeName(),
        library->value(oneKey).toString()
        ));
      if (oneKey != library->keys().at(library->keys().size() - 1)) { bArray.append("||"); }
    }
    qDebug() << "List1:" << list;
    return bArray;
  }


  Data::~Data() { }

  //////////////////////////////////////////////////////////////////////////////////

  DataCustomize::DataCustomize() : Data()
  {
    dataType = Type::CUSTOMIZE;
  }

  DataCustomize::~DataCustomize()  {  }

  //////////////////////////////////////////////////////////////////////////////////

  DataProxy::DataProxy() : Data()
  {
    dataType = Type::PROXYSPACE;
  }

  void DataProxy::setValue(Key key, char* value)
  {
    Data::setValue("tempo", value);
    qDebug() << "DataProxy::setValue [key:" << key << ", value:" << value << "]";
    //library->insert(key, value);
  }
  QString DataProxy::getValue_string(Key key) { return Data::getValue_stringNEW("tempo"); }

  DataProxy::~DataProxy()  {  }
}
