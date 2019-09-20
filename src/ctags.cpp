/***************************************************************************
                          ctags.cpp  -  description
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


#include <qlayout.h>
#include <qlabel.h>

#include "ctags.h"

CTags::CTags(QString *header, QWidget *parent, const char *name, bool modal):
	QTabDialog(parent, name, modal)
{
//	setResult(QDialog::Rejected);

	m_header = header;
	
	this->setCancelButton("Cancel");	

	setup7Tags();
	
	resize(300, 200);

	connect(this,SIGNAL(applyButtonPressed()),SLOT(slotOKPressed()));
}

CTags::~CTags(){
}

void CTags::setHeader()
{
  int i;

  *m_header = "";
  for(i=0;i<7;i++)
  {
    *m_header	+= "[";
    *m_header += e_7tag[i]->name();
    *m_header += " \"";
    *m_header += e_7tag[i]->text();
    *m_header += "\"]\n";		
  }
}

void CTags::slotOKPressed()
{
	setHeader();
	setResult(QDialog::Accepted);
}

void CTags::setup7Tags()
{
  QWidget *tagW;	
  QLabel  *l;
  tagW = new QWidget(this);

  int k=0;
  for(int i=0;i<MAX_TAGS;i++)
  {
    if(!g_7tags.contains(g_tags[i]))continue;

    l = new QLabel(tagW);
    l->setGeometry( 20, 20+k*22, 60, 22 );
    l->setText(g_tags[i]);
    e_7tag[k] = new QLineEdit(tagW, g_tags[i]);
    e_7tag[k]->setText(extractTag(*m_header, g_tags[i]));
    e_7tag[k]->setGeometry( 80, 20+k*22, 200, 22 );
    k++;
  }

  tagW->setMinimumSize(300, 200);

  this->addTab(tagW, "&Main");
}











































