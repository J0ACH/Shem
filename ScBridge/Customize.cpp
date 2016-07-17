#include "Customize.h"

namespace SupercolliderBridge
{
  Customize::Customize(QObject *parent) : QObject(parent)
  {
    QString path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    this->initConfigFile(path);
    this->configFileAssocciation();
    this->defaultLibrary();
    this->readConfigFileNEW();
    this->postprocessingLibrary();
    this->writeConfigFileNEW();

    emit actDataChanged(library);
  }

  void Customize::refresh()  { emit actCustomizeChanged(this); emit actDataChanged(library); }

  void Customize::initConfigFile(QString systemExtensionDir)
  {
    QDir configDir;
    configDir.setPath(systemExtensionDir);
    configDir.mkdir("Shem");
    configDir.setPath(QDir::fromNativeSeparators(tr("%1/Shem").arg(systemExtensionDir)));

    configFile = new QFile(tr("%1/ShemConfig.txt").arg(configDir.path()));
  }

  // bude odstraneno
  void Customize::mergeConfigData()
  {
    QMap<QString, QVariant*> oldConfig;
    QMap<QString, QVariant*> newConfig;
    QMap<QString, QVariant*> mergeConfig;

    if (configFile->exists())
    {
      oldConfig = this->readConfigFile();
      newConfig = this->defaultConfig();

      foreach(QString key, newConfig.keys())
      {
        if (oldConfig.contains(key)) { mergeConfig.insert(key, oldConfig[key]); }
        else
        {
          mergeConfig.insert(key, newConfig[key]);
          qDebug() << "Customize -> merge insert key " << key;
        }
      }
    }
    else { mergeConfig = this->defaultConfig(); }

    mergeConfig = this->processingConfigData(mergeConfig);
    this->writeConfigFile(mergeConfig);

    //    this->prepareData(mergeConfig);
    foreach(QString key, mergeConfig.keys())
    {
      //qDebug() << "configKey [" << key << "] ->" << mergeConfig[key]->toString();
      this->setProperty(key.toStdString().c_str(), *mergeConfig[key]);
    };
    /*

    foreach(QString oneProp, this->dynamicPropertyNames())
    {
    qDebug() << "configKey [" << oneProp << "] ->" << this->property(oneProp.toStdString().c_str());
    }
    */


  }

  // bude odstraneno
  QMap<QString, QVariant*> Customize::processingConfigData(QMap<QString, QVariant*> data)
  {
    // SET USERNAME BY SYSTEM
    QString userName = data.value("string_shem_UserName")->value<QString>();
    if (userName.isEmpty())
    {
      userName = qgetenv("USERNAME");
      data.insert("string_shem_UserName", new QVariant(userName));
    }

    // ANTIALIAS FONTS
    QFont font;
    bool textAntialias = data.value("bool_shem_TextAntialias")->value<bool>();
    if (textAntialias)
    {
      font = data["font_shem_TextBig"]->value<QFont>();
      font.setStyleStrategy(QFont::PreferAntialias);
      data.insert("font_shem_TextBig", new QVariant(font));

      font = data["font_shem_TextSmall"]->value<QFont>();
      font.setStyleStrategy(QFont::PreferAntialias);
      data.insert("font_shem_TextSmall", new QVariant(font));

      font = data["font_shem_TextCode"]->value<QFont>();
      font.setStyleStrategy(QFont::PreferAntialias);
      data.insert("font_shem_TextCode", new QVariant(font));

      font = data["font_shem_TextConsole"]->value<QFont>();
      font.setStyleStrategy(QFont::PreferAntialias);
      data.insert("font_shem_TextConsole", new QVariant(font));
    }
    else
    {
      font = data["font_shem_TextBig"]->value<QFont>();
      font.setStyleStrategy(QFont::NoAntialias);
      data.insert("font_shem_TextBig", new QVariant(font));

      font = data["font_shem_TextSmall"]->value<QFont>();
      font.setStyleStrategy(QFont::NoAntialias);
      data.insert("font_shem_TextSmall", new QVariant(font));

      font = data["font_shem_TextCode"]->value<QFont>();
      font.setStyleStrategy(QFont::NoAntialias);
      data.insert("font_shem_TextCode", new QVariant(font));

      font = data["font_shem_TextConsole"]->value<QFont>();
      font.setStyleStrategy(QFont::NoAntialias);
      data.insert("font_shem_TextConsole", new QVariant(font));
    }

    return data;
  }

