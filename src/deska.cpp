/***************************************************************************
                          deska.cpp  -  description
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qaccel.h>
#include <qclipboard.h>
#include <stdlib.h>

#include "deska.h"
#include "coptions.h"
#include "../rsc/icons/gamesave.xpm"
#include "../rsc/icons/fileopen.xpm"
#include "../rsc/icons/gamenew.xpm"
#include "../rsc/icons/left.xpm"
#include "../rsc/icons/right.xpm"
#include "../rsc/icons/first.xpm"
#include "../rsc/icons/play.xpm"

DeskaApp::DeskaApp()
{
  setCaption(tr("Deska " VERSION));
	setStdSettings();
	loadSettings();	


  ///////////////////////////////////////////////////////////////////
  // call inits to invoke all other construction parts
  initActions();
  initMenuBar();
  initToolBar();
  initStatusBar();

	//g_engine.setAutoDelete(true);
	//g_fileName.setAutoDelete(true);

	//engines
  engineMenu->clear();
  for ( int i=0 ; i < (int) g_recentEngine.count(); i++)
  {
    engineMenu->insertItem(g_recentEngine.at(i));
  }
	//files
  for ( int i=0 ; i < (int) g_fileName.count(); i++)
  {
    recentFilesMenu->insertItem(g_fileName.at(i));
  }

  initDoc();
  initView();

	move(g_xpos, g_ypos);
	resize(g_width, g_height);

  viewToolBar->setOn(true);
  viewStatusBar->setOn(true);
}

DeskaApp::~DeskaApp()
{
}

/** initializes all QActions of the application */
void DeskaApp::initActions(){

  QPixmap openIcon, saveIcon, newIcon,
					leftIcon, rightIcon, firstIcon, playIcon;
  newIcon = QPixmap((const char**)gamenew_xpm);
  openIcon = QPixmap((const char**)fileopen_xpm);
  saveIcon = QPixmap((const char**)gamesave_xpm);
	leftIcon = QPixmap((const char**)left_xpm);
	rightIcon = QPixmap((const char**)right_xpm);
	firstIcon = QPixmap((const char**)first_xpm);
	playIcon = QPixmap((const char**)play_xpm);


  fileNew = new QAction(tr("New game"), newIcon, tr("&New game"), QAccel::stringToKey(tr("Ctrl+N")), this);
  fileNew->setStatusTip(tr("Opens a new empty board"));
  fileNew->setWhatsThis(tr("New game\n\nOpens a new empty board"));
  connect(fileNew, SIGNAL(activated()), this, SLOT(slotFileNew()));

  fileOpen = new QAction(tr("Open file"), openIcon, tr("&Open..."), 0, this);
  fileOpen->setStatusTip(tr("Opens an existing document"));
  fileOpen->setWhatsThis(tr("Open file\n\nOpens an existing document"));
  connect(fileOpen, SIGNAL(activated()), this, SLOT(slotFileOpen()));

  fileSaveGame = new QAction(tr("Save game"), saveIcon, tr("&Save game"), QAccel::stringToKey(tr("Ctrl+S")), this);
  fileSaveGame->setStatusTip(tr("Appends actual game to the current file"));
  fileSaveGame->setWhatsThis(tr("Save game\n\nAppends actual game to current file"));
  connect(fileSaveGame, SIGNAL(activated()), this, SLOT(slotFileSaveGame()));

  fileSaveGameAs = new QAction(tr("Save game as"), saveIcon, tr("&Save game as"),0 , this);
  fileSaveGameAs->setStatusTip(tr("Appends actual game to a new file"));
  fileSaveGameAs->setWhatsThis(tr("Save game as\n\nAppends actual game to a new file"));
  connect(fileSaveGameAs, SIGNAL(activated()), this, SLOT(slotFileSaveGameAs()));

  fileClose = new QAction(tr("Close File"), tr("&Close"), QAccel::stringToKey(tr("Ctrl+W")), this);
  fileClose->setStatusTip(tr("Closes the actual document"));
  fileClose->setWhatsThis(tr("Close File\n\nCloses the actual document"));
  connect(fileClose, SIGNAL(activated()), this, SLOT(slotFileClose()));

  filePrint = new QAction(tr("Print File"), tr("&Print"), QAccel::stringToKey(tr("Ctrl+P")), this);
  filePrint->setStatusTip(tr("Prints out the actual document"));
  filePrint->setWhatsThis(tr("Print File\n\nPrints out the actual document"));
  connect(filePrint, SIGNAL(activated()), this, SLOT(slotFilePrint()));

	fileEngine= new QAction(tr("Load engine"), tr("&Load engine"), 0, this);
  fileEngine->setStatusTip(tr("Loads a new engine"));
  fileEngine->setWhatsThis(tr("Load engine\n\nLoads a new engine"));
  connect(fileEngine, SIGNAL(activated()), this, SLOT(slotLoadNewEngine()));

  fileQuit = new QAction(tr("Exit"), tr("E&xit"), QAccel::stringToKey(tr("Ctrl+Q")), this);
  fileQuit->setStatusTip(tr("Quits the application"));
  fileQuit->setWhatsThis(tr("Exit\n\nQuits the application"));
  connect(fileQuit, SIGNAL(activated()), this, SLOT(slotFileQuit()));

  editCopy = new QAction(tr("Copy"), tr("&Copy"), QAccel::stringToKey(tr("Ctrl+C")), this);
  editCopy->setStatusTip(tr("Copies the actual game to the clipboard"));
  editCopy->setWhatsThis(tr("Copy\n\nCopies the actual game to the clipboard"));
  connect(editCopy, SIGNAL(activated()), this, SLOT(slotEditCopy()));

  editPaste = new QAction(tr("Paste"), tr("&Paste"), QAccel::stringToKey(tr("Ctrl+V")), this);
  editPaste->setStatusTip(tr("Pastes the clipboard contents to the board"));
  editPaste->setWhatsThis(tr("Paste\n\nPastes the clipboard contents to the board"));
  connect(editPaste, SIGNAL(activated()), this, SLOT(slotEditPaste()));

  editTags = new QAction(tr("Tags"), tr("&Tags"), QAccel::stringToKey(tr("Ctrl+T")), this);
  editTags->setStatusTip(tr("Edit game tags"));
  editTags->setWhatsThis(tr("Tags\n\nEdit game tags"));
  connect(editTags, SIGNAL(activated()), this, SLOT(slotEditTags()));
  editComment = new QAction(tr("Annotation"), tr("&Annotate"), QAccel::stringToKey(tr("Ctrl+A")), this);
  editComment->setStatusTip(tr("Annotate current move"));
  editComment->setWhatsThis(tr("Annotation\n\nAnnotate current move"));
  connect(editComment, SIGNAL(activated()), this, SLOT(slotEditComment()));

  viewToolBar = new QAction(tr("Toolbar"), tr("Tool&bar"), 0, this, 0, true);
  viewToolBar->setStatusTip(tr("Enables/disables the toolbar"));
  viewToolBar->setWhatsThis(tr("Toolbar\n\nEnables/disables the toolbar"));
  connect(viewToolBar, SIGNAL(toggled(bool)), this, SLOT(slotViewToolBar(bool)));

  viewStatusBar = new QAction(tr("Statusbar"), tr("&Statusbar"), 0, this, 0, true);
  viewStatusBar->setStatusTip(tr("Enables/disables the statusbar"));
  viewStatusBar->setWhatsThis(tr("Statusbar\n\nEnables/disables the statusbar"));
  connect(viewStatusBar, SIGNAL(toggled(bool)), this, SLOT(slotViewStatusBar(bool)));

	viewList = new QAction(tr("List"), tr("&List"), Key_Escape, this);
  viewList->setStatusTip(tr("Toggles between list and board view"));
  viewList->setWhatsThis(tr("List\n\nToggles between list and board view"));
  connect(viewList, SIGNAL(activated()), this, SLOT(slotToggleList()));

  viewFilter = new QAction(tr("Filter"), tr("&Filter"), QAccel::stringToKey(tr("Ctrl+F")), this);
  viewFilter->setStatusTip(tr("Apply filter to the list view"));
  viewFilter->setWhatsThis(tr("Filter\n\nApply filter to the list view"));
  connect(viewFilter, SIGNAL(activated()), this, SLOT(slotViewFilter()));

	viewFlipBoard = new QAction(tr("Flip board"), tr("&Flip board"), Key_F, this);
  connect(viewFlipBoard, SIGNAL(activated()), this, SLOT(slotViewFlipBoard()));

  helpAboutApp = new QAction(tr("About"), tr("&About..."), 0, this);
  helpAboutApp->setStatusTip(tr("About the application"));
  helpAboutApp->setWhatsThis(tr("About\n\nAbout the application"));
  connect(helpAboutApp, SIGNAL(activated()), this, SLOT(slotHelpAbout()));

	//navigation actions
	gameGotoStart = new QAction(tr("Goto start"), firstIcon, tr("&Goto start"), Key_Home, this);
  connect(gameGotoStart, SIGNAL(activated()), this, SLOT(slotGameGotoStart()));
	gameNextMove = new QAction(tr("Next move"), rightIcon, tr("&Next move"), Key_Right, this);
  connect(gameNextMove, SIGNAL(activated()), this, SLOT(slotGameNextMove()));
	gamePrevMove = new QAction(tr("Prev move"), leftIcon, tr("&Prev move"), Key_Left, this);
  connect(gamePrevMove, SIGNAL(activated()), this, SLOT(slotGamePrevMove()));
	gameNextGame = new QAction(tr("Next game"), tr("&Next game"), Key_PageDown, this);
  connect(gameNextGame, SIGNAL(activated()), this, SLOT(slotViewShowNextGame()));
	gamePrevGame = new QAction(tr("Prev game"), tr("&Prev game"), Key_PageUp, this);
  connect(gamePrevGame, SIGNAL(activated()), this, SLOT(slotViewShowPrevGame()));
	gameGamePlay = new QAction(tr("Play"), playIcon, tr("&Play"), Key_Return, this);
  connect(gameGamePlay, SIGNAL(activated()), this, SLOT(slotGamePlay()));

	//extras
  extrasSettings = new QAction(tr("Settings"), tr("&Settings..."), 0, this);
  extrasSettings->setStatusTip(tr("Setup the application"));
  extrasSettings->setWhatsThis(tr("Settings\n\nSetup the application"));
  connect(extrasSettings, SIGNAL(activated()), this, SLOT(slotSettings()));

}

