/***************************************************************************
                          resource.h  -  description
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

#ifndef RESOURCE_H
#define RESOURCE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

///////////////////////////////////////////////////////////////////
// resource.h  -- contains macros used for commands


///////////////////////////////////////////////////////////////////
// COMMAND VALUES FOR MENUBAR AND TOOLBAR ENTRIES


///////////////////////////////////////////////////////////////////
// File-menu entries
#define ID_FILE_NEW_WINDOW          10010
#define ID_FILE_NEW                 10020
#define ID_FILE_NEW_GAME            10025
#define ID_FILE_OPEN                10030
#define ID_FILE_OPEN_RECENT         10040
#define ID_FILE_CLOSE               10050

#define ID_FILE_SAVE                10060
#define ID_FILE_SAVE_AS             10070

#define ID_FILE_PRINT               10080

#define ID_FILE_QUIT                10090
#define ID_FILE_LOAD_ENGINE         10100
#define ID_FILE_ENGINES			        10110

///////////////////////////////////////////////////////////////////
// Edit-menu entries
#define ID_EDIT_COPY                11010
#define ID_EDIT_CUT                 11020
#define ID_EDIT_PASTE               11030
#define ID_EDIT_TAGS								11040
#define ID_EDIT_FILTER							11050

///////////////////////////////////////////////////////////////////
// View-menu entries                    
#define ID_VIEW_TOOLBAR             12010
#define ID_VIEW_STATUSBAR           12020
#define ID_VIEW_LIST								12030

///////////////////////////////////////////////////////////////////
// Game-menu entries
#define ID_GAME_NEXT_MOVE						13010
#define ID_BOARD_BMP								13020
#define ID_ENGINE_LOAD              13030
#define ID_BOARD_OPTIONS            13040
#define ID_BOARD_FLIP		            13045
#define ID_GAME_PREV_MOVE						13050
#define ID_GAME_GOTO_START					13080
#define ID_GAME_PLAY_AUTO						13090

///////////////////////////////////////////////////////////////////
// Engine-menu entries
#define	ID_ENGINE_ANALYZE						14010
#define	ID_ENGINE_GO								14020
#define	ID_ENGINE_POST							14030
#define	ID_ENGINE_MOVE_NOW					14040
#define	ID_ENGINE_PLAY							14050
#define	ID_ENGINE_CLOSE							14060
#define	ID_ENGINE_STOP							14070
#define	ID_ENGINE_LEVEL							14080
#define	ID_ENGINE_WHITE							14090

///////////////////////////////////////////////////////////////////
// Gamelist-menu entries
#define	ID_GAMELIST_PREVIEW					15010
#define	ID_GAMELIST_MOREBOARDS			15020


///////////////////////////////////////////////////////////////////
// Help-menu entries
#define ID_HELP_CONTENTS            1002

///////////////////////////////////////////////////////////////////
// General application values
#define ID_STATUS_MSG               1001

#define IDS_STATUS_DEFAULT          "Ready."

#endif // RESOURCE_H
