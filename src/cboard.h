/***************************************************************************
                          cboard.h  -  description
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

#ifndef CBOARD_H
#define CBOARD_H

#include <qwidget.h>
#include <qpixmap.h>
#include <qptrlist.h>

#include "global.h"

/**
  * Implements all non visual functions
  * of a chess board like movegenerator and other usefull
  * functions. Stores the current position, all possible moves etc.
  * SaveLegal and generateMoes are the main functions of the movegenerator.
  * IsAttacked, isChecked, isEnd and so on are some usefull functions of the movegenerator.
  * The constructor sets the default chess position. Call analyzePGN with a valid FEN string
  * for another starting position. AnalysePGN expects the game notation (QString) as another
  * parameter.
  */

class CBoard : public QWidget
{
  Q_OBJECT
public: 
  CBoard( QWidget *parent=0, const char *name=0);
  ~CBoard();

  /** Reads a PGN-game into internal structures. Comments are
      ignored. */  
  bool analyzePGN(QString, QString FEN=0);	
  /** Gets the previous move from the move list. */	
  void prevMove(); 									
  /** Gets the next move from the move list. */
  bool nextMove();													
  /** Goto the begining of the current game. */
  void gotoStart();
  /** Resets the internal structures to the initial chess position. 
      Clears the move list. */
  void initStartPosition(QString FEN=0);

  /** Makes a SAN-move and saves it in the game list. 
      Used by analyzePGN to check legality of moves from a PGN file. */
  bool moveFromText(char*, bool show=false);
  /** Saves a move in the game list and makes it on the internal board. */
  void makeMove(int,int, bool show=true);
  /** Truncates the move list. 
      Deletes all moves which follow after the current move. */
  void	truncate(int);
  /** Gets the current half-move number. */
  int getCurListNO();
  /** Generates all possible moves for the current position.
      Saves them in the legalMoves-list. */
  void generateMoves();
  /** Returns the comment for a move. */
  QString getComment(int nr);
  /** Returns the move for a half-move number. */
  moveTyp* getMove(int nr);
  /** Checks if the  field is attacked by the oponent. 
      \param  field: e.g A1      
      \param  color: Color of the attacked piece e.g. WHITE
      \return 0 if not attacked 
  */
  int isAttacked(int,int);
  /** Check if this move is in the legalMoves-list. 
      \param  from: Start field
      \param  to: Destination field
      \return 0 if not a legal move
  */
  int isLegal(int,int);
  /** Checks for mate and stalemate. */
  int isEND(int);
  /** Checks if the king is checked after this move. 
      \param  from: Start field e.g A1
      \param  to: Destination field e.g. A8
      \param  color: Color of the king (WHITE or BLACK)
      \return 0 if not checked
  */  
  int isChecked(int,int,int);

  // inline
  /** Gets current color to move. */
  int  getColor(){return color;}
  /** Gets the current game position. */
  int*	getPos(){return field;}
  /** Gets the current half-move number. */	
  int getMoveNr(){return gameList.at();}
  /** Checks if moves were made from a start position. */
  bool isInitialPos(){if(moveCount!=1||color!=startColor)return false;else return true;}
  moveTyp* getCurMove(){return gameList.current();}
  QString* getCurComment() {return commentList.current();}
  void setCurComment(const QString s) {(*commentList.current())= s.copy();}//deep copy
  QList<moveTyp> getList(){return gameList;}
  void setList(QList<moveTyp> list){gameList = list;}
  int getFieldContent(int i){return field[i];}
  int getKingPos(int c){return kingPos[c];}

private:
  int saveLegal(int,int,int);

  int field[H8+1];      // the internal chess board
  int startPos[H8+1];   // initial position
  int moved[H8+1];      // used for right to castle
  int kingPos[2];       // to avoid searching for the king, used by isChecked
  int startColor;
  int listCount;
  int epField;
  int color;
  int moveCount;
  int SP;
  QPtrList<moveTyp> legalMove;   // all legal moves in the current position
  QPtrList<moveTyp> gameList;		// moves for the current game
  QPtrList<QString> commentList; // list for move comments

signals:
  /** Emits a field if it changes its value. 
      If a piece is moved this signal is emitted
      for the start and the destination field. */
  void fieldChanged(int);      // redraw the field

};

#endif





















































































































































































