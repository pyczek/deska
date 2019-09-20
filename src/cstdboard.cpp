/***************************************************************************
                          cstdboard.cpp  -  description
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


#include <qapp.h>

#include <qpixmap.h>
#include <qbitmap.h>
#include <qpainter.h>
#include <qmessagebox.h>

#include "cstdboard.h"

// standard pieces
#include "../rsc/pieces/std/wp.xpm"
#include "../rsc/pieces/std/wn.xpm"
#include "../rsc/pieces/std/wb.xpm"
#include "../rsc/pieces/std/wr.xpm"
#include "../rsc/pieces/std/wq.xpm"
#include "../rsc/pieces/std/wk.xpm"
#include "../rsc/pieces/std/bp.xpm"
#include "../rsc/pieces/std/bn.xpm"
#include "../rsc/pieces/std/bb.xpm"
#include "../rsc/pieces/std/br.xpm"
#include "../rsc/pieces/std/bq.xpm"
#include "../rsc/pieces/std/bk.xpm"

CStdBoard::CStdBoard(QWidget *parent,const char *name): CBoard(parent,name)
{
  QString path;

  m_newUpdatedPos=m_oldUpdatedPos=-1;
	m_flip=1;
	m_isHolding=false;
	m_takenFrom=-1;

	if(g_pieces.isEmpty()||g_pieces=="berlin37")
		setPieces();
	else
	path=loadRsc();
 	if(path!=NULL)
	{
		QString msg;
		msg=tr("Using standard pieces due to error!");
		msg+="\n";
		msg+=tr("No legal pieces at:");
		msg+="\n";
		msg+=path;
		QMessageBox::information(0,"Deska",msg);			
		setPieces();			
		g_pieces="";
	}			

	connect(this, SIGNAL(fieldChanged(int)), this, SLOT(slotUpdateField(int)));
}

CStdBoard::~CStdBoard(){
}

void CStdBoard::slotUpdateField(int pos)
{
  m_oldUpdatedPos=m_newUpdatedPos;
  m_newUpdatedPos=-1;

  if(m_oldUpdatedPos!=-1)
  {
    repaint(transX(m_oldUpdatedPos%8)*FIELD_SIZE,transY(m_oldUpdatedPos/8)*FIELD_SIZE,
        FIELD_SIZE, FIELD_SIZE, false);
  }

  m_newUpdatedPos = pos;
  repaint(transX(pos%8)*FIELD_SIZE,transY(pos/8)*FIELD_SIZE,
      FIELD_SIZE, FIELD_SIZE, false);
}


void CStdBoard::drawMark(int pos, QPainter *p, QColor markColor, bool solid)
{
	int x,y;
  QPen     pen( markColor, 2);
	QBrush   brush(markColor );

	x=pos%8;y=pos/8;	
	x=transX(x);y=transY(y);

	x*=FIELD_SIZE;
	y*=FIELD_SIZE;	

	p->setPen( pen );		
	if(solid)
	{
		p->setRasterOp(NotEraseROP);	
		p->setBrush(brush);
	}
	else
		p->setBrush(NoBrush);

  p->drawRect(x+1,y+1,FIELD_SIZE-1, FIELD_SIZE-1);	
}


void CStdBoard::drawPiece(int piece,int pos, QPaintDevice *pd)
{
	if(pd==NULL)pd=this;
	int x,y;
	QPixmap *bmp, *rBmp;

	x=pos%8;y=pos/8;
	
	x=transX(x);y=transY(y);		

	switch(piece)
	{
		case PAWN:if(g_frame)rBmp=&wp;if(g_transparent)bmp=&wp;else bmp=&bp;break;
		case KNIGHT:if(g_frame)rBmp=&wn;if(g_transparent)bmp=&wn;else bmp=&bn;break;
		case BISHOP:if(g_frame)rBmp=&wb;if(g_transparent)bmp=&wb;else bmp=&bb;break;
		case ROOK:if(g_frame)rBmp=&wr;if(g_transparent)bmp=&wr;else bmp=&br;break;
		case QUEEN:if(g_frame)rBmp=&wq;if(g_transparent)bmp=&wq;else bmp=&bq;break;
		case KING:if(g_frame)rBmp=&wk;if(g_transparent)bmp=&wk;else bmp=&bk;break;
		case -PAWN:bmp=&bp;break;
		case -KNIGHT:bmp=&bn;break;
		case -BISHOP:bmp=&bb;break;
		case -ROOK:bmp=&br;break;
		case -QUEEN:bmp=&bq;break;
		case -KING:bmp=&bk;break;			
		default: return;		
	}

	if(g_transparent||piece<0)
		bitBlt( pd, PO+x*FIELD_SIZE,PO+y*FIELD_SIZE, bmp, 0, 0,-1, -1, NotEraseROP, true );
	else
	{
		bitBlt( pd, PO+x*FIELD_SIZE,PO+y*FIELD_SIZE, bmp, 0, 0,-1, -1, NotOrROP, true );
		if(piece>0&&g_frame)
			bitBlt( pd, PO+x*FIELD_SIZE,PO+y*FIELD_SIZE, rBmp, 0, 0,-1, -1, NotEraseROP, true );
	}
}

void CStdBoard::setPieces()
{
	wp=QPixmap((const char**)wp_xpm);
	wn=QPixmap((const char**)wn_xpm);
	wb=QPixmap((const char**)wb_xpm);
	wr=QPixmap((const char**)wr_xpm);
	wq=QPixmap((const char**)wq_xpm);
	wk=QPixmap((const char**)wk_xpm);
	bp=QPixmap((const char**)bp_xpm);
	bn=QPixmap((const char**)bn_xpm);
	bb=QPixmap((const char**)bb_xpm);
	br=QPixmap((const char**)br_xpm);
	bq=QPixmap((const char**)bq_xpm);
	bk=QPixmap((const char**)bk_xpm);

  FIELD_SIZE=bk.width();
  PO=(int)(FIELD_SIZE*0.1);
	FIELD_SIZE+=PO*2;

	setupBoard();
}

QString CStdBoard::loadRsc()
{
	// TODO width=height, all present
	// load bitmaps

	QString pPath = g_root + "/share/deska/pieces/"+g_pieces+"/";

	if(!wp.load(pPath+"wp.xpm"))return pPath;
	if(!wn.load(pPath+"wn.xpm"))return pPath;
	if(!wb.load(pPath+"wb.xpm"))return pPath;
	if(!wr.load(pPath+"wr.xpm"))return pPath;
	if(!wk.load(pPath+"wk.xpm"))return pPath;
	if(!wq.load(pPath+"wq.xpm"))return pPath;
	if(!bp.load(pPath+"bp.xpm"))return pPath;
	if(!bn.load(pPath+"bn.xpm"))return pPath;
	if(!bb.load(pPath+"bb.xpm"))return pPath;
	if(!br.load(pPath+"br.xpm"))return pPath;
	if(!bq.load(pPath+"bq.xpm"))return pPath;
	if(!bk.load(pPath+"bk.xpm"))return pPath;

  FIELD_SIZE=bk.width();
  PO=(int)(FIELD_SIZE*0.1);
	FIELD_SIZE+=PO*2;

	setupBoard();

	return 0;
}

/*---------------------------------------------------------*
 *	user puts down a piece																 *
 *---------------------------------------------------------*/
