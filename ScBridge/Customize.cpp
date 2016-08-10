#include "Customize.h"

namespace SupercolliderBridge
{
  Customize::Customize(QObject *parent) : QObject(parent)
  {
    QString path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    this->initConfigFile(path);
    this->defaultLibrary();
    this->readConfigFile();
    this->postprocessingData();
    this->writeConfigFile();

    emit actDataChanged(dataCustomize);
  }

  void Customize::refresh()  { emit actDataChanged(dataCustomize); }

  void Customize::initConfigFile(QString systemExtensionDir)
  {
    QDir configDir;
    configDir.setPath(systemExtensionDir);
    configDir.mkdir("Shem");
    configDir.setPath(QDir::fromNativeSeparators(tr("%1/Shem").arg(systemExtensionDir)));

    configFile = new QFile(tr("%1/ShemConfig.txt").arg(configDir.path()));
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

    dataCustomize.setValue(DataCustomize::BOOL_BOOT_NETWORK, false);
    dataCustomize.setValue(DataCustomize::BOOL_BOOT_INTERPRETR, false);
    dataCustomize.setValue(DataCustomize::BOOL_BOOT_SERVER, false);
  }

  void Customize::readConfigFile()
  {
    configFile->open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(configFile);
    QString txt;
    while (!in.atEnd())
    {
      QString line = in.readLine();
      txt += line;
    }

    if (!txt.isEmpty()) { dataCustomize = DataCustomize(txt.toUtf8()); }
    configFile->close();
  }

  void Customize::postprocessingData()
  {
    // SET USERNAME BY SYSTEM
    QString userName = dataCustomize.getValue_string(DataCustomize::Key::USERNAME);
    if (userName.isEmpty())
    {
      userName = qgetenv("USERNAME");
      dataCustomize.setValue(DataCustomize::Key::USERNAME, userName);
    }

    // ANTIALIAS FONTS
    QFont fontBig = dataCustomize.getValue_font(DataCustomize::Key::FONT_BIG);
    QFont fontSmall = dataCustomize.getValue_font(DataCustomize::Key::FONT_SMALL);
    QFont fontConsole = dataCustomize.getValue_font(DataCustomize::Key::FONT_CONSOLE);
    QFont fontCode = dataCustomize.getValue_font(DataCustomize::Key::FONT_CODE);

    if (dataCustomize.getValue_bool(DataCustomize::Key::BOOL_TEXT_ANTIALIASING))
    {
      fontBig.setStyleStrategy(QFont::PreferAntialias);
      fontSmall.setStyleStrategy(QFont::PreferAntialias);
      fontConsole.setStyleStrategy(QFont::PreferAntialias);
      fontCode.setStyleStrategy(QFont::PreferAntialias);
    }
    else
    {
      fontBig.setStyleStrategy(QFont::NoAntialias);
      fontSmall.setStyleStrategy(QFont::NoAntialias);
      fontConsole.setStyleStrategy(QFont::NoAntialias);
      fontCode.setStyleStrategy(QFont::NoAntialias);
    }

    dataCustomize.setValue(DataCustomize::Key::FONT_BIG, fontBig);
    dataCustomize.setValue(DataCustomize::Key::FONT_SMALL, fontSmall);
    dataCustomize.setValue(DataCustomize::Key::FONT_CONSOLE, fontConsole);
    dataCustomize.setValue(DataCustomize::Key::FONT_CODE, fontCode);
  }

  void Customize::writeConfigFile()
  {
    configFile->open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(configFile);

    out << dataCustomize.wrap();

    configFile->close();
  }

  void Customize::onModify(DataCustomize data)
  {
    dataCustomize = data;
    emit actDataChanged(dataCustomize);
  }
  void Customize::onSave(DataCustomize data)
  {
    dataCustomize = data;
    this->writeConfigFile();
  }

  Customize::~Customize() { }
}
