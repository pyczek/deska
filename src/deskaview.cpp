/***************************************************************************
                          deskaview.cpp  -  description
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

#include <stdlib.h> 
 
#include "deska.h"
#include "coptions.h"
#include "ctags.h"
#include "cfilter.h"
#include "deskaview.h"
#include <qprogressdialog.h>
#include <qregexp.h>
#include <qdialog.h>
#include <qapplication.h>
#include <qlayout.h>

#define PO 5
#define BTN_HEIGHT 20
#define C_PREV_MOVES 10

DeskaView::DeskaView(QWidget *parent, DeskaDoc *doc) : QWidget(parent)
{  
  m_engine.setAutoDelete(FALSE);

  m_doc = doc;

  m_listVisible=false;
  m_list=NULL;

  m_pvBoard = 0;
  m_mpvBoard[0]=0;
  m_timer=0;
  m_tagname = new QString[MAX_TAGS];
  m_tagval = new QString[MAX_TAGS];

  m_board = new CStdBoard(this);
  m_board->move(PO,PO);

  setupInfo();
  setupMemo();
  initList();
  initGame();

  initPopupMenue();
  setStdSize();
  
  loadActiveEngines();

  /** connect doc with the view*/
  connect(doc, SIGNAL(documentChanged()), this, SLOT(slotDocumentChanged()));
  connect(m_board, SIGNAL(userMove(int, int)),this, SLOT(slotUserMove(int, int)));
  //connect(m_board, SIGNAL(gameChanged()),this, SLOT(slotGameChanged()));

  slotUpdateBoard();
}

DeskaView::~DeskaView()
{
	saveActiveEngines();

	//update all engines
	if(!m_engine.isEmpty())
	{
		CEngine *pEngine;
		pEngine	= m_engine.first();
		
		do
		{
				m_engine.remove();
				m_memo->removePage(pEngine);
				//delete pEngine;
		}
		while((pEngine = m_engine.next()));
	}
}

void DeskaView::slotDocumentChanged()
{
  //TODO update the view

}

/*
void DeskaView::mousePressEvent( QMouseEvent* e)
{
	if(m_timer){delete m_timer;m_timer=NULL;}
}
*/

void DeskaView::slotTimer()
{
	if(!slotNextMove()){delete m_timer;m_timer=NULL;}
}


void DeskaView::setupInfo()
{
	m_info = new QTabWidget(this);

	m_info->move(m_board->y()+m_board->width()+PO, m_board->y());
	//m_info->resize((m_board->width())/3*2, m_board->height());
	m_info->setFocusPolicy(NoFocus);

	//TODO04 disconnect(m_info, SIGNAL(cancelButtonPressed()),this, SLOT(QDialog::reject()));

	setupTraining();
	setupInfoAll();
}

void DeskaView::setupTraining()
{
	m_training = new QLabel( this );

	m_training->setBackgroundMode(PaletteLight );
  m_training->setAlignment(WordBreak);
	m_training->setAutoResize(false);
	m_training->setFrameStyle( QFrame::Panel | QFrame::Sunken );
	m_info->addTab(m_training,"&Training");
}


void DeskaView::setupMemo()
{
	m_memo = new QTabWidget(this);
	m_memo->move(PO,m_board->y()+m_board->height()+PO);
	m_memo->setFocusPolicy(NoFocus);

	m_memo_info = new QTextEdit(m_memo);
	m_memo_info->setReadOnly(true);
	m_memo->addTab(m_memo_info,"&Info");

	m_memo_output = new QMultiLineEdit(m_memo);
	m_memo_output->setReadOnly(true);
	m_memo->addTab(m_memo_output,"&Output");
}

void DeskaView::setupInfoAll()
{

	m_infoAll = new CNotation(this);

	m_infoAll->setBackgroundMode(PaletteLight );
	m_infoAll->setFrameStyle( QFrame::Panel | QFrame::Sunken );

	m_info->addTab(m_infoAll,"&Full");

	//TODO04 connect(m_infoAll,SIGNAL(moveSelected(QString)),this,SLOT(slotGotoMove(QString)));
}

