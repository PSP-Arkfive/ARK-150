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
#include <pspthreadman_kernel.h>
#include <pspsysevent.h>
#include <pspiofilemgr.h>
#include <pspsysmem_kernel.h>
#include <pspinit.h>

#include <cfwmacros.h>
#include <systemctrl.h>
#include <systemctrl_se.h>
#include <systemctrl_ark.h>


SEConfigARK se_config = {
    .magic = ARK_CONFIG_MAGIC,
    .umdseek = 0,
    .cpubus_clock = 0,
    .disable_pause = 0,
    .hidedlc = 0,
    .umdregion = 0,
    .vshregion = 0,
    .usbdevice = 0,
    .usbcharge = 0,
    .hidemac = 0,
    .launcher_mode = 0,
    .hidepics = 0,
    .qaflags = 0,
    .usbdevice_rdonly = 2,
    .skiplogos = 0,
    .noumd = 0,
    .hibblock = 0,
    .noanalog = 0,
    .oldplugin = 0,
    .msspeed = 0,
    .iso_cache_type = 0,
    .iso_cache_size_kb = 4,
    .iso_cache_num = 8,
    .iso_cache_partition = PSP_MEMORY_PARTITION_KERNEL,
    .noled = 0, // always false
    .wpa2 = 0, /* not used by default */
    .force_high_memory = 0,
};

// we keep this here for compatibility
// ARK doesn't use this and it isn't persistent

/**
 * Gets the SE configuration.
 * Avoid using this function, it may corrupt your program.
 * Use sctrlSEGetCongiEx function instead.
 *
 * @param config - pointer to a SEConfig structure that receives the SE configuration
 * @returns pointer to original SEConfig structure in SystemControl
*/
int sctrlSEGetConfig(SEConfig *config)
{
    if (config) memcpy(config, &se_config, sizeof(SEConfigARK));
    return 0;
}

/**
 * Gets the SE configuration
 *
 * @param config - pointer to a SEConfig structure that receives the SE configuration
 * @param size - The size of the structure
 * @returns pointer to original SEConfig structure in SystemControl
*/
int sctrlSEGetConfigEx(SEConfig *config, int size)
{
    if (config && size == sizeof(SEConfigARK)){
        memcpy(config, &se_config, size);
    }
    return 0;
}

/**
 * Sets the SE configuration
 * This function can corrupt the configuration in flash, use
 * sctrlSESetConfigEx instead.
 *
 * @param config - pointer to a SEConfig structure that has the SE configuration to set
 * @returns 0 on success
*/
int sctrlSESetConfig(SEConfig *config)
{
    memcpy(&se_config, config, sizeof(SEConfigARK));
    return 0;
}

/**
 * Sets the SE configuration
 *
 * @param config - pointer to a SEConfig structure that has the SE configuration to set
 * @param size - the size of the structure
 * @returns 0 on success
*/
int sctrlSESetConfigEx(SEConfig *config, int size)
{
    if (config && size == sizeof(SEConfigARK)){
        memcpy(&se_config, config, size);
        return 0;
    }
    return -1;
}

void sctrlSEApplyConfig(SEConfig *config){
    sctrlSESetConfig(config);
}

SEConfig* sctrlSEGetConfigInternal(){
    return (SEConfig*)&se_config;
}

int sctrlSEGetVersion()
{
    return 150;
}
