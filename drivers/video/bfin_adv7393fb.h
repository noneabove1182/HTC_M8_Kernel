/*
 * Frame buffer driver for ADV7393/2 video encoder
 *
 * Copyright 2006-2009 Analog Devices Inc.
 * Licensed under the GPL-2 or late.
 */

#ifndef __BFIN_ADV7393FB_H__
#define __BFIN_ADV7393FB_H__

#define BFIN_LCD_NBR_PALETTE_ENTRIES	256

#ifdef CONFIG_NTSC
# define VMODE 0
#endif
#ifdef CONFIG_PAL
# define VMODE 1
#endif
#ifdef CONFIG_NTSC_640x480
# define VMODE 2
#endif
#ifdef CONFIG_PAL_640x480
# define VMODE 3
#endif
#ifdef CONFIG_NTSC_YCBCR
# define VMODE 4
#endif
#ifdef CONFIG_PAL_YCBCR
# define VMODE 5
#endif

#ifndef VMODE
# define VMODE 1
#endif

#ifdef CONFIG_ADV7393_2XMEM
# define VMEM 2
#else
# define VMEM 1
#endif

#if defined(CONFIG_BF537) || defined(CONFIG_BF536) || defined(CONFIG_BF534)
# define DMA_CFG_VAL	0x7935	
# define VB_DUMMY_MEMORY_SOURCE	L1_DATA_B_START
#else
# define DMA_CFG_VAL	0x7915
# define VB_DUMMY_MEMORY_SOURCE	BOOT_ROM_START
#endif

enum {
	DESTRUCT,
	BUILD,
};

enum {
	POWER_ON,
	POWER_DOWN,
	BLANK_ON,
	BLANK_OFF,
};

#define DRIVER_NAME "bfin-adv7393"

struct adv7393fb_modes {
	const s8 name[25];	
	u16 xres;		
	u16 yres;		
	u16 bpp;
	u16 vmode;
	u16 a_lines;		
	u16 vb1_lines;		
	u16 vb2_lines;		
	u16 tot_lines;		
	u16 boeft_blank;	
	u16 aoeft_blank;	
	const s8 *adv7393_i2c_initd;
	u16 adv7393_i2c_initd_len;
};

static const u8 init_NTSC_TESTPATTERN[] = {
	0x00, 0x1E,	
	0x01, 0x00,	
	0x80, 0x10,	
	0x82, 0xCB,	
	0x84, 0x40,	
};

static const u8 init_NTSC[] = {
	0x00, 0x1E,	
	0xC3, 0x26,	
	0xC5, 0x12,	
	0xC2, 0x4A,	
	0xC6, 0x5E,	
	0xBD, 0x19,	
	0xBF, 0x42,	
	0x8C, 0x1F,	
	0x8D, 0x7C,	
	0x8E, 0xF0,	
	0x8F, 0x21,	
	0x01, 0x00,	
	0x80, 0x30,	
	0x82, 0x8B,	
	0x87, 0x80,	
	0x86, 0x82,
	0x8B, 0x11,
	0x88, 0x20,
	0x8A, 0x0d,
};

static const u8 init_PAL[] = {
	0x00, 0x1E,	
	0xC3, 0x26,	
	0xC5, 0x12,	
	0xC2, 0x4A,	
	0xC6, 0x5E,	
	0xBD, 0x19,	
	0xBF, 0x42,	
	0x8C, 0xCB,	
	0x8D, 0x8A,	
	0x8E, 0x09,	
	0x8F, 0x2A,	
	0x01, 0x00,	
	0x80, 0x11,	
	0x82, 0x8B,	
	0x87, 0x80,	
	0x86, 0x82,
	0x8B, 0x11,
	0x88, 0x20,
	0x8A, 0x0d,
};

static const u8 init_NTSC_YCbCr[] = {
	0x00, 0x1E,	
	0x8C, 0x1F,	
	0x8D, 0x7C,	
	0x8E, 0xF0,	
	0x8F, 0x21,	
	0x01, 0x00,	
	0x80, 0x30,	
	0x82, 0x8B,	
	0x87, 0x00,	
	0x86, 0x82,
	0x8B, 0x11,
	0x88, 0x08,
	0x8A, 0x0d,
};

static const u8 init_PAL_YCbCr[] = {
	0x00, 0x1E,	
	0x8C, 0xCB,	
	0x8D, 0x8A,	
	0x8E, 0x09,	
	0x8F, 0x2A,	
	0x01, 0x00,	
	0x80, 0x11,	
	0x82, 0x8B,	
	0x87, 0x00,	
	0x86, 0x82,
	0x8B, 0x11,
	0x88, 0x08,
	0x8A, 0x0d,
};

static struct adv7393fb_modes known_modes[] = {
	