void DeskaView::setSize()
{    
  m_info->move(m_board->y()+m_board->width()+PO, m_board->y());
  m_info->resize(width()-m_board->width()-m_board->x()-2*PO, m_board->height());
  m_memo->move(PO,m_board->y()+m_board->height()+PO);
  m_memo->resize(width()-2*PO,height()-m_board->y()-m_board->height()-2*PO);	
  if(m_list)m_list->resize(width(), height());

  initInfoAll();
}

//set game notation, info->all(full)
void DeskaView::initInfoAll()
{
	QString s;

	// calc chars per line
	//int w=m_infoAll->width();
	//QFont f("Helvetica", 12, QFont::Normal);
	//m_infoAll->setFont(f);

	s=setGameNotation();
	s+="\n";		
	s+=getResult();

	m_infoAll->setUpdatesEnabled( FALSE );
	m_infoAll->setText(s);
	m_infoAll->setUpdatesEnabled( TRUE );
}

QString DeskaView::setGameNotation()
{
	int i=0;
  QString n,s;
	moveTyp *move;

	n="";

	//comment bevore first move
	s=m_board->getComment(i);
	{
		if(!s.isEmpty())
		{
			n+="{ ";
			n+=s;
			n+=" } ";
		}
	}

	//notation
	while((move=m_board->getMove(i)))
	{
		if(move->piece/abs(move->piece)==WHITE)
		{
			s.setNum((i+2)/2);	// 0-moveCount-1
			s+=". ";
			n+=s;
		}

		if(g_notation=="LONG")
			n+=moveToLongStr(move) + ' ';
		else
			n+=moveToShortStr(move) + ' ';

		//comment after move
		s=m_board->getComment(i+1);
		{
			if(!s.isEmpty())
			{
					n+="{ ";
					n+=s;
					n+=" } ";
			}
		}

		i++;
	}

	m_game.notation=n;//TODO m_game.notation different from infoAll
	return n;

}

QString DeskaView::moveToLongStr(moveTyp *m)
{
	QString sMove=0;
	char cMove;
	int from=m->from, to=m->to;

	//The piece is now on the dest. pos
	switch(abs(m->piece))
	{
		case KING: sMove="K";break;
		case QUEEN: sMove="Q";break;
		case ROOK: sMove="R";break;
		case BISHOP: sMove="B";break;
		case KNIGHT: sMove="N";break;
	}

  cMove=from%8+97;sMove+=cMove;
	cMove=from/8+48+1;sMove+=cMove;
	
	switch(m->flag&PIECE_MASK)
	{
		case PAWN:
		case KNIGHT:
		case BISHOP:
		case ROOK:
		case QUEEN:
			sMove+="x";
		break;
		default:
			sMove+="-";
	}
	
	cMove=to%8+97;sMove+=cMove;
	cMove=to/8+48+1;sMove+=cMove;	

	//check
/*	int c=m_board->getColor();
if(c)c=0;else c=1;
	int p=m_board->getKingPos(c);
	                if(c)c=0;else c=1;
	if(m_board->isAttacked(p,c))sMove+="+";
  */

	return sMove;
}

QString DeskaView::moveToShortStr(moveTyp *m)
{
	QString sMove=0;
	char cMove;
	int from=m->from, to=m->to;
	bool	isPawn=false;
	bool	isCapt=false;

	//The piece is now on the dest. pos
	switch(abs(m->piece)&PIECE_MASK)
	{
		case KING: sMove="K";break;
		case QUEEN: sMove="Q";break;
		case ROOK: sMove="R";break;
		case BISHOP: sMove="B";break;
		case KNIGHT: sMove="N";break;
		case PAWN: isPawn=true;
	}

	//Capture
	if((m->flag&PIECE_MASK)!=0)
	{
		isCapt=true;
		if(isPawn)
		{
			cMove=from%8+97;sMove+=cMove;
		}
		else
			sMove+="x";
	}

	cMove=to%8+97;sMove+=cMove;
	if(!isPawn||!isCapt){cMove=to/8+48+1;sMove+=cMove;}

	//check
/*	int c=m_board->getColor();
if(c)c=0;else c=1;
	int p=m_board->getKingPos(c);
	                if(c)c=0;else c=1;
	if(m_board->isAttacked(p,c))sMove+="+";
  */

	return sMove;
}

