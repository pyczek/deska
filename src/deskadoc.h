/***************************************************************************
                          deskadoc.h  -  description
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
#ifndef DESKADOC_H
#define DESKADOC_H

// include files for QT
#include <qobject.h>
#include <qfile.h>
#include <qfileinfo.h>

#define L_NR 8

// application specific includes
#include "global.h"

/**
  * The doucment class of the application. It handles the file access and
  * manages all data for the board and the list view.
  * All data is stored here and not in the associated views (DeskaView).
  */

class DeskaDoc : public QObject
{
  Q_OBJECT

  public:
    DeskaDoc();
    ~DeskaDoc();

    bool  load(const QString &filename);
    bool  isModified() const;
    bool  fileOpened(){if(m_file) return true; else return false;}
    void  close();

    void    setCurGameNr(int nr){m_curGameNr=nr;}
    QString getPath(){return m_filename;}
    int     getCount(){return m_count;}
    gameTyp getGame(int);
    bool    getHeader(QString*, int);

    int  getNextGame();
    int  getPrevGame();
    /** sets the filename of the document */
    void setTitle(const QString &_t);
    /** returns the title of the document */
    const QString &getTitle() const;
    /** sets the path to the file connected with the document */
    void setAbsFilePath(const QString &filename);
    /** returns the pathname of the current document file*/
    const QString &getAbsFilePath() const;

    /** Append current game to open file or save as new file*/
    bool	saveGame(gameTyp*, QString fileName=0, int nr=-1);

  signals:
    void documentChanged();
    void docClosing();
    void viewNextGame();
    void viewPrevGame();
    void  updateView();

  protected:
    bool modified;

  private:

 /** Store the position of a game in a list for acceleration purposes*/ 
  bool	generateIdxList();
  QList<gamePosTyp> m_gamePosList;

  QString  m_filename;
  QFile   *m_file;	
  int      m_count;
  int      m_curGameNr;
  QString  title;
  QString  absFilePath;
};

#endif