	{
		.name = "NTSC 720x480",
		.xres = 720,
		.yres = 480,
		.bpp = 16,
		.vmode = FB_VMODE_INTERLACED,
		.a_lines = 240,
		.vb1_lines = 22,
		.vb2_lines = 23,
		.tot_lines = 525,
		.boeft_blank = 16,
		.aoeft_blank = 122,
		.adv7393_i2c_initd = init_NTSC,
		.adv7393_i2c_initd_len = sizeof(init_NTSC)
	},
	
	{
		.name = "PAL 720x576",
		.xres = 720,
		.yres = 576,
		.bpp = 16,
		.vmode = FB_VMODE_INTERLACED,
		.a_lines = 288,
		.vb1_lines = 24,
		.vb2_lines = 25,
		.tot_lines = 625,
		.boeft_blank = 12,
		.aoeft_blank = 132,
		.adv7393_i2c_initd = init_PAL,
		.adv7393_i2c_initd_len = sizeof(init_PAL)
	},
	
	{
		.name = "NTSC 640x480",
		.xres = 640,
		.yres = 480,
		.bpp = 16,
		.vmode = FB_VMODE_INTERLACED,
		.a_lines = 240,
		.vb1_lines = 22,
		.vb2_lines = 23,
		.tot_lines = 525,
		.boeft_blank = 16 + 40,
		.aoeft_blank = 122 + 40,
		.adv7393_i2c_initd = init_NTSC,
		.adv7393_i2c_initd_len = sizeof(init_NTSC)
	},
	
	{
		.name = "PAL 640x480",
		.xres = 640,
		.yres = 480,
		.bpp = 16,
		.vmode = FB_VMODE_INTERLACED,
		.a_lines = 288 - 20,
		.vb1_lines = 24 + 20,
		.vb2_lines = 25 + 20,
		.tot_lines = 625,
		.boeft_blank = 12 + 40,
		.aoeft_blank = 132 + 40,
		.adv7393_i2c_initd = init_PAL,
		.adv7393_i2c_initd_len = sizeof(init_PAL)
	},
	
	{
		.name = "NTSC 720x480 YCbCR",
		.xres = 720,
		.yres = 480,
		.bpp = 16,
		.vmode = FB_VMODE_INTERLACED,
		.a_lines = 240,
		.vb1_lines = 22,
		.vb2_lines = 23,
		.tot_lines = 525,
		.boeft_blank = 16,
		.aoeft_blank = 122,
		.adv7393_i2c_initd = init_NTSC_YCbCr,
		.adv7393_i2c_initd_len = sizeof(init_NTSC_YCbCr)
	},
	
	{
		.name = "PAL 720x576 YCbCR",
		.xres = 720,
		.yres = 576,
		.bpp = 16,
		.vmode = FB_VMODE_INTERLACED,
		.a_lines = 288,
		.vb1_lines = 24,
		.vb2_lines = 25,
		.tot_lines = 625,
		.boeft_blank = 12,
		.aoeft_blank = 132,
		.adv7393_i2c_initd = init_PAL_YCbCr,
		.adv7393_i2c_initd_len = sizeof(init_PAL_YCbCr)
	}
};

struct adv7393fb_regs {

};

struct adv7393fb_device {
	struct fb_info info;	

	struct i2c_client *client;

	struct dmasg *descriptor_list_head;
	struct dmasg *vb1;
	struct dmasg *av1;
	struct dmasg *vb2;
	struct dmasg *av2;

	dma_addr_t dma_handle;

	struct fb_info bfin_adv7393_fb;

	struct adv7393fb_modes *modes;

	struct adv7393fb_regs *regs;	
	size_t regs_len;
	size_t fb_len;
	size_t line_len;
	u16 open;
	u16 *fb_mem;		

};

#define to_adv7393fb_device(_info) \
	  (_info ? container_of(_info, struct adv7393fb_device, info) : NULL);

static int bfin_adv7393_fb_open(struct fb_info *info, int user);
static int bfin_adv7393_fb_release(struct fb_info *info, int user);
static int bfin_adv7393_fb_check_var(struct fb_var_screeninfo *var,
				     struct fb_info *info);

static int bfin_adv7393_fb_pan_display(struct fb_var_screeninfo *var,
				       struct fb_info *info);

static int bfin_adv7393_fb_blank(int blank, struct fb_info *info);

static void bfin_config_ppi(struct adv7393fb_device *fbdev);
static int bfin_config_dma(struct adv7393fb_device *fbdev);
static void bfin_disable_dma(void);
static void bfin_enable_ppi(void);
static void bfin_disable_ppi(void);

static inline int adv7393_write(struct i2c_client *client, u8 reg, u8 value);
static inline int adv7393_read(struct i2c_client *client, u8 reg);
static int adv7393_write_block(struct i2c_client *client, const u8 *data,
			       unsigned int len);

int bfin_adv7393_fb_cursor(struct fb_info *info, struct fb_cursor *cursor);
static int bfin_adv7393_fb_setcolreg(u_int, u_int, u_int, u_int,
				     u_int, struct fb_info *info);

#endif
