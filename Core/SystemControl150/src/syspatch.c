#include <string.h>
#include <pspkernel.h>
#include <pspreg.h>
#include <pspumd.h>
#include <pspsysevent.h>
#include <pspidstorage.h>
#include <pspdisplay_kernel.h>

#include <cfwmacros.h>
#include <systemctrl.h>
#include <systemctrl_se.h>
#include <systemctrl_ark.h>

#include "main.h"
#include "modulemanager.h"
#include "loadexec.h"
#include "controller.h"


extern ARKConfig* ark_config;
extern SEConfigARK150 se_config;

void sceDisplaySetBrightnessPatched(int level, int unk1);
int SysEventHandler(int eventId, char *eventName, void *param, int *result);

int last_br;
int last_unk;
int (* GetMsSize)(void);

// Previous Module Start Handler
STMOD_HANDLER previous = NULL;

PspSysEventHandler sysEventHandler =
{
    .size = sizeof(PspSysEventHandler),
    .name = "",
    .type_mask = 0x0000FF00,
    .handler = SysEventHandler
};

int ValidateSeekPatched(u32 *drv_str, SceOff ofs)
{
    if ((ofs & 0x1FF))
        return 0;

    SceOff max_size = GetMsSize(); // size of partition in sectors
    max_size *= 512; // size in bytes

    if (ofs >= max_size)
        return 0;

    return 1;
}

int ValidateSeekP1Patched(u32 *drv_str, SceOff ofs)
{
    if ((ofs & 0x1FF))
        return 0;

    u32 *p = (u32 *)drv_str[0x10/4];
    u32 *q = (u32 *)p[8/4];
    SceOff max_size = (SceOff)q[4/4]; // partition_start

    max_size = GetMsSize() - max_size; // size of partition in sectors
    max_size *= 512; // size in bytes

    if (ofs >= max_size)
        return 0;

    return 1;
}

REGHANDLE lang_hk = -1;

int sceRegGetKeyValuePatched(REGHANDLE hd, REGHANDLE hk, void *buffer, SceSize size)
{
    int res = sceRegGetKeyValue(hd, hk, buffer, size);
    if (res >= 0 && hk == lang_hk)
    {
        if (*(u32 *)buffer > 8)
        	*(u32 *)buffer = 1;
        
        lang_hk = -1;
    }
    return res;
}

int sceRegGetKeyInfoPatched(REGHANDLE hd, const char *name, REGHANDLE *hk, unsigned int *type, SceSize *size)
{
    int res = sceRegGetKeyInfo(hd, name, hk, type, size);

    if (res >= 0 && strcmp(name, "language") == 0)
    {
        if (hk)
        	lang_hk = *hk;
    }

    return res;
}

void sceDisplaySetBrightnessPatched(int level, int unk1)
{
    last_br = level;
    last_unk = unk1;

    if (level < 100)
    {
        if (level >= 70)
        	level = 100 - level;
        else if (level >= 35)
        	level = 100 - level - 5;
        else
        	level = 100 - level - 10;
    }
    else if (level == 100)
    {
        level = 1;
    }

    sceDisplaySetBrightness(level, unk1);
}

int (*_sceSysconWriteScratchPad)(int dest, void *src, int size);

int sceSysconWriteScratchPadPatched(int dest, void *src, int size)
{
    int fw_150 = FW_150;

    // Write firmware version to (hopefully) unused area of syscon scratchpad mem
    // This is read by cIPL to determine which IPL to load when coming out of suspend
    _sceSysconWriteScratchPad(SYSCON_SCRATCHPAD_RESUME_FW_ADDR, &fw_150, sizeof(fw_150));

    return _sceSysconWriteScratchPad(dest, src, size);
}

int (*_sceSysconCtrlLEDOrig)(int, int);
void disableLEDs(){
    int (*_sceSysconCtrlLED)(int, int);
    _sceSysconCtrlLED = (void*)sctrlHENFindFunction("sceSYSCON_Driver", "sceSyscon_driver", 0x18BFBE65);
    for (int i=0; i<4; i++) _sceSysconCtrlLED(i, 0);
    static u32 dummy[2] = {JR_RA, LI_V0(0)};
    HIJACK_FUNCTION(_sceSysconCtrlLED, dummy, _sceSysconCtrlLEDOrig);
    sctrlFlushCache();
}

static int sceGpioPortReadPatched(void) {
    int GPRValue = *((int *) 0xBE240004);
    GPRValue = GPRValue & 0xFBFFFFFF;
    return GPRValue;
}

int sceUmdRegisterUMDCallBackPatched(int cbid) {
    int k1 = pspSdkSetK1(0);
    int res = sceKernelNotifyCallback(cbid, PSP_UMD_NOT_PRESENT);
    pspSdkSetK1(k1);
    return res;
}

