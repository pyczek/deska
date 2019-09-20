/***************************************************************************
                          cnotation.h  -  description
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



#ifndef CNOTATION_H
#define CNOTATION_H

#include <qwidget.h>
#include <qmultilinedit.h>

/**
  * The move notation of a game.
  */

class CNotation : public QMultiLineEdit
{
   Q_OBJECT
public: 
	CNotation(QWidget *parent=0, const char *name=0);
	~CNotation();
protected:
	void mouseDoubleClickEvent ( QMouseEvent *);
	bool hasMarkedText();

private:
	bool m_dbl;

signals:
	void moveSelected(QString);
};

#endif
