#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <pspsdk.h>
#include <pspkernel.h>

#include <systemctrl_ark.h>
#include <cfwmacros.h>

#include "list.h"
#include "settings.h"
#include "plugins.h"
#include "main.h"
#include "utils.h"

List plugins;
List iplugins;

extern ARKConfig ark_config;
char * strtrim(char * text);

static char* sample_plugin_path = "game, ms0:/SEPLUGINS/example.prx";

int cur_place = 0;


void plugin_list_cleaner(void* item){
    if (item == NULL) return;
    Plugin* plugin = (Plugin*)item;
    if (plugin->name) free(plugin->name);
    if (plugin->surname) free(plugin->surname);
    if (plugin->path) free(plugin->path);
    if (plugin->runlevel) free(plugin->runlevel);
    free(plugin);
}

static void processCustomLine(char* line){
    Plugin* plugin = (Plugin*)malloc(sizeof(Plugin));
    memset(plugin, 0, sizeof(Plugin));
    plugin->path = line;
    plugin->place = cur_place;
    add_list(&plugins, plugin);
}

static int processPlugin(char* runlevel, char* path, char* enabled){
    
    int n = plugins.count;
    Plugin* plugin = (Plugin*)malloc(sizeof(Plugin));
    memset(plugin, 0, sizeof(Plugin));

    plugin->name = malloc(20);
    snprintf(plugin->name, 20, "plugin_%d", n);

    plugin->surname = malloc(20);
    snprintf(plugin->surname, 20, "plugins%d", n);

    plugin->path = malloc(strlen(path) + 1);
    strcpy(plugin->path, path);

    plugin->runlevel = malloc(strlen(runlevel) + 1);
    strcpy(plugin->runlevel, runlevel);

    plugin->place = cur_place;
    plugin->active = isRunlevelEnabled(enabled);

    add_list(&plugins, plugin);

    return 1;
}

void loadPlugins(){
    clear_list(&plugins, &plugin_list_cleaner);

    cur_place = PLACE_MS0;
    ProcessConfigFile(PLUGINS150_PATH, &processPlugin, &processCustomLine);

    if (plugins.count == 0){
        // Add example plugin
        Plugin* plugin = (Plugin*)malloc(sizeof(Plugin));
        plugin->name = (char*)malloc(20);
        plugin->surname = (char*)malloc(20);
        plugin->path = (char*)malloc(strlen(sample_plugin_path)+1);
        plugin->active = 1;
        plugin->place = 0;
        strcpy(plugin->name, "plugin_0");
        strcpy(plugin->surname, "plugins0");
        strcpy(plugin->path, sample_plugin_path);
        add_list(&plugins, plugin);
    }
}

void savePlugins(){

    if (plugins.count == 0) return;

    if (plugins.count == 1){
        Plugin* plugin = (Plugin*)(plugins.table[0]);
        if (strcmp(plugin->path, sample_plugin_path) == 0){
            return;
        }
    }

    SceUID fd = sceIoOpen(PLUGINS150_PATH, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);

    for (int i=0; i<plugins.count; i++){
        Plugin* plugin = (Plugin*)(plugins.table[i]);
        if (plugin->active == PLUGIN_REMOVED) continue;
        if (plugin->name){
            char* sep = ", ";
            char* enabled = (plugin->active)? "on" : "off";
            sceIoWrite(fd, plugin->runlevel, strlen(plugin->runlevel));
            sceIoWrite(fd, sep, strlen(sep));
            sceIoWrite(fd, plugin->path, strlen(plugin->path));
            sceIoWrite(fd, sep, strlen(sep));
            sceIoWrite(fd, enabled, strlen(enabled));
        }
        else { // Custom line
            sceIoWrite(fd, plugin->path, strlen(plugin->path));
        }
        sceIoWrite(fd, "\n", 1);
    }

    sceIoClose(fd);
}

