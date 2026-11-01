#include <systemctrl.h>

#include "launcher.h"

void exec_recovery_menu(vsh_Menu *vsh) {
    sctrlArkGetConfig(&vsh->config.ark);
    vsh->config.ark.recovery = 1;
    sctrlArkSetConfig(&vsh->config.ark);
    sctrlKernelExitVSH(NULL);
}
