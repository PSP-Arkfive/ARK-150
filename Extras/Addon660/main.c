#include <pspsdk.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <psppower.h>

#include <systemctrl_ark.h>
#include <rebootexconfig.h>
#include <libpspexploit.h>
#include <mini2d.h>

#include "main.h"

PSP_MODULE_INFO("ARK150_Addon660", 0x0800, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_VSH);


int working = 1;
char* curtext = NULL;

Image* background;
Image* icon;

char** options = NULL;
int nopts = 0;
char msg[256];
int msg_type = INFO_MSG;
int msg_colors[] = { GREEN_COLOR, YELLOW_COLOR, RED_COLOR };
const char* HEADER = "ARK-150 Addon Installer for 6.60";


void setInfoMsg(int type, char* txt){
    msg_type = type;
    strcpy(msg, txt);
}

void ExitWithMessage(int type, int reboot, int milisecs, char *fmt, ...) 
{
    va_list list;
    va_start(list, fmt);
    msg_type = type;
    vsprintf(msg, fmt, list);
    va_end(list);
    sceKernelDelayThread(milisecs*1000);
    if (reboot)
        scePowerRequestColdReset(0);
    else
        sceKernelExitGame(); 
}

void drawMenu(){
    // now draw our menu stuff
    int w = 260;
    int h = 100;
    int x = (480-w)/2;
    int y = (272-h)/2;

    u32 color = 0x80808000;

    // menu window
    fillScreenRect(color, x, y, w, h);

    int tl = strlen(HEADER);
    int dx = ((w-8*tl)/2);
    fillScreenRect(0x8000ff00, x+dx, y+5, 8*tl, 8);
    printTextScreen(x + dx, y+5, HEADER, WHITE_COLOR);

    // menu items
    int cur_x;
    int cur_y = y + (h-(10*nopts))/2;
    for (int i=0; i<nopts; i++){
        int tl = strlen(options[i]);
        cur_x = x + ((w-(8*tl))/2);
        fillScreenRect(color&0x00FFFFFF, cur_x, cur_y+4, 8*tl, 8);
        if(i==0)
            printTextScreen(cur_x, cur_y+4, options[i], WHITE_COLOR);
        else
            printTextScreen(cur_x, cur_y+5, options[i], WHITE_COLOR);
        cur_y += 10;
    }

    if (msg[0]){
        printTextScreen(480-8*strlen(msg), TOP+15, msg, msg_colors[msg_type]);
    }
}

int drawthread(SceSize args, void *argp){
    
    while (working){
        clearScreen(CLEAR_COLOR);
        
        blitAlphaImageToScreen(0, 0, 480, 272, background, 0, 0);
        blitAlphaImageToScreen(0, 0, icon->imageWidth, icon->imageHeight, icon, 0, 272-icon->imageHeight);

        if (options != NULL && nopts > 0)
            drawMenu();

        flipScreen();
        sceDisplayWaitVblankStart();
    }

    return 0;
}

void loadGraphics(int argc, char** argv){
    initGraphics();

    PBPHeader header;
    
    int fd = sceIoOpen(argv[0], PSP_O_RDONLY, 0777);
    sceIoRead(fd, &header, sizeof(PBPHeader));
    sceIoClose(fd);
    
    background = loadImage(argv[0], header.pic1_offset);
    icon = loadImage(argv[0], header.icon0_offset);

    SceUID thid = sceKernelCreateThread("draw_thread", &drawthread, 0x10, 0x20000, PSP_THREAD_ATTR_USER|PSP_THREAD_ATTR_VFPU, NULL);
    if (thid >= 0){
        // start thread and wait for it to end
        sceKernelStartThread(thid, 0, NULL);
    }
}


int main(int argc, char** argv){

    loadGraphics(argc, argv);
    
    addon660_install();
    
    sceKernelExitGame();
    
    return 0;
}