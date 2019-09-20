/***************************************************************************
                          main.cpp  -  description
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

#include <qapplication.h>
#include <qfont.h>
#include <qstring.h>
#include <qtextcodec.h>
#include <qtranslator.h>

#include "deska.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  //a.setFont(QFont("Arial", 10));
  QTranslator tor( 0 );
  // set the location where your .qm files are in load() below as the last parameter instead of "."
  // for development, use "/" to use the english original as
  // .qm files are stored in the base project directory.
  tor.load( QString("deska.") + QTextCodec::locale(), "." );
  a.installTranslator( &tor );
  /* uncomment the following line, if you want a Windows 95 look*/
  // a.setStyle(WindowsStyle);
    
  DeskaApp *deska=new DeskaApp();
  a.setMainWidget(deska);

  deska->show();

	//params
	if(argc>1)
	{
		if(deska->getDoc()->load(QString(argv[1])))
		{
		  deska->getView()->initList();
		  if(deska->getDoc()->getCount()>1)			
			  deska->getView()->slotShowList();				
			else
			  deska->getView()->slotShowFirstGame();
		}
	}

  return a.exec();
}


