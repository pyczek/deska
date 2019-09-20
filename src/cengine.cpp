/***************************************************************************
                          cengine.cpp  -  description
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

#include <stdlib.h> 
 
// qt
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qcursor.h>
#include <qregexp.h>

#include	"cengine.h"
#include	"global.h"
#include	"resource.h"

CEngine::CEngine(QWidget *parent,const char *name, QString path, CBoard *board)
:QTextEdit(parent,name)
{
	m_board=board;
	m_path=path;
	m_engineOK = false;
	m_analyze = false;
	
	setReadOnly(true);

	initMenue();
	init();
}

CEngine::~CEngine()
{
	slotQuit();
}

//load a new engine
int CEngine::load()
{
	QString fileName;
	QFileDialog *f = new QFileDialog(this, "openengine", true);
	f->setCaption(tr("Choose a chess engine.."));
  f->setMode( QFileDialog::AnyFile );
  f->setViewMode( QFileDialog::Detail );

  if(f->exec() == QDialog::Accepted)
    fileName = f->selectedFile();

  if (!fileName.isEmpty())
	{
		m_path=fileName;
		return 1;
	}
	else
		return 0;
}

//parse moves annd other messages from engines
//telluser MSG
//tellusererror MSG
//askuser REPTAG MSG
void CEngine::slotAnalyzeEngineOutput()
{
  int p;
	QString s, info;
	
	info = (QString) m_proc->readStdout();	
  info = info.stripWhiteSpace();
	
	//error occured
	//Illegal move: MOVE
	//Illegal move (REASON): move
	//Error (ERRORTYPE): COMMAND
	if(info.find(tr("illegal"))!=-1||info.find(tr("error"))!=-1)
	{
		emit engineMsg(m_name + ":\n" + info + "\n");
		QMessageBox::information(this,"Deska",tr("Error message from engine."));				
		return;
	}

  //handle post output
  QString ply, score, time, nodes, pv;
	ply=score=time=nodes=pv="";
  QStringList postList(QStringList::split(" ", info));
  QStringList::Iterator it2 = postList.begin();
	s="";
	p=0;
  for (;it2!=postList.end();++it2)
	{
		switch (p)
		{
			case 0:
	      ply+=*it2;
			break;
			case 1:
	      score+=*it2;
			break;
			case 2:
	      time+=*it2;
			break;
			case 3:
	      nodes+=*it2;
			break;
			default:
	      pv+=*it2;
				pv+=" ";
			break;
		}
		p++;
  }
	//verify output is valid
	if(!ply.isEmpty()&&!score.isEmpty()&&!time.isEmpty()&&!nodes.isEmpty()&&!pv.isEmpty())
	{		
		//numbers only
		float numval;
		QRegExp num("\\d+");
		if((num.search(ply)>-1)&&(num.search(score)>-1)&&(num.search(time)>-1)&&(num.search(nodes)>-1))
		{
			//transform values
			numval =	score.toFloat(); numval/=100; score.setNum(numval);
			numval =	time.toFloat(); numval/=100; time.setNum(numval);
			s="Depth: " + ply + " Score: " + score + " Time: " + time + " Nodes: " + nodes + "\n" + pv;
			info=s;				
			//show whole output in textbox
			showInfo(info);
			return;
		}
	}
	else
		emit engineMsg(m_name + ":\n" + info + "\n");
}

//engine is ready
//if there are still a message in the buffer, send it
void CEngine::slotCheckReady()
{
	m_bufferReady = true;
	if(!m_buffer.isEmpty())writeTo("");
}

//stop mode
void CEngine::stop()
{
	showInfo(m_name + " " + tr("is stopped.") + "\n" + 
													tr("Use the right mouse button on this window."));

	if(m_analyze)writeTo("exit\n");
	m_analyze = false;	
	writeTo("force\n");
	showMode();	
}

//init and start the engine
bool CEngine::init()
{
	m_bufferReady = true;
	m_buffer="";

	// Path to engine is set
	if(m_path.isEmpty())
	{
		if(!load())return false;
 	}

	m_proc = new QProcess(m_path);

	if(m_proc->start())
	{
		connect(m_proc, SIGNAL(readyReadStdout()),
			this, SLOT(slotAnalyzeEngineOutput()));
		connect(m_proc, SIGNAL(processExited()),this, SLOT(slotQuit()));
		connect(m_proc, SIGNAL(wroteToStdin ()),this, SLOT(slotCheckReady()));

		m_name=extractFilename(m_path);	

		writeTo("xboard\n");			
		
		m_engineOK=true;			
			
		analyze();
		//stop();

		return true;
	}

	QMessageBox::information(0, "Deska",m_path + tr("Can't start engine!"));		

	return false;
}

//undo move
void CEngine::back()
{	
	writeTo("undo\n");

}

//quit engine
void CEngine::slotQuit()
{
	stop();

	writeTo("quit\n");
	showInfo(m_name + tr(" is off"));

	m_proc->tryTerminate();

	m_engineOK=false;
	
	emit engineClosed(m_path);
}

//transfer current position to engine
void CEngine::posToEngine()
{
		int *pos, piece, i, c;
		char cMove;
		pos=m_board->getPos();
		QString move;

	  writeTo("edit\n");
		writeTo("#\n");		

		// white pieces first		
		for(i=0;i<=H8;i++)
		{
			if(pos[i]==EMPTY)continue;
			c=pos[i]/abs(pos[i]);
			piece=(abs(pos[i])&PIECE_MASK)*c;
			if(piece<0)continue;
			switch(piece)
			{
				case PAWN: move="P"; break;
				case KNIGHT: move="N"; break;
				case BISHOP: move="B"; break;
				case ROOK: move="R"; break;
				case QUEEN: move="Q"; break;
				case KING: move="K"; break;
				default: continue;
			}
			cMove=i%8+97;move+=cMove;
			cMove=i/8+48+1;move+=cMove;
			move+="\n";

			writeTo(move);	
		}
	
		writeTo("c\n");

		// black pieces
		for(i=0;i<=H8;i++)
		{
			if(pos[i]==EMPTY)continue;
			c=pos[i]/abs(pos[i]);
			piece=(abs(pos[i])&PIECE_MASK)*c;
			if(piece>0)continue;
			switch(piece)
			{
				case -PAWN: move="P"; break;
				case -KNIGHT: move="N"; break;
				case -BISHOP: move="B"; break;
				case -ROOK: move="R"; break;
				case -QUEEN: move="Q"; break;
				case -KING: move="K"; break;
				default: continue;
			}

			cMove=i%8+97;move+=cMove;
			cMove=i/8+48+1;move+=cMove;
			move+="\n";

			writeTo(move);	

    }
		writeTo(".\n");		

		if(m_board->getColor()==WHITE)
			writeTo("white\n");		
		else
			writeTo("black\n");

}

//convert internal move to (a-h)(1-8)(a-h)(1-8)(nnrq)		 *
QString CEngine::moveTo(int from ,int to)
{
	QString sMove=0;
	char move;
	
  move=from%8+97;sMove+=move;
	move=from/8+48+1;sMove+=move;
	move=to%8+97;sMove+=move;
	move=to/8+48+1;sMove+=move;	
  sMove+="\n";
	
	return sMove;
}

//write string to engine
bool CEngine::writeTo(QString msg)
{	
	//save message if engine not ready	
	if(!m_bufferReady)
	{
		m_buffer+=msg;
		return false;
	}

	//engine ready for message
	msg=m_buffer+msg;
	m_bufferReady=false;
	m_buffer="";
	m_proc->writeToStdin(msg);

	return true;
}

//restart engine using the current mode
void CEngine::restart()
{		
  if(m_analyze)
	{
	  stop();		
	  posToEngine();	
		analyze();
	}
}

//setup controls depending on current mode
void CEngine::showMode()
{	
	m_menue->setItemChecked(ID_ENGINE_ANALYZE, m_analyze);		
}

//set analyzis mode
void CEngine::analyze()
{
	if(m_analyze)return;

	stop();

	writeTo("new\n");
	writeTo("force\n");
	writeTo("analyze\n");			
	posToEngine();	
	writeTo("post\n");							
	m_analyze = true;

	showInfo(m_name + "\n" + tr("Analyzing..."));

	showMode();
}

//send move to engine
void CEngine::move(int from, int to)
{
	writeTo(moveTo(from, to));
}

//popup
QPopupMenu *CEngine::createPopupMenu(const QPoint & pos)
{
	m_menue->exec(QCursor::pos());	
	return NULL;//m_menue;
}

//init popup
void CEngine::initMenue()
{
	m_menue = new QPopupMenu();
	m_menue->setCheckable(true);
	m_menue->insertItem(tr("Analyze"), ID_ENGINE_ANALYZE);
	m_menue->setItemEnabled(ID_ENGINE_ANALYZE, true);
	m_menue->insertSeparator();
	m_menue->insertItem(tr("Close"), ID_ENGINE_CLOSE);	

	connect(m_menue,SIGNAL(activated(int)),this,SLOT(slotMenueActivated(int)));
}

//menu events
void CEngine::slotMenueActivated(int item)
{
	switch(item)
	{
		case ID_ENGINE_ANALYZE:
		  if(m_analyze)
			  stop();
			else		
			  analyze();
		break;				

		case ID_ENGINE_CLOSE:			
				slotQuit();			
			
		break;

	}
}

//show engine infos in the text box
void CEngine::showInfo(QString info)
{
	setText(info);
	setCursorPosition(lines()-1, 0);
}






















































