#ifndef DATA_H
#define DATA_H

#include <QDebug>
#include <QMap>
#include <QColor>
#include <QFont>
#include <QDataStream>
#include <QMetaEnum>

namespace SupercolliderBridge
{
  enum class MessageType{ NORMAL, STATUS, EVAULATE, ANSWER, ERROR, WARNING, BUNDLE };

  class Data
  {
    Q_GADGET
      Q_ENUMS(DataType)
      
  public:
    enum DataType { CUSTOMIZE, USER, PROXY, NODE };

    Data();
    Data(QByteArray);

    QString print(QString comment = "");
    QByteArray wrap();

    void setSender(QString);
    void setTargetObject(char*);
    void setTargetObject(QString);
    void setTargetMethod(char*);
    void setTargetMethod(QString);

    QString getSender();
    int getType();
    QString getTarget();
    QString getMethod();

    static bool isFromOtherOwener(QByteArray, QString);
    static int getType(QByteArray);
    static const QString getSender(QByteArray);
    static const QString getTarget(QByteArray);
    static const QString getMethod(QByteArray);

  protected:
    void setType(DataType);
    void setValue(QString, QVariant);
    void setValue(QString, QString, QVariant);
    QVariant getValue(QString);
    QVariant getValue(QString, QString);

    void deleteValue(QString);
    void deleteValue(QString, QString);

  private:
    QMetaEnum metaEnum_type;
    QMap <QString, QString> *header;
    QMap <QString, QVariant> *library;
  };

  //////////////////////////////////////////////////////////////////////////////////

  class DataCustomize : public Data
  {
    Q_GADGET
      Q_ENUMS(Key)

  public:
    enum Key{
      USERNAME,
      BOOL_BOOT_NETWORK, BOOL_BOOT_INTERPRETR, BOOL_BOOT_SERVER, BOOL_TEXT_ANTIALIASING,
      COLOR_APP_HEADER, COLOR_APP_BACKGROUND, COLOR_PANEL_HEADER, COLOR_PANEL_BACKGROUND, COLOR_NORMAL, COLOR_OVER, COLOR_ACTIVE, COLOR_TEXT,
      COLOR_MSG_NORMAL, COLOR_MSG_STATUS, COLOR_MSG_EVALUATE, COLOR_MSG_ANSWER, COLOR_MSG_ERROR, COLOR_MSG_WARNINIG, COLOR_MSG_BUNDLE,
      FONT_BIG, FONT_SMALL, FONT_CONSOLE, FONT_CODE
    };

    DataCustomize();
    DataCustomize(QByteArray);

    void setValue(Key, QVariant);
    QString getValue_string(Key);
    bool getValue_bool(Key);
    int getValue_int(Key);
    float getValue_double(Key);
    QFont getValue_font(Key);
    QColor getValue_color(Key);

    QString toStyleSheet(Key);
    
  private:
    QMetaEnum metaEnum;
  };

  //////////////////////////////////////////////////////////////////////////////////

  class DataUser : public Data
  {
    Q_GADGET
      Q_ENUMS(Key)

  public:
    enum Key { NAME, SERVER_METER, SERVER_CNTGROUPS, SERVER_CNTSYNTHS };

    DataUser();
    DataUser(QByteArray);
    
    void setValue(Key, QVariant);
    QString getValue_string(Key);
    bool getValue_bool(Key);
    int getValue_int(Key);
    float getValue_double(Key);
    QFont getValue_font(Key);
    QColor getValue_color(Key);


  private:
    QMetaEnum metaEnum;
  };

  //////////////////////////////////////////////////////////////////////////////////

  class DataProxy : public Data
  {
    Q_GADGET
      Q_ENUMS(Key)

  public:
    enum Key { BEATS, BPM };

    DataProxy();
    DataProxy(QByteArray);

    void setValue(Key, QVariant);
    QString getValue_string(Key);
    bool getValue_bool(Key);
    int getValue_int(Key);
    float getValue_double(Key);
    
  private:
    QMetaEnum metaEnum;
  };

  class DataNode : public Data
  {
    Q_GADGET
      Q_ENUMS(Key)

  public:
    enum Key { NAME, VOLUME, FTIME, INDEX_SIZE, INDEX_CHANGE, CODES, SOURCE_CURSOR };

    DataNode();
    DataNode(QByteArray);
  
    void setValue(Key, QVariant);
    void setValue(Key, int, QVariant);
    QString getValue_string(Key);
    QString getValue_string(Key, int);
    bool getValue_bool(Key);
    int getValue_int(Key);
    float getValue_double(Key);

    void deleteValue(Key, int);
    
  private:
    QMetaEnum metaEnum;
  };

}

#endif // DATA


