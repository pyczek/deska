/***************************************************************************
                          cstdboard.h  -  description
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

#ifndef STDBOARD_H
#define STDBOARD_H

#include <qpaintdevice.h>

#include "cboard.h"

/**
  * Graphical representation of the internal board CBoard.
  * Looks for piece sets in DESKAROOT + "/share/deska/pieces/". If no pieces sets can be found,
  * the default set will be used.
  * Override loadRsc to use an other default piece set.
  *
  */

class CStdBoard : public CBoard  {
	Q_OBJECT
public: 
	CStdBoard(QWidget *parent=0, const char *name=0);
	~CStdBoard();
	QPixmap wp, wn, wb, wr, wk ,wq,
					bp, bn, bb, br, bk, bq;
	int FIELD_SIZE;
	int PO;

	QString loadRsc();
	void setupBoard();
	void setPieces();
	void flipBoard(){m_flip=!m_flip;}
	void redrawBoard(QRect*, QPaintDevice *pd=NULL);

protected:
	void paintEvent( QPaintEvent * );
	void mouseReleaseEvent( QMouseEvent* );
	void mousePressEvent( QMouseEvent* );

private:
  int m_newUpdatedPos, m_oldUpdatedPos;
	int 	m_takenFrom;
	bool m_isHolding;
	int	m_flip;
	void drawPiece(int piece,int pos, QPaintDevice *pd=NULL);
	int transY(int);
	int transX(int);
	void drawMark(int, QPainter*, QColor, bool solid=true);

private slots:
	void slotUpdateField(int);

signals:
	void	gameChanged();
	void	addMove();
	void	showPopup();
	void	userMove(int, int);

};

#endif




























