
///////////////////////////////////////////////////////////////////////////////
// Headers.

#include <stdint.h>
#include "system.h"
#include <stdio.h>

//#include "prarie_rgb333.h"
#include "prarie_idx4.h"

///////////////////////////////////////////////////////////////////////////////
// HW stuff.

#define WAIT_UNITL_0(x) while(x != 0){}
#define WAIT_UNITL_1(x) while(x != 1){}

#define SCREEN_IDX1_W 512
#define SCREEN_IDX1_H 512
#define SCREEN_IDX4_W 256
#define SCREEN_IDX4_H 256
#define SCREEN_RGB333_W 160
#define SCREEN_RGB333_H 160

#define SCREEN_IDX4_W8 (SCREEN_IDX4_W/8)

#define gpu_p32 ((volatile uint32_t*)LPRS2_GPU_BASE)
#define palette_p32 ((volatile uint32_t*)(LPRS2_GPU_BASE+0x1000))
#define unpack_idx1_p32 ((volatile uint32_t*)(LPRS2_GPU_BASE+0x400000))
#define pack_idx1_p32 ((volatile uint32_t*)(LPRS2_GPU_BASE+0x600000))
#define unpack_idx4_p32 ((volatile uint32_t*)(LPRS2_GPU_BASE+0x800000))
#define pack_idx4_p32 ((volatile uint32_t*)(LPRS2_GPU_BASE+0xa00000))
#define unpack_rgb333_p32 ((volatile uint32_t*)(LPRS2_GPU_BASE+0xc00000))
#define joypad_p32 ((volatile uint32_t*)LPRS2_JOYPAD_BASE)

typedef struct {
	unsigned a      : 1;
	unsigned b      : 1;
	unsigned z      : 1;
	unsigned start  : 1;
	unsigned up     : 1;
	unsigned down   : 1;
	unsigned left   : 1;
	unsigned right  : 1;
} bf_joypad;
#define joypad (*((volatile bf_joypad*)LPRS2_JOYPAD_BASE))

typedef struct {
	uint32_t m[SCREEN_IDX1_H][SCREEN_IDX1_W];
} bf_unpack_idx1;
#define unpack_idx1 (*((volatile bf_unpack_idx1*)unpack_idx1_p32))

///////////////////////////////////////////////////////////////////////////////
// Game config.

#define STEP 1

#define STEP_BULLET 4

#define ANIM_DELAY 5

///////////////////////////////////////////////////////////////////////////////
// Game data structures.

typedef struct {
	uint16_t x;
	uint16_t y;
} point_t;

uint32_t* background[256] = {
	king_dirt_bush__p, king_dirt_bush__p, king_dirt_bush__p, king_dirt_bush__p, king_dirt_bush__p, king_dirt_bush__p, king_dirt1__p,king_dirt1__p,
	king_dirt1__p,king_dirt1__p, king_dirt_bush__p, king_dirt_bush__p, king_dirt_bush__p, king_dirt_bush__p, king_dirt_bush__p, king_dirt_bush__p,
	
	king_dirt_bush__p, king_dirt_bush__p, king_dirt1__p,  king_dirt1__p,  king_dirt1__p,  king_dirt1__p, king_dirt3__p, king_dirt3__p,
	king_dirt3__p, king_dirt3__p, king_dirt1__p, king_dirt1__p, king_dirt1__p, king_dirt1__p, king_dirt_bush__p, king_dirt_bush__p,
	
	king_dirt_bush__p, king_dirt1__p, king_dirt3__p, king_dirt3__p, king_dirt3__p, king_dirt3__p, king_dirt4__p, king_dirt4__p,
	king_dirt4__p, king_dirt4__p, king_dirt3__p, king_dirt3__p, king_dirt3__p, king_dirt3__p, king_dirt1__p, king_dirt_bush__p,
	
	king_dirt_bush__p, king_dirt1__p, king_dirt3__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p,
	king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt3__p, king_dirt1__p, king_dirt_bush__p,
	
	king_dirt_bush__p, king_dirt1__p, king_dirt3__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p,
	king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt3__p, king_dirt1__p, king_dirt_bush__p,
	
	king_dirt_bush__p, king_dirt1__p, king_dirt3__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p,
	king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt3__p, king_dirt1__p, king_dirt_bush__p,
	
	king_dirt1__p, king_dirt3__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p,
	king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt3__p, king_dirt1__p,
	
	king_dirt1__p, king_dirt3__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p,
	king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt3__p, king_dirt1__p,
	
	king_dirt1__p, king_dirt3__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p,
	king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt3__p, king_dirt1__p,
	
	king_dirt1__p, king_dirt3__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p,
	king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt3__p, king_dirt1__p,
	
	king_dirt_bush__p, king_dirt1__p, king_dirt3__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p,
	king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt3__p, king_dirt1__p, king_dirt_bush__p,
	
	king_dirt_bush__p, king_dirt1__p, king_dirt3__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p,
	king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt3__p, king_dirt1__p, king_dirt_bush__p,
	
	king_dirt_bush__p, king_dirt1__p, king_dirt3__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p,
	king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt4__p, king_dirt3__p, king_dirt1__p, king_dirt_bush__p,
	
	king_dirt_bush__p, king_dirt1__p, king_dirt3__p, king_dirt3__p, king_dirt3__p, king_dirt3__p, king_dirt4__p, king_dirt4__p,
	king_dirt4__p, king_dirt4__p, king_dirt3__p, king_dirt3__p, king_dirt3__p, king_dirt3__p, king_dirt1__p, king_dirt_bush__p,
	
	king_dirt_bush__p, king_dirt_bush__p, king_dirt1__p,  king_dirt1__p,  king_dirt1__p,  king_dirt1__p, king_dirt3__p, king_dirt3__p,
	king_dirt3__p, king_dirt3__p, king_dirt1__p, king_dirt1__p, king_dirt1__p, king_dirt1__p, king_dirt_bush__p, king_dirt_bush__p,
	
	king_dirt_bush__p, king_dirt_bush__p, king_dirt_bush__p, king_dirt_bush__p, king_dirt_bush__p, king_dirt_bush__p, king_dirt1__p,king_dirt1__p,
	king_dirt1__p,king_dirt1__p, king_dirt_bush__p, king_dirt_bush__p, king_dirt_bush__p, king_dirt_bush__p, king_dirt_bush__p, king_dirt_bush__p,
};