void DeskaView::resizeEvent( QResizeEvent* )
{
  setSize();
}

void DeskaView::initPopupMenue()
{
	//popup menue board
	m_boardMenue = new QPopupMenu();
	m_boardMenue->insertItem("Flip", ID_BOARD_FLIP);
	m_boardMenue->insertItem("Board style...", ID_BOARD_OPTIONS);
	m_boardMenue->insertItem("To BMP...", ID_BOARD_BMP);

	connect(m_board,SIGNAL(showPopup()),this,SLOT(slotBoardPopup()));
	connect(m_boardMenue,SIGNAL(activated(int)),this,SLOT(slotBoardPopupActivated(int)));
}

void DeskaView::slotBoardPopup()
{
	m_boardMenue->exec(QCursor::pos());
}

void DeskaView::slotShowPrevGame()
{
	gameTyp g;				
	QString nr;					
	QListViewItem *i;

	i=m_list->currentItem(); if(!i)return;
	i=i->itemAbove();	if(!i)return;

	m_list->setCurrentItem(i);					
	nr=m_list->currentItem()->text(0);				
	g = m_doc->getGame(nr.toInt()-1);
	initGame(&g);		
}

void DeskaView::slotShowNextGame()
{
	gameTyp g;				
	QString nr;					
	QListViewItem *i;

	i=m_list->currentItem(); if(!i)return;
	i=i->itemBelow();if(!i)return;

	m_list->setCurrentItem(i);					
	nr=m_list->currentItem()->text(0);				
	g = m_doc->getGame(nr.toInt()-1);
	initGame(&g);		
}

void DeskaView::slotShowCurGame()
{
	if(!m_list->hasFocus()){slotPlay();return;}

	gameTyp g;
	QString nr;	
	nr=m_list->currentItem()->text(0);
	g = m_doc->getGame(nr.toInt()-1);
	initGame(&g);		
	toggleList();
}

void DeskaView::slotShowList()
{
	if(!m_list)return;

	m_list->show();
	m_list->setFocus();
	m_listVisible = true;

}

void DeskaView::slotBoardPopupActivated(int item)
{
  switch(item)
  {
  case ID_BOARD_BMP:
  {
      QPixmap *pm=new QPixmap(m_board->width(),m_board->height());		
      m_board->redrawBoard(0, pm);

       QString fileName = QFileDialog::getSaveFileName(
                    getenv( "HOME" ),"Images (*.bmp)",//"HOME" ),"Images (*.bmp *.png *.xpm *.jpg)"
                    this, tr("Save board as image..."),
                    tr("Choose a file"));

      if (!fileName.isEmpty())pm->save(fileName, "BMP");		
  }
  break;

  case ID_BOARD_OPTIONS:
  {
      COptions *opt=new COptions(0);
      connect(opt, SIGNAL(optionsChanged()), this, SLOT(slotUpdateBoard()));
      opt->show();
  }
  break;

  case ID_BOARD_FLIP:
  {
    slotFlipBoard();
  }


  }
}

void DeskaView::slotFlipBoard()
{
	m_board->flipBoard();
	m_board->repaint();
}

//goto beginning of the current game
void DeskaView::slotGotoStart()
{
	m_board->gotoStart();
	m_board->repaint();
	initTraining();

	//update all engines
	if(!m_engine.isEmpty())
	{
		CEngine *pEngine;
		pEngine	= m_engine.first();

		do
		{
			pEngine->restart();
		}
		while((pEngine = m_engine.next()));
	}

}

