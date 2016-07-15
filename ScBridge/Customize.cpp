#include "Customize.h"

namespace SupercolliderBridge
{
  Customize::Customize(QObject *parent) : QObject(parent)
  {
    QString path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    this->initConfigFile(path);
    this->mergeConfigData();

    emit actCustomizeChanged(this);
  }

  void Customize::refresh()  { emit actCustomizeChanged(this); }

  void Customize::initConfigFile(QString systemExtensionDir)
  {
    QDir configDir;
    configDir.setPath(systemExtensionDir);
    configDir.mkdir("Shem");
    configDir.setPath(QDir::fromNativeSeparators(tr("%1/Shem").arg(systemExtensionDir)));

    configFile = new QFile(tr("%1/ShemConfig.txt").arg(configDir.path()));
  }

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

    foreach(QString key, mergeConfig.keys())
    {
      //qDebug() << "configKey [" << key << "] ->" << mergeConfig[key]->toString();
      this->setProperty(key.toStdString().c_str(), *mergeConfig[key]);
    };

    foreach(QString oneProp, this->dynamicPropertyNames())
    {
      qDebug() << "configKey [" << oneProp << "] ->" << this->property(oneProp.toStdString().c_str());
    }

  }

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