void DeskaApp::initMenuBar()
{
  ///////////////////////////////////////////////////////////////////
  // MENUBAR

  ///////////////////////////////////////////////////////////////////
  // menuBar entry fileMenu
  fileMenu=new QPopupMenu();
  fileNew->addTo(fileMenu);
  fileOpen->addTo(fileMenu);
  fileClose->addTo(fileMenu);
  fileMenu->insertSeparator();
  fileSaveGame->addTo(fileMenu);
  fileSaveGameAs->addTo(fileMenu);
  fileMenu->insertSeparator();
  filePrint->addTo(fileMenu);
  fileMenu->insertSeparator();
	recentFilesMenu=new QPopupMenu();
  connect(recentFilesMenu, SIGNAL(activated(int)), SLOT(slotFileOpenRecent(int)));
	fileMenu->insertItem(tr("Recent files..."), recentFilesMenu);
  fileMenu->insertSeparator();	
  fileEngine->addTo(fileMenu);
	engineMenu=new QPopupMenu();
	connect(engineMenu, SIGNAL(activated(int)), SLOT(slotLoadEngine(int)));
	fileMenu->insertItem(tr("Available engines..."), engineMenu);
  fileMenu->insertSeparator();
  fileQuit->addTo(fileMenu);

  ///////////////////////////////////////////////////////////////////
  // menuBar entry editMenu
  editMenu=new QPopupMenu();
  editCopy->addTo(editMenu);
  editPaste->addTo(editMenu);
  editMenu->insertSeparator();
  editTags->addTo(editMenu);
  editComment->addTo(editMenu);

  ///////////////////////////////////////////////////////////////////
  // menuBar entry viewMenu
  viewMenu=new QPopupMenu();
  viewMenu->setCheckable(true);
	viewList->addTo(viewMenu);
  viewFilter->addTo(viewMenu);
  viewMenu->insertSeparator();
  viewToolBar->addTo(viewMenu);
  viewStatusBar->addTo(viewMenu);
  ///////////////////////////////////////////////////////////////////

	extrasMenu=new QPopupMenu();
	extrasSettings->addTo(extrasMenu);

  ///////////////////////////////////////////////////////////////////
  // menuBar entry helpMenu
  helpMenu=new QPopupMenu();
  helpAboutApp->addTo(helpMenu);

  ///////////////////////////////////////////////////////////////////
  // MENUBAR CONFIGURATION
  menuBar()->insertItem(tr("&File"), fileMenu);
  menuBar()->insertItem(tr("&Edit"), editMenu);
  menuBar()->insertItem(tr("&View"), viewMenu);
  menuBar()->insertItem(tr("&Extras"), extrasMenu);
  menuBar()->insertSeparator();
  menuBar()->insertItem(tr("&Help"), helpMenu);

}

