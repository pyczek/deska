/***************************************************************************
                          cboard.cpp  -  description
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

#include <stdlib.h> 
 
#include <qregexp.h>
#include <qmessagebox.h>

#include "cboard.h"
#include "global.h"


// offset for movegen
int pawnOffset[]={7,8,9,16};
int rookOffset[]={-1,8,1,-8};
int bishopOffset[]={-9,7,9,-7};
int knightOffset[]={-17,-10,6,15,17,10,-6,-15};	

CBoard::CBoard( QWidget *parent,const char *name):
	QWidget(parent,name)
{
	gameList.setAutoDelete(true);
	initStartPosition();
}

CBoard::~CBoard()
{
}

bool CBoard::analyzePGN(QString notation, QString FEN)
{

	QRegExp r("[PNBRQK]?[a-h]?[1-8]?[x-]?[a-h][1-8]?[+#NBRQ]?");	
	QString s, tag, err, c, comment;
	int l, i=0, p, h;
	
	initStartPosition(FEN);
		
	s=notation;	
	l=s.length();
  tag="";
	i=0;

	commentList.append(new QString());								
	while(i<l)
	{
		c=s.mid(i,1);
		if(c=="{")
		{
			p=s.find("}",i);
			if(p!=-1)
			{
				comment=s.mid(i+1,p-i-1);
				comment=comment.simplifyWhiteSpace();				
				i=p;
			}		
		}//end comment
		if(c=="(")//Variations not yet supported, show as comment
		{
			//recursive variations
			p=i;
			h=i;
			do
			{
				p=s.find(")",p+1);
				h=s.find("(",h+1);
			}
			while(h!=-1&&h<p);

			if(p!=-1)
			{
				comment=s.mid(i+1,p-i-1);
				comment=comment.simplifyWhiteSpace();				
				i=p;
			}		
		}//end variation
		else
		if(c==" ")
		{
      tag=tag.stripWhiteSpace();
			if(!tag.isEmpty())
			{				
				p=0;
				// TODO: add castling to RegExp
        if(((p=r.match(tag))!=-1)||
					  ((p=tag.find("O-O-0"))!=-1)||
						((p=tag.find("O-O"))!=-1)||
						((p=tag.find("o-o-o"))!=-1)||
						((p=tag.find("o-o"))!=-1)||
						((p=tag.find("O-O-O"))!=-1)||
						((p=tag.find("O-O"))!=-1))
				{	
					//comment for the previous move
					//if new move made, comment will be added
					//commentList.append(new QString(comment));								
					setCurComment(comment);

					if(p>0)tag=tag.mid(p,tag.length()-p);
					// try to make this move			

					char *tagc = new char[tag.length()+1];
					strcpy(tagc, (const char*) tag);
					if(!moveFromText(tagc))
					{
						//err=err.setNum(i);
						//err+=".";
						//if(color==BLACK)err+="..";
						err+=tag;
						QMessageBox::information(0,"Deska",tr("Illegal move: ") + err + "\n" + tr("Please edit and try again."));				
						return false;
					}								

					comment="";							
				}
			}									
			tag="";			
		}		
		tag+=c;	
		i++;
	}
	//append comment for the last move
	//commentList.append(new QString(comment));								
	setCurComment(comment);

	//changed in makeMove
	gotoStart();


  emit fieldChanged(-1);//clear marks
}

//remove remaining moves
void CBoard::truncate(int i)
{
	int k=gameList.at();
	gameList.last();
	while(gameList.at()>i)gameList.removeLast();	
	gameList.at(k);
}

//set the starting position from a FEN-string
void CBoard::initStartPosition(QString FEN)
{
	int i;

	epField=ILLEGAL;
	listCount=ILLEGAL;
	moveCount=1;
	
	startColor=color=WHITE;
	
	for(i=0;i<=H8;i++)
	{
		field[i]=EMPTY;
		moved[i]=EMPTY;
	}			
	
	if(FEN)
	{
		QString c;		
		int n, pos,row=0, l;

		// position
		l=FEN.find(" ");
		if(l==-1)QMessageBox::information(0,"Deska", tr("Error"));							
		pos=(7-row)*8;	
		for(i=0;i<=l;i++)
		{
			c=FEN.mid(i,1);
			
			if((n=c.toInt())) // empty fields
			{
				pos+=n;				
			}		
			else  // pieces
			{
				if(c=="/")
				{
					row++;
					pos=(7-row)*8;	
				}
				else
				{					
					//TODO: c as char and switch
					if(c=="P")field[pos]=PAWN;
					else if(c=="p")field[pos]=-PAWN;
					else if(c=="N")field[pos]=KNIGHT;
					else if(c=="n")field[pos]=-KNIGHT;
					else if(c=="B")field[pos]=BISHOP;
					else if(c=="b")field[pos]=-BISHOP;
					else if(c=="R")field[pos]=ROOK;
					else if(c=="r")field[pos]=-ROOK;
					else if(c=="Q")field[pos]=QUEEN;
					else if(c=="q")field[pos]=-QUEEN;
					else if(c=="K"){kingPos[0]=pos;field[pos]=KING;}
					else if(c=="k"){kingPos[1]=pos;field[pos]=-KING;}
					pos++;
				}
			}
		}
		// color		
		l=FEN.find(" ");	
		if(l==-1)QMessageBox::information(0,"Deska",tr("Error"));							
		l++;
		c=FEN.mid(l,1);
		c=c.upper();
		if(c=="W")
			startColor=color=WHITE;
		else
			startColor=color=BLACK;		

		// TODO
		// castling
		// EP
		// Halfmove clock
		// Fullmove number	
	}	
	else
	{
		field[A1]=ROOK;field[B1]=KNIGHT;field[C1]=BISHOP;field[D1]=QUEEN;
		field[E1]=KING;field[F1]=BISHOP;field[G1]=KNIGHT;field[H1]=ROOK;
		field[8]=PAWN;field[9]=PAWN;field[10]=PAWN;field[11]=PAWN;
		field[12]=PAWN;field[13]=PAWN;field[14]=PAWN;field[15]=PAWN;
	
		field[A8]=-ROOK;field[B8]=-KNIGHT;field[C8]=-BISHOP;field[D8]=-QUEEN;
		field[E8]=-KING;field[F8]=-BISHOP;field[G8]=-KNIGHT;field[H8]=-ROOK;
		field[48]=-PAWN;field[49]=-PAWN;field[50]=-PAWN;field[51]=-PAWN;
		field[52]=-PAWN;field[53]=-PAWN;field[54]=-PAWN;field[55]=-PAWN;
		
		kingPos[0]=E1;
		kingPos[1]=E8;
	}

	for(i=0;i<=H8;i++)startPos[i]=field[i];
	
	gameList.clear();
	commentList.clear();
}


//end of game
int CBoard::isEND(int lColor)
{
	// mate if no moves and
	// king attacked
	if(isAttacked(kingPos[abs((lColor-1)/2)],lColor))
		return MATE;

	return STALEMATE;
}

//king attacked
int CBoard::isChecked(int from,int to,int lColor)
{
	int lKPos,h,illegalMove=0;

	lKPos=kingPos[abs((lColor-1)/2)];
	
	if(field[from]==KING*lColor)lKPos=to;
	
	h=field[to];
	field[to]=field[from];
	field[from]=EMPTY;

	if(isAttacked(lKPos,lColor))
		illegalMove=1;
	
	field[from]=field[to];
	field[to]=h;
	
	return illegalMove;
}

//if the move is possible save it as a legal move
int CBoard::saveLegal(int from,int last, int to)
{	
	int xDif,yDif;
	int toVal,lastVal,fromVal;		

	fromVal=field[from];
	lastVal=field[last];
	toVal=field[to];

	xDif=abs(last%8-to%8);
	yDif=abs(last/8-to/8);
	
	switch (fromVal*color)
	{
		case KNIGHT:				
			if
			(
				(xDif!=2&&xDif!=1)||					
				(yDif!=2&&yDif!=1)||
				(to>H8)||(to<A1)||						
				(toVal*fromVal>0)	
			)						
				return 0;
					
				
		break;

		case PAWN:			
			if
			(
				(yDif>2)||(xDif>1)||
				((xDif==1)&&(yDif!=1))||
				((xDif==1)&&(yDif==1)&&(toVal*color>=0)&&(to!=epField))||
				((xDif==0)&&((toVal*color)!=0))||
				((yDif==2)&&((from/8*2+5*color)!=7))||
				((yDif==2)&&((field[to-8*color])!=0))||
				(to>H8)||(to<A1)||
				(toVal*fromVal>0)
			)
				return 0;
			break;

		case KING:
			if
			(				
				/* TODO!!! */
			 	((abs(from%8-to%8)>1)&&(from!=(32-28*color)))||
				((abs(from%8-to%8)>1)&&(moved[from]!=0))|| /* king moved */
				(((from%8-to%8)>1)&&(moved[28-28*color]!=0))||	/* short rook moved */
				(((from%8-to%8)>1)&&(field[28-28*color]!=ROOK*color))||	/* not short rook */
				(((from%8-to%8)<-1)&&(moved[35-28*color]!=0))||	/* long rook moved */
				(((from%8-to%8)<-1)&&(field[35-28*color]!=ROOK*color))||	/* no long rook */
				(((from%8-to%8)<-1)&&(field[34-28*color]!=0))||	
				(abs(from%8-to%8)>2)||
				(abs(from/8-to/8)>1)||
				(to>H8)||(to<A1)||
				(toVal*fromVal>0)||	
				(lastVal*color<0)				
			)											
				return 0;		

				if(isAttacked(to,color)) return 0;
			
			break;

		default:		
			if
			(
				(xDif>1)||
				(yDif>1)||
				(to>H8)||(to<A1)||
				(toVal*fromVal>0)||	
				(lastVal*color<0)				
			)											
				return 0;					
	}
	

	moveTyp *move=new moveTyp;
	move->from=from;
	move->to=to;
	move->flag=toVal;

	legalMove.append(move);
	
	SP++;			

	return 1;
}

