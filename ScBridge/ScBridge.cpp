#include "ScBridge.h"
#include <QtWidgets/QApplication>


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
    stateInterpret = StateInterpret::OFF;
    stateServer = StateServer::OFF;

    lateFlagBreakTime = 500;
    tempo = 1;
    beat = 0;

    connect(this, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(mIpcServer, SIGNAL(newConnection()), this, SLOT(onNewIpcConnection()));
  }

  void ScBridge::initOSC()
  {
    //qDebug("OSCtest start");
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
  }

  void ScBridge::killBridge()
  {
    this->evaluate("Server.killAll", true);
    this->killInterpreter();
    emit killBridgeDoneAct();
  }

  void ScBridge::changeInterpretState()
  {
    switch (stateInterpret)
    {
    case StateInterpret::OFF:
      emit interpretBootInitAct();
      startInterpretr();
      break;

    case StateInterpret::RUNNING:
      emit interpretKillInitAct();
      killInterpreter();
      break;
    }
  }
  void ScBridge::changeServerState()
  {
    QString oscFunc;

    switch (stateServer)
    {
    case StateServer::OFF:
      emit serverBootInitAct();
      evaluate("Server.local = Server.default = s;");
      // evaluateCode("s.dumpOSC;");
      evaluate("s.boot;");
      break;
    case StateServer::RUNNING:
      emit serverKillInitAct();
      evaluate("s.quit;");
      break;
    }
  }

  bool ScBridge::evaluate(QString code, bool print, bool silent)
  {
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
      emit msgStatusAct(tr("Interpreter is not running!\r\n"));
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
        emit msgStatusAct(tr("Error when passing data to interpreter!\r\n"));
        return false;
      }

      char commandChar = silent ? '\x1b' : '\x0c';
      if (print) { emit msgEvaluateAct(tr("evaluate: %1\r\n").arg(code)); }
      write(&commandChar, 1);

      loop.exec();
    }

    syncTime = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();
    durationTime = syncTime - evalTime;

    if (print) {
      emit msgNormalAct(tr("synced [%1 ms]\r\n").arg(QString::number(durationTime)));
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
        emit msgStatusAct(tr("Interpreter is not running!\r\n"));
        return QStringList();
      }
      QByteArray bytesToWrite = command.toUtf8();
      size_t writtenBytes = write(bytesToWrite);

      if (writtenBytes != bytesToWrite.size()) {
        emit msgStatusAct(tr("Error when passing data to interpreter!\r\n"));
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
      emit msgResultAct(tr("QA [%1 ms]: %2 = %3\r\n").arg(
        QString::number(durationTime),
        code,
        txt
        ));
    }

    return answer;
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
      emit msgStatusAct(tr("Failed to start interpreter!"));
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
      emit msgStatusAct(tr("Interpreter is not running!"));
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
        emit msgStatusAct(tr("Failed to stop interpreter!"));
      else
      {
        stateInterpret = StateInterpret::OFF;
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
    this->msgFilter(postString);
  }

  void ScBridge::msgFilter(QString msg)
  {
    //qDebug() << "msg: " << msg;

    if (msg.contains("ERROR"))
    {
      emit msgErrorAct(msg);
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
    else if (msg.contains("WARNING")) { emit msgWarningAct(msg); }
    else if (msg.contains("***")) { emit msgStatusAct(msg); }
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
        emit msgResultAct(msg);
      }
    }
    else if (msg.contains("bundle")) { emit msgBundleAct(msg); }
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
        emit msgBundleAct(tr("synth [ %1 ] started").arg(QString::number(nodeID)));
      }
      else if (nodeMSG[6].toInt() == 1)
      {
        emit actGroupAdd(nodeMSG[2].toInt());
        emit msgBundleAct(tr("group [ %1 ] started").arg(QString::number(nodeID)));
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
        emit msgBundleAct(tr("synth [ %1 ] end").arg(QString::number(nodeID)));
      }
      else if (nodeMSG[6].toInt() == 1)
      {
        emit actGroupFree(nodeMSG[2].toInt());
        emit msgBundleAct(tr("group [ %1 ] end").arg(QString::number(nodeID)));
      }
    }
    else {
      //qDebug() << msg;
      if (msg.startsWith("\r\n"))	{ msg = msg.replace("\r\n", ""); }
      if (!msg.isEmpty())	{ emit msgNormalAct(msg); }
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
    if (mIpcSocket)
      // we can handle only one ipc connection at a time
      mIpcSocket->disconnect();

    mIpcSocket = mIpcServer->nextPendingConnection();

    connect(mIpcSocket, SIGNAL(disconnected()), this, SLOT(finalizeConnection()));
    connect(mIpcSocket, SIGNAL(readyRead()), this, SLOT(onIpcData()));

    stateInterpret = StateInterpret::RUNNING;
    emit interpretBootDoneAct();
  }

  void ScBridge::finalizeConnection()
  {
    mIpcData.clear();
    mIpcSocket->deleteLater();
    mIpcSocket = NULL;
    emit interpretKillDoneAct();
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

    //	emit msgStatusAct(tr("SELECTOR: %1").arg(selector));

    if (selector == serverRunningSelector)
    {
      // DATA O STAVU SERVERU - msg[0] bool STATE; msg[1] int IP; msg[2] int PORT!!!!!!!!!!!
      QStringList msg = data.split("\n");

      //bool serverRunning;
      //int ip;
      //int port;

      //emit msgStatusAct(tr("SERVER msg size: %1").arg(msg.size()));
      //emit msgStatusAct(tr("SERVER msg[0]: %1").arg(msg[0]));
      //emit msgStatusAct(tr("SERVER msg[1]: %1").arg(msg[1]));
      //emit msgStatusAct(tr("SERVER msg[2]: %1").arg(msg[2]));

      if (msg[0] == "- false")
      {
        stateServer = StateServer::OFF;
        emit serverKillDoneAct();
      }
      else if (msg[0] == "- true")
      {
        stateServer = StateServer::RUNNING;
        emit serverBootDoneAct();
      }

      //emit msgStatusAct(tr("STATUS: %1").arg(data));
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