void DeskaApp::initToolBar()
{
  ///////////////////////////////////////////////////////////////////
  // TOOLBAR
  fileToolbar = new QToolBar(this, "file operations");
  fileNew->addTo(fileToolbar);
  fileOpen->addTo(fileToolbar);
  fileSaveGame->addTo(fileToolbar);
  fileToolbar->addSeparator();
	gameGotoStart->addTo(fileToolbar);
	gamePrevMove->addTo(fileToolbar);
	gameGamePlay->addTo(fileToolbar);
	gameNextMove->addTo(fileToolbar);
  QWhatsThis::whatsThisButton(fileToolbar);

}

void DeskaApp::initStatusBar()
{
  ///////////////////////////////////////////////////////////////////
  //STATUSBAR
  statusBar()->message(tr("Ready."), 2000);
}

void DeskaApp::initDoc()
{
   doc=new DeskaDoc();
}

void DeskaApp::initView()
{
  ////////////////////////////////////////////////////////////////////
  // set the main widget here
  view=new DeskaView(this, doc);
  setCentralWidget(view);
}


bool DeskaApp::queryExit()
{
  int exit=QMessageBox::information(this, tr("Quit..."),
                                    tr("Do your really want to quit?"),
                                    QMessageBox::Ok, QMessageBox::Cancel);

  if (exit==1)
  {

  }
  else
  {

  };

  return (exit==1);
}

