/***************************************************************************
                          FILENAME  -  description                              
                             -------------------                                         
    begin                : DATE                                           
    copyright            : (C) YEAR by AUTHOR                         
    email                : EMAIL                                     
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/


#ifndef CMINIBOARD_H
#define CMINIBOARD_H

#include <qwidget.h>
#include <cstdboard.h>

/**
  * A small board for preview in the game list.
  */

class CMiniBoard : public CStdBoard  {
   Q_OBJECT
public: 
	CMiniBoard(QWidget *parent=0, const char *name=0);
	~CMiniBoard();

private:
	void setPieces();
	void mouseReleaseEvent( QMouseEvent* ){};
	void mousePressEvent( QMouseEvent* ){};


};

#endif