//generate all possible moves
void CBoard::generateMoves()
{
	
	int i,k;
	int from,to;

	SP=0;
	legalMove.clear();

	for (i=A1;i<=H8;i++)		
	{
		from=i;		
		
		switch(field[i]*color)
		{
			case QUEEN:				
				for(k=0;k<=3;k++)
				{
					to=from;
					while(saveLegal(from,to,to + rookOffset[k]))
						to+=rookOffset[k];
				}
				for(k=0;k<=3;k++)
				{
					to=from;
					while(saveLegal(from,to,to + bishopOffset[k]))
						to+=bishopOffset[k];
				}
			break;

			case KING:				
				for(k=0;k<=3;k++)
				{					
					to=from;
					while(saveLegal(from,to,to + rookOffset[k]))
						to+=rookOffset[k];
				}
				for(k=0;k<=3;k++)
				{
					to=from;
					saveLegal(from,to,to + bishopOffset[k]);
				}

			break;

			case BISHOP:
				for(k=0;k<=3;k++)
				{
					to=from;
					while(saveLegal(from,to,to + bishopOffset[k]))
						to+=bishopOffset[k];					
				}

			break;

			case ROOK:
				for(k=0;k<=3;k++)
				{
					to=from;
					while(saveLegal(from,to,to + rookOffset[k]))
						to+=rookOffset[k];						
				}
			break;

			case KNIGHT:
				for(k=0;k<=7;k++)
				{
					to=from;					
					saveLegal(from,to,to + knightOffset[k]);
										
				}
			break;

			case PAWN:	
				for(k=0;k<=3;k++)
				{
					to=from;
					saveLegal(from,to,to + pawnOffset[k]*color);					
				}
			break;
			
		}
	}
	
	if(SP==0)
	{
		switch(isEND(color))
		{
			case MATE:
				if(color==WHITE)
					;//m_memo->setText("White is allready MATE!");
				else
					;//m_memo->setText("Black is allready MATE!");
				break;
				
			default:
					;//m_memo->setText("STALEMATE!");

		}
	}
}