/////////////////////////////////////////////////////////////////////
// SLOT IMPLEMENTATION
/////////////////////////////////////////////////////////////////////


void DeskaApp::slotFileNew()
{
  statusBar()->message(tr("Creating new file..."));
  view->initGame();
  statusBar()->message(tr("Ready."));
}

void DeskaApp::slotFileOpen()
{
  statusBar()->message(tr("Opening file..."));

  QString fileName = QFileDialog::getOpenFileName(0,0,this);
  if (!fileName.isEmpty())
  {
    if(doc->load(fileName))
		{		
      setCaption(fileName);
      addRecentFile(fileName);
      QString message=tr("Loaded document: ")+fileName;
      statusBar()->message(message, 2000);      
      view->initList();
      if(doc->getCount() > 1)
      {
        if(!view->listVisible())slotToggleList();
        view->slotShowList();
      }
      else	
    	  view->slotShowFirstGame();
		}
  }
  else
  {
    statusBar()->message(tr("Opening aborted"), 2000);
  }
}


void DeskaApp::slotFileSaveGame()
{
	if(!doc->fileOpened())
		slotFileSaveGameAs();
	else
	{
	  statusBar()->message(tr("Appending game to current file..."));
	  doc->saveGame(view->getGame());
  	statusBar()->message(tr("Ready."));
	}
}

void DeskaApp::slotFileSaveGameAs()
{
	QString fileName = QFileDialog::getSaveFileName(
       getenv( "HOME" ),"Portable Game Notation (*.pgn)",
                    this, tr("Save game as..."),
                    tr("Choose a file"));


  statusBar()->message(tr("Appending game to a new file..."));

  doc->saveGame(view->getGame(), fileName);

  statusBar()->message(tr("Ready."));
}


