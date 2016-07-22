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
    //    enum Key { };
    //QString senderName;
    QMap <DataKey, QVariant> *library;
    QMap <QString, QVariant> *libraryNEW;


  };


  //////////////////////////////////////////////////////////////////////////////////

  class DataNEW
  {
  public:
    DataNEW();
    DataNEW(QByteArray);

    void print(QString comment = "");
    QByteArray wrap();

  protected:
    void setValue(QString, QVariant);
    QVariant getValue(QString);

  private:
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

  class DataProxy : public DataNEW
  {
    Q_GADGET
      Q_ENUMS(Key)

  public:
    enum Key { BEATS, TEMPO, PLAYING, color, font };

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

}

#endif // DATA


