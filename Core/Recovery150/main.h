/*
    6.39 TN-A, XmbControl
    Copyright (C) 2011, Total_Noob
    Copyright (C) 2011, Frostegater

    main.h: XmbControl main header file
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __MAIN_H__
#define __MAIN_H__

#include <psptypes.h>

typedef struct
{
    u32 magic;
    int clock_game;
    int clock_vsh;
    int mscache;
    int skiplogos;
    int hidepics;
    int hidemac;
    int noled;
    int noumd;
    int noanalog;
    int vitamute;
    int vshregion;
} CFWConfig;


void loadSettings();
void saveSettings();
void settings_submenu();

void loadPlugins();
void savePlugins();
void plugins_submenu();

#endif