void DeskaView::initGame(gameTyp *game)
{	
	
	if(game)m_game=*game;
	else
	{
		gameTyp g;		
		g.header="[White \"N.N.\"]\n[Black \"N.N.\"]\n[Date \"??.??.????\"]\n[Site \"?\"]\n[Event \"?\"]\n[Round \"?\"]\n[Result \"*\"]\n";		
		m_game=g;
	}

	m_board->analyzePGN(m_game.notation, extractTag(m_game.header,"FEN"));
	m_board->repaint();
	
	m_memo_info->setText(getMainHeader());
  initInfoAll();
	initTraining(true);

	if(!m_engine.isEmpty())
	{
			CEngine *pEngine;
			pEngine	= m_engine.first();
			
			do{pEngine->restart();}while((pEngine = m_engine.next()));
	}
}

QString DeskaView::getMainHeader()
{
	return 	extractTag(m_game.header,"White")+" vs "+
					extractTag(m_game.header,"Black")+"\n"+
					extractTag(m_game.header,"Event")+"-"+
					extractTag(m_game.header,"Site")+", "+	
					extractTag(m_game.header,"Date")+"\n"+	
					extractTag(m_game.header,"Result");
}

void DeskaView::initTraining(bool initial)
{
	QString s, c;
	moveTyp *move;
	int nr;

	nr=m_board->getCurListNO();
	move=m_board->getMove(nr-1);

	if(move&&!initial)	// legal move and no new game
	{
		s.setNum((int)(nr+1)/2);	// 0-moveCount-1
		s+=".";
		if(m_board->getColor()==WHITE)s+="..";
		if(g_notation=="LONG")
			s+=moveToLongStr(move);
		else
			s+=moveToShortStr(move);
		s+="\n\n";
	}

	c=m_board->getComment(nr);
	s+=c;

	m_training->setBackgroundColor( white );

	if(s.isEmpty())	// no comment before game
	{
		if(m_board->getColor()==WHITE)
			s="White to move";
		else
		  s="Black to move";
	}
	m_training->setText(s);
}

bool DeskaView::slotNextMove()
{
	if(!m_board->nextMove())
	{
		m_training->setBackgroundColor( white );
		m_training->setText(getResult());
		return false;
	}
	else
	{
		initTraining();

		moveTyp *move=m_board->getMove(m_board->getCurListNO()-1);

		if(!m_engine.isEmpty())
		{
			CEngine *pEngine;
			pEngine	= m_engine.first();
			
			do
			{
				pEngine->move(move->from, move->to);
			}
			while((pEngine = m_engine.next()));
		}
	}

	return true;
}

void DeskaView::slotPrevMove()
{
	m_board->prevMove();

		if(!m_engine.isEmpty())
		{
			CEngine *pEngine;
			pEngine	= m_engine.first();

			do{pEngine->back();}while((pEngine = m_engine.next()));
		}


	initTraining();
}

void DeskaView::slotPlay()
{
	if(m_timer){delete m_timer;m_timer=NULL;}
	else
	{
		m_timer = new QTimer( this );
		connect( m_timer, SIGNAL(timeout()),this, SLOT(slotTimer()));
		m_timer->start( 500);
	}
}

void DeskaView::toggleList()
{
	if(!m_list)return;
  
	if(m_listVisible)
	{
		m_list->hide();
		m_board->setFocus();
		m_listVisible=false;
	}
	else
	{    
		m_list->show();
		m_list->setFocus();
		m_listVisible = true;
	}		
}

void DeskaView::setupList()
{
  m_list = new CGameList(this, "gameList");
  m_list->hide();  
  
  m_list->addColumn("No.");
  for(int i=0;i<MAX_TAGS;i++)  
    m_list->addColumn(g_tags[i]);    

  connect(m_list,SIGNAL(doubleClicked ( QListViewItem * )),this,SLOT(slotShowCurGame()));        

  m_list->resize(height(), width());

  m_list->setAllColumnsShowFocus(true);
}

