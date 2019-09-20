/***************************************************************************
                          cengine.h  -  description
                             -------------------
    begin                : Thu Oct 12 2000
    copyright            : (C) 2000 by Andreas Jan Pyka
    email                : andreas.pyka@matikon.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef CENGINE_H
#define CENGINE_H

// qt
#include	<qstring.h>
#include	<qprocess.h>
#include	<qpopupmenu.h>
#include	<qtextedit.h>
#include	<qlcdnumber.h>
#include	<qtimer.h>

#include "cboard.h"

#define L_MSG 512

/**
  * Handles the comunication between chess engines and deska.
  * CEngine is derived from QTextEdit, so the engine output can be shown
  * in a textbox (e.g. here below the chess board). All output from an engine
  * is parsed in slotAnalyzeEngineOutput.
  */

class CEngine: public QTextEdit
{
   Q_OBJECT
public:
	CEngine(QWidget *parent=0, const char *name=0, QString path=0, CBoard* board=0);
	~CEngine();
	void 	move(int, int);
	QString getName(){return m_name;}
	QString getOutput(){return this->text();}
	QString getPath(){return m_path;}
	bool isAnalyzing(){return m_analyze;}
	void restart();
	void back();
	void analyze();
	int	 load();
protected:	
	QPopupMenu *createPopupMenu(const QPoint & pos);

private:	
	void showInfo(QString info);
	bool init();
	void posToEngine();	
	QString readFrom();		
	bool writeTo(QString);
	bool moveFrom(QString);
	QString moveTo(int,int);
	void	stop();	
	void	showMode();	
	
private slots:
	void 	slotQuit();
	void 	slotAnalyzeEngineOutput();
	void 	slotMenueActivated(int item);
	void	slotCheckReady();	

private:
  bool m_analyze;
	CBoard *m_board;
	QProcess *m_proc;	
	QString	m_path;
	QString m_name;
	QString m_buffer;
	QPopupMenu *m_menue;
	bool m_bufferReady;
	bool m_engineOK;
	
	void initMenue();
		
signals:
	void	engineMsg(QString);
	void	engineClosed(QString);
};

#endif







