/* returns number greater 0 if a legal move */	
int CBoard::isLegal(int qFrom, int qTo)
{	
/*	int i;
	
	for(i=0;i<SP;i++)
		if((legalMove[i].from==qFrom)&&(legalMove[i].to==qTo))	
			// move ok, but is king checked after move?
			if(!isChecked(qFrom,qTo,abs(field[qFrom])/field[qFrom]))
				return 1;*/

	moveTyp *move;
	for(move=legalMove.first(); move!=0; move=legalMove.next())
		if(move->from==qFrom&&move->to==qTo)
			if(!isChecked(qFrom, qTo, abs(field[qFrom])/field[qFrom]))
				return legalMove.at()+1;

	return 0;
}

//the field is attacked by oponent
int CBoard::isAttacked(int pos,int lColor)
{
	int i,dest,oldDest,xDif,yDif;

		
	/* knight */
	for(i=0;i<=7;i++)
	{
		dest=pos+knightOffset[i];
		if((dest<=H8)&&(dest>=A1))
		{
			// 24.05.1999
			// hyper knight			
			xDif=abs(pos%8-dest%8);
			yDif=abs(pos/8-dest/8);
			if((xDif!=2&&xDif!=1)||(yDif!=2&&yDif!=1))continue;
			// --			

			if(field[dest]==(-lColor*KNIGHT))return 1;
		}
	}

	/* king */
	for(i=0;i<=3;i++)
	{
		dest=pos+rookOffset[i];
		if((dest<=H8)&&(dest>=A1))
			if(field[dest]==(-lColor*KING))
				return 1;
	
		dest=pos+bishopOffset[i];
		if((dest<=H8)&&(dest>=A1))
			if(field[dest]==(-lColor*KING))
					return 1;
	}

	/* rook, bishop and queen */
	for(i=0;i<=3;i++)
	{	
		oldDest=pos;
		dest=pos+rookOffset[i];
		for(;;)
		{
			if((dest<=H8)&&(dest>=A1))
			{
				xDif=abs(oldDest%8-dest%8);
				yDif=abs(oldDest/8-dest/8);
					
				if((xDif>1)||(yDif>1))break;
				
				if((field[dest]==(-lColor*ROOK))||(field[dest]==(-lColor*QUEEN)))return 1;				
				if((field[dest]!=EMPTY)&&(field[dest]!=KING*lColor)) break;
			}
			else
				break;
			
			oldDest=dest;
			dest+=rookOffset[i];
		}	
	
		oldDest=pos;
		dest=pos+bishopOffset[i];
		for(;;)
		{
			xDif=abs(oldDest%8-dest%8);
			yDif=abs(oldDest/8-dest/8);
					
			if((xDif>1)||(yDif>1))break;

			if((dest<=H8)&&(dest>=A1))
			{	
				if((field[dest]==(-lColor*BISHOP))||(field[dest]==(-lColor*QUEEN)))
					return 1;
				if((field[dest]!=EMPTY)&&(field[dest]!=KING*lColor)) break;
			}
			else
				break;

			oldDest=dest;
			dest+=bishopOffset[i];

		}		
	}

	/* pawn */
	dest=pos+lColor*7;

	if((dest<=H8)&&(dest>=A1))
		if(abs(dest%8-pos%8)<2)
			if(field[dest]==(-lColor*PAWN))
				return 1;

	dest=pos+lColor*9;
	if((dest<=H8)&&(dest>=A1))
		if(abs(dest%8-pos%8)<2)
			if(field[dest]==(-lColor*PAWN))
				return 1;
	
	return 0;	
}

