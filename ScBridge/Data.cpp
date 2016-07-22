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
  QString Data::getValue_string(QString key) { return libraryNEW->value(key).toString(); }

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





  DataNEW::DataNEW()
  {
    //qDebug() << "DataNEW BUILD -> class:" << typeid(this).name();
    library = new QMap<QString, QVariant>();
  }

  DataNEW::DataNEW(QByteArray wrapedData)
  {
    library = new QMap<QString, QVariant>();

    QString dataMsg = QString::fromUtf8(wrapedData);
    QStringList dataList = dataMsg.split("||");

    foreach(QString oneLine, dataList)
    {
      QStringList args = oneLine.split("|");
      QString key = args[0];
      QString type = args[1];
      // args[2] JE VALUE TXT

      //qDebug() << "\t - line:" << oneLine;
      //qDebug() << "\t - key:" << args[0] << ", type:" << args[1] << ", value:" << args[2];
      bool typeFound = false;
      if (type == "QString") { library->insert(key, args[2]); typeFound = true; }
      else if (type == "int") { library->insert(key, args[2].toInt()); typeFound = true; }
      else if (type == "double") { library->insert(key, args[2].toDouble()); typeFound = true; }
      else if (type == "bool") {
        if (args[2] == "true") { library->insert(key, true); typeFound = true; }
        else if (args[2] == "false"){ library->insert(key, false); typeFound = true; }
        else { typeFound = false; }
      }
      else if (type == "QColor") { library->insert(key, QColor(args[2])); typeFound = true; }
      else if (type == "QFont") {
        QFont font;
        font.fromString(args[2]);
        library->insert(key, font);
        typeFound = true;
      }

      if (!typeFound)
      {
        qWarning() << "CHYBA : DataNEW(QByteArray) -> CHYBA V PREVODU NA QVARIANT TYP key:" << args[0] << ", type:" << args[1] << ", value:" << args[2];
        library->insert(key, args[2]);
      }
    }
  }

  void DataNEW::setValue(QString key, QVariant value)  { library->insert(key, value); }
  QVariant DataNEW::getValue(QString key)
  {
    switch (library->value(key).type())
    {
    case QVariant::Invalid: return "NaN"; break;
    default: return library->value(key); break;
    }
  }

  void DataNEW::print(QString comment)
  {
    qDebug() << "DataNEW::print [" << comment << "]";
    foreach(QString oneKey, library->keys())
    {
      qDebug()
        << "\t - key:" << oneKey
        << ", type:" << library->value(oneKey).typeName()
        << ", value:" << library->value(oneKey).toString();
    }
  }

  QByteArray DataNEW::wrap()
  {
    QStringList list;
    QByteArray bArray;

    //qDebug() << "DataNEW::wrap() library keys:" << library->keys();
    //qDebug() << "DataNEW::wrap() library values:" << library->values();
    qDebug() << "DataNEW::wrap()";
    foreach(QString oneKey, library->keys())
    {
      qDebug() << "\t - key:" << oneKey << ", type:" << library->value(oneKey).typeName() << ", value:" << library->value(oneKey).toString();
      list.append(QString("%1|%2|%3").arg(oneKey, library->value(oneKey).typeName(), library->value(oneKey).toString()));

      bArray.append(QString("%1|%2|%3").arg(
        oneKey,
        library->value(oneKey).typeName(),
        library->value(oneKey).toString()
        ));
      if (oneKey != library->keys().at(library->keys().size() - 1)) { bArray.append("||"); }
    }
    //qDebug() << "List1:" << list;
    return bArray.toStdString().c_str();
  }


  //////////////////////////////////////////////////////////////////////////////////

  DataCustomize::DataCustomize() : DataNEW()
  {
    const QMetaObject &mo = DataCustomize::staticMetaObject;
    metaEnum = mo.enumerator(mo.indexOfEnumerator("Key"));
  }

  void DataCustomize::setValue(Key key, QVariant value)  { DataNEW::setValue(metaEnum.valueToKey(key), value); }
  QString DataCustomize::getValue_string(Key key) { return DataNEW::getValue(metaEnum.valueToKey(key)).toString(); }
  bool DataCustomize::getValue_bool(Key key) { return DataNEW::getValue(metaEnum.valueToKey(key)).toBool(); }
  int DataCustomize::getValue_int(Key key) { return DataNEW::getValue(metaEnum.valueToKey(key)).toInt(); }
  float DataCustomize::getValue_double(Key key) { return DataNEW::getValue(metaEnum.valueToKey(key)).toDouble(); }
  QFont DataCustomize::getValue_font(Key key) { return DataNEW::getValue(metaEnum.valueToKey(key)).value<QFont>(); }
  QColor DataCustomize::getValue_color(Key key) { return DataNEW::getValue(metaEnum.valueToKey(key)).value<QColor>(); }

  QString DataCustomize::toStyleSheet(Key key)
  {
    QFont font;
    switch (DataNEW::getValue(metaEnum.valueToKey(key)).type())
    {
    case QVariant::Int:
    case QVariant::Double:
    case QVariant::String: return this->getValue_string(key);  break;
    case QVariant::Bool:
      if (this->getValue_bool(key)) { return "true"; }
      else { return "false"; }
      break;
    case QVariant::Font:
      font = this->getValue_font(key);
      return QString("\"%1\"").arg(font.toString());
      break;
    case QVariant::Color:
      return this->getValue_color(key).name();
      break;
    default:
      qDebug() << "CHYBA : DataCustomize::toStyleSheet() NESPECIFIKOVANY TYP ("
        << "key : " << metaEnum.valueToKey(key)
        << ", type : " << DataNEW::getValue(metaEnum.valueToKey(key)).type()
        << ", value : " << this->getValue_string(key);
      return "NaN";
      break;
    }
  }

  //////////////////////////////////////////////////////////////////////////////////

  DataProxy::DataProxy() : DataNEW()
  {
    const QMetaObject &mo = DataProxy::staticMetaObject;
    metaEnum = mo.enumerator(mo.indexOfEnumerator("Key"));
  }

  DataProxy::DataProxy(QByteArray wrapedData) : DataNEW(wrapedData)
  {
    const QMetaObject &mo = DataProxy::staticMetaObject;
    metaEnum = mo.enumerator(mo.indexOfEnumerator("Key"));
  }

  void DataProxy::setValue(Key key, QVariant value)  { DataNEW::setValue(metaEnum.valueToKey(key), value); }
  QString DataProxy::getValue_string(Key key) { return DataNEW::getValue(metaEnum.valueToKey(key)).toString(); }
  bool DataProxy::getValue_bool(Key key) { return DataNEW::getValue(metaEnum.valueToKey(key)).toBool(); }
  int DataProxy::getValue_int(Key key) { return DataNEW::getValue(metaEnum.valueToKey(key)).toInt(); }
  float DataProxy::getValue_double(Key key) { return DataNEW::getValue(metaEnum.valueToKey(key)).toDouble(); }
  QFont DataProxy::getValue_font(Key key) { return DataNEW::getValue(metaEnum.valueToKey(key)).value<QFont>(); }
  QColor DataProxy::getValue_color(Key key) { return DataNEW::getValue(metaEnum.valueToKey(key)).value<QColor>(); }

}
