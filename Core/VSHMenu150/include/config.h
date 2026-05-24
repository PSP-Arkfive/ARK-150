#ifndef _CONFIG_H
#define _CONFIG_H


#include "vsh.h"

int button_accept(u32 button);
int button_decline(u32 button);

void config_check(vsh_Menu *vsh);
void config_load(vsh_Menu *vsh);
void config_save(vsh_Menu *vsh);

void reset_ark_settings(vsh_Menu *vsh);

#endif