#include <string.h>
#include <pspkernel.h>

#include <systemctrl.h>
#include <systemctrl_ark.h>
#include <cfwmacros.h>


extern ARKConfig* ark_config;

void* sctrlArkGetConfig(ARKConfig* conf){
    if (conf) memcpy(conf, ark_config, sizeof(ARKConfig));
    return ark_config;
}

// Set User Level
int sctrlKernelSetUserLevel(int level)
{
    // Elevate Permission Level
    unsigned int k1 = pspSdkSetK1(0);

    // Backup User Level
    int previouslevel = sceKernelGetUserLevel();


    u32 _sceKernelReleaseThreadEventHandler = sctrlHENFindFunction("sceThreadManager", "ThreadManForKernel", 0x72F3C145);

    u32 addr = _sceKernelReleaseThreadEventHandler + 0x4;
    do {
        addr += 4;
    } while ((_lw(addr)&0xFFF00000) != 0x24B00000);

    u32 threadman_userlevel_struct = _lh(_sceKernelReleaseThreadEventHandler + 0x4)<<16;
    threadman_userlevel_struct += (short)_lh(addr);


    // Set User Level
    _sw((level ^ 8) << 28, *(unsigned int *)(threadman_userlevel_struct) + 0x14);

    // Flush Cache
    sctrlFlushCache();

    // Restore Permission Level
    pspSdkSetK1(k1);

    // Return previous User Level
    return previouslevel;
}
