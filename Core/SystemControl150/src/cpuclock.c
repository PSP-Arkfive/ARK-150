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
#include <pspsysmem_kernel.h>
#include <pspkernel.h>
#include <psputilsforkernel.h>
#include <psppower.h>
#include <pspinit.h>

#include <cfwmacros.h>
#include <systemctrl.h>
        
int (* scePowerSetClockFrequency_k)(int cpufreq, int ramfreq, int busfreq);

static const u32 cpu_nid_list[] = {
    0xB8D7B3FB,//scePowerSetBusClockFrequency
    0x843FBF43,//scePowerSetCpuClockFrequency
};

void sctrlHENSetSpeed(int cpu, int bus)
{
    int k1 = pspSdkSetK1(0);
    scePowerSetClockFrequency_k = (void*)sctrlHENFindFunction("scePower_Service", "scePower", 0x737486F2); //scePowerSetClockFrequency
    scePowerSetClockFrequency_k(cpu, cpu, bus);

    MAKE_DUMMY_FUNCTION_RETURN_0((u32)scePowerSetClockFrequency_k);

    for(int i=0;i<sizeof(cpu_nid_list)/sizeof(u32);i++)
    {
        u32 patch_addr = sctrlHENFindFunction("scePower_Service", "scePower", cpu_nid_list[i]);
        if (patch_addr) MAKE_DUMMY_FUNCTION_RETURN_0( patch_addr );
    }

    sctrlFlushCache();
    pspSdkSetK1(k1);
}

u32 sctrlHENGetSpeed(){
    u32 (*getCpuClockInt)() = (void*)sctrlHENFindFunction("scePower_Service", "scePower", 0xFDB5BFE9);
    if (getCpuClockInt) return getCpuClockInt();
    return 222;
}