//goto start of the game
void CBoard::gotoStart()
{
	int i;
	
	moveCount=1;	
  color=startColor;	
	gameList.first();
	commentList.first();
	
	for(i=0;i<=H8;i++)
	{
		moved[i]=EMPTY;
		field[i]=startPos[i];
	}
}

//make a move
void CBoard::makeMove(int from,int to, bool show)
{
	//extract to and flag from to
	int queenpiece=to&(255-PIECE_MASK);
	to&=PIECE_MASK;

	int piece=field[from], SANpiece=piece;
	int to_x,to_y,from_x,from_y;
	int lColor=abs(piece)/piece;	
	int listNO=getCurListNO();
	int captPiece=(abs(field[to])&PIECE_MASK);
	int dest;
	
	from_x=from%8;from_y=from/8;
	to_x=to%8;to_y=to/8;
	
	/* add for EP-move */
	if((abs(piece)==PAWN)&&(abs(to_x-from_x)==1))
	{
		dest=to_x+from_y*8;		
		
		if(to==epField)//epField set by saveLegal
		{
			if((dest>=A1)&&(dest<=H8))
			{
				field[dest]=EMPTY;
	
				if(show)emit fieldChanged(dest);
			}
			captPiece=PAWN;
			captPiece|=EP;	//EP-mark stored in moveTyp.flag

		}
	}
	
	/* clear old EP-field */
	epField=ILLEGAL;
	
	/* add for quuening */
	if((abs(piece)==PAWN)&&((to/8*2)==(7+7*lColor)))
	{				
		switch(queenpiece)
		{
			case ROOKING: piece=ROOK; break;	//check rooking first!
			case KNIGHTING: piece=KNIGHT; break;
			case BISHOPING: piece=BISHOP; break;			
			default: piece=QUEEN;
		}
		piece*=lColor;
		captPiece|=QUEENING;	//needed for prevMove
	}

	/* make normal move */
	field[to]=piece;
	if(field[to]==KING)kingPos[0]=to;
	if(field[to]==-KING)kingPos[1]=to;
	field[from]=EMPTY;
	
	// show move
	if(show)
	{
		emit fieldChanged(from);
		emit fieldChanged(to);
	}

	/* save move in list */		
	//don't forget queening information
	to+=queenpiece;		//needed for makeMove
	moveTyp *move = new moveTyp;		
	move->from=from;
	move->to=to;
	move->flag=captPiece;	
	move->piece=SANpiece;

	//move with listNO (0 to n-1) available
	//overwrite it
	if((uint)listNO<gameList.count())
		gameList.remove(listNO);
	gameList.insert(listNO, move);
	gameList.next();
	if(!commentList.next())
		commentList.append(new QString());

	/* add for castling, move the rook */
	if((abs(piece)==KING)&&(abs(to_x-from_x)>1))
	{
	
		if(to==C1){to=D1;from=A1;}
		else if(to==C8){to=D8;from=A8;}
		else if(to==G1){to=F1;from=H1;}
		else if(to==G8){to=F8;from=H8;}
		
		from_x=from%8;from_y=from/8;to_x=to%8;to_y=to/8;
		piece=ROOK*lColor;
		
		field[to]=piece;
		field[from]=EMPTY;
	
		if(show)
		{
			emit fieldChanged(from);
			emit fieldChanged(to);	
		}
	}
	
	/* save new en passant mark */
	if((abs(piece)==PAWN)&&(abs(to_y-from_y)>1))
	{
		epField=to-8*lColor;
	}

	color*=-1;
	if(color==WHITE)moveCount++;	
	if(listCount<listNO)listCount=listNO;
	
	moved[from]++;
	moved[to]++;
}

