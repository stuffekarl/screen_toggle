#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ogcsys.h>
#include <gccore.h>

#define BORDER 10

static u32 *xfb;
static GXRModeObj *rmode;
bool white = false;

void DrawBox(int x1,int y1, int x2, int y2, int colour);
void toggleScreen();

void Initialise() {
	VIDEO_Init();
	PAD_Init();
 
	rmode = VIDEO_GetPreferredMode(NULL);

	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
 
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
}


int main() {
	u16 old_down = 0;
	u16 old_up = 0;//, old_held = 0;
	u32 count = 0;
	
	Initialise();
	printf("\n\n\nPress A/B/X to start toggling the white box!\n");
	
	while(1){
		VIDEO_ClearFrameBuffer(rmode, xfb, COLOR_BLACK);
		PAD_ScanPads();
		u16 buttonsDown = PAD_ButtonsDown(0);
		u16 buttonsUp = PAD_ButtonsUp(0);
		u16 buttonsHeld = PAD_ButtonsHeld(0);
		
		if(buttonsDown != old_down){
			if(buttonsDown & PAD_BUTTON_A){
				toggleScreen();
			}
			else if(buttonsDown & PAD_BUTTON_B){
				toggleScreen();
			}
			else if(buttonsDown & PAD_BUTTON_START){
				return 0;
			}
			else if(buttonsDown & PAD_BUTTON_Y){
				count = 0;
			}
		}
		if(buttonsUp != old_up){
			if(buttonsUp & PAD_BUTTON_B){
				toggleScreen();
			}
		}
		if(buttonsHeld & PAD_BUTTON_X){
			toggleScreen();
		}

		old_down = buttonsDown;
		old_up = buttonsUp;
		//old_Held = buttonsHeld;
		count++;
		printf("         f: %d, s: %d\r", count, count/60);
		printf("%d", rmode->viTVMode);
		if(white)
		DrawBox(BORDER, BORDER, (rmode->fbWidth - BORDER), (rmode->xfbHeight) - BORDER, COLOR_WHITE);
		VIDEO_Flush();
		VIDEO_WaitVSync();
	}
	return 0;
}

void DrawHLine (int x1, int x2, int y, int color){
    int i;
    y = 320 * y;
    x1 >>= 1;
    x2 >>= 1;
    for (i = x1; i <= x2; i++) {
        u32 *tmpfb = xfb;
        tmpfb[y+i] = color;
    }
}

void DrawBox (int x1, int y1, int x2, int y2, int color) {
	int i = 0;
	for (i = y1; i <= y2; i++){
		DrawHLine (x1, x2, i, color);
	}
}

void toggleScreen(){
	if(white){
		white = false;
	}
	else{
		white = true;
	}
}