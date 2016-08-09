#include "Customize.h"

namespace SupercolliderBridge
{
  Customize::Customize(QObject *parent) : QObject(parent)
  {
    QString path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    this->initConfigFile(path);
    // this->configFileAssocciation();
    this->defaultLibrary();
    this->readConfigFileNEW();
    this->readConfigFile2();
    this->postprocessingLibrary();
    this->writeConfigFileNEW();
    this->writeConfigFile2();

    qDebug() << dataCustomize.print("Customize done LIBRARY_NEW");
    qDebug() << "Key2String: " << dataCustomize.key2string(DataCustomize::Key::BOOL_TEXT_ANTIALIASING);

    emit actDataChanged2(dataCustomize);
    emit actDataChanged(library);
  }

  void Customize::refresh()  { emit actCustomizeChanged(this); emit actDataChanged(library); emit actDataChanged2(dataCustomize); }

  void Customize::initConfigFile(QString systemExtensionDir)
  {
    QDir configDir;
    configDir.setPath(systemExtensionDir);
    configDir.mkdir("Shem");
    configDir.setPath(QDir::fromNativeSeparators(tr("%1/Shem").arg(systemExtensionDir)));

    configFile = new QFile(tr("%1/ShemConfig.txt").arg(configDir.path()));
    configFile2 = new QFile(tr("%1/ShemConfig2.txt").arg(configDir.path()));
  }

  void Customize::configFileAssocciation()
  {
    configFileKeys.insert(DataKey::USERNAME, "string_UserName");

    configFileKeys.insert(DataKey::COLOR_APP_HEADER, "color_App_Header");
    configFileKeys.insert(DataKey::COLOR_APP_BACKGROUND, "color_App_Background");
    configFileKeys.insert(DataKey::COLOR_PANEL_HEADER, "color_Panel_Header");
    configFileKeys.insert(DataKey::COLOR_PANEL_BACKGROUND, "color_Panel_Background");
    configFileKeys.insert(DataKey::COLOR_NORMAL, "color_Normal");
    configFileKeys.insert(DataKey::COLOR_OVER, "color_Over");
    configFileKeys.insert(DataKey::COLOR_ACTIVE, "color_Active");
    configFileKeys.insert(DataKey::COLOR_TEXT, "color_Text");

    configFileKeys.insert(DataKey::COLOR_MSG_NORMAL, "color_Msg_Normal");
    configFileKeys.insert(DataKey::COLOR_MSG_STATUS, "color_Msg_Status");
    configFileKeys.insert(DataKey::COLOR_MSG_EVALUATE, "color_Msg_Evaluate");
    configFileKeys.insert(DataKey::COLOR_MSG_ANSWER, "color_Msg_Answer");
    configFileKeys.insert(DataKey::COLOR_MSG_ERROR, "color_Msg_Error");
    configFileKeys.insert(DataKey::COLOR_MSG_WARNINIG, "color_Msg_Warning");
    configFileKeys.insert(DataKey::COLOR_MSG_BUNDLE, "color_Msg_Bundle");

    configFileKeys.insert(DataKey::BOOL_TEXT_ANTIALIASING, "bool_Text_Antialias");
    configFileKeys.insert(DataKey::FONT_BIG, "font_Text_Big");
    configFileKeys.insert(DataKey::FONT_SMALL, "font_Text_Small");
    configFileKeys.insert(DataKey::FONT_CODE, "font_Text_Code");
    configFileKeys.insert(DataKey::FONT_CONSOLE, "font_Text_Console");

    configFileKeys.insert(DataKey::BOOL_BOOT_INTERPRETR, "bool_App_bootOnStart_Interpret");
    configFileKeys.insert(DataKey::BOOL_BOOT_SERVER, "bool_App_bootOnStart_Server");
  }

