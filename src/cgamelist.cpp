/***************************************************************************
                          cgamelist.cpp  -  description
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

#include <qcursor.h>
#include <qmessagebox.h>

#include "cgamelist.h"
#include "resource.h"
#include "deskaview.h"


CGameList::CGameList(QWidget *parent,const char *name)
:QListView(parent,name)
{
	m_parent = parent;	
	
	m_listMenue = new QPopupMenu(this, "popupGameList");
	m_listMenue->setCheckable(true);
	m_listMenue->insertItem(tr("Preview"), ID_GAMELIST_PREVIEW);
	m_listMenue->insertItem(tr("Overview"), ID_GAMELIST_MOREBOARDS);
		
	connect(m_listMenue, SIGNAL(activated(int)), this, SLOT(slotListPopupActivated(int)));		
}

CGameList::~CGameList(){
}

void CGameList::mousePressEvent( QMouseEvent* e)
{
	//if(!hasFocus())setFocus();
	if(e->button()==RightButton)
	{
		m_listMenue->exec(QCursor::pos());		
	}
}

void CGameList::slotListPopupActivated(int item)
{
	switch(item)
	{
		case ID_GAMELIST_PREVIEW:
			DeskaView(m_parent).toggleMoreBoards();
		break;
		case ID_GAMELIST_MOREBOARDS:
			DeskaView(m_parent).togglePrevBoard();
		break;
	}
}
