/***************************************************************************
                          ctags.h  -  description
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

#ifndef CTags_H
#define CTags_H

#include <qcombobox.h>
#include <qlineedit.h>
#include <qtabdialog.h>

#include "global.h"

/**
  * The tags dialog, game information, game header.
  */

class CTags : public QTabDialog  {
  Q_OBJECT

public:
  CTags(QString*, QWidget * parent=0, const char * name=0, bool modal=TRUE);
  ~CTags();

  void setup7Tags();
  void setHeader();

private slots:
  void slotOKPressed();

private:
  QLineEdit  *e_7tag[7];
  QString *m_header;
};

#endif






















