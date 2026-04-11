#include <string.h>
#include <pspsdk.h>
#include <psputilsforkernel.h>

#include <cfwmacros.h>
#include <systemctrl.h>
#include <systemctrl_ark.h>
#include <rebootexconfig.h>
#include <bootloadex.h>


#include "../Rebootex/Rebootex150/rebootex150.h"
#include "../Rebootex/Rebootex660/payload.h"
#include "../Rebootex/Rebootex660/payload_tmctrl.h"

REBOOT150_BUFFER
REBOOT661_BUFFER

// custom rebootex
void* custom_rebootex = NULL;

// LoadReboot handler
int (* LoadRebootOverrideHandler)(void * arg1, unsigned int arg2, void * arg3, unsigned int arg4) = NULL;

// Original LoadExecAction Function
int (* OrigLoadExecAction)(int apitype, void *a1, void *a2, void *a3, void *t0);

extern int sceKernelMemset(void *buf, int ch, int size);

extern u8 keyseed[0x20];
extern ARKConfig* ark_config;

// Reboot Buffer Backup
RebootexConfigARK rebootex_config = {
    .magic = ARK_CONFIG_MAGIC,
    .reboot_buffer_size = REBOOTEX_MAX_SIZE
};

static int reboot66x = 0;

int LoadExecActionPatched(int apitype, void *a1, void *a2, void *a3, void *t0)
{
    if ((apitype & 0x200) == 0x200) /* vsh */
    {
        reboot66x = 0;
    }

    return OrigLoadExecAction(apitype, a1, a2, a3, t0);
}

int sceKernelMemsetPatched(void *buf, int ch, int size)
{
    if (reboot66x)
    {
        buf = (void *)0x88600000;
        size = 0x200000;

        sceKernelSetDdrMemoryProtection((void *)0x88400000, 0x00400000, 0xC);
    }

    return sceKernelMemset(buf, ch, size);
}

// Backup Reboot Buffer
void backupRebootBuffer(void)
{

    // Copy Reboot Buffer Configuration
    RebootexConfigARK* backup_conf = (RebootexConfigARK*)REBOOTEX_CONFIG;
    if (IS_ARK_CONFIG(backup_conf) && backup_conf->reboot_buffer_size){
        memcpy(&rebootex_config, backup_conf, sizeof(RebootexConfigARK));
    }

    // Copy ARK runtime Config
    if (IS_ARK_CONFIG(ARK_CONFIG)) {
        memcpy(ark_config, (void*)ARK_CONFIG, sizeof(ARKConfig));
    }
    
    // Flush Cache
    sctrlFlushCache();
}

// Restore Reboot Buffer
void restoreRebootBuffer(void)
{
    rebootex_config.boot_from_fw_version = sceKernelDevkitVersion();
        
    // Restore Reboot Buffer Configuration
    memcpy((void *)REBOOTEX_CONFIG, &rebootex_config, sizeof(RebootexConfigARK));

    // Restore ARK Configuration
    memcpy((void *)ARK_CONFIG, ark_config, sizeof(ARKConfig));
}

// Reboot Buffer Loader
int sceKernelGzipDecompressPatched(u8 *dest, u32 destSize, const u8 *src, void *unknown)
{
    // If a LoadReboot override handler has been set call it instead
    if (LoadRebootOverrideHandler)
        return LoadRebootOverrideHandler(dest, destSize, src, (u32)unknown);

    u8 *output = (u8 *)0xbfc00200;
    for (int i = 0; i < 0x10; i++)
    {
        output[i] = keyseed[i];    
    }
    
    restoreRebootBuffer();

    if (reboot66x)
    {
        if (strcmp(ark_config->exploit_id, DC_EXPLOIT_ID) == 0) {
            sceKernelGzipDecompress((unsigned char *)REBOOTEX_TEXT, REBOOTEX_MAX_SIZE, rebootbuffer_ms_psp, NULL);
        }
        else {
            sceKernelGzipDecompress((unsigned char *)REBOOTEX_TEXT, REBOOTEX_MAX_SIZE, rebootbuffer_psp, NULL);
        }

        memcpy((void*)REBOOT_TEXT, reboot661 + 0x10, (sizeof(reboot661) - 0x10));

        // Do the following additional patches when going from 1.50 to 6.61
        _sw(0, 0x886000f0);
        _sw(0, 0x88600108);    

        return 0;
    }

    memcpy((void*)REBOOTEX_TEXT, rebootex150, size_rebootex150);

    return sceKernelGzipDecompress(dest, destSize, src, unknown);
}
