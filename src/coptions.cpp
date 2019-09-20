/***************************************************************************
                          coptions.cpp  -  description
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

#include <qmessagebox.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qdir.h>
#include <qframe.h>
#include <qcolordialog.h>

#include "coptions.h"
#include "global.h"

#define SPORTVERLAG "Sportverlag"
#define CHESSY			"Chessy 2.0"
#define XBOARD			"XBoard"
#define	DEFAULT			"Default"

#define BERLIN37		"berlin37"
#define XBOARD54    "xboard54"
#define XCHESS64		"xchess64"

COptions::COptions(QWidget *parent, const char *name ) : QTabDialog(parent,name)
{
	setCaption("Global options");
	setCancelButton("Cancel");
	setupBoardTab();

	resize(320, 380);

	for(int i=0;i<m_pieceSet->count();i++)
	{
		if(m_pieceSet->text(i)==g_pieces)
		{
			m_pieceSet->setCurrentItem(i);
			break;
		}
	}
	
	connect(this,SIGNAL(applyButtonPressed()),SLOT(slotOKPressed()));
}

COptions::~COptions()
{
}

void COptions::addTag()
{
	int i;
	i=m_allTags->currentItem();
	if(i==-1)return;
	if(m_curTags->count()>=(uint)MAX_TAGS)return;
	for(int k=0;(uint)k<m_curTags->count();k++)
		if((QString)m_curTags->text(k)==(QString)m_allTags->text(i))return;
	m_curTags->insertItem(m_allTags->text(i));	
}

void COptions::remTag()
{
	int i;
	i=m_curTags->currentItem();
	if(i==-1)return;
	m_curTags->removeItem(i);	
}


void COptions::slotOKPressed()
{
	// pieces TODO
	g_pieces=m_pieceSet->currentText();
	g_hatched = m_hatched->isChecked();
	g_transparent = m_transparent->isChecked();
	g_frame = m_frame->isChecked();
	g_w_color = m_whiteField->text();
	g_b_color = m_blackField->text();	

	emit optionsChanged();
}

void COptions::setupTagTab()
{
	int s, e;
	QString t;
	QGridLayout *grd, *cgrd;	
	QWidget			*tagOptions;	
 	
	tagOptions = new QFrame(this);
	
	// grids
	grd = new QGridLayout(tagOptions, 3,3);
	cgrd = new QGridLayout(tagOptions, 9,1);	

	// list all
	QLabel *at = new QLabel(tagOptions);	
  at->setText("All Tags");
	m_allTags = new QListBox(tagOptions);
	t=g_header;//TODO: allTAGS
	s=t.find(",",0);
	for(;;)
	{
		e=t.find(",",s+1);
		if(e==-1)break;
		
		m_allTags->insertItem(t.mid(s+1,e-s-1));
		s=e;
	}

	grd->addWidget(m_allTags,1,0);
	grd->addWidget(at,0,0);	

	// current list
	QLabel *ct = new QLabel(tagOptions);	
	ct->setText("Current Tags");	
	m_curTags = new QListBox(tagOptions);
	for(int i=0;i<MAX_TAGS;i++)
		if(!g_tags[i].isEmpty())m_curTags->insertItem(g_tags[i]);
	grd->addWidget(m_curTags,1,2);
	grd->addWidget(ct,0,2);

	// buttons
	m_btnAdd = new QPushButton("add >>", tagOptions);
	m_btnRem = new QPushButton("<< remove", tagOptions);	
	cgrd->addWidget(m_btnAdd,3,0);
	cgrd->addWidget(m_btnRem,4,0);
	m_btnUp = new QPushButton("^", tagOptions);	
	cgrd->addWidget(m_btnUp,7,0);

	// size
	cgrd->setRowStretch(0,5);
	cgrd->setRowStretch(1,10);
	cgrd->setRowStretch(2,10);
	cgrd->setRowStretch(3,10);
	cgrd->setRowStretch(4,10);
	cgrd->setRowStretch(5,10);
	cgrd->setRowStretch(6,10);
	cgrd->setRowStretch(7,10);
	cgrd->setRowStretch(8,5);

	grd->setRowStretch(0,10);
	grd->setRowStretch(1,90);
	grd->setRowStretch(2,10);

	grd->setColStretch(0,40);
	grd->setColStretch(1,20);
	grd->setColStretch(2,40);

	grd->addLayout(cgrd,1,1);
	this->addTab(tagOptions, "&Tags");

	connect(m_btnAdd,SIGNAL(clicked()),SLOT(addTag()));
  connect(m_btnRem,SIGNAL(clicked()),SLOT(remTag()));
}


void COptions::setupBoardTab()
{
	QWidget	*boardOptions=new QFrame(this);

  QDir piecesDir(g_root + "/share/deska/pieces/");

	//--START--
	m_pieceSet = new QComboBox( FALSE, boardOptions, "cboPieces" );
	m_pieceSet->setGeometry( 120, 20, 180, 30 );

	QLabel* qtarch_lblPieces;
	qtarch_lblPieces = new QLabel( boardOptions, "lblPieces" );
	qtarch_lblPieces->setGeometry( 10, 20, 100, 30 );
	qtarch_lblPieces->setText( "Piece Set:" );
	qtarch_lblPieces->adjustSize();
	qtarch_lblPieces->setAlignment( 289 );
	qtarch_lblPieces->setMargin( -1 );

	m_whiteField = new QPushButton(boardOptions, "edtWhiteSquares" );
	m_whiteField->setGeometry( 120, 120, 180, 30 );
	//m_whiteField->setFrame( TRUE );
	m_whiteField->setPaletteBackgroundColor(g_w_color);
	m_whiteField->setPaletteForegroundColor(!g_w_color);
	m_whiteField->setText(g_w_color);
	connect(m_whiteField, SIGNAL(clicked()), SLOT(slotColorWhiteField()));

	m_blackField = new QPushButton(boardOptions, "edtBlackSquares" );
	m_blackField->setGeometry( 120, 160, 180, 30 );
	//m_blackField->setFrame( TRUE );
	m_blackField->setPaletteBackgroundColor(g_b_color);
	m_blackField->setPaletteForegroundColor(!g_b_color);
	m_blackField->setText(g_b_color);
	connect(m_blackField, SIGNAL(clicked()), SLOT(slotColorBlackField()));

	QLabel* qtarch_lblWhiteSquares;
	qtarch_lblWhiteSquares = new QLabel( boardOptions, "lblWhiteSquares" );
	qtarch_lblWhiteSquares->setGeometry( 10, 120, 100, 30 );
	qtarch_lblWhiteSquares->setText( "White Squares:" );
	qtarch_lblWhiteSquares->setAlignment( 289 );
	qtarch_lblWhiteSquares->adjustSize();
	qtarch_lblWhiteSquares->setMargin( -1 );

	QLabel* qtarch_lblBlackSquares;
	qtarch_lblBlackSquares = new QLabel( boardOptions, "lblBlackSquares" );
	qtarch_lblBlackSquares->setGeometry( 10, 160, 100, 30 );
	qtarch_lblBlackSquares->setText( "Black Squares:" );
	qtarch_lblBlackSquares->setAlignment( 289 );
	qtarch_lblBlackSquares->adjustSize();
	qtarch_lblBlackSquares->setMargin( -1 );

	m_frame = new QCheckBox( boardOptions, "rbtFrame" );
	m_frame->setGeometry( 10, 60, 100, 30 );
	m_frame->setText( "Frame" );
	m_frame->adjustSize();
  if(g_frame)m_frame->setChecked(true);else m_frame->setChecked(false);

	m_transparent = new QCheckBox( boardOptions, "rbtTransparent" );
	m_transparent->setGeometry( 110, 60, 100, 30 );
	m_transparent->setText( "Transparent" );
	m_transparent->adjustSize();
	if(g_transparent)m_transparent->setChecked(true);else m_transparent->setChecked(false);

	m_hatched = new QCheckBox( boardOptions, "rbtnHatched" );
	m_hatched->setGeometry( 10, 200, 100, 30 );
	m_hatched->setText( "Hatched" );
	m_hatched->adjustSize();
	if(g_hatched)m_hatched->setChecked(true);else m_hatched->setChecked(false);

	QLabel *qtarch_lblSchema;
	qtarch_lblSchema = new QLabel( boardOptions, "lblSchema" );
	qtarch_lblSchema->setGeometry( 10, 240, 100, 30 );
	qtarch_lblSchema->setText( "Schema:" );
	qtarch_lblSchema->adjustSize();

	m_schema = new QComboBox( FALSE, boardOptions, "cboSchema" );
	m_schema->setGeometry( 120, 240, 180, 30 );

	boardOptions->setMinimumSize( 320, 300 );
	//--END--

	//insert peices items dynamically
	const QFileInfoList * files = piecesDir.entryInfoList();
	if ( files )
	{
		QFileInfoListIterator it( *files );
		QFileInfo * f;
		while( (f=it.current()) != 0 )
		{
			++it;
      if ( f->fileName() == "." || f->fileName() == ".." ); // nothing
			else if ( f->isDir() )m_pieceSet->insertItem(f->fileName());
		}
	}	
	m_pieceSet->insertItem(BERLIN37);

	//schema
	m_schema->insertItem(SPORTVERLAG);
	m_schema->insertItem(CHESSY);
	m_schema->insertItem(XBOARD);
	m_schema->insertItem(DEFAULT);	

	this->addTab(boardOptions, "&Board");
	connect(m_schema,SIGNAL(activated(int)),SLOT(slotSchemaChanged(int)));

}

void COptions::slotColorBlackField()
{
  QColor c = QColorDialog::getColor(m_blackField->paletteBackgroundColor(), this);
  if(c.isValid())
	m_blackField->setPaletteBackgroundColor(c);
	m_blackField->setPaletteForegroundColor(!c.name());
	m_blackField->setText(c.name());
}

void COptions::slotColorWhiteField()
{
  QColor c = QColorDialog::getColor(m_whiteField->paletteBackgroundColor(), this);
  if(c.isValid())
	m_whiteField->setPaletteBackgroundColor(c);
	m_whiteField->setPaletteForegroundColor(!c.name());
	m_whiteField->setText(c.name());
}

void COptions::slotSchemaChanged(int)
{
	QString v, p;
	v = m_schema->currentText();
	if(v==SPORTVERLAG)
	{
		m_hatched->setChecked(true);
		m_whiteField->setText("White");
		m_blackField->setText("White");

		m_transparent->setChecked(false);
		m_frame->setChecked(true);

		for(int i=0;i<m_pieceSet->count();i++)
		{
			p = m_pieceSet->text(i);
			if(p.contains(BERLIN37 ,false))
			{
				m_pieceSet->setCurrentItem(i);
				break;
			}
		}

	}
	else if(v==CHESSY)
	{
		m_hatched->setChecked(false);
		m_whiteField->setText("WhiteSmoke");
		m_blackField->setText("Gainsboro");

		m_transparent->setChecked(true);
		m_frame->setChecked(false);

		for(int i=0;i<m_pieceSet->count();i++)
		{
			p = m_pieceSet->text(i);
			if(p.contains(BERLIN37, false))
			{
				m_pieceSet->setCurrentItem(i);
				break;
			}
		}

	}
	else if(v==XBOARD)
	{
		m_hatched->setChecked(false);
		m_whiteField->setText("#CDC262");
		m_blackField->setText("#73A16A");

		m_transparent->setChecked(false);
		m_frame->setChecked(false);
		for(int i=0;i<m_pieceSet->count();i++)
		{
			p = m_pieceSet->text(i);
			if(p.contains(XBOARD54, false))
			{
				m_pieceSet->setCurrentItem(i);
				break;
			}
		}
	}	
	else if(v==DEFAULT)
	{
		m_hatched->setChecked(true);
		m_whiteField->setText("WhiteSmoke");
		m_blackField->setText("#f9e9ca");

		m_transparent->setChecked(false);
		m_frame->setChecked(true);
		for(int i=0;i<m_pieceSet->count();i++)
		{
			p = m_pieceSet->text(i);
			if(p.contains(BERLIN37, false))
			{
				m_pieceSet->setCurrentItem(i);
				break;
			}
		}
	}

	m_whiteField->setPaletteBackgroundColor(m_whiteField->text());
	m_whiteField->setPaletteForegroundColor(!m_whiteField->text());		
	m_blackField->setPaletteBackgroundColor(m_blackField->text());
	m_blackField->setPaletteForegroundColor(!m_blackField->text());		

}















































































