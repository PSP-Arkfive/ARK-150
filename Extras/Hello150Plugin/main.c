#include <malloc.h>
#include <psppower.h>
#include <pspdisplay.h>
#include <pspkernel.h>
#include <pspsdk.h>
#include <pspgu.h>
#include <pspgum.h>
#include <psputilsforkernel.h>

#include <cfwmacros.h>
#include <systemctrl.h>
#include <vshctrl.h>
#include <kubridge.h>
#include <tinyfont.h>

PSP_MODULE_INFO("Hello150Plugin", 0, 2, 2);


static TinyFontState fontstate = {.glow = 1};
static SctrlFunctionPatchData display_patch;
static int (*prevDisplaySetFrameBuf)(void*, int, int, int) = NULL;


static int vshDisplaySetFrameBuf(void *frameBuf, int bufferwidth, int pixelformat, int sync) {  
    void* frame = (void*)(0x1fffffff & (u32)frameBuf);

    if (frame){
        tinyFontPrintTextScreenBuf(frame, msx, 10, 10, "Hello 150 Plugin!", -1, &fontstate);
    }

    return prevDisplaySetFrameBuf(frameBuf, bufferwidth, pixelformat, sync);
}

int module_start() {

    void* func_addr = (void*)sctrlHENFindFunction("sceDisplay_Service", "sceDisplay", 0x289D82FE);
    sctrlHENHijackFunction(&display_patch, func_addr, vshDisplaySetFrameBuf, (void**)&prevDisplaySetFrameBuf);

    return 0;
}
