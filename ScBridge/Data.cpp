#include "Data.h"

namespace SupercolliderBridge
{

  Data::Data()
  {
    library = new QMap<DataKey, QVariant>();
    // libraryNEW = new QMap<QString, QVariant>();
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
    //    libraryNEW->insert(key, value);
  }
  // QString Data::getValue_string(QString key) { return libraryNEW->value(key).toString(); }

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
    const QMetaObject &mo = DataCustomize::staticMetaObject;
    metaEnum_type = mo.enumerator(mo.indexOfEnumerator("DataType"));

    header = new QMap<QString, QString>();
    library = new QMap<QString, QVariant>();

    header->insert("OWENER", "NaN");
    header->insert("TIME", "NaN");
    header->insert("TYPE", "NaN");
    header->insert("TARGET", "NaN");
    header->insert("METHOD", "NaN");
  }

  DataNEW::DataNEW(QByteArray wrapedData)
  {
    const QMetaObject &mo = DataNEW::staticMetaObject;
    metaEnum_type = mo.enumerator(mo.indexOfEnumerator("DataType"));

    header = new QMap<QString, QString>();
    library = new QMap<QString, QVariant>();

    QString dataMsg = QString::fromUtf8(wrapedData);
    QStringList dataList = dataMsg.split("||");

    //qDebug() << "DataNEW(QByteArray)";
    //qDebug() << "DataNEW(QByteArray) dataMsg:\n" << dataMsg;

    foreach(QString oneLine, dataList)
    {
      QStringList args = oneLine.split("|");

      if (args[0].startsWith("HEADER_"))
      {
        QString key = args[0].remove(0, 7);
        QString value = args[1];

        // qDebug() << "HEADER key:" << key << ", value:" << value;
        header->insert(key, value);
      }
      else
      {
        QString key = args[0];
        QString type = args[1];
        QString value = args[2];

        // qDebug() << "\t - line:" << oneLine;
        //qDebug() << "\t - key:" << args[0] << ", type:" << args[1] << ", value:" << args[2];
        bool typeFound = false;
        if (type == "QString") { library->insert(key, value); typeFound = true; }
        else if (type == "int") { library->insert(key, value.toInt()); typeFound = true; }
        else if (type == "double") { library->insert(key, value.toDouble()); typeFound = true; }
        else if (type == "bool") {
          if (value == "true") { library->insert(key, true); typeFound = true; }
          else if (value == "false"){ library->insert(key, false); typeFound = true; }
          else { typeFound = false; }
        }
        else if (type == "QColor") { library->insert(key, QColor(value)); typeFound = true; }
        else if (type == "QFont") {
          QFont font;
          font.fromString(value);
          library->insert(key, font);
          typeFound = true;
        }

        if (!typeFound)
        {
          qWarning() << "CHYBA : DataNEW(QByteArray) -> CHYBA V PREVODU NA QVARIANT TYP key:" << key << ", type:" << type << ", value:" << value;
          library->insert(key, value);
        }
      }
    }
  }

  void DataNEW::setOwener(QString name)  { header->insert("OWENER", name); }
  void DataNEW::setType(DataNEW::DataType type)  { header->insert("TYPE", metaEnum_type.valueToKey(type)); }
  void DataNEW::setTargetObject(char* targetObject)
  {
    header->insert("TARGET", targetObject);
  }
  void DataNEW::setTargetObject(QString targetObject)
  {
    header->insert("TARGET", targetObject);
  }
  void DataNEW::setTargetMethod(char* targetMethod)
  {
    header->insert("METHOD", targetMethod);
  } 
  void DataNEW::setTargetMethod(QString targetMethod)
  {
    header->insert("METHOD", targetMethod);
  }


  bool DataNEW::isFromOtherOwener(QByteArray wrapedData, QString myName)
  {
    QString dataMsg = QString::fromUtf8(wrapedData);
    QStringList dataList = dataMsg.split("||");

    foreach(QString oneLine, dataList)
    {
      if (oneLine.startsWith("HEADER_OWENER"))
      {
        QStringList args = oneLine.split("|");
        if (args[1] != myName) { return true; }
        else { break; }
      }
    }
    return false;
  }
  int DataNEW::getType(QByteArray wrapedData)
  {
    const QMetaObject &mo = DataNEW::staticMetaObject;
    QMetaEnum metaEnum_type = mo.enumerator(mo.indexOfEnumerator("DataType"));

    QString dataMsg = QString::fromUtf8(wrapedData);
    QStringList dataList = dataMsg.split("||");

    foreach(QString oneLine, dataList)
    {
      if (oneLine.startsWith("HEADER_TYPE"))
      {
        QStringList args = oneLine.split("|");
        return metaEnum_type.keyToValue(args[1].toStdString().c_str());
      }
    }
    return -1;
  }

  const QString DataNEW::getTarget(QByteArray wrapedData)
  {
    QString dataMsg = QString::fromUtf8(wrapedData);
    QStringList dataList = dataMsg.split("||");

    foreach(QString oneLine, dataList)
    {
      if (oneLine.startsWith("HEADER_TARGET"))
      {
        QStringList args = oneLine.split("|");
        //qDebug() << "DataNEW::getTarget: " << args[1];
        return args[1];
      }
    }
    return "";
  }

  const QString DataNEW::getMethod(QByteArray wrapedData)
  {
    QString dataMsg = QString::fromUtf8(wrapedData);
    QStringList dataList = dataMsg.split("||");

    QString targetObject, targetMethod;

    foreach(QString oneLine, dataList)
    {
      if (oneLine.startsWith("HEADER_METHOD"))
      {
        QStringList args = oneLine.split("|");
        //qDebug() << "DataNEW::getMethod: " << args[1];
        return args[1];
      }
    }
    return "";
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

  QString DataNEW::print(QString comment)
  {
    QString txt;
    txt = "DataNEW::print [" + comment + "]\n";
    txt += "HEADER\n";
    foreach(QString oneKey, header->keys())
    {
      txt +=
        "\t - " + oneKey + " : " + header->value(oneKey) + "\n";
    }
    txt += "BODY\n";
    foreach(QString oneKey, library->keys())
    {
      txt +=
        "\t - key: " + oneKey
        + ", type: " + library->value(oneKey).typeName()
        + ", value: " + library->value(oneKey).toString() + "\n";
    }
    return txt;
  }

  QByteArray DataNEW::wrap()
  {
    QStringList list;
    QByteArray bArray;

    //qDebug() << "DataNEW::wrap() library keys:" << library->keys();
    //qDebug() << "DataNEW::wrap() library values:" << library->values();

    foreach(QString oneHeaderKey, header->keys())
    {
      bArray.append(QString("HEADER_%1|%2").arg(
        oneHeaderKey,
        header->value(oneHeaderKey)
        ));
      bArray.append("||");
      //if (oneHeaderKey != header->keys().at(header->keys().size() - 1)) { bArray.append("||"); }
    }

    foreach(QString oneKey, library->keys())
    {
      // qDebug() << "\t - key:" << oneKey << ", type:" << library->value(oneKey).typeName() << ", value:" << library->value(oneKey).toString();
      // list.append(QString("%1|%2|%3").arg(oneKey, library->value(oneKey).typeName(), library->value(oneKey).toString()));

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

  DataUser::DataUser() : DataNEW()
  {
    const QMetaObject &mo = DataUser::staticMetaObject;
    metaEnum = mo.enumerator(mo.indexOfEnumerator("Key"));

    this->setType(DataNEW::DataType::USER);
  }

  DataUser::DataUser(QByteArray wrapedData) : DataNEW(wrapedData)
  {
    const QMetaObject &mo = DataUser::staticMetaObject;
    metaEnum = mo.enumerator(mo.indexOfEnumerator("Key"));

    this->setType(DataNEW::DataType::USER);
  }

  void DataUser::setValue(Key key, QVariant value)  { DataNEW::setValue(metaEnum.valueToKey(key), value); }
  QString DataUser::getValue_string(Key key) { return DataNEW::getValue(metaEnum.valueToKey(key)).toString(); }
  bool DataUser::getValue_bool(Key key) { return DataNEW::getValue(metaEnum.valueToKey(key)).toBool(); }
  int DataUser::getValue_int(Key key) { return DataNEW::getValue(metaEnum.valueToKey(key)).toInt(); }
  float DataUser::getValue_double(Key key) { return DataNEW::getValue(metaEnum.valueToKey(key)).toDouble(); }
  QFont DataUser::getValue_font(Key key) { return DataNEW::getValue(metaEnum.valueToKey(key)).value<QFont>(); }
  QColor DataUser::getValue_color(Key key) { return DataNEW::getValue(metaEnum.valueToKey(key)).value<QColor>(); }

  //////////////////////////////////////////////////////////////////////////////////

  DataProxy::DataProxy() : DataNEW()
  {
    const QMetaObject &mo = DataProxy::staticMetaObject;
    metaEnum = mo.enumerator(mo.indexOfEnumerator("Key"));

    this->setType(DataNEW::DataType::PROXY);
  }

  DataProxy::DataProxy(QByteArray wrapedData) : DataNEW(wrapedData)
  {
    const QMetaObject &mo = DataProxy::staticMetaObject;
    metaEnum = mo.enumerator(mo.indexOfEnumerator("Key"));

    this->setType(DataNEW::DataType::PROXY);
  }

  void DataProxy::setValue(Key key, QVariant value)  { DataNEW::setValue(metaEnum.valueToKey(key), value); }
  QString DataProxy::getValue_string(Key key) { return DataNEW::getValue(metaEnum.valueToKey(key)).toString(); }
  bool DataProxy::getValue_bool(Key key) { return DataNEW::getValue(metaEnum.valueToKey(key)).toBool(); }
  int DataProxy::getValue_int(Key key) { return DataNEW::getValue(metaEnum.valueToKey(key)).toInt(); }
  float DataProxy::getValue_double(Key key) { return DataNEW::getValue(metaEnum.valueToKey(key)).toDouble(); }
  QFont DataProxy::getValue_font(Key key) { return DataNEW::getValue(metaEnum.valueToKey(key)).value<QFont>(); }
  QColor DataProxy::getValue_color(Key key) { return DataNEW::getValue(metaEnum.valueToKey(key)).value<QColor>(); }

}
