#include "ScBridge.h"

namespace SupercolliderBridge
{

  ScBridge::ScBridge(QObject * parent) :
    QProcess(parent),
    mIpcServer(new QLocalServer(this)),
    mIpcSocket(NULL),
    mIpcServerName("SCBridge_" + QString::number(QCoreApplication::applicationPid())),
    mTerminationRequested(false),
    mCompiled(false)
  {
    mInterpretState = StateInterpret::OFF;
    mServerState = StateServer::OFF;
    mBridgeProcess = BridgeProcess::NaN;

    const QMetaObject &mo = ScBridge::staticMetaObject;
    metaEnum_flagType = mo.enumerator(mo.indexOfEnumerator("FlagType"));

    lateFlagBreakTime = 500;
    tempo = 1;
    beat = 0;

    connect(this, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(mIpcServer, SIGNAL(newConnection()), this, SLOT(onNewIpcConnection()));
  }

  void ScBridge::initOSC()
  {
    qDebug() << "ScBridge::initOSC";

    QString oscFunc =
      "("
      "OSCdef.newMatching(\\SC_status, { | msg, time, addr, recvPort |"
      "(\"ServerStatus\" ++ msg).postln; "
      "}, '/status.reply', Server.default.addr );"
      ")";

    this->evaluate(oscFunc);

    /*
    QString oscFunc = "(";

    oscFunc += "OSCdef.newMatching(\\SC_status,";
    oscFunc += "{ | msg, time, addr, recvPort |";
    oscFunc += "(\"statusFlag,\" ++ msg).postln; ";
    oscFunc += "}, '/status.reply', Server.default.addr );";

    oscFunc += "OSCdef.newMatching(\\SC_nodeGo,";
    oscFunc += "{ | msg, time, addr, recvPort |";
    oscFunc += "(\"nodeFlag_GO,\" ++ msg).postln; ";
    oscFunc += "}, '/n_go', Server.default.addr );";

    oscFunc += "OSCdef.newMatching(\\SC_nodeEnd,";
    oscFunc += "{ | msg, time, addr, recvPort |";
    oscFunc += "(\"nodeFlag_END,\" ++ msg).postln; ";
    oscFunc += "}, '/n_end', Server.default.addr );";

    oscFunc += ")";

    this->evaluate(oscFunc);
    */
  }

  void ScBridge::killBridge()
  {
    if (mInterpretState == StateInterpret::RUN)
    {
      this->evaluate("Server.killAll", false, true);
      this->evaluate("0.exit", false, true);
    }
  }

  void ScBridge::changeInterpretState()
  {
    switch (mInterpretState)
    {
    case StateInterpret::OFF:
      mBridgeProcess = BridgeProcess::INTERPRET_BOOTING;
      emit actInterpretInit();
      startInterpretr();
      break;

    case StateInterpret::RUN:
      if (mServerState == StateServer::RUN) { this->changeServerState(); }
      mBridgeProcess = BridgeProcess::INTERPRET_KILLING;
      emit actInterpretKill();
      killInterpreter();
      break;
    }
  }
  void ScBridge::changeServerState()
  {
    switch (mServerState)
    {
    case StateServer::OFF:
      emit actServerInit();
      //evaluate("Server.local = Server.default = s;");
      mBridgeProcess = BridgeProcess::SERVER_BOOTING; // musi byt az po definici Server.local -> CHYBA!!!! onResponde probehne 2x
      evaluate("Server.local = Server.default = s; s.boot");
      //evaluate("s.boot;");
      break;
    case StateServer::RUN:
      mBridgeProcess = BridgeProcess::SERVER_KILLING;
      emit actServerKill();
      evaluate("s.quit;");
      break;
    }
  }

  bool ScBridge::evaluate(QString code, bool print, bool silent)
  {
    qDebug() << "ScBridge::evaluate " << code;
    bool synced = false;
    silent = false;

    qint64 evalTime = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();
    qint64 syncTime;
    int durationTime;

    QTimer time;
    time.setSingleShot(true);
    time.start(lateFlagBreakTime);
    //answer.append("NaN");

    if (state() != QProcess::Running) {
      emit actPrint(tr("Interpreter is not running!\r\n"), MessageType::STATUS);
      return false;
    }

    QString command;
    {
      QEventLoop loop;
      loop.connect(&time, SIGNAL(timeout()), SLOT(quit()));
      loop.connect(this, SIGNAL(actSynced()), SLOT(quit()));
      command = QStringLiteral("[\"syncFlag\",%1]").arg(code);

      QByteArray bytesToWrite = command.toUtf8();
      size_t writtenBytes = write(bytesToWrite);

      if (writtenBytes != bytesToWrite.size()) {
        emit actPrint(tr("Error when passing data to interpreter!\r\n"), MessageType::ERROR);
        return false;
      }

      char commandChar = silent ? '\x1b' : '\x0c';
      if (print) { actPrint(tr("evaluate: %1\r\n").arg(code), MessageType::EVAULATE); }
      write(&commandChar, 1);

      loop.exec();
    }

    syncTime = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();
    durationTime = syncTime - evalTime;

    if (print) {
      emit actPrint(tr("synced [%1 ms]\r\n").arg(QString::number(durationTime)));
    }

    return true;
  }
  QVariant ScBridge::question(QString code, bool print)
  {
    answer = QStringList();
    //answer = NULL;

    qint64 evalTime = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();
    qint64 answerTime;
    int durationTime;

    QTimer time;
    time.setSingleShot(true);
    time.start(lateFlagBreakTime);
    //answer.append("NaN");

    {
      QEventLoop loop;
      loop.connect(&time, SIGNAL(timeout()), SLOT(quit()));
      loop.connect(this, SIGNAL(actAnswered()), SLOT(quit()));
      QString command = QStringLiteral("[\"answerFlag\",%1]").arg(code);

      //this->evaluateCode(command, false, false);
      if (state() != QProcess::Running) {
        emit actPrint(tr("Interpreter is not running!\r\n"), MessageType::STATUS);
        return QStringList();
      }
      QByteArray bytesToWrite = command.toUtf8();
      size_t writtenBytes = write(bytesToWrite);

      if (writtenBytes != bytesToWrite.size()) {
        emit actPrint(tr("Error when passing data to interpreter!\r\n"), MessageType::ERROR);
        return QStringList();
      }

      char commandChar = '\x0c';
      write(&commandChar, 1);

      loop.exec();
    }

    answerTime = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();
    durationTime = answerTime - evalTime;

    if (print) {
      QString txt;
      if (answer.toList().isEmpty()) { txt = answer.toString(); }
      else
      {
        txt = answer.toStringList().join(" || ");
      }
      emit actPrint(tr("QA [%1 ms]: %2 = %3\r\n").arg(
        QString::number(durationTime),
        code,
        txt
        ), MessageType::ANSWER);
    }

    return answer;
  }

  bool ScBridge::isInterpretRunning()
  {
    switch (mInterpretState)
    {
    case StateInterpret::OFF: return false; break;
    case StateInterpret::RUN: return true; break;
    }
    return false;
  }
  bool ScBridge::isServerRunning()
  {
    switch (mServerState)
    {
    case StateServer::OFF: return false; break;
    case StateServer::RUN: return true; break;
    }
    return false;
  }

  void ScBridge::startInterpretr()
  {
    QString sclangCommand = "sclang";
    QString configFile;

    QStringList sclangArguments;
    if (!configFile.isEmpty())
      sclangArguments << "-l" << configFile;
    sclangArguments << "-i" << "scqt";

    QProcess::start(sclangCommand, sclangArguments);
    bool processStarted = QProcess::waitForStarted();
    if (!processStarted)
    {
      emit actPrint(tr("Failed to start interpreter!"), MessageType::STATUS);
    }
    else
    {
      if (!mIpcServer->isListening()) // avoid a warning on stderr
        mIpcServer->listen(mIpcServerName);

      QString command = QStringLiteral("ScIDE.connect(\"%1\")").arg(mIpcServerName);
      this->evaluate(command);
    }
  }
  void ScBridge::killInterpreter()
  {
    if (state() != QProcess::Running) {
      emit actPrint(tr("Interpreter is not running!"), MessageType::STATUS);
      return;
    }

    this->evaluate("0.exit", false, true);
    closeWriteChannel();

    mCompiled = false;
    mTerminationRequested = true;
    mTerminationRequestTime = QDateTime::currentDateTimeUtc();

    bool finished = waitForFinished(200);
    if (!finished && (state() != QProcess::NotRunning)) {
#ifdef Q_OS_WIN32
      kill();
#else
      terminate();
#endif
      bool reallyFinished = waitForFinished(200);
      if (!reallyFinished)
        emit actPrint(tr("Failed to stop interpreter!"), MessageType::STATUS);
      else
      {

      }
    }
    mTerminationRequested = false;
  }

  void ScBridge::onReadyRead()
  {
    if (mTerminationRequested) {
      // when stopping the language, we don't want to post for longer than 200 ms to prevent the UI to freeze
      if (QDateTime::currentDateTimeUtc().toMSecsSinceEpoch() - mTerminationRequestTime.toMSecsSinceEpoch() > 200)
        return;
    }

    QByteArray out = QProcess::readAll();
    QString postString = QString::fromUtf8(out);

    //qDebug() << "onReadyRead" << postString;
    //  this->msgFilter(postString);
    this->msgFilterNEW(postString);
  }

  void ScBridge::msgFilterNEW(QString msg)
  {
    //QStringList msgType = msg.split("||");
    QStringList data = msg.split(QRegExp("\\W\\D+"), QString::SkipEmptyParts);
    //qDebug() << "msg" << msg;
    //qDebug() << "msgType size" << msgType.size();
    QString type;
    //    QStringList data;
    if (data.size() > 1)
    {
      type = data[0];
      data.removeAt(0);
    }
    //qDebug() << "msgType" << data;
    //qDebug() << "msgData" << data;


    QStringList statusData;

    switch (metaEnum_flagType.keysToValue(type.toUtf8().constData()))
    {
    case ScBridge::FlagType::ServerStatus:
      //qDebug() << "ScBridge::msgFilterNEW -> ServerStatus";
      //qDebug() << "msgData" << data;

      statusData.append(data[6]); // serverPeak
      statusData.append(data[2]); // cntSynths
      statusData.append(data[3]); // cntGroups

      emit actServerStatus(statusData);
      break;

    default:
      if (!data.isEmpty())
      {
        qDebug() << "ScBridge::msgFilterNEW -> DEFAULT";
        qDebug() << "msgData" << data;
        emit actPrint(msg, MessageType::NORMAL);
      }
      break;
    }

    //qDebug() << "ScBridge::msgFilterNEW type: " << type << "msgType.Size: " << msgType.size();
  }

  void ScBridge::msgFilter(QString msg)
  {
    //qDebug() << "msg: " << msg;

    if (msg.contains("ERROR"))
    {
      emit actPrint(msg, MessageType::ERROR);
      /*
      QStringList msgLines = postString.split("\n");
      for (int i = 0; i < msgLines.size(); i = i + 1)
      {
      QString msg = msgLines.at(i);
      msg = msg.replace("\r", "");

      if (msg.startsWith("ERROR:"))
      {
      emit msgErrorAct(msg);
      }
      else
      {
      //emit
      }
      */
    }
    else if (msg.contains("WARNING")) { emit actPrint(msg, MessageType::WARNING); }
    else if (msg.contains("***")) { emit actPrint(msg, MessageType::STATUS); }
    else if (msg.contains("->"))
    {

      if (msg.contains("syncFlag"))	{ emit actSynced(); }

      else if (msg.contains("answerFlag"))
      {
        answer = QVariant();
        //qDebug() << "msg [answerFlag]: " << msg;
        QStringList incomingMSG = msg.split("->");
        foreach(QString oneMSG, incomingMSG)
        {
          if (oneMSG.contains("answerFlag"))
          {
            oneMSG = oneMSG.replace("\r", "");
            oneMSG = oneMSG.replace("\n", "");
            //qDebug() << "oneMSG: " << oneMSG;

            QStringList msgParts = oneMSG.split(",");
            if (msgParts.size() == 1)
            {
              QString onePart = msgParts.at(1);
              onePart = onePart.replace(" ", "");
              onePart = onePart.replace("[", "");
              onePart = onePart.replace("]", "");

              //qDebug() << "oneAnswer : " << msgParts;
              answer = QVariant(onePart);
            }
            else
            {
              QStringList answerList;
              for (int i = 1; i < msgParts.size(); i = i + 1)
              {
                QString onePart = msgParts.at(i);
                onePart = onePart.replace(" ", "");
                onePart = onePart.replace("[", "");
                onePart = onePart.replace("]", "");

                answerList.append(onePart);
              }
              answer = QVariant(answerList);
            }
          }
        }
        emit actAnswered();
      }
      else {
        //qDebug() << msg;
        //if (msg.startsWith("\r\n"))	{ msg = msg.replace("\r\n", ""); }
        //if (!msg.isEmpty())	{ emit msgResultAct(msg); }
        //emit msgResultAct(tr("%1\r\n").arg(msg));
        emit actPrint(msg, MessageType::ANSWER);
      }
    }
    else if (msg.contains("bundle")) { emit actPrint(msg, MessageType::BUNDLE); }
    else if (msg.contains("beatFlag"))
    {
      // int beat2 = this->question("p.clock.beats").toString().toInt();
      emit actNextBeat(beat);
      beat++;
    }
    else if (msg.contains("statusFlag"))
    {
      // qDebug() << "statusMSG" << msg;
      QStringList statusMSG = msg.split(",");
      QStringList statusData;
      statusData.append(statusMSG[8]); // serverPeak
      statusData.append(statusMSG[4]); // cntSynths
      statusData.append(statusMSG[5]); // cntGroups

      emit actServerStatus(statusData);
    }
    else if (msg.contains("nodeFlag_GO"))
    {
      //qDebug() << "nodeFlag_GO" << msg;

      QStringList nodeMSG = msg.split(",");
      int nodeID = nodeMSG[2].toInt();

      if (nodeMSG[6].toInt() == 0)
      {
        emit actSynthAdd(nodeMSG[2].toInt());
        //emit msgBundleAct(tr("synth [ %1 ] started").arg(QString::number(nodeID)));
      }
      else if (nodeMSG[6].toInt() == 1)
      {
        emit actGroupAdd(nodeMSG[2].toInt());
        // emit msgBundleAct(tr("group [ %1 ] started").arg(QString::number(nodeID)));
      }
    }
    else if (msg.contains("nodeFlag_END"))
    {
      //qDebug() << "nodeFlag_END" << msg;

      QStringList nodeMSG = msg.split(",");
      int nodeID = nodeMSG[2].toInt();

      if (nodeMSG[6].toInt() == 0)
      {
        emit actSynthFree(nodeMSG[2].toInt());
        //emit msgBundleAct(tr("synth [ %1 ] end").arg(QString::number(nodeID)));
      }
      else if (nodeMSG[6].toInt() == 1)
      {
        emit actGroupFree(nodeMSG[2].toInt());
        // emit msgBundleAct(tr("group [ %1 ] end").arg(QString::number(nodeID)));
      }
    }
    else {
      //qDebug() << msg;
      if (msg.startsWith("\r\n"))	{ msg = msg.replace("\r\n", ""); }
      if (!msg.isEmpty())	{ emit actPrint(msg); }
    }
  }

  QString ScBridge::nextID()
  {
    QString newID = this->question("s.nextNodeID", true).toString();
    qDebug() << "newID " << newID;
    return newID;
  }

  void ScBridge::onNewIpcConnection()
  {
    qDebug() << "ScBridge::onNewIpcConnection";
    if (mIpcSocket)
      // we can handle only one ipc connection at a time
      mIpcSocket->disconnect();

    mIpcSocket = mIpcServer->nextPendingConnection();

    connect(mIpcSocket, SIGNAL(disconnected()), this, SLOT(finalizeConnection()));
    connect(mIpcSocket, SIGNAL(readyRead()), this, SLOT(onIpcData()));
  }

  void ScBridge::finalizeConnection()
  {
    qDebug() << "ScBridge::finalizeConnection";
    mIpcData.clear();
    mIpcSocket->deleteLater();
    mIpcSocket = NULL;

    mInterpretState = StateInterpret::OFF;
    mBridgeProcess = BridgeProcess::NaN;
    emit actInterpretKillDone();
  }

  void ScBridge::onIpcData()
  {
    mIpcData.append(mIpcSocket->readAll());

    while (mIpcData.size()) {
      QBuffer receivedData(&mIpcData);
      receivedData.open(QIODevice::ReadOnly);

      QDataStream in(&receivedData);
      in.setVersion(QDataStream::Qt_4_6);
      QString selector, message;
      in >> selector;
      if (in.status() != QDataStream::Ok)
        return;

      in >> message;
      if (in.status() != QDataStream::Ok)
        return;

      mIpcData.remove(0, receivedData.pos());

      onResponse(selector, message);
      //emit response(selector, message);
    }
  }

  void ScBridge::onResponse(const QString & selector, const QString & data)
  {
    static QString serverRunningSelector("defaultServerRunningChanged");
    static QString introspectionSelector("introspection");
    static QString classLibraryRecompiledSelector("classLibraryRecompiled");
    static QString requestCurrentPathSelector("requestCurrentPath");

    // qDebug() << "ScBridge::onResponse selector: " << selector;

    if (selector == serverRunningSelector)
    {
      // DATA O STAVU SERVERU - msg[0] bool STATE; msg[1] int IP; msg[2] int PORT!!!!!!!!!!!
      QStringList msg = data.split("\n");
      /*
      qDebug() << "SERVER msg size: " << msg.size();
      qDebug() << "SERVER msg[0]: " << msg[0];
      qDebug() << "SERVER msg[1]: " << msg[1];
      qDebug() << "SERVER msg[2]: " << msg[2];
      qDebug() << "SERVER msg[3]: " << msg[3];
      */

      if (mBridgeProcess == BridgeProcess::INTERPRET_BOOTING || mBridgeProcess == BridgeProcess::INTERPRET_KILLING)
      {
        switch (mInterpretState)
        {
        case StateInterpret::OFF:
          qDebug() << "INTERPRET INIT FOUND ";
          emit actInterpretInitDone();
          mInterpretState = StateInterpret::RUN;
          break;
        }
      }

      if (mBridgeProcess == BridgeProcess::SERVER_BOOTING || mBridgeProcess == BridgeProcess::SERVER_KILLING)
      {
        switch (mServerState)
        {
        case StateServer::OFF:
          qDebug() << "SERVER INIT FOUND ";
          emit actServerInitDone();
          mServerState = StateServer::RUN;
          break;
        case StateServer::RUN:
          qDebug() << "SERVER KILL FOUND ";
          emit actServerKillDone();
          mServerState = StateServer::OFF;
          break;
        }
      }

      mBridgeProcess = BridgeProcess::NaN;
    }
    else if (selector == introspectionSelector)
    {
      //qDebug() << "INTROSPECTION" << data;

      // DATA O VSECH CLASS PRO SUPERCOLIDER!!!!!!
      // emit msgStatusAct(tr("INTROSPECTION message: %1").arg(data));
    }
    else if (selector == classLibraryRecompiledSelector)
    {
      //qDebug() << "INTROSPECTION" << data;

      // DATA O VSECH CLASS PRO SUPERCOLIDER!!!!!!
      //emit msgStatusAct(tr("LibraryRecompiled message: %1").arg(data));
    }
    else
    {
      //emit msgStatusAct(tr("IPC message: %1").arg(data));
    }
  }

  void ScBridge::onChangeTempo(QString BPM)
  {
    this->evaluate(tr("p.clock.tempo_(%1/60)").arg(BPM), true);
    tempo = this->question("p.clock.tempo", true).toString().toFloat();
    emit actTempoChanged();
  }

  ScBridge::~ScBridge() {	}
}


