/*
 * This file is part of PRO CFW.

 * PRO CFW is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * PRO CFW is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PRO CFW. If not, see <http://www.gnu.org/licenses/ .
 */

/*
    PSP VSH
*/

#include <cfwmacros.h>

#include "clock.h"
#include "common.h"
#include "vsh.h"


#define ROLL_OVER(val, min, max) ( ((val) < (min)) ? (max): ((val) > (max)) ? (min) : (val) )



void change_bg_color(int dir) {
    vsh_Menu *vsh = vsh_menu_pointer();
    
    int sel = vsh->config.ark_menu.vsh_bg_color;
    sel = ROLL_OVER(sel+dir, 0, 28);
    vsh->config.ark_menu.vsh_bg_color=sel;
}

void change_fg_color(int dir) {
    vsh_Menu *vsh = vsh_menu_pointer();
    
    int sel = vsh->config.ark_menu.vsh_fg_color;
    sel = ROLL_OVER(sel+dir, 0, 28);
    vsh->config.ark_menu.vsh_fg_color=sel;
}

void change_font(int dir) {
    vsh_Menu *vsh = vsh_menu_pointer();
    
    int sel = vsh->config.ark_menu.vsh_font;
    sel = ROLL_OVER(sel+dir, 0, 59);
    vsh->config.ark_menu.vsh_font=sel;
}

void change_design(int dir) {
    vsh_Menu *vsh = vsh_menu_pointer();
    
    int sel = vsh->config.ark_menu.window_mode;
    sel = ROLL_OVER(sel+dir, 0, 1);
    vsh->config.ark_menu.window_mode = sel;
}
void change_menu(int dir) {
    vsh_Menu *vsh = vsh_menu_pointer();
    
    int sel = vsh->config.ark_menu.advanced_vsh;
    sel = ROLL_OVER(sel+dir, 0, 1);
    vsh->config.ark_menu.advanced_vsh = sel;
}

void change_bool_option(int *p, int direction) {
    int sel = *p;

    sel = ROLL_OVER(sel+direction, 0, 1);
    *p=sel;
}
