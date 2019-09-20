/***************************************************************************
                          deska.h  -  description
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

#ifndef DESKA_H
#define DESKA_H

#define VERSION "0.8"

// include files for QT
#include <qapp.h>
#include <qmainwindow.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qstatusbar.h>
#include <qwhatsthis.h>
#include <qstring.h>
#include <qpixmap.h>
#include <qmsgbox.h>
#include <qfiledialog.h>
#include <qprinter.h>
#include <qpainter.h>

// application specific includes
#include "deskaview.h"
#include "deskadoc.h"

/**
  * This Class is the base class for your application. It sets up the main
  * window and providing a menubar, toolbar
  * and statusbar. For the main view, an instance of class DeskaView is
  * created which creates your view.
  */
class DeskaApp : public QMainWindow
{
  Q_OBJECT
  
  public:
    /** construtor */
    DeskaApp();
    /** destructor */
    ~DeskaApp();
    /** initializes all QActions of the application */
    void initActions();
    /** initMenuBar creates the menu_bar and inserts the menuitems */
    void initMenuBar();
    /** this creates the toolbars. Change the toobar look and add new toolbars in this
     * function */
    void initToolBar();
    /** setup the statusbar */
    void initStatusBar();
    /** setup the document*/
    void initDoc();
    /** setup the mainview*/
    void initView();

    /** overloaded for Message box on last window exit */
    bool queryExit();

		DeskaDoc *getDoc(){return doc;}
		DeskaView *getView(){return view;}
		
  public slots:

    /** generate a new document in the actual view */
    void slotFileNew();
    /** open a document */
    void slotFileOpen();
    /** save a document */
    void slotFileSaveGame();
    void slotFileSaveGameAs();
    /** close the actual file */
    void slotFileClose();
    /** print the actual file */
    void slotFilePrint();
    /** exits the application */
    void slotFileQuit();
    /** put the marked text/object into the clipboard and remove
     * it from the document */
    void slotEditCopy();
    /** paste the clipboard into the document*/
    void slotEditPaste();
    /** toggle the toolbar*/
    void slotViewToolBar(bool toggle);
    /** toggle the statusbar*/
    void slotViewStatusBar(bool toggle);

    /** shows an about dlg*/
    void slotHelpAbout();

		/** toggles between list board*/
		void	slotToggleList();
	protected:
		void	closeEvent ( QCloseEvent * );

  private:

    /** view is the main widget which represents your working area. The View
     * class should handle all events of the view widget.  It is kept empty so
     * you can create your view according to your application's needs by
     * changing the view class.
     */
    DeskaView *view;
    /** doc represents your actual document and is created only once. It keeps
     * information such as filename and does the serialization of your files.
     */
    DeskaDoc *doc;
  
    /** file_menu contains all items of the menubar entry "File" */
    QPopupMenu *fileMenu;
    QPopupMenu *engineMenu;
    QPopupMenu *recentFilesMenu;
		QPopupMenu *extrasMenu;
    /** edit_menu contains all items of the menubar entry "Edit" */
    QPopupMenu *editMenu;
    /** view_menu contains all items of the menubar entry "View" */
    QPopupMenu *viewMenu;
    /** view_menu contains all items of the menubar entry "Help" */
    QPopupMenu *helpMenu;
    /** the main toolbar */
    QToolBar *fileToolbar;
    /** actions for the application initialized in initActions() and used to en/disable them
      * according to your needs during the program */
    QAction *fileNew;
    QAction *fileOpen;
    QAction *fileSaveGame;
    QAction *fileSaveGameAs;
    QAction *fileClose;
    QAction *filePrint;
    QAction *fileQuit;
		QAction *fileEngine;

    QAction *editCopy;
    QAction *editPaste;
		QAction *editTags;
		QAction *editComment;

    QAction *viewToolBar;
    QAction *viewStatusBar;
		QAction	*viewList;
		QAction	*viewFlipBoard;
		QAction *viewFilter;

    QAction *gameGotoStart;
		QAction *gameNextMove;
		QAction *gamePrevMove;
		QAction *gameNextGame;
		QAction *gamePrevGame;
		QAction *gameGamePlay;

		QAction *extrasSettings;

    QAction *helpAboutApp;

		void addRecentFile(const QString &file);
		void addEngine(const QString &file);

	private slots:
		void	slotGameNextMove();
		void	slotGamePrevMove();
		void	slotEditTags();
		void	slotEditComment();
		void	slotGameGotoStart();
		void 	slotViewFlipBoard();
		void	slotGamePlay();
		void	slotViewShowList();
		void	slotViewShowNextGame();
		void	slotViewShowPrevGame();
		void 	slotLoadEngine(int id);
		void	slotLoadNewEngine();
		void	slotSettings();
		void 	slotFileOpenRecent(int id);
		void 	slotViewFilter();
};
#endif 

