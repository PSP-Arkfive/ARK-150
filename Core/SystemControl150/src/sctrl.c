#include <string.h>
#include <pspkernel.h>

#include <systemctrl.h>
#include <systemctrl_ark.h>
#include <cfwmacros.h>


extern ARKConfig* ark_config;

ARKConfig* sctrlArkGetConfig(ARKConfig* conf){
    if (conf){
        memcpy(conf, ark_config, sizeof(ARKConfig));
        return conf;
    }
    return ark_config;
}

void sctrlHENHijackFunction(SctrlFunctionPatchData* patch_data, void* func_addr, void* patch_func, void** orig_func) {

    void* ptr = patch_data;

    int is_kernel_patch = IS_KERNEL_ADDR(func_addr);
    if (is_kernel_patch){
        patch_func = (void*)KERNELIFY(patch_func);
        ptr = (void*)KERNELIFY(ptr);
    }

    // do hijack
    {
        u32 _pb_ = (u32)patch_data;
        u32 a = (u32)func_addr;
        u32 f = (u32)patch_func;

        _sw(_lw(a), _pb_);
        _sw(_lw(a + 4), _pb_ + 4);
        _sw(NOP, _pb_ + 8);
        _sw(NOP, _pb_ + 16);
        MAKE_JUMP_PATCH(_pb_ + 12, a + 8);
        _sw(0x08000000 | ((f >> 2) & 0x03FFFFFF), a); \
        _sw(NOP, a + 4);

    }

    *orig_func = ptr;

    sctrlFlushCache();
}

// Set User Level
int sctrlKernelSetUserLevel(int level)
{
    return 0; // TODO: not working on 1.50 -> crashes
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