void DeskaApp::slotFileClose()
{
  statusBar()->message(tr("Closing file..."));
	doc->close();
  setCaption(tr("Deska " VERSION));
	view->initList();
  statusBar()->message(tr("Ready."));
}

void DeskaApp::slotFilePrint()
{
  statusBar()->message(tr("Printing..."));
  QPrinter printer;
  if (printer.setup(this))
  {
    QPainter painter;
    painter.begin(&printer);

    ///////////////////////////////////////////////////////////////////
    // TODO: Define printing by using the QPainter methods here

    painter.end();
  };

  statusBar()->message(tr("Ready."));
}

void DeskaApp::slotFileQuit()
{
  statusBar()->message(tr("Exiting application..."));
  ///////////////////////////////////////////////////////////////////
  // exits the Application
  if(doc->isModified())
  {
    if(queryExit())
    {
			close();//save settings
      qApp->quit();
    }
    else
    {

    };
  }
  else
  {
		close();
    qApp->quit();
  };

  statusBar()->message(tr("Ready."));
}

void DeskaApp::slotEditTags()
{
	statusBar()->message(tr("Editting tags..."));

	view->slotEditTags();

  statusBar()->message(tr("Ready."));
}

void DeskaApp::slotEditComment()
{
	statusBar()->message(tr("Annotating current move..."));

	view->slotEditComment();

  statusBar()->message(tr("Ready."));
}

void DeskaApp::slotEditCopy()
{
	QClipboard *cb = QApplication::clipboard();
  QString t, nt, s;
	gameTyp *game;

	statusBar()->message(tr("Copying game to clipboard..."));

	game = view->getGame();
	t = game->header;
	t += "\n";

	// game notation
	int k=70, i=0;
	nt = game->notation;
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
			t += s + "\n";
			i=k;
			k+=70;
		}
	}
  t += s + "\n";
	t += extractTag(game->header,"Result") + "\n";
	
	cb->setText(t);

  statusBar()->message(tr("Ready."));
}

//paste PGN-game from clipboard
void DeskaApp::slotEditPaste()
{
	bool headerOK=false, gameFound=false;
  QClipboard *cb = QApplication::clipboard();
  QString clipText, line="", header="", notation="";
	gameTyp g;

  statusBar()->message(tr("Pasting clipboard contents to the board..."));

  clipText = cb->text();
	clipText.simplifyWhiteSpace();
	QTextStream t(clipText, IO_ReadOnly);
	
	while (!t.eof())
	{
		line = t.readLine();
		if(line.left(1)=="[")	// tag found		
		{
			gameFound=true;
			if(headerOK)break;
			header+="\n";
			header+=line;
		}
		else
		{
			if(!gameFound)continue;
			headerOK=true;
			notation+=" ";
			notation+=line;
		}
  }

	header.simplifyWhiteSpace();
	notation.simplifyWhiteSpace();
	g.header = header;
	g.notation = notation;
	view->initGame(&g);			

  statusBar()->message(tr("Ready."));
}


void DeskaApp::slotViewToolBar(bool toggle)
{
  statusBar()->message(tr("Toggle toolbar..."));
  ///////////////////////////////////////////////////////////////////
  // turn Toolbar on or off

  if (toggle== false)
  {
    fileToolbar->hide();
  }
  else
  {
    fileToolbar->show();
  };

  statusBar()->message(tr("Ready."));
}

void DeskaApp::slotViewStatusBar(bool toggle)
{
  statusBar()->message(tr("Toggle statusbar..."));
  ///////////////////////////////////////////////////////////////////
  //turn Statusbar on or off

  if (toggle == false)
  {
    statusBar()->hide();
  }
  else
  {
    statusBar()->show();
  }

  statusBar()->message(tr("Ready."));
}

