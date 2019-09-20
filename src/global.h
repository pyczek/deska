/***************************************************************************
                          global.h  -  description
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
#ifndef __GLOBAL_H_
#define __GLOBAL_H_

#include <qstring.h>
#include <qstrlist.h>
#include <qptrlist.h>

/*---------------------------------------------------------*
 *	move generator definitions                             *
 *---------------------------------------------------------*/

/* stores position of a game */
typedef struct
{
	int header;
	int notation;
	int nextgame;
}gamePosTyp; 
 
/* stores a PGN-game */
typedef struct
{
	QString header;
	QString notation;
}gameTyp;

//class moveTyp;
/* stores a move, and a pointer to a Variation */
/*class moveTyp
{
	public:
		int from;
		int to;
		int flag;
		int	piece;
};
*/

/* stores a move */
typedef struct
{
	int from;
	int to;
	int flag;
	int	piece;
}moveTyp;

const int MATE       = 1;
const int STALEMATE  = 0;

const int EMPTY		   = 0;
const int PAWN		   = 1;
const int KNIGHT		 = 2;
const int BISHOP		 = 4;
const int ROOK		   = 8;
const int QUEEN		   = 16;
const int KING		   = 32;
const int PIECE_MASK = 63;
const int EP		     = 64;
const int QUEENING   = 128;
const int KNIGHTING  =  64;
const int BISHOPING  = 128;
const int ROOKING    = 192;

const int VAL_KING   = 255;
const int VAL_QUEEN  = 9;
const int VAL_ROOK   = 5;
const int VAL_BISHOP = 3;
const int VAL_KNIGHT = 3;
const int VAL_PAWN   = 1;

const int BLACK = -1;
const int WHITE =  1;

const int ILLEGAL = -1;

//	fields									
////////////////////////////////////////////////////////////			
const int A1 = 0;
const int B1 = 1;
const int C1 = 2;
const int D1 = 3;
const int E1 = 4;
const int F1 = 5;
const int G1 = 6;
const int H1 = 7;

const int A8 = 56;
const int B8 = 57;
const int C8 = 58;
const int D8 = 59;
const int E8 = 60;
const int F8 = 61;
const int G8 = 62;
const int H8 = 63;

/*---------------------------------------------------------*
 *	settings                                               *
 *---------------------------------------------------------*/

#define SUPER_GRAY	0xdfdfdf

extern int g_xpos, g_ypos, g_width, g_height;
extern int MAX_TAGS;
extern QString *g_tags, g_7tags, g_header;
extern QString g_root, g_pieces;
extern int g_hatched;
extern QString g_w_color, g_b_color;
extern int g_transparent, g_frame;
extern QString g_notation;	
extern QStrList g_recentEngine, g_fileName, g_activeEngine;

void loadSettings();
void saveSettings();
void setStdSettings();
QString stripComment(QString src);
QString stripVariation(QString src);
QString extractTag(QString s, QString tag);
QString extractFilename(QString path);
void setTags();

#endif






















