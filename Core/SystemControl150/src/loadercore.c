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

#include <stdio.h>
#include <string.h>
#include <pspsdk.h>
#include <pspkernel.h>
#include <psputilsforkernel.h>

#include <cfwmacros.h>
#include <systemctrl.h>
#include <systemctrl_se.h>
#include <systemctrl_ark.h>


extern SEConfigARK se_config;
extern ARKConfig* ark_config;

extern void LoadPlugins();

// Plugin Loader Status
int pluginLoaded = 0;
int settingsLoaded = 0;

// init.prx Custom sceKernelStartModule Handler
int (* customStartModule)(int modid, SceSize argsize, void * argp, int * modstatus, SceKernelSMOption * opt) = NULL;


// Init Start Module Hook
int InitKernelStartModule(int modid, SceSize argsize, void * argp, int * modstatus, SceKernelSMOption * opt)
{
    char modname[28]; memset(modname, 0, sizeof(modname));
    SceModule* mod = (SceModule*) sceKernelFindModuleByUID(modid);
    strncpy(modname, mod->modname, sizeof(modname));

    int result = -1;
    u32* vshmain_args = NULL;

    // VSH replacement
    if (strcmp(modname, "vsh_module") == 0){
        // skip bootup animation
        if (se_config.skiplogos == 1 || se_config.skiplogos == 3) {
            vshmain_args = oe_malloc(1024);
            memset(vshmain_args, 0, 1024);
    
            if (argp != NULL && argsize != 0 ) {
                memcpy( vshmain_args , argp ,  argsize);
            }
    
            vshmain_args[0] = 1024;
            vshmain_args[1] = 0x20;
            vshmain_args[16] = 1;
            argp = vshmain_args;
            argsize = 1024;
        }
    }

    // Custom Handler registered
    if(customStartModule != NULL)
    {
        // Forward to Handler
        result = customStartModule(modid, argsize, argp, modstatus, opt);
    }

    // load settings before impose module
    if (!settingsLoaded && strcmp(modname, "sceImpose_Driver") == 0){
        // load settings
        //loadSettings();
        // Remember it
        settingsLoaded = 1;
    }

    // load plugins before starting mediasync
    if (!pluginLoaded && strcmp(modname, "sceMediaSync") == 0)
    {
        // Load Plugins
        LoadPlugins();
        // Remember it
        pluginLoaded = 1;
    }
    
    // start module
    if (result < 0) result = sceKernelStartModule(modid, argsize, argp, modstatus, opt);

    // cleanup
    if (vshmain_args) oe_free(vshmain_args);

    return result;
}

// sceKernelStartModule Hook
int patch_sceKernelStartModule_in_bootstart(int (* bootstart)(SceSize, void *), void * argp)
{
    u32 StartModule = JUMP(sctrlHENFindFunction("sceModuleManager", "ModuleMgrForUser", 0x50F0C1EC));
    u32 addr = (u32)bootstart;
    int patches = 1;
    for (;patches; addr+=4){
        if (_lw(addr) == StartModule){
            // Replace Stub
            _sw(JUMP(InitKernelStartModule), addr);
            _sw(NOP, addr + 4);
            patches--;
        }
    }
    // Passthrough
    return bootstart(4, argp);
}

// Patch Loader Core Module
SceModule* patchLoaderCore(void)
{
    // Find Module
    SceModule* mod = (SceModule *)sceKernelFindModuleByName("sceLoaderCore");

    // Fetch Text Address
    u32 start_addr = mod->text_addr;
    u32 topaddr = mod->text_addr+mod->text_size;

    // start the dynamic patching
    for (u32 addr = start_addr; addr<topaddr; addr+=4){
        u32 data = _lw(addr);
        if (data == 0x02E0F809){
            // Hook sceInit StartModule Call
            _sw(JAL(patch_sceKernelStartModule_in_bootstart), addr);
            // Move Real Bootstart into Argument #1
            _sw(0x02E02021, addr+4);
        }
    }

    // Flush Cache
    sctrlFlushCache();

    return mod;
}
