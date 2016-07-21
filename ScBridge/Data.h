#ifndef DATA_H
#define DATA_H

#include <QDebug>
#include <QMap>
#include <QColor>
#include <QFont>
#include <QDataStream>

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
   
    enum Key { };

    Data();
    Data(QByteArray);
    ~Data();

    QString enum2str(Key);
    
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
    QString getValue_stringNEW(QString);

  private:
    //QString senderName;
    QMap <DataKey, QVariant> *library;
    QMap <QString, QVariant> *libraryNEW;
  };

  //////////////////////////////////////////////////////////////////////////////////

  class DataCustomize : public Data
  {
  public:
    enum Key{
      USERNAME,
      BOOL_BOOT_INTERPRETR, BOOL_BOOT_SERVER, BOOL_TEXT_ANTIALIASING,
      COLOR_APP_HEADER, COLOR_APP_BACKGROUND, COLOR_PANEL_HEADER, COLOR_PANEL_BACKGROUND, COLOR_NORMAL, COLOR_OVER, COLOR_ACTIVE, COLOR_TEXT,
      COLOR_MSG_NORMAL, COLOR_MSG_STATUS, COLOR_MSG_EVALUATE, COLOR_MSG_ANSWER, COLOR_MSG_ERROR, COLOR_MSG_WARNINIG, COLOR_MSG_BUNDLE,
      FONT_BIG, FONT_SMALL, FONT_CONSOLE, FONT_CODE
    };

    DataCustomize();
    ~DataCustomize();
  };

  //////////////////////////////////////////////////////////////////////////////////

  class DataProxy : public Data
  {
  public:
     enum Key{ BEATS, TEMPO };
    
    DataProxy();
    ~DataProxy();

    void setValue(Key, char*);
    QString getValue_string(Key);
  };

}

#endif // DATA


