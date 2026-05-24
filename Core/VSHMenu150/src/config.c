#include <string.h>
#include <pspiofilemgr.h>



#include <kubridge.h>
#include <vshctrl.h>
#include <systemctrl.h>

#include "common.h"
#include "config.h"
#include "battery.h"
#include "color.h"
#include "registry.h"
#include "vpl.h"
#include "advanced.h"

int button_accept(u32 button){
    vsh_Menu* vsh = vsh_menu_pointer();
    return (vsh->status.swap_xo && (button & PSP_CTRL_CROSS)) || (!vsh->status.swap_xo && (button & PSP_CTRL_CIRCLE));
}

int button_decline(u32 button){
    vsh_Menu* vsh = vsh_menu_pointer();
    return (vsh->status.swap_xo && (button & PSP_CTRL_CIRCLE)) || (!vsh->status.swap_xo && (button & PSP_CTRL_CROSS));
}

void config_reset(vsh_Menu *vsh){
    memset(&vsh->config.ark_menu, 0, sizeof(vsh->config.ark_menu));
    vsh->config.ark_menu.syslang = 1;
    vsh->config.ark_menu.font = 1;
    vsh->config.ark_menu.sort_entries = 1;
    vsh->config.ark_menu.show_recovery = 1;
    vsh->config.ark_menu.text_glow = 3;
    vsh->config.ark_menu.screensaver = 2;
    vsh->config.ark_menu.redirect_ms0 = 1;
    vsh->config.ark_menu.menusize = 2;
    vsh->config.ark_menu.browser_icon0 = 1;
}

void config_load(vsh_Menu *vsh) {
    int is_pandora;
    char path[ARK_PATH_SIZE];
    strcpy(path, vsh->config.ark.arkpath);
    strcat(path, "ARKMENU150.BIN");

    config_reset(vsh);

    int fp = sceIoOpen(path, PSP_O_RDONLY, 0777);
    if (fp >= 0){
        sceIoRead(fp, &vsh->config.ark_menu, sizeof(vsh->config.ark_menu));
        sceIoClose(fp);
    }

    vctrlGetRegistryValue("/CONFIG/SYSTEM/XMB", "button_assign", &vsh->status.swap_xo);

    vsh->battery = battery_check();
    
    vsh->codecs = codecs_activated();

    color_check_random(vsh);
}

void config_save(vsh_Menu *vsh){
    int fp;
    char path[ARK_PATH_SIZE];
    strcpy(path, vsh->config.ark.arkpath);
    strcat(path, MENU_SETTINGS);

    fp = sceIoOpen(path, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
    if (fp >= 0){
        sceIoWrite(fp, &vsh->config.ark_menu, sizeof(vsh->config.ark_menu));
        sceIoClose(fp);
    }

}

void config_check(vsh_Menu *vsh) {
    color_check_random(vsh);
    if (memcmp(&vsh->config.old_se, &vsh->config.se, sizeof(vsh->config.se)) || memcmp(&vsh->config.old_ark_menu, &vsh->config.ark_menu, sizeof(vsh->config.ark_menu))){
        vctrlVSHUpdateConfig((SEConfig*)&vsh->config.se);
        config_save(vsh);	
    }
}

void reset_ark_settings(vsh_Menu *vsh){
    char arkMenuPath[ARK_PATH_SIZE];
    strcpy(arkMenuPath, vsh->config.ark.arkpath);
    strcat(arkMenuPath, MENU_SETTINGS);

    sceIoRemove(arkMenuPath);

    vsh->status.reset_vsh = 1; 
}
