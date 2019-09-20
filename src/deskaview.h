/***************************************************************************
                          deskaview.h  -  description
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

#ifndef DESKAVIEW_H
#define DESKAVIEW_H

// include files for QT
#include <qwidget.h>
#include <qmultilinedit.h>
#include <qlabel.h>
#include <qtabwidget.h>
#include <qpopupmenu.h>
#include <qlistview.h>

// application specific includes
#include "deskadoc.h"
#include "cstdboard.h"
#include "cminiboard.h"
#include "cnotation.h"
#include "resource.h"
#include "cengine.h"
#include "cgamelist.h"

#define C_BOARDS 8

/**
 *  This class provides a base view for the application.
 *  The game list and the board are two differnet views for the doucment DeskaDoc.
 *  The board shows the current game the game list shows all headers.
 */

class DeskaView : public QWidget
{
  Q_OBJECT
  public:
    DeskaView(QWidget *parent=0, DeskaDoc* doc=0);
    ~DeskaView();

    /** Toggles between the game list and the board. */
    void toggleList();
    /** Initializes the structures for a game. */
    void initGame(gameTyp* game=0);
    /** Loads all game headers matching the filter criteria into the list. */
    void initList();
    /** Loads an engine. */
    QString loadEngine(QString name);
    void toggleMoreBoards(bool hideOnly=false);
    void togglePrevBoard(bool hideOnly=false);
    gameTyp *getGame(){return &m_game;};
    bool listVisible() {return m_listVisible;}; 
  protected:
    void	resizeEvent( QResizeEvent*);			
    //void	mousePressEvent( QMouseEvent*);

  protected slots:
    void slotDocumentChanged();

  private:
    DeskaDoc *m_doc;  
    //controls
    gameTyp	m_game;
		CStdBoard	*m_board;//board
		QTabWidget 	*m_info, *m_memo;//info, memo
		QLabel	*m_training;//training
		CNotation		*m_infoAll;//notation
		QTextEdit	*m_memo_output, *m_memo_info;//memo
		QPopupMenu *m_boardMenue;//popup

		QList<CEngine> m_engine;
		QTimer	*m_timer;        

		QString getResult(){return extractTag(m_game.header,"Result");}
    void	setStdSize(){setGeometry(g_xpos, g_ypos, g_width, g_height);}
    //resize(m_board->width()*1.5, m_board->height()*1.2);}

		//setup view
		void initInfoAll();
		void initTraining(bool=false);
		void setupInfo();
		void setupInfoAll();
		void setupTraining();
		void setupMemo();
    /** Set size of info and memo windows relative to the board size. */
	  void setSize();
		void saveActiveEngines();
		void loadActiveEngines();

		//list
		CGameList *m_list;	
		bool m_listVisible;
		QString *m_tagname, *m_tagval, m_crit;
		void 	deleteList(){if(m_list){delete m_list; m_list=0;}}
		void setupList();
		QString getMainHeader();
    /** Checks whether a header matches the 
        current filter criteria. */    
		bool matchKrit(QString*);
		//preview
		CMiniBoard	*m_pvBoard;
		CMiniBoard	*m_mpvBoard[C_BOARDS];
		void prevBoard();
		void moreBoards(int);

		QString	setGameNotation();
		QString moveToLongStr(moveTyp*);
		QString moveToShortStr(moveTyp*);

		void initPopupMenue();

   
	private slots:
		void	slotBoardPopupActivated(int);
		void	slotBoardPopup();			
		void 	slotTimer();
		void	slotUserMove(int from, int to);
		void	slotEngineMove();
		void	slotAddLineToOutput(QString);
		void	slotRemoveEngine(QString);		

	public slots:
		bool 	slotSetupFilter();
		void	slotEditComment();
		bool	slotEditTags();
		void	slotGotoStart();
		void 	slotFlipBoard();
		bool	slotNextMove();
		void	slotPrevMove();
		void	slotPlay();
		void	slotShowList();
		void	slotShowNextGame();
		void	slotShowPrevGame();
		void	slotShowCurGame();
		void 	slotUpdateBoard();
    void  slotShowFirstGame();
	signals:
		void gameChanged();
};



#endif