//load all game headers into the list
void DeskaView::initList()
{	
	int gameNr, k;
	QString s, no;
	QString vals[MAX_TAGS];

	gameNr=1;
	deleteList();
	setupList();

	for(k=0;k<MAX_TAGS;k++)
		m_tagname[k]=m_list->columnText(k+1);				
	for(k=0;k<MAX_TAGS;k++)
		m_tagval[k]=extractTag(m_crit, m_tagname[k]);

	QProgressDialog progress( "Searching...", "Abort", m_doc->getCount(), this,"progress", true);
	progress.setProgress( 0 );			

  m_list->resize(width(), height());
  
	for(;;)
	{			
		qApp->processEvents();

		if(!(m_doc->getHeader(&s, gameNr-1)))break;

 		no=no.setNum(gameNr);
		no=no.rightJustify(5, '0');
						
		gameNr++;
  	progress.setProgress(gameNr);
		if(progress.wasCancelled())break;			
		if(!matchKrit(&s))continue;

		for(k=0;k<MAX_TAGS;k++)vals[k]= extractTag(s, m_tagname[k]);

		QListViewItem *item = new QListViewItem(m_list);
		item->setText(0, no);				
		for(k=0;k<MAX_TAGS;k++)item->setText(k+1,vals[k]);				
  }

	progress.setProgress(m_doc->getCount());

}

bool DeskaView::matchKrit(QString *s)
{
	QString t, tag,find;

	for(int i=0;i<MAX_TAGS;i++)
	{		

		tag=m_tagname[i];
		find=m_tagval[i];
	
		if(!find.isEmpty())
		{	
			t="\\["+ tag +" *\""   + find +	".*\".*\\].*";
			if(s->find(QRegExp(t),0)==-1)return false;
		}
  }

	return true;
}

QString DeskaView::loadEngine(QString name)
{
		CEngine *engine=new CEngine(m_memo, 0, name, m_board);
		if(engine->getName()==""){delete engine; return "";}
		m_memo->addTab(engine,"&" + engine->getName());
		m_memo->showPage(engine);	
		m_engine.append(engine);

		connect(engine, SIGNAL(engineMsg(QString)),this, SLOT(slotAddLineToOutput(QString)));
		connect(engine, SIGNAL(engineClosed(QString)),this, SLOT(slotRemoveEngine(QString)));
		
		return engine->getPath();
}

void DeskaView::saveActiveEngines()
{
	if(m_engine.first())
	{
		g_activeEngine.clear();
		do
		{
			g_activeEngine.append((m_engine.current())->getPath());
		}
		while(m_engine.next());
	}  
}

void DeskaView::loadActiveEngines()
{
	if(g_activeEngine.first())
	{		
		do
		{
	  	loadEngine(g_activeEngine.current());
		}
		while(g_activeEngine.next());
	}
}

void DeskaView::slotUpdateBoard()
{
  //resizes the board, depending on the piece size
  if(m_board->loadRsc()!=NULL)m_board->setPieces();  
  //resize the main window and the other objects relative to the board  
  resize(m_board->width()*1.3, m_board->height()*1.2);  
  //set size of other elements depending on the main window size
  setSize();    
  
  setMinimumSize(m_board->width() + 2*PO, m_board->height() + 2*PO);  
  
  m_board->repaint();    
  m_info->repaint();
  m_memo->repaint();
    
}

