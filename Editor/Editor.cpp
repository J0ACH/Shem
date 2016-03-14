/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>

#include "Editor.h"


CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}



int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}



void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}



void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}



void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}



void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}



void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

/*
#include "Editor.h"
#include <QApplication>

using namespace EditorIDE;
using namespace SupercolliderBridge;

int main(int argc, char** argv){

	QApplication app(argc, argv);

	Editor *win = new Editor();
	win->setGeometry(20, 100, 900, 500);
	win->show();

	return app.exec();
}

Editor::Editor()
{
	bridge = new ScBridge(this);

	this->initControlers();

	connect(buttStartLang, SIGNAL(pressed()), bridge, SLOT(startLang()));
	connect(buttKillLang, SIGNAL(pressed()), bridge, SLOT(killLang()));

	connect(buttStartServer, SIGNAL(pressed()), bridge, SLOT(startServer()));
	connect(buttKillServer, SIGNAL(pressed()), bridge, SLOT(killServer()));

	connect(bridge, SIGNAL(bootedLang(bool)), runningLang, SLOT(setVisible(bool)));
	connect(bridge, SIGNAL(bootedServer(bool)), runningServer, SLOT(setVisible(bool)));

	connect(bridge, SIGNAL(scPost(QString)), this, SLOT(consoleAddMsg(QString)));
	connect(bridge, SIGNAL(statusMessage(QString)), this, SLOT(consoleAddMsg(QString)));

	connect(buttBeep, SIGNAL(pressed()), this, SLOT(beep()));
	connect(buttCode, SIGNAL(pressed()), this, SLOT(sendCode()));
	
			
	
	//bridge->startLang();
	//bridge->startServer();
}

void Editor::initControlers()
{
	buttStartLang = new QPushButton(this);
	buttStartLang->setGeometry(10, 10, 190, 30);
	buttStartLang->setText("ScBridge.startLang()");

	buttKillLang = new QPushButton(this);
	buttKillLang->setGeometry(10, 45, 190, 30);
	buttKillLang->setText("ScBridge.killLang()");

	buttStartServer = new QPushButton(this);
	buttStartServer->setGeometry(10, 85, 190, 30);
	buttStartServer->setText("ScBridge.startServer()");

	buttKillServer = new QPushButton(this);
	buttKillServer->setGeometry(10, 120, 190, 30);
	buttKillServer->setText("ScBridge.killServer()");

	console = new QTextEdit(this);
	console->setGeometry(210, 10, 680, 320);
	console->setReadOnly(true);
	console->setOverwriteMode(false);
	console->setFont(QFont("Consolas", 8));
	console->append(tr("Console init test..."));

	runningLang = new QLabel(this);
	runningLang->setGeometry(10, 160, 190, 20);
	runningLang->setText("Lang is running...");
	runningLang->setVisible(false);

	runningServer = new QLabel(this);
	runningServer->setGeometry(10, 180, 190, 20);
	runningServer->setText("Server is running...");
	runningServer->setVisible(false);

	buttBeep = new QPushButton(this);
	buttBeep->setGeometry(10, 210, 190, 50);
	buttBeep->setText("BEEP!");

	editor = new QTextEdit(this);
	editor->setGeometry(210, 350, 680, 50);
	editor->setFont(QFont("Consolas", 8));
	editor->append(tr("Ndef('test', {SinOsc.ar(150!2, 0, Saw.kr(1))}).play;"));

	buttCode = new QPushButton(this);
	buttCode->setGeometry(10, 350, 190, 50);
	buttCode->setText("ScBridge.evaluateCode(String)");

}

void Editor::beep()
{
	bridge->evaluateCode("().play");
}

void Editor::sendCode()
{
	bridge->evaluateCode(editor->toPlainText());
}

void Editor::consoleAddMsg(QString msg)
{
	console->append(msg);
}


Editor::~Editor()
{
	
}

void Editor::closeEvent(QCloseEvent *event)
{
	//bridge->evaluateCode("Server.local.quit;"); // not working?
	//bridge->killLang();
	//close();

	//bridge->killServer();
	//bridge->killLang();
}
*/