void CStdBoard::mouseReleaseEvent( QMouseEvent* e)
{
	int x,y, genNo;
	int to;
	bool 	undo=false,
				chngd=false;
	
	if(!m_isHolding)return;	// piece is in hand

	// destination field	
	x=e->x();
	x/=FIELD_SIZE;
	y=e->y();
	y/=FIELD_SIZE;	
	to=transX(x)+8*transY(y);
	m_isHolding=false;

	// legal move made
	if((genNo=isLegal(m_takenFrom, to)))
	{						
			// there is a move so overwrite
			int i=getCurListNO();
			moveTyp *m;
			if((m=getMove(i)))
			{
				if(m->from!=m_takenFrom||m->to!=to)
				{
					if(QMessageBox::information(this,
							"Deska",tr("Not the game move. Overwrite?"),QMessageBox::Ok,QMessageBox::Cancel)==1)				
					{
						truncate(i);
						chngd = true;
					}
					else
						undo=true;
				}				
			}
			chngd=true; // new move, append

			if(!undo)
			{
				makeMove(m_takenFrom, to);
				emit userMove(m_takenFrom, to);				

				if(chngd)emit gameChanged(); // update info
				return;										
			}
	}
	else
		undo=true;

  if(undo)
	{
		x=m_takenFrom%8;y=m_takenFrom/8;
		x=transX(x);y=transY(y);
		repaint(x*FIELD_SIZE,y*FIELD_SIZE,
				FIELD_SIZE, FIELD_SIZE, false);
	}


}