void DeskaView::moreBoards(int c)
{
	if(!m_listVisible)return;
	
	static QString oldNr="-1";

	if(!m_list->currentItem())return;
	//int nr=m_list->getNr(m_list->currentRow());
	QString nr=m_list->currentItem()->text(0);				

	//if(oldNr==nr&&m_pvBoard){m_pvBoard->nextMove();return;}

	toggleMoreBoards(true);
	oldNr=nr;	

	gameTyp g;
	g=m_doc->getGame(nr.toInt()-1);
	
	//CBoard *pvBoard;

	int k, mnr=0, x=0, y=0, w, h;

	m_mpvBoard[0]=new CMiniBoard(m_list, "miniBoard");
	m_mpvBoard[0]->analyzePGN(g.notation);
	m_mpvBoard[0]->move(x,y);
	k=0;
	while(k<C_PREV_MOVES+mnr&&m_mpvBoard[0]->nextMove()){k++;}				
	mnr=k;
 	m_mpvBoard[0]->show();

	h=m_mpvBoard[0]->height();
	w=m_mpvBoard[0]->width();
	x=w;

	for(int i=1;i<c;i++)
	{
		m_mpvBoard[i]=new CMiniBoard(m_list, "miniBoard");
		//m_mpvBoard[i]->setList(m_mpvBoard[0]->getList());
		m_mpvBoard[i]->analyzePGN(g.notation);

		k=0;
		while(k<C_PREV_MOVES+mnr&&m_mpvBoard[i]->nextMove()){k++;}				
		if(x+w>m_list->width()-w/2)
		{	
			x=0;
			y+=h;
		}		
		m_mpvBoard[i]->move(x,y);
  	m_mpvBoard[i]->show();
    //m_mpvBoard[i]->update();
		x+=w;
		mnr+=k;
	}
}


void DeskaView::prevBoard()
{
	if(!m_listVisible)return;

	static QString oldNr="-1";

	//int nr=m_list->getNr(m_list->currentRow());
	if(!m_list->currentItem())return;
	QString nr=m_list->currentItem()->text(0);	
	
	if(oldNr==nr&&m_pvBoard){m_pvBoard->nextMove();return;}

	togglePrevBoard(true);
	oldNr=nr;	

	gameTyp g;
	//g=m_doc->getGame(nr-1);
	g=m_doc->getGame(nr.toInt()-1);
		
	m_pvBoard=new CMiniBoard(m_list, "miniBoard");
	m_pvBoard->analyzePGN(g.notation, extractTag(g.header, "FEN"));
	int i=0;while(i<C_PREV_MOVES&&m_pvBoard->nextMove()){i++;}
	m_pvBoard->move(0,0);
  m_pvBoard->show();

}

void DeskaView::toggleMoreBoards(bool hideOnly)
{
	if(!m_listVisible)return;

	if(m_mpvBoard[0])
	{
		for(int i=0;i<C_BOARDS;i++)
			m_mpvBoard[i]->hide();

		delete[] m_pvBoard;
		m_mpvBoard[0]=0;
	}
	else
		if(!hideOnly)
		{		
			prevBoard();
			//m_menue->setItemChecked(ID_LIST_PREV, true);
		}
}


void DeskaView::togglePrevBoard(bool hideOnly)
{
	if(!m_listVisible)return;

	if(m_pvBoard)
	{
		m_pvBoard->hide();
		delete m_pvBoard;
		//m_menue->setItemChecked(ID_LIST_PREV, false);
		m_pvBoard=0;
	}
	else
		if(!hideOnly)moreBoards(C_BOARDS);
}

void DeskaView::slotRemoveEngine(QString path)
{
	//remove the engine with the given path engine 
	if(!m_engine.isEmpty())
	{
		CEngine *pEngine;
		pEngine	= m_engine.first();

		do
		{
			if(pEngine->getPath() == path)
			{
				m_engine.remove();
				m_memo->removePage(pEngine);			  
				//delete pEngine;
				break;
			}
		}
		while((pEngine = m_engine.next()));
	}
}

void DeskaView::slotAddLineToOutput(QString msg)
{
	m_memo_output->append(msg);
}

void DeskaView::slotEngineMove()
{
	initTraining();
	initInfoAll();
}


void DeskaView::slotUserMove(int from, int to)
{
	initTraining();
	initInfoAll();
	
	m_game.notation = setGameNotation();
	
	if(!m_engine.isEmpty())
	{
		CEngine *pEngine;
		pEngine	= m_engine.first();
		
		do{pEngine->move(from, to);}while((pEngine = m_engine.next()));
	}

}

bool DeskaView::slotEditTags()
{
	CTags *c=new CTags(&(m_game.header), this, "edittags", true);
	c->setCaption(tr("Edit tags..."));
	c->exec();
	
	if(c->result()==QDialog::Accepted)
	{
		setCaption(getMainHeader());
		m_memo_info->setText(getMainHeader());
		return true;
	}


	return false;	
}