void disableUMD(){
    // redirect UMD callback to DISC_NOT_PRESENT
    u32 f = sctrlHENFindFunction("sceUmd_driver", "sceUmdUser", 0xAEE7404D);
    if (f){
        REDIRECT_FUNCTION(f, sceUmdRegisterUMDCallBackPatched);
    }
    // remove umd driver
    sceIoDelDrv("umd");
    // force UMD check medium to always return 0 (no medium)
    u32 CheckMedium = sctrlHENFindFunction("sceUmd_driver", "sceUmdUser", 0x46EBB729);
    if (CheckMedium){
        MAKE_DUMMY_FUNCTION_RETURN_0(CheckMedium);
    }
    // patch GPIO to disable UMD drive electrically
    u32 sceGpioPortRead = sctrlHENFindFunction("sceGPIO_Driver", "sceGpio_driver", 0x4250D44A);
    REDIRECT_FUNCTION(sceGpioPortRead, sceGpioPortReadPatched);
}

// Module Start Handler
static int ARKSyspatchOnModuleStart(SceModule * mod)
{

    // System fully booted Status
    static int booted = 0;

    char *moduleName = mod->modname;
    u32 text_addr = mod->text_addr;

    if (strcmp(moduleName, "sceRegistry_Service") == 0)
    {
        _sw((u32)sceRegGetKeyInfoPatched, text_addr+0x76DC);
        _sw((u32)sceRegGetKeyValuePatched, text_addr+0x76E0);

        sctrlFlushCache();
    }
    else if (strcmp(moduleName, "scePower_Service") == 0)
    {
        _sw(0x00e02021, text_addr+0x558); //ADDU $a0 $a3 $zero

        MAKE_CALL(text_addr + 0x115c, sceSysconWriteScratchPadPatched);
        _sceSysconWriteScratchPad = (void*)(text_addr + 0x4360);

        sctrlFlushCache();
    }
    else if (strcmp(moduleName, "sceDisplay_Service") == 0)
    {
        int lcd;
        if (sceIdStorageLookup(8, 0, &lcd, 4) >= 0 && lcd == 0x4C434470)
        {
        	_sw((u32)sceDisplaySetBrightnessPatched, text_addr + 0x2858);
        	sctrlFlushCache();
        }
    }
    else if (strcmp(moduleName, "sceMSstor_Driver") == 0)
    {
        REDIRECT_FUNCTION(text_addr + 0x5138, ValidateSeekPatched);
        REDIRECT_FUNCTION(text_addr + 0x51bc, ValidateSeekP1Patched);
        GetMsSize = (void *)(text_addr + 0x0288);

        sctrlFlushCache();
    }
    else if (strcmp(moduleName, "sceLoadExec") == 0)
    {
        patchLoadExec();
        sctrlFlushCache();
    }
    else if (strcmp(mod->modname, "sceController_Service") == 0){
        // Allow exiting through key combo
        patchController(mod);
        sctrlFlushCache();
    }

    // Boot Complete Action not done yet
    if (booted == 0)
    {
        // Boot is complete
        if (sctrlHENIsSystemBooted())
        {
            // handle settings
            if (se_config.msspeed) sctrlMsCacheInit("msstor0p", MSCACHE_BUFSIZE_MIN);
            if (se_config.noled) disableLEDs();
            if (se_config.noumd) disableUMD();

            // handle CPU speed settings
            switch (se_config.cpubus_clock){
                case CPU_BUS_CLOCK_333:    sctrlHENSetSpeed(333, 166); break;
                case CPU_BUS_CLOCK_222:    sctrlHENSetSpeed(222, 111); break;
                case CPU_BUS_CLOCK_133:    sctrlHENSetSpeed(133, 66);  break;
                case CPU_BUS_CLOCK_CUSTOM:
                    sctrlHENSetSpeed(
                        se_config.custom_cpu_clock,
                        se_config.custom_bus_clock
                    );
                    break;
            }

            ark_config->recovery = 0; // reset recovery mode for next reboot

            // Boot Complete Action done
            booted = 1;
        }
    }

    if (previous) return previous(mod);
    return 0;
}

// Add Module Start Patcher
void syspatchInit(void)
{
    // Register Module Start Handler
    previous = sctrlHENSetStartModuleHandler(ARKSyspatchOnModuleStart);
    sceKernelRegisterSysEventHandler(&sysEventHandler);
}

int SysEventHandler(int eventId, char *eventName, void *param, int *result)
{
    if (eventId == 0x1000B)
    {
        if (last_br == 100)
        	sceDisplaySetBrightnessPatched(last_br, last_unk);
    }
    else if (eventId == 0x400000) { // resume complete
        // disable LEDs
        if (se_config.noled && _sceSysconCtrlLEDOrig){
            for (int i=0; i<4; i++) _sceSysconCtrlLEDOrig(i, 0);
        }
    }

    return 0;
}