//undo the last move
void CBoard::prevMove()
{	
	int dest;
	int piece;
	int to_x,to_y,from_x,from_y;
	int lColor;	
	int listNO=getCurListNO()-1;
	int from,to,captPiece;
	int flag;
		
	if(((listNO<0)||(listCount==ILLEGAL)))//||((listNO==0)&&(startColor==BLACK)))
	{
		//showStatus("You are allready at the beginning of the move list.",1);
		//TODO:WarningBeep();
		return;
	}

	moveTyp *move = gameList.at(listNO);
	if(commentList.count()>(uint)listNO)commentList.at(listNO);

	from=move->from;
	to=move->to;	
	to&=PIECE_MASK;
	piece=field[to];
	lColor=abs(piece)/piece;
	captPiece=move->flag&PIECE_MASK;
	flag=move->flag&(255-PIECE_MASK);

	from_x=from%8;from_y=from/8;
	to_x=to%8;to_y=to/8;
		
	/* clear old EP-field */
	epField=ILLEGAL;
	
	/* en passant, upate EP-field */
	if((flag&EP)==EP)
	{
		dest=to_x+from_y*8;		
		
		if((dest>=A1)&&(dest<=H8))
		{
			captPiece*=-lColor;
			field[dest]=captPiece;
			
			emit fieldChanged(dest);

			captPiece=EMPTY;
			epField=to;
		}
	}

	/* quuening, restore the pawn */
	if((flag&QUEENING)==QUEENING)piece=PAWN*lColor;
		
	// captured piece opposite color
	captPiece*=-lColor;
	
	//normal move
	field[to]=captPiece;
	field[from]=piece;
	if(field[from]==KING)kingPos[0]=from;
	if(field[from]==-KING)kingPos[1]=from;

	emit fieldChanged(from);	
	emit fieldChanged(to);	
	
	/* castling, move the rook */
	if((abs(piece)==KING)&&(abs(to_x-from_x)>1))
	{
		if(to==C1){to=A1;from=D1;}
		else if(to==G1){to=H1;from=F1;}
		else if(to==C8){to=A8;from=D8;}
		else if(to==G8){to=H8;from=F8;}
		
		from_x=from%8;from_y=from/8;to_x=to%8;to_y=to/8;
		piece=ROOK*lColor;
		
		field[to]=piece;
		field[from]=EMPTY;
	
		emit fieldChanged(from);	
		emit fieldChanged(to);	
	}
		
	color*=-1;

	if(color==BLACK)moveCount--;
	
	moved[from]--;
	moved[to]--;

}

//return comment for a move no.
QString CBoard::getComment(int nr)
{
	QString *s;
	int i;

	i=commentList.at();
	if((uint)nr<commentList.count())
		s=commentList.at(nr);
	else
		return 0;

	if(i!=-1)commentList.at(i);//do not change position

	return *s;
}


