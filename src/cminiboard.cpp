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

#include "cminiboard.h"
#include "global.h"

// preview pieces
#include "../rsc/pieces/mini/wp.xpm"
#include "../rsc/pieces/mini/wn.xpm"
#include "../rsc/pieces/mini/wb.xpm"
#include "../rsc/pieces/mini/wr.xpm"
#include "../rsc/pieces/mini/wq.xpm"
#include "../rsc/pieces/mini/wk.xpm"
#include "../rsc/pieces/mini/bp.xpm"
#include "../rsc/pieces/mini/bn.xpm"
#include "../rsc/pieces/mini/bb.xpm"
#include "../rsc/pieces/mini/br.xpm"
#include "../rsc/pieces/mini/bq.xpm"
#include "../rsc/pieces/mini/bk.xpm"


CMiniBoard::CMiniBoard(QWidget *parent, const char *name ) : CStdBoard(parent,name)
{	
	setPieces();	
}

CMiniBoard::~CMiniBoard(){
}

void CMiniBoard::setPieces()
{
	wp=QPixmap((const char**)mwp_xpm);
	wn=QPixmap((const char**)mwn_xpm);
	wb=QPixmap((const char**)mwb_xpm);
	wr=QPixmap((const char**)mwr_xpm);
	wq=QPixmap((const char**)mwq_xpm);
	wk=QPixmap((const char**)mwk_xpm);
	bp=QPixmap((const char**)mbp_xpm);
	bn=QPixmap((const char**)mbn_xpm);
	bb=QPixmap((const char**)mbb_xpm);
	br=QPixmap((const char**)mbr_xpm);
	bq=QPixmap((const char**)mbq_xpm);
	bk=QPixmap((const char**)mbk_xpm);

  FIELD_SIZE=bk.width();
  PO=(int)(FIELD_SIZE*0.1);
	FIELD_SIZE+=PO*2;

	setupBoard();
}



















