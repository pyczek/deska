/***************************************************************************
                          cgamelist.h  -  description
                             -------------------
    begin                : Fri Dec 28 2001
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

#ifndef CGAMELIST_H
#define CGAMELIST_H

#include <qlistview.h>
#include <qpopupmenu.h>
#include <qwidget.h>

/**
  *@author Andreas Jan Pyka
  */

class CGameList : public QListView
{
  Q_OBJECT
  public:
    CGameList(QWidget *parent=0, const char *name=0);
    ~CGameList();
  protected:
    void mousePressEvent( QMouseEvent* );

  private:	
    QWidget *m_parent;
    QPopupMenu *m_listMenue;
  
  private slots:	
  void  slotListPopupActivated(int);

};

#endif