//return a move for move no.
moveTyp* CBoard::getMove(int nr)
{
	moveTyp *m;
	int i;

	i=gameList.at();

	if((uint)nr<gameList.count())
		m=gameList.at(nr);
	else
		return 0;

	if(i!=-1)gameList.at(i);

	return m;
}

//return gameList counter
int CBoard::getCurListNO()
{
	int listNO;

	if(color==BLACK)
		listNO=moveCount*2-1;
	else
		listNO=moveCount*2-2;

	if(startColor==BLACK)listNO--;

	return listNO;
}

//make next move from the gameList
bool CBoard::nextMove()
{
	int listNO=getCurListNO();

	if((listNO>listCount)||(listCount==ILLEGAL))
	{
		//showStatus("You are allready at the end of the move list.",1);
		//TODO:WarningBeep();
		return false;
	}
	
	//TODO:showStatus(moveToStr(buf,gameList[listNO].from,gameList[listNO].to,gameList[listNO].piece),1);
	moveTyp *move;

	if(!(move=gameList.at(listNO)))return false;

	makeMove(move->from,move->to);

	return true;
}


/*---------------------------------------------------------*
 *		translates a string to internal move notation        *
 *		makes move if possible                               *
 *---------------------------------------------------------*/
bool CBoard::moveFromText(char move[10],bool show)
{
	int i;
	bool legal;
	int to=ILLEGAL,
	    to_x=ILLEGAL,
	    to_y=ILLEGAL,
	    piece=ILLEGAL,
	    from_x=ILLEGAL,
	    from_y=ILLEGAL,
	    Ocount=0,
	    queening=0;

	for(i=strlen(move)-1;i>=0;i--)
	{
		if(piece==ILLEGAL)
		switch(move[i])
		{
			case 48:Ocount++;break;
			case 79:Ocount++;break;
			case 111:Ocount++;break;
			case 78:piece=KNIGHT;break;
			case 66:piece=BISHOP;break;
			case 82:piece=ROOK;break;
			case 81:piece=QUEEN;break;
			case 75:piece=KING;break;
		}
			
		if ((piece!=ILLEGAL)&&(to_x==ILLEGAL))queening=piece;
		
		if ((move[i]>=49)&&(move[i]<=56))
			if(to_y==ILLEGAL)
				to_y=move[i]-49;
			else
				from_y=move[i]-49;
		
		if ((move[i]>=97)&&(move[i]<=104))
			if(to_x==ILLEGAL)
				to_x=move[i]-97;
			else
				from_x=move[i]-97;		
	}
	
	generateMoves();
	to=to_x+to_y*8;
	
	// castling
	if(Ocount==2)
	{
		piece=KING;
		if(color==WHITE)
			to=G1;
		else
			to=G8;
	}
	if(Ocount==3)
	{
		piece=KING;
		if(color==WHITE)
			to=C1;
		else
			to=C8;
	}
	
	if(queening)piece=PAWN;
	
	// compare moves on stack with current move
	legal=false;

	moveTyp *lMove;
	for(lMove=legalMove.first(); lMove!=0; lMove=legalMove.next())
	{
		// found dest field or pawn and no to-row
		if((lMove->to==to)||((abs(field[lMove->from])==PAWN)&&to_y==ILLEGAL&&to_x==lMove->to%8))
		{
			if((piece==ILLEGAL)||(abs(field[lMove->from])==piece))
			{
				// no piece in SAN or not from-to is ILLEGAL
				if((piece==ILLEGAL)&&(abs(field[lMove->from])!=PAWN)&&(from_x==ILLEGAL||from_y==ILLEGAL))
					continue;
	
				if((from_x!=ILLEGAL)&&((lMove->from)%8!=from_x))
					continue;
			
				if((from_y!=ILLEGAL)&&((lMove->from)/8!=from_y))
					continue;				

					// pseudo legal king moves					
					if(isChecked(lMove->from,lMove->to,color))continue;

					to=lMove->to;
					//additional queening information
					if(queening)
					{
	
						switch(queening)
						{
							case KNIGHT: to=lMove->to|KNIGHTING; break;
							case BISHOP: to=lMove->to|BISHOPING; break;
							case ROOK: to=lMove->to|ROOKING; break;			
						}

 					}

					makeMove(lMove->from,to,show);
					legal=true;
					break;
				
			}
		}
	}
	
	return legal;	

}


































































































































































































































































































































































































































































































