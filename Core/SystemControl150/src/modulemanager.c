#include <string.h>
#include <pspsdk.h>
#include <pspkernel.h>

#include <cfwmacros.h>
#include <systemctrl.h>
#include <systemctrl_ark.h>

#include "modulemanager.h"

// Module Start Handler
STMOD_HANDLER g_module_start_handler = NULL;

// Prologue Module Function
int (* prologue_module)(void *, SceModule *) = NULL;

// Prologue Module Hook
int prologue_module_hook(void * unk0, SceModule * mod)
{
    // Forward Call
    int result = prologue_module(unk0, mod);
    
    // Load Success
    if(result >= 0)
    {
        // Module Start Handler
        if(g_module_start_handler != NULL)
        {
            // Call Module Start Handler
            g_module_start_handler(mod);
        }
    }
    
    // Return Result
    return result;
}

void patchModuleManager()
{
    SceModule *mod = sceKernelFindModuleByName("sceModuleManager");

    /* bne t4, zero, +43 -> beq zero, zero, +43 : 
        Force always to take the size of the data.psp instead of
        the size of the PBP to avoid the error 0x80020001 */
    _sw(0x1000002A, mod->text_addr + 0x3f28);    

    prologue_module = (int (*)(void *, SceModule *))(mod->text_addr + 0x3b34);

    MAKE_CALL(mod->text_addr + 0x2d90, prologue_module_hook);
}