void DeskaApp::slotHelpAbout()
{
  QMessageBox::about(this,tr("About..."),
  tr("Deska\nThe PGN-Reader for Linux\nVersion " VERSION "\n(c) 2001-2004 by Andreas J. Pyka\nhttp://www.matikon.de") );
}

void DeskaApp::slotToggleList()
{
  statusBar()->message(tr("Toggle views..."));

  if(!view->listVisible())
    viewList->setMenuText(tr("Board"));
  else
    viewList->setMenuText(tr("List"));

  view->toggleList();    

  statusBar()->message(tr("Ready."));
}

void DeskaApp::slotLoadNewEngine()
{
	slotLoadEngine(-1);
}

//load a new engine and put into recent list
void DeskaApp::slotLoadEngine(int id)
{
	QString enginePath="";

	if(id!=-1)enginePath=engineMenu->text(id);

  enginePath = view->loadEngine(enginePath);
	
	//TODO remove invalid engines from menu

	if(id!=-1)return;
	if(enginePath=="")return;
	
  addEngine(enginePath);
}

void DeskaApp::slotSettings()
{
	COptions *opt=new COptions(0);
	connect(opt, SIGNAL(optionsChanged()), view, SLOT(slotUpdateBoard()));			
	opt->show();	
}

void DeskaApp::closeEvent ( QCloseEvent *e )
{
  g_xpos = x();
  g_ypos = y();
  g_width = width();
  g_height = height();

  saveSettings();
  e->accept();
}


void DeskaApp::slotGameGotoStart()		{	view->slotGotoStart();}
void DeskaApp::slotViewFlipBoard()		{	view->slotFlipBoard();}


//TODO rename
void DeskaApp::slotGameNextMove()			
{	
	view->slotNextMove();
	view->toggleMoreBoards();
}
void DeskaApp::slotGamePrevMove()			
{	
	view->slotPrevMove();
	view->togglePrevBoard();
}

void DeskaApp::slotViewShowList()			{	view->slotShowList();}
void DeskaApp::slotViewShowNextGame()	{	view->slotShowNextGame();}
void DeskaApp::slotViewShowPrevGame()	{	view->slotShowPrevGame();}
void DeskaApp::slotGamePlay()					{	view->slotShowCurGame();}


void DeskaApp::addRecentFile(const QString &file)
{
  if(g_fileName.find(file) == -1)
  {
    if( g_fileName.count() < 5)
    {
      g_fileName.insert(0, file);
    }
    else
    {
      g_fileName.remove(4);
      g_fileName.insert(0, file);
    }
    recentFilesMenu->clear();
    for ( int i=0 ; i < (int) g_fileName.count(); i++)
    {
      recentFilesMenu->insertItem(g_fileName.at(i));
    }
  }
}

void DeskaApp::addEngine(const QString &file)
{
  if(g_recentEngine.find(file) == -1)
  {
    if( g_recentEngine.count() < 5)
    {
      g_recentEngine.insert(0, file);
    }
    else
    {
      g_recentEngine.remove(4);
      g_recentEngine.insert(0, file);
    }
    engineMenu->clear();
    for ( int i=0 ; i < (int) g_recentEngine.count(); i++)
    {
      engineMenu->insertItem(g_recentEngine.at(i));
    }
  }
}

void DeskaApp::slotFileOpenRecent(int id)
{
	QString fileName = QString(recentFilesMenu->text(id));

  statusBar()->message(tr("Opening file..."));


  if(doc->load(fileName))
	{
	  setCaption(fileName);
	  QString message=tr("Loaded document: ")+fileName;
	  statusBar()->message(message, 2000);
    if(!view->listVisible())slotToggleList();
    view->initList();
		view->slotShowList();
	}
	else
	{
  	if(QMessageBox::information(this, "Deska",
                                    tr("Remove file from recent menu?"),
                                    QMessageBox::Ok, QMessageBox::Cancel)==1)
		{
			recentFilesMenu->removeItem(id);
			for(int i=0; (uint)i<g_fileName.count();i++)
		  	if(g_fileName.at(i)==fileName)g_fileName.remove(i);
		}
	}
	
}


void DeskaApp::slotViewFilter()
{
	view->slotSetupFilter();
}