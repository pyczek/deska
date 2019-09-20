/***************************************************************************
                          deskadoc.cpp  -  description
                             -------------------
    begin                : Tue Dec 25 10:22:58 CET 2001
    copyright            : (C) 2001 by Andreas Jan Pyka
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

#include <qtextstream.h>
#include <qstring.h>
#include <qprogressdialog.h>
#include <qmessagebox.h>
#include <qfileinfo.h>

//#include "deskaview.h"
#include "deskadoc.h"

DeskaDoc::DeskaDoc()
{
  modified = false;

  m_filename = "";
  m_count=0;
  m_file=0;	
}

DeskaDoc::~DeskaDoc()
{
  close();
}

bool DeskaDoc::isModified() const
{
  return modified;
}

void DeskaDoc::close()
{
  emit docClosing();

  if(m_file)m_file->close();	
  m_file=0;
  delete m_file;	
}

bool DeskaDoc::saveGame(gameTyp* game, QString fileName, int nr)
{
  QString s, nt;
  QFile *l_file;	
  gamePosTyp gamePos;

  //if no path save to actual file
  //if no current file then error
  if(fileName==NULL)
  {
    if(m_file)
      fileName=m_filename;
    else
    {
      QMessageBox::information(0, "Deska", tr("No file opened."));
      return false;
    }
  }

  //will be destroyed closed at the end of this function
  l_file=new QFile(fileName);

  //if file does not exist
  //it will be created
  //if file exists and m_file not set than warning
  if(l_file->exists()&&!m_file)
  {
    if(QMessageBox::information(0, QString("Deska"),
                                    tr("File exists, use anyway?"),
                                    QMessageBox::Yes, QMessageBox::No)!=QMessageBox::Yes)
    return false;
  }

  //readonly
  if(!l_file->open(IO_Append | IO_WriteOnly))
  {
    {QMessageBox::information(0, fileName, tr("File is read only."));return false;}
  }

	QTextStream t(l_file );

	//pos of header	
	gamePos.header = l_file->at();

	// header, pos of game
	t << "\n";
	t << game->header;
	t << "\n";	
	gamePos.notation = l_file->at();	

	// game notation
	int k=70, i=0;
	nt=game->notation;
	for(;;)
	{	
		k=nt.find(' ',k+1);				
		if(k==-1)
		{
			s=nt.mid(i, nt.length()-i);
			s=s.simplifyWhiteSpace();
			break;
		}
		else
		{
      s=nt.mid(i, k-i);
			s=s.simplifyWhiteSpace();
			t << s + "\n";
			i=k;
			k+=70;
		}
	}
 // t << s + "\n";
	t << extractTag(game->header,"Result") + "\n";

	// end of game index	
	gamePos.nextgame = l_file->at();

	l_file->close();
	
	emit documentChanged();
	
	return true;
}

/*---------------------------------------------------------*
 *		read game from file, nr=game number -1               *
 *---------------------------------------------------------*/
gameTyp DeskaDoc::getGame(int nr)
{
	QString s,n;
	gameTyp g;
	gamePosTyp* gamePos;

	g.header="";
	g.notation="";	

	if(!m_file)return g;

	long posH,posG,posE,curPos;

	QTextStream t( m_file );
	
	gamePos = m_gamePosList.at(nr);
	posH = gamePos->header;
	posG = gamePos->notation;
	posE = gamePos->nextgame;
		
	m_file->at(posH);
	curPos=0;
	s="";
	while(( curPos<posG )&&(!t.eof()))
	{		
		s+=t.readLine();
		s+="\n";
		curPos=m_file->at();
	}					
	g.header=s;
	
	s="";
	curPos=0;

	while(( curPos<posE )&&(!t.eof()))
	{		
		s+=t.readLine();
    s+=' ';
		curPos=m_file->at();
	}					
	g.notation=s;

	return g;
}

/*---------------------------------------------------------*
 *	read header from file, nr=header number - 1
 *---------------------------------------------------------*/		
bool DeskaDoc::getHeader(QString *tags, int i)
{  	
	if(!m_file)return false;
	if((i<0)||(i>=m_gamePosList.count()))return false;	

	gamePosTyp* gamePos;	
	QString s=0, n;
	long posH,posG,curPos;	
	QTextStream t( m_file );		
		
	gamePos = m_gamePosList.at(i);
	posH = gamePos->header;
	posG = gamePos->notation;	
		
	if(!m_file->at(posH))return false;
	curPos=0;
	while(( curPos<posG )&&(!t.eof()))
	{		
		s+=t.readLine();
    curPos=m_file->at();
	}	
				
	*tags=s;

	if(s.isEmpty())return false;

	return true;	
}

void DeskaDoc::setTitle(const QString &_t)
{
  title=_t;
}

const QString &DeskaDoc::getTitle() const
{
  return title;
}

bool DeskaDoc::generateIdxList()
{	
	QString s;
	int c, pos;
	bool newGame=false, canWrite=false, gameFound=false;
	gamePosTyp* gamePos;

	c= m_file->size();

	QTextStream t( m_file );
	
  m_gamePosList.clear();
	QProgressDialog progress(tr("Working..."),tr("Abort"),c,0,tr("progress"), true);
	progress.setProgress(0);
	
	while ( !t.eof() )
	{
		pos=m_file->at();		// position of current line
		s = t.readLine();

		if(progress.wasCancelled())break;
		progress.setProgress(m_file->at());	
	
		if(s.left(1)=="[")	// tag found		
		{
			if(canWrite)
			{
				gamePos->nextgame = pos;
				m_gamePosList.append(gamePos);				
				canWrite=false;
				newGame=false;
			}

			if(!newGame)			// first tag ?
			{
				gamePos = new(gamePosTyp);			
				newGame=true;
				canWrite=false;
				gamePos->header = pos;
			}
		}
		else                // no tag
		{					
			if(!canWrite&&newGame)
			{			
			  gamePos->notation = pos;
        canWrite=true;				
				gameFound=true;
			}
		}
  }

	// last game
	if(gameFound)
	{
		gamePos->nextgame = pos;
		m_gamePosList.append(gamePos);						

	}

	m_file->at(0);				
  progress.setProgress(c);
	return true;
}


bool DeskaDoc::load(const QString &filename)
{
	if(m_file)close();

	m_filename = filename;

	if(!m_file)
	{		
		m_file=new QFile(m_filename);
	}

	m_file->open(IO_ReadWrite);
	if(!m_file->exists())
	{
		QMessageBox::information(0,m_filename, tr("File not found."));
		return false;
	}
		
	generateIdxList();

	m_count=m_gamePosList.count();

  emit updateView();

  emit documentChanged();

  return true;

}

int DeskaDoc::getNextGame()
{
	if(!m_file)
	{		
		QMessageBox::information(0,"Deska",tr("No file!"));
		return m_curGameNr;
	}
	emit viewNextGame();		
	return m_curGameNr;
}

int DeskaDoc::getPrevGame()
{
	if(!m_file)
	{
		QMessageBox::information(0,"Deska",tr("No file!"));
		return m_curGameNr;
	}
	emit viewPrevGame();
	return m_curGameNr;
}