uint32_t* king_backgnd__p[5] = {
	king_dirt1__p, king_dirt2__p, king_dirt3__p, king_dirt4__p, king_dirt_border__p
};

uint32_t* king_states__p[15] = {
	king_down1__p, king_down2__p, king_down3__p,
	king_left1__p, king_left2__p, king_left3__p,
	king_right1__p, king_right2__p, king_right3__p,
	king_up1__p, king_up2__p, king_up3__p,
	king_hello__p, king_stand__p, king_bullet__p
};

uint32_t* badGuy_states__p[2] = {
	king_badGuy1__p, king_badGuy2__p
};

uint32_t* king_screens__p[3] = {
	king_title__p, king_end__p, king_gameOver__p
};

static inline uint32_t shift_div_with_round_down(uint32_t num, uint32_t shift){
	uint32_t d = num >> shift;
	return d;
}

static inline uint32_t shift_div_with_round_up(uint32_t num, uint32_t shift){
	uint32_t d = num >> shift;
	uint32_t mask = (1<<shift)-1;
	if((num & mask) != 0){
		d++;
	}
	return d;
}

typedef enum {
	START_HELLO,
	STANDING_IDLE,
	RIGHT_WALK_L,
	RIGHT_WALK_R,
	RIGHT_WALK_STAND,
	UP_WALK_L,
	UP_WALK_R,
	UP_WALK_STAND,
	LEFT_WALK_L,
	LEFT_WALK_R,
	LEFT_STAND,
	DOWN_WALK_L,
	DOWN_WALK_R,
	DOWN_WALK_STAND
} king_anim_states_t;

typedef enum {
	TITLE,
	END,
	PLAY,
	GAME_OVER
} screen_state;

typedef enum {
	WALK_R,
	WALK_L
} badGuy_anim_states_t;

typedef struct {
	badGuy_anim_states_t state;
	uint8_t delay_cnt;
} badGuy_anim_t;

typedef struct {
	point_t pos;
	int visible;
	badGuy_anim_t anim;
} badGuy1_t;

typedef struct {
	point_t pos;
	int visible;
	badGuy_anim_t anim;
} badGuy2_t;

typedef struct {
	king_anim_states_t state;
	uint8_t delay_cnt;
} king_anim_t;

typedef struct {
	point_t pos;
	king_anim_t anim;
	int killCount;
} king_t;

typedef struct {
	point_t pos;
	int visible;
	int u;
	int d;
	int l;
	int r;
} bullet_t;

typedef struct {
	king_t king;
	bullet_t bullet;
	badGuy1_t badGuy1;
	badGuy2_t badGuy2;
} game_state_t;

void draw_sprite(
	uint32_t* src_p,
	uint16_t src_w,
	uint16_t src_h,
	uint16_t dst_x,
	uint16_t dst_y
) {
	
	uint16_t dst_x8 = shift_div_with_round_down(dst_x, 3);
	uint16_t src_w8 = shift_div_with_round_up(src_w, 3);
	
	for(uint16_t y = 0; y < src_h; y++){
		for(uint16_t x8 = 0; x8 < src_w8; x8++){
			uint32_t src_idx = y*src_w8 + x8;
			uint32_t pixels = src_p[src_idx];
			uint32_t dst_idx =
				(dst_y+y)*SCREEN_IDX4_W8 +
				(dst_x8+x8);
				
				for(uint8_t i = 0; i < 8; i++) {
				
					uint32_t px = (pixels>>(4*i))&0b1111;
					if(px != 0) {
						pack_idx4_p32[dst_idx] &= ~(0b1111 << (4*i));
					}
				
				}	
				pack_idx4_p32[dst_idx] |= pixels;
		}
	}
	
}