/*---------------------------------------------------------*
 *	user takes a piece																		 *
 *---------------------------------------------------------*/
void CStdBoard::mousePressEvent( QMouseEvent* e)
{
	//if(!hasFocus())setFocus();

	if(e->button()==RightButton)
	{
		emit showPopup();
		return;
	}

	int x,y;

	x=e->x();
	x/=FIELD_SIZE;
	y=e->y();
	y/=FIELD_SIZE;
	
	m_takenFrom=transX(x)+8*transY(y);
	if(getFieldContent(m_takenFrom)*getColor()>0)
	{
		m_isHolding=true;
		repaint(x*FIELD_SIZE,y*FIELD_SIZE,
					FIELD_SIZE, FIELD_SIZE, false);
	}
	else
	{
		m_isHolding=false;
		return;
	}

	generateMoves();	
}


void CStdBoard::setupBoard()
{
  resize(8*FIELD_SIZE, 8*FIELD_SIZE);
  //setFixedSize(width(),height());
}

int CStdBoard::transX(int x)
{
		if(!m_flip)return (7-x);
		return x;
}

int CStdBoard::transY(int y)
{
		if(m_flip)return (7-y);
		return y;
}

void CStdBoard::redrawBoard(QRect *s, QPaintDevice *pd)
{
	QRect r;
	if(pd==0)pd=this;
	if(s==0)r=this->geometry();else r=*s;
	QPainter p(pd);
  QBrush   brush( white );
  QPen     pen( NoPen );
	QColor   bc, wc;
	
	QRect d;
	int x,y;

	//pen.setWidth(1);

  //p.begin(this);
	
	p.setPen( pen );			
	
	bc.setNamedColor( (const char*) g_b_color);
	wc.setNamedColor( (const char*) g_w_color);

	// white
	brush.setColor ( wc );
	p.setBrush( brush );

	for(y=0;y<8;y++)
		for(x=0;x<8;x++)
		{
			d.setRect(FIELD_SIZE*x, y*FIELD_SIZE, FIELD_SIZE, FIELD_SIZE);
			if(r.intersects( d ))p.drawRect(d);
		}

	// black
  brush.setColor ( bc );	
	p.setBrush( brush );

	for(y=0;y<8;y++)
		for(x=0;x<8;x++)
   		if((x+y)%2)
			{
				d.setRect(FIELD_SIZE*x, y*FIELD_SIZE, FIELD_SIZE, FIELD_SIZE);
				if(r.intersects( d ))p.drawRect( d );
      }

	// hatched
	if(g_hatched)
	{
  	brush.setColor ( black );		
		brush.setStyle(  BDiagPattern );
		p.setBrush( brush );
		for(y=0;y<8;y++)
			for(x=0;x<8;x++)
   			if((x+y)%2)
				{
					d.setRect(FIELD_SIZE*x, y*FIELD_SIZE, FIELD_SIZE, FIELD_SIZE);
					if(r.intersects( d ))p.drawRect( d );
  	    }
	}
	
	// pieces and marks
  for(int i=0;i<=H8;i++)
  {
    d.setRect(transX(i%8)*FIELD_SIZE, transY(i/8)*FIELD_SIZE, FIELD_SIZE, FIELD_SIZE);
    if(r.intersects( d ))
    {
      drawPiece(getFieldContent(i),i, pd);		
      if(m_isHolding&&(i==m_takenFrom))drawMark(m_takenFrom, &p, black, false );
      if(m_newUpdatedPos!=-1)drawMark(m_newUpdatedPos, &p, black, false );
     }
  }

  // outline
  p.setPen(black );		
  for(y=0;y<8;y++)
  {
    p.drawLine(FIELD_SIZE*y,0,FIELD_SIZE*y+FIELD_SIZE, 0);
    p.drawLine(0, FIELD_SIZE*y,0,FIELD_SIZE*y+FIELD_SIZE);
    p.drawLine( FIELD_SIZE*y,8*FIELD_SIZE-1,FIELD_SIZE*y+FIELD_SIZE, 8*FIELD_SIZE-1);
    p.drawLine(8*FIELD_SIZE-1, FIELD_SIZE*y,8*FIELD_SIZE-1,FIELD_SIZE*y+FIELD_SIZE);
  }


  p.end();
}


void CStdBoard::paintEvent( QPaintEvent *r )
{
	setUpdatesEnabled( FALSE );
	redrawBoard((QRect*) &(r->rect()));
	setUpdatesEnabled( TRUE );
}




































































