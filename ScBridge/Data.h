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
  enum DataKey {
    USERNAME,
    BOOL_BOOT_INTERPRETR, BOOL_BOOT_SERVER, BOOL_TEXT_ANTIALIASING,
    COLOR_APP_HEADER, COLOR_APP_BACKGROUND, COLOR_PANEL_HEADER, COLOR_PANEL_BACKGROUND, COLOR_NORMAL, COLOR_OVER, COLOR_ACTIVE, COLOR_TEXT,
    COLOR_MSG_NORMAL, COLOR_MSG_STATUS, COLOR_MSG_EVALUATE, COLOR_MSG_ANSWER, COLOR_MSG_ERROR, COLOR_MSG_WARNINIG, COLOR_MSG_BUNDLE,
    FONT_BIG, FONT_SMALL, FONT_CONSOLE, FONT_CODE
  };

  enum class MessageType{ NORMAL, STATUS, EVAULATE, ANSWER, ERROR, WARNING, BUNDLE };

  class Data
  {
  public:


    Data();
    Data(QByteArray);
    ~Data();

    //QString enum2str(Key);

    void setValue(DataKey, char*);
    void setValue(DataKey, QString);
    void setValue(DataKey, bool);
    void setValue(DataKey, int);
    void setValue(DataKey, double);
    void setValue(DataKey, QFont);
    void setValue(DataKey, QColor);
    //void setValue(DataKey key, QMap<DataKey, QVariant>);

    //test
    // void setValue(Key, char*);

    QString getValue_string(DataKey);
    bool getValue_bool(DataKey);
    int getValue_int(DataKey);
    float getValue_float(DataKey);
    QFont getValue_font(DataKey);
    QColor getValue_color(DataKey);

    void print(DataKey);
    QString toString(DataKey);
    QString toStyleSheet(DataKey);

    QByteArray wrap();


  protected:
    enum Type { CUSTOMIZE, PROXYSPACE };
    Type dataType;

    void setValue(QString, char*);
    QString getValue_string(QString);

  private:
    QMap <DataKey, QVariant> *library;
  };

  //////////////////////////////////////////////////////////////////////////////////


  class DataNEW
  {
    Q_GADGET
      Q_ENUMS(DataType)


  public:
    enum DataType { CUSTOMIZE, USER, PROXY, NODE };

    DataNEW();
    DataNEW(QByteArray);

    QString print(QString comment = "");
    QByteArray wrap();

    void setSender(QString);
    void setTargetObject(char*);
    void setTargetObject(QString);
    void setTargetMethod(char*);
    void setTargetMethod(QString);

    QString getSender();
    int getType();
    
    static bool isFromOtherOwener(QByteArray, QString);
    static int getType(QByteArray);

    static const QString getTarget(QByteArray);
    QString getTarget();
    static const QString getMethod(QByteArray);
    QString getMethod();

  protected:
    void setType(DataType);
    void setValue(QString, QVariant);
    QVariant getValue(QString);

  private:
    QMetaEnum metaEnum_type;
    QMap <QString, QString> *header;
    QMap <QString, QVariant> *library;
  };

  //////////////////////////////////////////////////////////////////////////////////

  class DataCustomize : public DataNEW
  {
    Q_GADGET
      Q_ENUMS(Key)

  public:
    enum Key{
      USERNAME,
      BOOL_BOOT_INTERPRETR, BOOL_BOOT_SERVER, BOOL_TEXT_ANTIALIASING,
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

    QStringList keys();
    QString key2string(Key);

  private:
    QMetaEnum metaEnum;
  };

  //////////////////////////////////////////////////////////////////////////////////

  class DataUser : public DataNEW
  {
    Q_GADGET
      Q_ENUMS(Key)

  public:
    enum Key { NAME, SERVER_METER, SERVER_CNTGROUPS, SERVER_CNTSYNTHS };

    DataUser();
    DataUser(QByteArray);
    DataUser(DataNEW);

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

  class DataProxy : public DataNEW
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
    QFont getValue_font(Key);
    QColor getValue_color(Key);


  private:
    QMetaEnum metaEnum;
  };

  class DataNode : public DataNEW
  {
    Q_GADGET
      Q_ENUMS(Key)

  public:
    enum Key { NAME, SOURCE };

    DataNode();
    DataNode(QByteArray);

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

}

#endif // DATA


