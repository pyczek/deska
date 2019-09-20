/***************************************************************************
                          coptions.h  -  description
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


#ifndef COPTIONS_H
#define COPTIONS_H

#include <qwidget.h>
#include <qtabdialog.h>
#include <qlistbox.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qcheckbox.h>

/**
  * The options dialog. Setup the board colors and choose the piece set.
  */

class COptions : public QTabDialog  {
   Q_OBJECT

public:
	COptions(QWidget *parent=0, const char *name=0);
	~COptions();

private:
	void setupBoardTab();
	void setupTagTab();

	QCheckBox		*m_transparent, *m_frame, *m_hatched;	
	QComboBox		*m_pieceSet, *m_schema;
	QLineEdit		*e_gameCount;
	QPushButton 		*m_blackField, *m_whiteField;
	QListBox		*m_curTags, *m_allTags;
	QPushButton	*m_btnAdd, *m_btnRem, *m_btnUp;

private slots:
	void 	slotOKPressed();
	void	addTag();
	void  remTag();
	void	slotSchemaChanged(int);
	void	slotColorWhiteField();
	void	slotColorBlackField();


signals:
	void optionsChanged();

};

#endif


































