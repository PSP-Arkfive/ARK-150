#ifndef _ADVANCED_H
#define _ADVANCED_H


#include "vsh.h"


#define SUBITEM_DEFAULT 0

#define SUBITEM_SWAPXO 5
#define SUBITEM_SWAPXO_END 6

#define SUBITEM_PANDORA 19
#define SUBITEM_PANDORA_END 20

#define SUBITEM_UNSUPPORTED 26

#define SUBITEM_NONE 27

#define SUBITEM_COLOR 32
#define SUBITEM_COLOR_END 60


enum {
    SUBMENU_FG_COLORS,
    SUBMENU_BG_COLORS,
    SUBMENU_FONT,
    SUBMENU_MENU_DESIGN,
    SUBMENU_MAIN_MENU,
    SUBMENU_CONVERT_BATTERY,
    SUBMENU_SWAP_XO_BUTTONS,
    SUBMENU_ACTIVATE_FLASH_WMA,
    SUBMENU_RESET_ARK_SETTINGS,
    SUBMENU_GO_BACK,
    SUBMENU_MAX
};


int submenu_draw(void);
int submenu_find_longest_string(void);
int submenu_setup(void);
int submenu_ctrl(u32 button_on);
void subbutton_func(vsh_Menu *vsh);


#endif