  void Customize::defaultLibrary()
  {

    dataCustomize.setValue(DataCustomize::USERNAME, "");

    dataCustomize.setValue(DataCustomize::COLOR_APP_HEADER, QColor(40, 40, 40));
    dataCustomize.setValue(DataCustomize::COLOR_APP_BACKGROUND, QColor(20, 20, 20));
    dataCustomize.setValue(DataCustomize::COLOR_PANEL_HEADER, QColor(30, 30, 30));
    dataCustomize.setValue(DataCustomize::COLOR_PANEL_BACKGROUND, QColor(25, 25, 25));
    dataCustomize.setValue(DataCustomize::COLOR_NORMAL, QColor(120, 120, 120));
    dataCustomize.setValue(DataCustomize::COLOR_OVER, QColor(255, 255, 255));
    dataCustomize.setValue(DataCustomize::COLOR_ACTIVE, QColor(70, 140, 210));
    dataCustomize.setValue(DataCustomize::COLOR_TEXT, QColor(230, 230, 230));

    dataCustomize.setValue(DataCustomize::COLOR_MSG_NORMAL, QColor(70, 70, 70));
    dataCustomize.setValue(DataCustomize::COLOR_MSG_STATUS, QColor(230, 230, 230));
    dataCustomize.setValue(DataCustomize::COLOR_MSG_EVALUATE, QColor(170, 230, 230));
    dataCustomize.setValue(DataCustomize::COLOR_MSG_ANSWER, QColor(170, 200, 160));
    dataCustomize.setValue(DataCustomize::COLOR_MSG_ERROR, QColor(230, 30, 30));
    dataCustomize.setValue(DataCustomize::COLOR_MSG_WARNINIG, QColor(230, 130, 30));
    dataCustomize.setValue(DataCustomize::COLOR_MSG_BUNDLE, QColor(170, 160, 20));

    dataCustomize.setValue(DataCustomize::BOOL_TEXT_ANTIALIASING, true);
    dataCustomize.setValue(DataCustomize::FONT_BIG, QFont("Univers Condensed", 13));
    dataCustomize.setValue(DataCustomize::FONT_SMALL, QFont("Univers Condensed", 10));
    dataCustomize.setValue(DataCustomize::FONT_CODE, QFont("Consolas", 8));
    dataCustomize.setValue(DataCustomize::FONT_CONSOLE, QFont("Univers 57 Condensed", 9));

    dataCustomize.setValue(DataCustomize::BOOL_BOOT_INTERPRETR, false);
    dataCustomize.setValue(DataCustomize::BOOL_BOOT_SERVER, false);

    // bude odstraneno ////////////////////////////

    library.setValue(DataKey::USERNAME, "");

    library.setValue(DataKey::COLOR_APP_HEADER, QColor(40, 40, 40));
    library.setValue(DataKey::COLOR_APP_BACKGROUND, QColor(20, 20, 20));
    library.setValue(DataKey::COLOR_PANEL_HEADER, QColor(30, 30, 30));
    library.setValue(DataKey::COLOR_PANEL_BACKGROUND, QColor(25, 25, 25));
    library.setValue(DataKey::COLOR_NORMAL, QColor(120, 120, 120));
    library.setValue(DataKey::COLOR_OVER, QColor(255, 255, 255));
    library.setValue(DataKey::COLOR_ACTIVE, QColor(70, 140, 210));
    library.setValue(DataKey::COLOR_TEXT, QColor(230, 230, 230));

    library.setValue(DataKey::COLOR_MSG_NORMAL, QColor(70, 70, 70));
    library.setValue(DataKey::COLOR_MSG_STATUS, QColor(230, 230, 230));
    library.setValue(DataKey::COLOR_MSG_EVALUATE, QColor(170, 230, 230));
    library.setValue(DataKey::COLOR_MSG_ANSWER, QColor(170, 200, 160));
    library.setValue(DataKey::COLOR_MSG_ERROR, QColor(230, 30, 30));
    library.setValue(DataKey::COLOR_MSG_WARNINIG, QColor(230, 130, 30));
    library.setValue(DataKey::COLOR_MSG_BUNDLE, QColor(170, 160, 20));

    library.setValue(DataKey::BOOL_TEXT_ANTIALIASING, true);
    library.setValue(DataKey::FONT_BIG, QFont("Univers Condensed", 13));
    library.setValue(DataKey::FONT_SMALL, QFont("Univers Condensed", 10));
    library.setValue(DataKey::FONT_CODE, QFont("Consolas", 8));
    library.setValue(DataKey::FONT_CONSOLE, QFont("Univers 57 Condensed", 9));

    library.setValue(DataKey::BOOL_BOOT_INTERPRETR, false);
    library.setValue(DataKey::BOOL_BOOT_SERVER, false);
  }

  void Customize::readConfigFile2()
  {
    configFile2->open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(configFile2);
    QString txt;
    while (!in.atEnd())
    {
      QString line = in.readLine();
      txt += line;
    }

    if (!txt.isEmpty()) { dataCustomize = DataCustomize(txt.toUtf8()); }
    configFile2->close();
  }

