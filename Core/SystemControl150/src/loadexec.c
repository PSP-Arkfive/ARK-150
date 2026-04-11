#include <pspsdk.h>
#include <pspkernel.h>

#include <systemctrl_ark.h>
#include <cfwmacros.h>

extern int sceKernelGzipDecompressPatched(u8 *dest, u32 destSize, const u8 *src, void *unknown);
extern int LoadExecActionPatched(int apitype, void *a1, void *a2, void *a3, void *t0);
extern int sceKernelMemsetPatched(void *buf, int ch, int size);
extern int (* OrigLoadExecAction)(int apitype, void *a1, void *a2, void *a3, void *t0);

void patchLoadExec() {

    SceModule *mod = sceKernelFindModuleByName("sceLoadExec");
    
    if (mod != NULL) {
        OrigLoadExecAction = (void *)(mod->text_addr + 0x2138);
        MAKE_CALL(mod->text_addr + 0x2090, LoadExecActionPatched);
        MAKE_CALL(mod->text_addr + 0x2344, sceKernelGzipDecompressPatched);
        MAKE_CALL(mod->text_addr + 0x232C, sceKernelMemsetPatched);

        _sw(0x3C0188FC, mod->text_addr + 0x2384);
    }
}
