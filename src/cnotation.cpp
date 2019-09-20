/***************************************************************************
                          cnotation.cpp  -  description
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


#include "cnotation.h"

CNotation::CNotation(QWidget *parent, const char *name ) : QMultiLineEdit(parent,name)
{
	m_dbl=false;
	setReadOnly(true);
}
CNotation::~CNotation()
{
}

void CNotation::mouseDoubleClickEvent ( QMouseEvent * m )
{
	m_dbl=true;
}

bool CNotation::hasMarkedText()
{
  return true;
}
