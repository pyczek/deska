/***************************************************************************
 *   Copyright (C) 2004 by Andreas J. Pyka                                 *
 *   andreas.pyka@matikon.de                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef CFILTER_H
#define CFILTER_H

#include "ctags.h"

/**
  Sets the filter criteria for the game list.
*/
class CFilter : public CTags  {
  Q_OBJECT

public:
  CFilter(QString*, QWidget * parent=0, const char * name=0, bool modal=TRUE);
  ~CFilter();

private slots:
  
private:
};


#endif