  void Customize::readConfigFileNEW()
  {
    // QMap<QString, QVariant*> oldConfigData;

    configFile->open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(configFile);
    while (!in.atEnd())
    {
      QString line = in.readLine();
      QStringList lineParts = line.remove("\n").split("=");

      /*
      qDebug() << "Customize::readConfigFile line: " << line;
      qDebug() << "Customize::key: " << lineParts[0];
      qDebug() << "Customize::args: " << lineParts[1];
      */

      QString key = lineParts[0].remove(" ");
      QStringList args = lineParts[1].split(",");

      if (key.startsWith("COLOR"))
      {
        //qDebug() << "ConfigDataTyp: color";
        bool isColor = true;
        int rgb[3];
        for (int i = 0; i < 3; i++)
        {
          bool isNumber;
          args[i] = args[i].remove(" ");
          rgb[i] = args[i].toInt(&isNumber, 10);
          if (!isNumber) { isColor = false; break; }
        }
        if (!isColor) { rgb[0] = 255; rgb[1] = 0; rgb[2] = 0; }
        if (isColor) {
          QColor color = QColor(rgb[0], rgb[1], rgb[2]);
          foreach(DataKey oneKey, configFileKeys.keys()) { if (key == configFileKeys.value(oneKey)) { library.setValue(oneKey, color); break; } }
          continue;
        }
      }
      if (key.startsWith("FONT"))
      {
        //qDebug() << "ConfigDataTyp: font";
        bool isFont = true;
        bool isNumber;
        int size = args[1].remove(" ").toInt(&isNumber, 10);
        if (!isNumber) { isFont = false; break; }

        if (isFont)
        {
          if (args[0].startsWith(" ")) { args[0].remove(0, 1); }
          QFont font;
          font.setFamily(args[0]);
          font.setPointSize(size);
          font.setStretch(QFont::Unstretched);
          foreach(DataKey oneKey, configFileKeys.keys()) { if (key == configFileKeys.value(oneKey)) { library.setValue(oneKey, font); break; } }
          continue;
        }
      }
      if (key.startsWith("BOOL"))
      {
        //qDebug() << "ConfigDataTyp: bool";
        QString value = args[0].remove(" ");

        if (value == "true" || value == "1")
        {
          foreach(DataKey oneKey, configFileKeys.keys()) { if (key == configFileKeys.value(oneKey)) { library.setValue(oneKey, true); break; } }
        }
        else
        {
          foreach(DataKey oneKey, configFileKeys.keys()) { if (key == configFileKeys.value(oneKey)) { library.setValue(oneKey, false); break; } }
        };
      }
      if (key.startsWith("string"))
      {
        //qDebug() << "ConfigDataTyp: string";
        QString value = args[0].remove(" ");
        foreach(DataKey oneKey, configFileKeys.keys()) { if (key == configFileKeys.value(oneKey)) { library.setValue(oneKey, value); break; } }
      }
    }

    // mBridge->msgNormalAct(tr("\ncustomization config file: %1\r").arg(configFile->fileName()));
    configFile->close();
  }

  void Customize::postprocessingLibrary()
  {
    // SET USERNAME BY SYSTEM
    /*
    QString userName = library.getValue_string(DataKey::USERNAME);
    if (userName.isEmpty())
    {
    userName = qgetenv("USERNAME");
    library.setValue(DataKey::USERNAME, userName);
    dataCustomize.setValue(DataCustomize::Key::USERNAME, userName);
    }
    */
    QString userName = dataCustomize.getValue_string(DataCustomize::Key::USERNAME);
    if (userName.isEmpty())
    {
      userName = qgetenv("USERNAME");
      // library.setValue(DataKey::USERNAME, userName); // bude odstraneno
      dataCustomize.setValue(DataCustomize::Key::USERNAME, userName);
    }

    // ANTIALIAS FONTS
    foreach(DataKey oneKey, configFileKeys.keys())
    {
      if (configFileKeys.value(oneKey).startsWith("font"))
      {
        QFont font = library.getValue_font(oneKey);
        if (library.getValue_bool(DataKey::BOOL_TEXT_ANTIALIASING))
        {
          font.setStyleStrategy(QFont::PreferAntialias);
        }
        else
        {
          font.setStyleStrategy(QFont::NoAntialias);
        }
      }
    }
  }

  void Customize::writeConfigFile2()
  {
    configFile2->open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(configFile2);

    out << dataCustomize.wrap();

    configFile2->close();
  }

  void Customize::writeConfigFileNEW()
  {
    configFile->open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(configFile);

    foreach(DataKey oneKey, configFileKeys.keys())
    {
      //qDebug() << "Customize::writeConfigFileNEW()"<< configFileKeys.value(oneKey) << "=" << library.toString(oneKey);
      out << configFileKeys.value(oneKey) << " = " << library.toString(oneKey) << "\n";
    }

    /*
    foreach(QString oneKey, dataCustomize.keys())
    {
    qDebug() << "Customize::writeConfigFileNEW()" << oneKey << " = " << dataCustomize.getValue(oneKey).toString();
    out << oneKey << " = " << dataCustomize.getValue(oneKey).toString() << "\n";
    }
    */

    configFile->close();
  }

  void Customize::onModify(Data data)
  {
    library = data;
    emit actDataChanged(library);
  }

  void Customize::onModify2(DataCustomize data)
  {
    dataCustomize = data;
    emit actDataChanged2(dataCustomize);
  }
  void Customize::onSave(Data data)
  {
    library = data;
    this->writeConfigFileNEW();
    emit actDataChanged(library);
  }
  void Customize::onSave2(DataCustomize data)
  {
    dataCustomize = data;
    this->writeConfigFile2();
    //emit actDataChanged2(dataCustomize);
  }

  Customize::~Customize() { }
}