  // bude odstraneno
  void Customize::writeConfigFile(QMap<QString, QVariant*> mergeConfigData)
  {
    //	qDebug() << "Customize::writeConfigFile " << configFile->fileName();

    configFile->open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream out(configFile);

    foreach(QString key, mergeConfigData.keys())
    {
      if (key.startsWith("color"))
      {
        QColor color = mergeConfigData[key]->value<QColor>();
        out << key << " = "
          << color.red() << ", "
          << color.green() << ", "
          << color.blue() << "\n";
      }
      if (key.startsWith("font"))
      {
        QFont font = mergeConfigData[key]->value<QFont>();
        out << key << " = "
          << font.family() << ", "
          << font.pointSize() << "\n";
      }
      if (key.startsWith("bool"))
      {
        bool boolean = mergeConfigData[key]->value<bool>();
        if (boolean) { out << key << " = true\n"; }
        else { out << key << " = false\n"; }
      }
      if (key.startsWith("string"))
      {
        QString text = mergeConfigData[key]->value<QString>();
        out << key << " = " << text << "\n";
      }
    }

    configFile->close();

    this->readConfigFile();
  }


  // bude odstraneno
  QMap<QString, QVariant*> Customize::defaultConfig()
  {
    QMap<QString, QVariant*> defaultConfig;
    defaultConfig.insert("string_shem_UserName", new QVariant(""));

    defaultConfig.insert("color_shem_AppBackground", new QVariant(QColor(20, 20, 20)));
    defaultConfig.insert("color_shem_AppHeaderBackground", new QVariant(QColor(40, 40, 40)));
    defaultConfig.insert("color_shem_PanelBackground", new QVariant(QColor(30, 30, 30)));
    defaultConfig.insert("color_shem_Normal", new QVariant(QColor(120, 120, 120)));
    defaultConfig.insert("color_shem_Frozen", new QVariant(QColor(70, 70, 70)));
    defaultConfig.insert("color_shem_Over", new QVariant(QColor(255, 255, 255)));
    defaultConfig.insert("color_shem_Active", new QVariant(QColor(70, 140, 210)));
    defaultConfig.insert("color_shem_Text", new QVariant(QColor(230, 230, 230)));

    defaultConfig.insert("color_shem_MsgNormal", new QVariant(QColor(70, 70, 70)));
    defaultConfig.insert("color_shem_MsgStatus", new QVariant(QColor(230, 230, 230)));
    defaultConfig.insert("color_shem_MsgEvaluate", new QVariant(QColor(170, 230, 230)));
    defaultConfig.insert("color_shem_MsgResult", new QVariant(QColor(170, 200, 160)));
    defaultConfig.insert("color_shem_MsgError", new QVariant(QColor(230, 30, 30)));
    defaultConfig.insert("color_shem_MsgWarning", new QVariant(QColor(230, 130, 30)));
    defaultConfig.insert("color_shem_MsgBundle", new QVariant(QColor(170, 160, 20)));

    defaultConfig.insert("bool_shem_TextAntialias", new QVariant(true));
    defaultConfig.insert("font_shem_TextBig", new QVariant(QFont("Univers Condensed", 13)));
    defaultConfig.insert("font_shem_TextSmall", new QVariant(QFont("Univers Condensed", 10)));
    defaultConfig.insert("font_shem_TextCode", new QVariant(QFont("Consolas", 8)));
    defaultConfig.insert("font_shem_TextConsole", new QVariant(QFont("Univers 57 Condensed", 9)));

    defaultConfig.insert("bool_shem_onStartBoot_Interpret", new QVariant(true));
    defaultConfig.insert("bool_shem_onStartBoot_Server", new QVariant(false));

    return defaultConfig;
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
      QVariant *value;



      if (key.startsWith("color"))
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
      if (key.startsWith("font"))
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
      if (key.startsWith("bool"))
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
    QString userName = library.getValue_string(DataKey::USERNAME);
    if (userName.isEmpty())
    {
      userName = qgetenv("USERNAME");
      library.setValue(DataKey::USERNAME, userName);
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

  void Customize::writeConfigFileNEW()
  {
    configFile->open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(configFile);

    foreach(DataKey oneKey, configFileKeys.keys())
    {
      //qDebug() << "Customize::writeConfigFileNEW()"<< configFileKeys.value(oneKey) << "=" << library.toString(oneKey);
      out << configFileKeys.value(oneKey) << " = " << library.toString(oneKey) << "\n";
    }

    configFile->close();
  }

  void Customize::onModify(Data data)
  {
    library = data;
    emit actDataChanged(library);
  }
  void Customize::onSave(Data data)
  {
    library = data;
    this->writeConfigFileNEW();
    emit actDataChanged(library);
  }

  // bude odstraneno
  QMap<QString, QVariant*> Customize::readConfigFile()
  {
    //qDebug() << "Customize::readConfigFile " << configFile->fileName();

    QMap<QString, QVariant*> oldConfigData;

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
      QVariant *value;

      if (key.startsWith("color"))
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

        if (isColor) {
          QColor color = QColor(rgb[0], rgb[1], rgb[2]);
          value = new QVariant(color);
          oldConfigData.insert(key, value);
          continue;
        }
      }
      if (key.startsWith("font"))
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
          value = new QVariant(font);
          oldConfigData.insert(key, value);
          continue;
        }
      }
      if (key.startsWith("bool"))
      {
        //qDebug() << "ConfigDataTyp: bool";
        QString value = args[0].remove(" ");

        if (value == "true" || value == "1")
        {
          oldConfigData.insert(key, new QVariant(true));
        }
        else
        {
          oldConfigData.insert(key, new QVariant(false));
        };
      }
      if (key.startsWith("string"))
      {
        //qDebug() << "ConfigDataTyp: string";
        QString value = args[0].remove(" ");
        oldConfigData.insert(key, new QVariant(value));
      }
    }

    // mBridge->msgNormalAct(tr("\ncustomization config file: %1\r").arg(configFile->fileName()));
    configFile->close();

    return oldConfigData;
  }

  /*
  void Customize::prepareData(QMap<QString, QVariant*> mergeConfigData)
  {
  foreach(QString key, mergeConfigData.keys())
  {
  qDebug() << "configKey [" << key << "] ->" << mergeConfigData[key]->toString();
  // if (key == "string_shem_UserName") { data.setValue(DataKey::USERNAME, mergeConfigData[key]); }
  }
  //this->setProperty(key.toStdString().c_str(), *mergeConfig[key]);

  }
  */

  // vse nize bude odstraneno
  QString Customize::getString(QString key)
  {
    QString text;
    text = this->property(key.toStdString().c_str()).value<QString>();
    return text;
  }
  QString Customize::getString(Customize::Keys key)
  {
    switch (key)
    {
    case userName: return this->getString("string_shem_UserName"); break;
    default: return "NaN"; break;
    }
  }

  QColor Customize::getColor(QString key)
  {
    QColor color;
    color = this->property(key.toStdString().c_str()).value<QColor>();
    if (!color.isValid())
    {
      qDebug() << "Customize::getColor [" << key << "] is not valid";
      color = QColor(255, 0, 0);
    }
    return color;
  }
  QColor Customize::getColor(Customize::Keys key)
  {
    QColor color;
    switch (key)
    {
    case colorHeaders: color = this->getColor("color_shem_AppHeaderBackground"); break;
    case colorBackground: color = this->getColor("color_shem_PanelBackground"); break;
    case colorNormal: color = this->getColor("color_shem_Normal"); break;
    case colorOver: color = this->getColor("color_shem_Over"); break;
    case colorActive: color = this->getColor("color_shem_Active"); break;
    case colorText: color = this->getColor("color_shem_Text"); break;
    default: color = QColor(255, 0, 0); break;
    }

    if (!color.isValid())
    {
      qDebug() << "Customize::getColor [" << key << "] is not valid";
      color = QColor(255, 0, 0);
    }
    return color;
  }
  void Customize::setColor(Customize::Keys key, QColor color)
  {
    switch (key)
    {
    case colorHeaders: color = this->setProperty("color_shem_AppHeaderBackground", color); break;
    case colorBackground: color = this->setProperty("color_shem_PanelBackground", color); break;
    case colorNormal: color = this->getColor("color_shem_Normal"); break;
    case colorOver: color = this->getColor("color_shem_Over"); break;
    case colorActive: color = this->getColor("color_shem_Active"); break;
    case colorText: color = this->getColor("color_shem_Text"); break;
    default: color = QColor(255, 0, 0); break;
    }

    foreach(QString oneProp, this->dynamicPropertyNames())
    {
      qDebug() << "configKey [" << oneProp << "] ->" << this->property(oneProp.toStdString().c_str());
    }

    emit actCustomizeChanged(this);
  }

  QFont Customize::getFont(QString key)
  {
    QFont font;
    font = this->property(key.toStdString().c_str()).value<QFont>();
    //qDebug() << "Customize::getFont [" << key << "] -> " << font;
    return font;
  }
  QFont Customize::getFont(Customize::Keys key)
  {
    switch (key)
    {
    case fontBig: return this->getFont("font_shem_TextBig"); break;
    case fontSmall: return this->getFont("font_shem_TextSmall"); break;
    case fontConsole: return this->getFont("font_shem_TextConsole"); break;
    case fontCode: return this->getFont("font_shem_TextCode"); break;
    default: return QFont("Consolas", 8); break;
    }
  }

  bool Customize::getBool(QString key)
  {
    bool boolean;
    boolean = this->property(key.toStdString().c_str()).value<bool>();
    return boolean;
  }
  bool Customize::getBool(Customize::Keys key)
  {
    switch (key)
    {
    case boolBootInterpretr: return this->getBool("bool_shem_onStartBoot_Interpret"); break;
    case boolBootServer: return this->getBool("bool_shem_onStartBoot_Server"); break;
    case boolTextAntialiasing: return this->getBool("bool_shem_TextAntialias"); break;
    default: return false; break;
    }
  }

  Customize::~Customize() { }
}