static void update_background (
	uint16_t dst_x8,
	uint16_t dst_y,
	uint32_t* block
) {
	uint16_t src_h = 16;
	uint16_t src_w8 = 2;
	
	for(uint16_t y = 0; y < src_h; y++){
		for(uint16_t x8 = 0; x8 < src_w8; x8++){
			uint32_t src_idx = y*src_w8 + x8;
			uint32_t pixels = block[src_idx];
			uint32_t dst_idx =
				(dst_y+y)*SCREEN_IDX4_W8 +
				(dst_x8+x8);
			pack_idx4_p32[dst_idx] = pixels;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// Game code.

screen_state sc = TITLE;

void startTitle(){
	// Setup.
	gpu_p32[0] = 2; // IDX4 mode.
	gpu_p32[1] = 1; // Packed mode.
	gpu_p32[0x800] = 0x00ff00ff; // Magenta for HUD.

	for(uint8_t i = 0; i < 16; i++){
		palette_p32[i] = palette[i];
	}

	/////////////////////////////////////
		// Poll controls.
		
	if(joypad.right){
		sc = PLAY;
	}else if(joypad.left){
		sc = PLAY;
	}else if(joypad.down){
		sc = PLAY;
	}else if(joypad.up){
		sc = PLAY;
	} else if(joypad.a){
		sc = PLAY;
	}
		
		/////////////////////////////////////
		// Gameplay.
		
		/////////////////////////////////////
		// Drawing.
			
		// Detecting rising edge of VSync.
	WAIT_UNITL_0(gpu_p32[2]);
	WAIT_UNITL_1(gpu_p32[2]);
		// Draw in buffer while it is in VSync.
			
	for(uint16_t r1 = 0; r1 < SCREEN_IDX4_H; r1++){
		for(uint16_t c8 = 0; c8 < SCREEN_IDX4_W8; c8++){
			pack_idx4_p32[r1*SCREEN_IDX4_W8 + c8] = 0x00000000;
		}
	}
			
	int src_h, src_w8, dst_y, dst_x8;
	src_h = 55;
	src_w8 = 12;
	dst_y = 81;
	dst_x8 =73;  
	
	for(uint16_t y = 0; y < src_h; y++){
		for(uint16_t x8 = 0; x8 < src_w8; x8++){
			uint32_t src_idx = y*src_w8 + x8;
			uint32_t pixels = king_title__p[src_idx];
			uint32_t dst_idx =
				(dst_y+y)*SCREEN_IDX4_W8 +
				(dst_x8+x8);
			pack_idx4_p32[dst_idx] = pixels;
		}
	}
}

void startGame(){
	// Setup.
	gpu_p32[0] = 2; // IDX4 mode.
	gpu_p32[1] = 1; // Packed mode.
	gpu_p32[0x800] = 0x00ff00ff; // Magenta for HUD.

	for(uint8_t i = 0; i < 16; i++){
		palette_p32[i] = palette[i];
	}

	// Game state.
	game_state_t gs;
	gs.king.pos.x = 120;
	gs.king.pos.y = 120;
	gs.king.anim.state = START_HELLO;
	gs.king.anim.delay_cnt = 0;
	gs.king.killCount = 0;
	
	gs.bullet.visible = 0;
	gs.bullet.pos.x = 0;
	gs.bullet.pos.y = 0;
	gs.bullet.u = 0;
	gs.bullet.d = 0;
	gs.bullet.l = 0;
	gs.bullet.r = 0;
	
	gs.badGuy1.visible = 0;
	gs.badGuy1.pos.x = 0;
	gs.badGuy1.pos.y = 0;
	gs.badGuy1.anim.state = WALK_R;
	gs.badGuy1.anim.delay_cnt = 0;
	
	gs.badGuy2.visible = 0;
	gs.badGuy2.pos.x = 0;
	gs.badGuy2.pos.y = 0;
	gs.badGuy2.anim.state = WALK_R;
	gs.badGuy2.anim.delay_cnt = 0;
	
	while(sc == PLAY){
	/////////////////////////////////////
		// Poll controls.
		int mov_x = 0;
		int mov_y = 0;
		int pressed_a = 0;
		uint16_t direction[4] = {0,0,0,0};// 1 - up, 10 - down, 100 - left, 1000 - right
		
		if(joypad.right){
			mov_x = +1;
			direction[3] = 1; 
		}else if(joypad.left){
			mov_x = mov_x -1;
			direction[2] = 1; 
		}else if(joypad.down){
			mov_y = +1;
			direction[1] = 1; 
		}else if(joypad.up){
			mov_y = mov_y -1;
			direction[0] = 1; 
		} else if(joypad.a){
			pressed_a +=1;
			if(!gs.bullet.visible){
				gs.bullet.visible = 1;
				gs.bullet.pos.x = gs.king.pos.x + 8;
				gs.bullet.pos.y = gs.king.pos.y + 8;	
				switch(gs.king.anim.state){
					case RIGHT_WALK_STAND:
						gs.bullet.r = 1;
						gs.bullet.u = 0;
						gs.bullet.d = 0;
						gs.bullet.l = 0;
						break;
					case LEFT_STAND:
						gs.bullet.l = 1;
						gs.bullet.u = 0;
						gs.bullet.d = 0;
						gs.bullet.r = 0;
						break;
					case UP_WALK_STAND:
						gs.bullet.u = 1;
						gs.bullet.r = 0;
						gs.bullet.d = 0;
						gs.bullet.l = 0;
						break;
					case DOWN_WALK_STAND:
						gs.bullet.d = 1;
						gs.bullet.u = 0;
						gs.bullet.r = 0;
						gs.bullet.l = 0;
						break;
					default:
						gs.bullet.r = 1;
						gs.bullet.u = 0;
						gs.bullet.d = 0;
						gs.bullet.l = 0;
						break;
				}
				if(!gs.badGuy1.visible){
					gs.badGuy1.visible = 1;
					gs.badGuy1.pos.x = 120;
					gs.badGuy1.pos.y = 242;
				}
				if(!gs.badGuy2.visible){
					gs.badGuy2.visible = 1;
					gs.badGuy2.pos.x = 232;
					gs.badGuy2.pos.y = 120;
				}
			}
		}
	
		/////////////////////////////////////
		// Gameplay.
		int newX = gs.king.pos.x + mov_x*STEP;
		int newY = gs.king.pos.y + mov_y*STEP;
		if(newX > 16 && newX < 232){
			gs.king.pos.x = newX;
		}
		if (newY >30 && newY < 242){
			gs.king.pos.y = newY;
		}

		if((gs.king.pos.x + 16 >= gs.badGuy1.pos.x) && (gs.king.pos.x < gs.badGuy1.pos.x + 16) && (gs.king.pos.y >= gs.badGuy1.pos.y) && (gs.king.pos.y < gs.badGuy1.pos.y + 16)){
				sc =  GAME_OVER;
		}
		
		if((gs.king.pos.x >= gs.badGuy2.pos.x) && (gs.king.pos.x < gs.badGuy2.pos.x + 16) && (gs.king.pos.y + 16 >= gs.badGuy2.pos.y) && (gs.king.pos.y < gs.badGuy2.pos.y + 16)){
				sc =  GAME_OVER;
		}

		if(gs.king.killCount == 10){
			sc = END;
		}
		
		if(gs.badGuy1.visible){
			gs.badGuy1.pos.y = gs.badGuy1.pos.y - 1*STEP;
			if(gs.badGuy1.pos.y < 32){
				gs.badGuy1.pos.x = 120;
				gs.badGuy1.pos.y = 242;
			}
		}
		if(gs.badGuy2.visible){
			gs.badGuy2.pos.x = gs.badGuy2.pos.x - 1*STEP;
			if(gs.badGuy2.pos.x < 16){
				gs.badGuy2.pos.x = 232;
				gs.badGuy2.pos.y = 120;
			}
		}
	
		if(gs.bullet.visible){
			if(gs.bullet.u){
				gs.bullet.pos.y =gs.bullet.pos.y - 1*STEP_BULLET;
				
				if(gs.bullet.pos.y < 32){
					gs.bullet.visible = 0;
				}
				
				if((gs.bullet.pos.x >= gs.badGuy1.pos.x) && (gs.bullet.pos.x < gs.badGuy1.pos.x+ 16) && (gs.bullet.pos.y >= gs.badGuy1.pos.y)&& (gs.bullet.pos.y < gs.badGuy1.pos.y + 16)){
					gs.badGuy1.pos.x = 120;
					gs.badGuy1.pos.y = 242;
					gs.bullet.visible = 0;
					gs.king.killCount++;
				}
		
				if((gs.bullet.pos.x > gs.badGuy2.pos.x) && (gs.bullet.pos.x <= gs.badGuy2.pos.x+ 16) && (gs.bullet.pos.y > gs.badGuy2.pos.y)&& (gs.bullet.pos.y <= gs.badGuy2.pos.y + 16)){
					gs.badGuy2.pos.x = 232;
					gs.badGuy2.pos.y = 120;
					gs.bullet.visible = 0;
					gs.king.killCount++;
				}
			}else if(gs.bullet.d){
				
				gs.bullet.pos.y =gs.bullet.pos.y + 1*STEP_BULLET;
				
				if(gs.bullet.pos.y > 242){
					gs.bullet.visible = 0;
				}
				
				if((gs.bullet.pos.x >= gs.badGuy1.pos.x) && (gs.bullet.pos.x < gs.badGuy1.pos.x+ 16) && (gs.bullet.pos.y >= gs.badGuy1.pos.y)&& (gs.bullet.pos.y < gs.badGuy1.pos.y + 16)){
					gs.badGuy1.pos.x = 120;
					gs.badGuy1.pos.y = 242;
					gs.bullet.visible = 0;
					gs.king.killCount++;
				}
		
				if((gs.bullet.pos.x > gs.badGuy2.pos.x) && (gs.bullet.pos.x <= gs.badGuy2.pos.x+ 16) && (gs.bullet.pos.y > gs.badGuy2.pos.y)&& (gs.bullet.pos.y <= gs.badGuy2.pos.y + 16)){
					gs.badGuy2.pos.x = 232;
					gs.badGuy2.pos.y = 120;
					gs.bullet.visible = 0;
					gs.king.killCount++;
				}
			}else if(gs.bullet.l){
				
				gs.bullet.pos.x =gs.bullet.pos.x - 1*STEP_BULLET;
				
				if(gs.bullet.pos.x < 16){
					gs.bullet.visible = 0;
				}
				
				if((gs.bullet.pos.x >= gs.badGuy1.pos.x) && (gs.bullet.pos.x < gs.badGuy1.pos.x+ 16) && (gs.bullet.pos.y >= gs.badGuy1.pos.y)&& (gs.bullet.pos.y+16 < gs.badGuy1.pos.y + 16)){
					gs.badGuy1.pos.x = 120;
					gs.badGuy1.pos.y = 242;
					gs.bullet.visible = 0;
					gs.king.killCount++;
				}
		
				if((gs.bullet.pos.x > gs.badGuy2.pos.x) && (gs.bullet.pos.x <= gs.badGuy2.pos.x+ 16) && (gs.bullet.pos.y > gs.badGuy2.pos.y)&& (gs.bullet.pos.y <= gs.badGuy2.pos.y + 16)){
					gs.badGuy2.pos.x = 232;
					gs.badGuy2.pos.y = 120;
					gs.bullet.visible = 0;
					gs.king.killCount++;
				}
			}else if(gs.bullet.r){
				
				gs.bullet.pos.x =gs.bullet.pos.x + 1*STEP_BULLET;
				
				if(gs.bullet.pos.x > 232){
					gs.bullet.visible = 0;
				}
				
				if((gs.bullet.pos.x >= gs.badGuy1.pos.x) && (gs.bullet.pos.x < gs.badGuy1.pos.x+ 16) && (gs.bullet.pos.y >= gs.badGuy1.pos.y)&& (gs.bullet.pos.y < gs.badGuy1.pos.y + 16)){
					gs.badGuy1.pos.x = 120;
					gs.badGuy1.pos.y = 242;
					gs.bullet.visible = 0;
					gs.king.killCount++;
				}
		
				if((gs.bullet.pos.x > gs.badGuy2.pos.x) && (gs.bullet.pos.x <= gs.badGuy2.pos.x+ 16) && (gs.bullet.pos.y > gs.badGuy2.pos.y)&& (gs.bullet.pos.y <= gs.badGuy2.pos.y + 16)){
					gs.badGuy2.pos.x = 232;
					gs.badGuy2.pos.y = 120;
					gs.bullet.visible = 0;
					gs.king.killCount++;
				}
			}
		}
		
		switch(gs.badGuy1.anim.state){
			case WALK_L:
				if(gs.badGuy1.anim.delay_cnt != 0){
					gs.badGuy1.anim.delay_cnt--;
				}else{
					gs.badGuy1.anim.delay_cnt = ANIM_DELAY;
					gs.badGuy1.anim.state = WALK_R;
				}
				break;
			case WALK_R:
				if(gs.badGuy1.anim.delay_cnt != 0){
					gs.badGuy1.anim.delay_cnt--;
				}else{
					gs.badGuy1.anim.delay_cnt = ANIM_DELAY;
					gs.badGuy1.anim.state = WALK_L;
				}
				break;
		}
		
		switch(gs.badGuy2.anim.state){
			case WALK_L:
				if(gs.badGuy2.anim.delay_cnt != 0){
					gs.badGuy2.anim.delay_cnt--;
				}else{
					gs.badGuy2.anim.delay_cnt = ANIM_DELAY;
					gs.badGuy2.anim.state = WALK_R;
				}
				break;
			case WALK_R:
				if(gs.badGuy2.anim.delay_cnt != 0){
					gs.badGuy2.anim.delay_cnt--;
				}else{
					gs.badGuy2.anim.delay_cnt = ANIM_DELAY;
					gs.badGuy2.anim.state = WALK_L;
				}
				break;
		}
		
		// 1 - up, 10 - down, 100 - left, 1000 - right
		switch(gs.king.anim.state){
			case START_HELLO:
				if(mov_x != 0 || mov_y == 0){
					gs.king.anim.delay_cnt = ANIM_DELAY;
					if(direction[2]){
						gs.king.anim.state = LEFT_WALK_L;
					}
					if(direction[3]){
						gs.king.anim.state = RIGHT_WALK_L;
					}
					if(direction[0]){
						gs.king.anim.state = UP_WALK_L;
					} 
					if(direction[1]){
						gs.king.anim.state = DOWN_WALK_L;
					}
				} 
				break;
			case RIGHT_WALK_L:
				if(gs.king.anim.delay_cnt != 0){
					gs.king.anim.delay_cnt--;
				}else{
					if(mov_x != 0 || mov_y != 0){
						gs.king.anim.delay_cnt = ANIM_DELAY;
					
						if(direction[2]){
							gs.king.anim.state = LEFT_WALK_R;
						}else if(direction[3]){
							gs.king.anim.state = RIGHT_WALK_R;
						}else if(direction[0]){
							gs.king.anim.state = UP_WALK_L;
						} else{
							gs.king.anim.state = DOWN_WALK_L;
						}
					}else{
					gs.king.anim.state = RIGHT_WALK_STAND;
					}
				}
				break;
			case RIGHT_WALK_R:
				if(gs.king.anim.delay_cnt != 0){
					gs.king.anim.delay_cnt--;
				}else{
					if(mov_x != 0 || mov_y != 0){
						gs.king.anim.delay_cnt = ANIM_DELAY;
						if(direction[2]){
							gs.king.anim.state = LEFT_WALK_L;
						}else if(direction[3]){
							gs.king.anim.state = RIGHT_WALK_L;
						}else if(direction[0]){
							gs.king.anim.state = UP_WALK_L;
						} else{
							gs.king.anim.state = DOWN_WALK_L;
						}
					}else{
					gs.king.anim.state = RIGHT_WALK_STAND;
					}
				}
				break;
			case RIGHT_WALK_STAND:
				if(mov_x != 0 || mov_y != 0){
						gs.king.anim.delay_cnt = ANIM_DELAY;
						if(direction[2]){
							gs.king.anim.state = LEFT_WALK_R;
						}else if(direction[3]){
							gs.king.anim.state = RIGHT_WALK_R;
						}else if(direction[0]){
							gs.king.anim.state = UP_WALK_L;
						} else{
							gs.king.anim.state = DOWN_WALK_L;
						}
				}
				break;
			case LEFT_WALK_R:
				if(gs.king.anim.delay_cnt != 0){
					gs.king.anim.delay_cnt--;
				}else{
					if(mov_x != 0 || mov_y != 0){
						gs.king.anim.delay_cnt = ANIM_DELAY;
						if(direction[2]){
							gs.king.anim.state = LEFT_WALK_L;
						}else if(direction[3]){
							gs.king.anim.state = RIGHT_WALK_L;
						}else if(direction[0]){
							gs.king.anim.state = UP_WALK_L;
						} else{
							gs.king.anim.state = DOWN_WALK_L;
						}
					}else{
					gs.king.anim.state = LEFT_STAND;
					}
				}
				break;
			case LEFT_WALK_L:
				if(gs.king.anim.delay_cnt != 0){
					gs.king.anim.delay_cnt--;
				}else{
					if(mov_x != 0 || mov_y != 0){
						gs.king.anim.delay_cnt = ANIM_DELAY;
						if(direction[2]){
							gs.king.anim.state = LEFT_WALK_R;
						}else if(direction[3]){
							gs.king.anim.state = RIGHT_WALK_R;
						}else if(direction[0]){
							gs.king.anim.state = UP_WALK_L;
						} else{
							gs.king.anim.state = DOWN_WALK_L;
						}
					}else{
					gs.king.anim.state = LEFT_STAND;
					}
				}
				break;
			case LEFT_STAND:
				if(mov_x != 0 || mov_y != 0){
					gs.king.anim.delay_cnt = ANIM_DELAY;
					if(direction[2]){
						gs.king.anim.state = LEFT_WALK_R;
					}else if(direction[3]){
						gs.king.anim.state = RIGHT_WALK_R;
					}else if(direction[0]){
						gs.king.anim.state = UP_WALK_L;
					} else{
						gs.king.anim.state = DOWN_WALK_L;
					}
				}
				break;
			case UP_WALK_L:
				if(gs.king.anim.delay_cnt != 0){
					gs.king.anim.delay_cnt--;
				}else{
					if(mov_x != 0 || mov_y != 0){
						gs.king.anim.delay_cnt = ANIM_DELAY;
						if(direction[2]){
							gs.king.anim.state = LEFT_WALK_R;
						}else if(direction[3]){
							gs.king.anim.state = RIGHT_WALK_R;
						}else if(direction[0]){
							gs.king.anim.state = UP_WALK_R;
						} else{
							gs.king.anim.state = DOWN_WALK_R;
						}
					}else{
					gs.king.anim.state = UP_WALK_STAND;
					}
				}
				break;
			case UP_WALK_R:
				if(gs.king.anim.delay_cnt != 0){
					gs.king.anim.delay_cnt--;
				}else{
					if(mov_x != 0 || mov_y != 0){
						gs.king.anim.delay_cnt = ANIM_DELAY;
						if(direction[2]){
							gs.king.anim.state = LEFT_WALK_R;
						}else if(direction[3]){
							gs.king.anim.state = RIGHT_WALK_R;
						}else if(direction[0]){
							gs.king.anim.state = UP_WALK_L;
						} else{
							gs.king.anim.state = DOWN_WALK_L;
						}
					}else{
					gs.king.anim.state = UP_WALK_STAND;
					}
				}
				break;
			case UP_WALK_STAND:
				if(mov_x != 0 || mov_y != 0){
					gs.king.anim.delay_cnt = ANIM_DELAY;
					if(direction[2]){
						gs.king.anim.state = LEFT_WALK_R;
					}else if(direction[3]){
						gs.king.anim.state = RIGHT_WALK_R;
					}else if(direction[0]){
						gs.king.anim.state = UP_WALK_L;
					} else{
						gs.king.anim.state = DOWN_WALK_L;
					}
				}
				break;
			case DOWN_WALK_L:
				if(gs.king.anim.delay_cnt != 0){
					gs.king.anim.delay_cnt--;
				}else{
					if(mov_x != 0 || mov_y != 0){
						gs.king.anim.delay_cnt = ANIM_DELAY;
						if(direction[2]){
							gs.king.anim.state = LEFT_WALK_R;
						}else if(direction[3]){
							gs.king.anim.state = RIGHT_WALK_R;
						}else if(direction[0]){
							gs.king.anim.state = UP_WALK_R;
						} else{
							gs.king.anim.state = DOWN_WALK_R;
						}
					}else{
					gs.king.anim.state = DOWN_WALK_STAND;
					}
				}
				break;
			case DOWN_WALK_R:
				if(gs.king.anim.delay_cnt != 0){
					gs.king.anim.delay_cnt--;
				}else{
					if(mov_x != 0 || mov_y != 0){
						gs.king.anim.delay_cnt = ANIM_DELAY;
						if(direction[2]){
							gs.king.anim.state = LEFT_WALK_R;
						}else if(direction[3]){
							gs.king.anim.state = RIGHT_WALK_R;
						}else if(direction[0]){
							gs.king.anim.state = UP_WALK_L;
						} else{
							gs.king.anim.state = DOWN_WALK_L;
						}
					}else{
					gs.king.anim.state = DOWN_WALK_STAND;
					}
				}
				break;
			case DOWN_WALK_STAND:
				if(gs.king.anim.delay_cnt != 0){
					gs.king.anim.delay_cnt--;
				}else{
					if(mov_x != 0 || mov_y != 0){
					gs.king.anim.delay_cnt = ANIM_DELAY;
					if(direction[2]){
						gs.king.anim.state = LEFT_WALK_R;
					}else if(direction[3]){
						gs.king.anim.state = RIGHT_WALK_R;
					}else if(direction[0]){
						gs.king.anim.state = UP_WALK_L;
					} else{
						gs.king.anim.state = DOWN_WALK_L;
						}
					}
				}
				break;
		}
		
		/////////////////////////////////////
		// Drawing.
		
		// Detecting rising edge of VSync.
		WAIT_UNITL_0(gpu_p32[2]);
		WAIT_UNITL_1(gpu_p32[2]);
		// Draw in buffer while it is in VSync.
		
		for(uint16_t r1 = 0; r1 < SCREEN_IDX4_H; r1++){
			for(uint16_t c8 = 0; c8 < SCREEN_IDX4_W8; c8++){
				pack_idx4_p32[r1*SCREEN_IDX4_W8 + c8] = 0x00000000;
			}
		}
		
		int i = 0;
		int j = 0;
		/* EZ A MAI GAME */
		for(int g = 0; g < SCREEN_IDX4_W8; g+=2, j++){
			i = 0;
			for(int f = 15; f < SCREEN_IDX4_H; f+=16, i++){
					update_background(g,f, background[j*16 + i]);
			}
		}
		
		//Draw le'bullet_t
			if(gs.bullet.visible){
				draw_sprite(
					king_states__p[14], 4, 4, gs.bullet.pos.x, gs.bullet.pos.y
				);
			}

		// Draw le'man.
			switch(gs.king.anim.state){
				case START_HELLO:
					draw_sprite(
						king_states__p[12], 16, 16, gs.king.pos.x, gs.king.pos.y
					);
					break;
				case RIGHT_WALK_R:
					draw_sprite(
						king_states__p[6], 16, 16, gs.king.pos.x, gs.king.pos.y
					);
					break;
				case RIGHT_WALK_STAND:
					draw_sprite(
						king_states__p[7], 16, 16, gs.king.pos.x, gs.king.pos.y
					);
					break;
				case RIGHT_WALK_L:
					draw_sprite(
						king_states__p[8], 16, 16, gs.king.pos.x, gs.king.pos.y
					);
					break;
				case LEFT_WALK_R:
					draw_sprite(
						king_states__p[3], 16, 16, gs.king.pos.x, gs.king.pos.y
					);
					break;
				case LEFT_STAND:
					draw_sprite(
						king_states__p[4], 16, 16, gs.king.pos.x, gs.king.pos.y
					);
					break;
				case LEFT_WALK_L:
					draw_sprite(
						king_states__p[5], 16, 16, gs.king.pos.x, gs.king.pos.y
					);
					break;
				case DOWN_WALK_R:
					draw_sprite(
						king_states__p[0], 16, 16, gs.king.pos.x, gs.king.pos.y
					);
					break;
				case DOWN_WALK_STAND:
					draw_sprite(
						king_states__p[1], 16, 16, gs.king.pos.x, gs.king.pos.y
					);
					break;
				case DOWN_WALK_L:
					draw_sprite(
						king_states__p[2], 16, 16, gs.king.pos.x, gs.king.pos.y
					);
					break;
				case UP_WALK_R:
					draw_sprite(
						king_states__p[9], 16, 16, gs.king.pos.x, gs.king.pos.y
					);
					break;
				case UP_WALK_STAND:
					draw_sprite(
						king_states__p[10], 16, 16, gs.king.pos.x, gs.king.pos.y
					);
					break;
				case UP_WALK_L:
					draw_sprite(
						king_states__p[11], 16, 16, gs.king.pos.x, gs.king.pos.y
					);
					break;
				}
				
		// Draw le'bad guys.
			if(gs.badGuy1.visible){
				switch(gs.badGuy1.anim.state){
				case WALK_R:
						draw_sprite(
							badGuy_states__p[0], 15, 15, gs.badGuy1.pos.x, gs.badGuy1.pos.y
						);
					break;
				case WALK_L:
					draw_sprite(
						badGuy_states__p[1], 15, 15, gs.badGuy1.pos.x, gs.badGuy1.pos.y
					);
					break;
				}	
			}
			if(gs.badGuy2.visible){
				switch(gs.badGuy2.anim.state){
				case WALK_R:
						draw_sprite(
							badGuy_states__p[0], 15, 15, gs.badGuy2.pos.x, gs.badGuy2.pos.y
						);
					break;
				case WALK_L:
					draw_sprite(
						badGuy_states__p[1], 15, 15, gs.badGuy2.pos.x, gs.badGuy2.pos.y
					);
					break;
				}
			}
	}
}

void gameOver(){
	// Setup.
	gpu_p32[0] = 2; // IDX4 mode.
	gpu_p32[1] = 1; // Packed mode.
	gpu_p32[0x800] = 0x00ff00ff; // Magenta for HUD.

	for(uint8_t i = 0; i < 16; i++){
		palette_p32[i] = palette[i];
	}

	/////////////////////////////////////
		// Poll controls.
		
	if(joypad.right){
		sc = PLAY;
	}else if(joypad.left){
		sc = PLAY;
	}else if(joypad.down){
		sc = PLAY;
	}else if(joypad.up){
		sc = PLAY;
	} else if(joypad.a){
		sc = PLAY;
	}
		
		/////////////////////////////////////
		// Gameplay.
		
		/////////////////////////////////////
		// Drawing.
		
		// Detecting rising edge of VSync.
	WAIT_UNITL_0(gpu_p32[2]);
	WAIT_UNITL_1(gpu_p32[2]);
		// Draw in buffer while it is in VSync.
		
	for(uint16_t r1 = 0; r1 < SCREEN_IDX4_H+16; r1++){
		for(uint16_t c8 = 0; c8 < SCREEN_IDX4_W8; c8++){
			pack_idx4_p32[r1*SCREEN_IDX4_W8 + c8] = 0x00000000;
		}
	}
			
	int src_h, src_w8, dst_y, dst_x8;
	src_h = 32;
	src_w8 = 4;
	dst_y = 81;
	dst_x8 =73;
	
	for(uint16_t y = 0; y < src_h; y++){
		for(uint16_t x8 = 0; x8 < src_w8; x8++){
			uint32_t src_idx = y*src_w8 + x8;
			uint32_t pixels = king_gameOver__p[src_idx];
			uint32_t dst_idx =
				(dst_y+y)*SCREEN_IDX4_W8 +
				(dst_x8+x8);
			pack_idx4_p32[dst_idx] = pixels;
		}
	}
}

void endTitle(){
	// Setup.
	gpu_p32[0] = 2; // IDX4 mode.
	gpu_p32[1] = 1; // Packed mode.
	gpu_p32[0x800] = 0x00ff00ff; // Magenta for HUD.

	for(uint8_t i = 0; i < 16; i++){
		palette_p32[i] = palette[i];
	}

	/////////////////////////////////////
		// Poll controls.
		
	if(joypad.right){
		sc = PLAY;
	}else if(joypad.left){
		sc = PLAY;
	}else if(joypad.down){
		sc = PLAY;
	}else if(joypad.up){
		sc = PLAY;
	} else if(joypad.a){
		sc = PLAY;
	}
		
		/////////////////////////////////////
		// Gameplay.
		
		/////////////////////////////////////
		// Drawing.
		
		// Detecting rising edge of VSync.
	WAIT_UNITL_0(gpu_p32[2]);
	WAIT_UNITL_1(gpu_p32[2]);
		// Draw in buffer while it is in VSync.
		
	for(uint16_t r1 = 0; r1 < SCREEN_IDX4_H+16; r1++){
		for(uint16_t c8 = 0; c8 < SCREEN_IDX4_W8; c8++){
			pack_idx4_p32[r1*SCREEN_IDX4_W8 + c8] = 0x00000000;
		}
	}
		
	int src_h, src_w8, dst_y, dst_x8;
	src_h = 46;
	src_w8 = 8;
	dst_y = 81;
	dst_x8 =73;  
	
	for(uint16_t y = 0; y < src_h; y++){
		for(uint16_t x8 = 0; x8 < src_w8; x8++){
			uint32_t src_idx = y*src_w8 + x8;
			uint32_t pixels = king_end__p[src_idx];
			uint32_t dst_idx =
				(dst_y+y)*SCREEN_IDX4_W8 +
				(dst_x8+x8);
			pack_idx4_p32[dst_idx] = pixels;
		}
	}
}

int main(void) {

	while(1){
		switch(sc){
			case TITLE: 
				startTitle();
				break;
			case PLAY:
				startGame();
				break;
			case GAME_OVER:
				gameOver();
				break;
			case END:
				endTitle();
				break;
		}
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
