#include "Data.h"

namespace SupercolliderBridge
{
  Data::Data()
  {
    //qDebug() << "Data BUILD -> class:" << typeid(this).name();
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
  Data::Data(QByteArray wrapedData)
  {
    const QMetaObject &mo = Data::staticMetaObject;
    metaEnum_type = mo.enumerator(mo.indexOfEnumerator("DataType"));

    header = new QMap<QString, QString>();
    library = new QMap<QString, QVariant>();

    QString dataMsg = QString::fromUtf8(wrapedData);
    QStringList dataList = dataMsg.split("||");

    //qDebug() << "Data(QByteArray)";
    //qDebug() << "Data(QByteArray) dataMsg:\n" << dataMsg;

    foreach(QString oneLine, dataList)
    {
      if (!oneLine.isEmpty())
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
            qWarning() << "CHYBA : Data(QByteArray) -> CHYBA V PREVODU NA QVARIANT TYP key:" << key << ", type:" << type << ", value:" << value;
            library->insert(key, value);
          }
        }
      }
    }
  }

  void Data::setSender(QString name)  { header->insert("OWENER", name); }
  QString Data::getSender()  { return header->value("OWENER", "NaN"); }
  void Data::setType(Data::DataType type)  { header->insert("TYPE", metaEnum_type.valueToKey(type)); }
  void Data::setTargetObject(char* targetObject)  { header->insert("TARGET", targetObject); }
  void Data::setTargetObject(QString targetObject)  { header->insert("TARGET", targetObject); }
  void Data::setTargetMethod(char* targetMethod)  { header->insert("METHOD", targetMethod); }
  void Data::setTargetMethod(QString targetMethod)  { header->insert("METHOD", targetMethod); }

  bool Data::isFromOtherOwener(QByteArray wrapedData, QString myName)
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
  int Data::getType(QByteArray wrapedData)
  {
    const QMetaObject &mo = Data::staticMetaObject;
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
  int Data::getType()
  {
    return metaEnum_type.keyToValue(header->value("TYPE").toStdString().c_str());
  }

  const QString Data::getSender(QByteArray wrapedData)
  {
    QString dataMsg = QString::fromUtf8(wrapedData);
    QStringList dataList = dataMsg.split("||");

    foreach(QString oneLine, dataList)
    {
      if (oneLine.startsWith("HEADER_OWENER"))
      {
        QStringList args = oneLine.split("|");
        //qDebug() << "Data::getTarget: " << args[1];
        return args[1];
      }
    }
    return "";
  }
  const QString Data::getTarget(QByteArray wrapedData)
  {
    QString dataMsg = QString::fromUtf8(wrapedData);
    QStringList dataList = dataMsg.split("||");

    foreach(QString oneLine, dataList)
    {
      if (oneLine.startsWith("HEADER_TARGET"))
      {
        QStringList args = oneLine.split("|");
        //qDebug() << "Data::getTarget: " << args[1];
        return args[1];
      }
    }
    return "";
  }
  QString Data::getTarget()  { return header->value("TARGET", "NaN"); }

  const QString Data::getMethod(QByteArray wrapedData)
  {
    QString dataMsg = QString::fromUtf8(wrapedData);
    QStringList dataList = dataMsg.split("||");

    QString targetObject, targetMethod;

    foreach(QString oneLine, dataList)
    {
      if (oneLine.startsWith("HEADER_METHOD"))
      {
        QStringList args = oneLine.split("|");
        //qDebug() << "Data::getMethod: " << args[1];
        return args[1];
      }
    }
    return "";
  }
  QString Data::getMethod()  { return header->value("METHOD", "NaN"); }

  void Data::setValue(QString key, QVariant value)  { library->insert(key, value); }
  void Data::setValue(QString key1, QString key2, QVariant value)
  {
    this->setValue(QString("%1/%2").arg(key1, key2), value);
  }

  QVariant Data::getValue(QString key)
  {
    switch (library->value(key).type())
    {
    case QVariant::Invalid: return "null"; break;
    default: return library->value(key); break;
    }
  }
  QVariant Data::getValue(QString key1, QString key2)
  {
    return this->getValue(QString("%1/%2").arg(key1, key2));
  }

  void Data::deleteValue(QString key) { library->remove(key); }
  void Data::deleteValue(QString key1, QString key2)
  {
    this->deleteValue(QString("%1/%2").arg(key1, key2));
  }

  QString Data::print(QString comment)
  {
    QString txt;
    txt = "Data::print [" + comment + "]\n";
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

  QByteArray Data::wrap()
  {
    QStringList list;
    QByteArray bArray;

    //qDebug() << "Data::wrap() library keys:" << library->keys();
    //qDebug() << "Data::wrap() library values:" << library->values();

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

      bArray.append(QString("%1|%2|%3").arg(
        oneKey,
        library->value(oneKey).typeName(),
        library->value(oneKey).toString()
        ));
      if (oneKey != library->keys().at(library->keys().size() - 1)) { bArray.append("||"); }
    }

    return bArray.toStdString().c_str();
  }


  //////////////////////////////////////////////////////////////////////////////////

  DataCustomize::DataCustomize() : Data()
  {
    const QMetaObject &mo = DataCustomize::staticMetaObject;
    metaEnum = mo.enumerator(mo.indexOfEnumerator("Key"));
  }
  DataCustomize::DataCustomize(QByteArray wrapedData) : Data(wrapedData)
  {
    const QMetaObject &mo = DataCustomize::staticMetaObject;
    metaEnum = mo.enumerator(mo.indexOfEnumerator("Key"));

    Data::Data(wrapedData);

    this->setType(Data::DataType::CUSTOMIZE);
  }

  void DataCustomize::setValue(Key key, QVariant value)  { Data::setValue(metaEnum.valueToKey(key), value); }
  QString DataCustomize::getValue_string(Key key) { return Data::getValue(metaEnum.valueToKey(key)).toString(); }
  bool DataCustomize::getValue_bool(Key key) { return Data::getValue(metaEnum.valueToKey(key)).toBool(); }
  int DataCustomize::getValue_int(Key key) { return Data::getValue(metaEnum.valueToKey(key)).toInt(); }
  float DataCustomize::getValue_double(Key key) { return Data::getValue(metaEnum.valueToKey(key)).toDouble(); }
  QFont DataCustomize::getValue_font(Key key) { return Data::getValue(metaEnum.valueToKey(key)).value<QFont>(); }
  QColor DataCustomize::getValue_color(Key key) { return Data::getValue(metaEnum.valueToKey(key)).value<QColor>(); }

  QString DataCustomize::toStyleSheet(Key key)
  {
    QFont font;
    switch (Data::getValue(metaEnum.valueToKey(key)).type())
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
        << ", type : " << Data::getValue(metaEnum.valueToKey(key)).type()
        << ", value : " << this->getValue_string(key);
      return "NaN";
      break;
    }
  }

  //////////////////////////////////////////////////////////////////////////////////

  DataUser::DataUser() : Data()
  {
    const QMetaObject &mo = DataUser::staticMetaObject;
    metaEnum = mo.enumerator(mo.indexOfEnumerator("Key"));

    this->setType(Data::DataType::USER);
  }
  DataUser::DataUser(QByteArray wrapedData) : Data(wrapedData)
  {
    const QMetaObject &mo = DataUser::staticMetaObject;
    metaEnum = mo.enumerator(mo.indexOfEnumerator("Key"));

    this->setType(Data::DataType::USER);
  }

  void DataUser::setValue(Key key, QVariant value)  { Data::setValue(metaEnum.valueToKey(key), value); }
  QString DataUser::getValue_string(Key key) { return Data::getValue(metaEnum.valueToKey(key)).toString(); }
  bool DataUser::getValue_bool(Key key) { return Data::getValue(metaEnum.valueToKey(key)).toBool(); }
  int DataUser::getValue_int(Key key) { return Data::getValue(metaEnum.valueToKey(key)).toInt(); }
  float DataUser::getValue_double(Key key) { return Data::getValue(metaEnum.valueToKey(key)).toDouble(); }
  QFont DataUser::getValue_font(Key key) { return Data::getValue(metaEnum.valueToKey(key)).value<QFont>(); }
  QColor DataUser::getValue_color(Key key) { return Data::getValue(metaEnum.valueToKey(key)).value<QColor>(); }

  //////////////////////////////////////////////////////////////////////////////////

  DataProxy::DataProxy() : Data()
  {
    const QMetaObject &mo = DataProxy::staticMetaObject;
    metaEnum = mo.enumerator(mo.indexOfEnumerator("Key"));

    this->setType(Data::DataType::PROXY);
  }
  DataProxy::DataProxy(QByteArray wrapedData) : Data(wrapedData)
  {
    const QMetaObject &mo = DataProxy::staticMetaObject;
    metaEnum = mo.enumerator(mo.indexOfEnumerator("Key"));

    this->setType(Data::DataType::PROXY);
  }

  void DataProxy::setValue(Key key, QVariant value)  { Data::setValue(metaEnum.valueToKey(key), value); }
  QString DataProxy::getValue_string(Key key) { return Data::getValue(metaEnum.valueToKey(key)).toString(); }
  bool DataProxy::getValue_bool(Key key) { return Data::getValue(metaEnum.valueToKey(key)).toBool(); }
  int DataProxy::getValue_int(Key key) { return Data::getValue(metaEnum.valueToKey(key)).toInt(); }
  float DataProxy::getValue_double(Key key) { return Data::getValue(metaEnum.valueToKey(key)).toDouble(); }

  //////////////////////////////////////////////////////////////////////////////////

  DataNode::DataNode() : Data()
  {
    const QMetaObject &mo = DataNode::staticMetaObject;
    metaEnum = mo.enumerator(mo.indexOfEnumerator("Key"));

    this->setType(Data::DataType::NODE);
  }
  DataNode::DataNode(QByteArray wrapedData) : Data(wrapedData)
  {
    const QMetaObject &mo = DataNode::staticMetaObject;
    metaEnum = mo.enumerator(mo.indexOfEnumerator("Key"));

    this->setType(Data::DataType::NODE);
  }

  void DataNode::setValue(Key key, QVariant value)  { Data::setValue(metaEnum.valueToKey(key), value); }
  void DataNode::setValue(Key key, int index, QVariant value)
  {
    QString strIndex = QString("index_%1").arg(QString::number(index));
    Data::setValue(metaEnum.valueToKey(key), strIndex, value);
  }
  QString DataNode::getValue_string(Key key) { return Data::getValue(metaEnum.valueToKey(key)).toString(); }
  QString DataNode::getValue_string(Key key, int index)
  {
    QString strIndex = QString("index_%1").arg(QString::number(index));
    return Data::getValue(metaEnum.valueToKey(key), strIndex).toString();
  }
  bool DataNode::getValue_bool(Key key) { return Data::getValue(metaEnum.valueToKey(key)).toBool(); }
  int DataNode::getValue_int(Key key) { return Data::getValue(metaEnum.valueToKey(key)).toInt(); }
  float DataNode::getValue_double(Key key) { return Data::getValue(metaEnum.valueToKey(key)).toDouble(); }

  void DataNode::deleteValue(Key key, int index)
  {
    QString strIndex = QString("index_%1").arg(QString::number(index));
    Data::deleteValue(metaEnum.valueToKey(key), strIndex);
  }
}