bool DeskaView::slotSetupFilter()
{
  int i;
  static QString header;

  for(i=0;i<MAX_TAGS;i++)
  {
    header += "[" + m_tagname[i] + " \"" + m_tagval[i] + "\"]\n";
  }

  CFilter *c=new CFilter(&header, this);  
  c->exec();

  if(c->result()==QDialog::Accepted)
  {
    m_crit=header;
    initList();
    slotShowList();

    return true;
  }

  return false;	
}

//edit move comment
void DeskaView::slotEditComment()
{
  const int SPACE = 4, BUTTON = 40;

  //dialog
  QDialog *commentDlg = new QDialog(this, "commentDlg", true);
  commentDlg->resize( m_board->width(), m_board->height() / 2 );
  commentDlg->setMaximumSize(commentDlg->size());
  commentDlg->setMinimumSize(commentDlg->size());
  commentDlg->setCaption( trUtf8( "Deska" ) );
  commentDlg->setSizeGripEnabled( TRUE );
  
  //layout
  QWidget* privateLayoutWidget = new QWidget( commentDlg, "Layout1" );
  privateLayoutWidget->setGeometry(QRect( SPACE, commentDlg->height() - BUTTON - 2 * SPACE, 
                                                 commentDlg->width() - 2 * SPACE, BUTTON + 2 * SPACE));
  QHBoxLayout *Layout1 = new QHBoxLayout( privateLayoutWidget, 0, 6, "Layout1");
  QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
  Layout1->addItem( spacer );

  //ok-button
  QPushButton *buttonOk = new QPushButton( privateLayoutWidget, "buttonOk" );
  buttonOk->setText( trUtf8( "OK" ) );
  buttonOk->setAccel( 0 );
  buttonOk->setAutoDefault( TRUE );
  buttonOk->setDefault( TRUE );
  Layout1->addWidget( buttonOk );

  //cancel-button
  QPushButton *buttonCancel = new QPushButton( privateLayoutWidget, "buttonCancel" );
  buttonCancel->setText( trUtf8("Cancel"));
  buttonCancel->setAccel( 0 );
  buttonCancel->setAutoDefault( TRUE );
  Layout1->addWidget( buttonCancel );  

  //text
  QTextEdit *TextEdit1 = new QTextEdit( commentDlg, "TextEdit1" );
  TextEdit1->setGeometry( QRect( SPACE, SPACE, commentDlg->width() - 2 * SPACE, 
                            commentDlg->height() - BUTTON -2 * SPACE) );    
  
	QString oldComment = *(m_board->getCurComment());
	//if there is no comment but engines are analyzing get the engine output
	if(oldComment.isEmpty())
	{
		if(!m_engine.isEmpty())
		{
			CEngine *pEngine;
			pEngine	= m_engine.first();
			
			do
			{
				if(pEngine->isAnalyzing())
				{
					oldComment += pEngine->getName();
					oldComment += "\n";
					oldComment += pEngine->getOutput();
					oldComment += "\n";
				}
			}
			while((pEngine = m_engine.next()));
		}
	}
	TextEdit1->setText(oldComment);

  // signals and slots connections
  connect( buttonOk, SIGNAL( clicked() ), commentDlg, SLOT(accept()));
	connect( buttonCancel, SIGNAL( clicked() ), commentDlg, SLOT(reject()));

	TextEdit1->setFocus();
	commentDlg->exec();

	if(commentDlg->result()==QDialog::Accepted)
	{
		QString newComment(TextEdit1->text());		
		m_board->setCurComment(newComment);

		if(oldComment!=newComment)
		{
			m_game.notation = setGameNotation();
			initTraining();
			initInfoAll();

			emit gameChanged();
		}
	}

	delete commentDlg;
}

//shows first game from list
void DeskaView::slotShowFirstGame()
{
	gameTyp g;
  g = m_doc->getGame(0);
  initGame(&g);
}
