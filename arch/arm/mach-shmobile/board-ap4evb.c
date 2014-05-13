/*
 * AP4EVB board support
 *
 * Copyright (C) 2010  Magnus Damm
 * Copyright (C) 2008  Yoshihiro Shimoda
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include <linux/clk.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/mfd/tmio.h>
#include <linux/mmc/host.h>
#include <linux/mmc/sh_mobile_sdhi.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/physmap.h>
#include <linux/mmc/sh_mmcif.h>
#include <linux/i2c.h>
#include <linux/i2c/tsc2007.h>
#include <linux/io.h>
#include <linux/smsc911x.h>
#include <linux/sh_intc.h>
#include <linux/sh_clk.h>
#include <linux/gpio.h>
#include <linux/input.h>
#include <linux/leds.h>
#include <linux/input/sh_keysc.h>
#include <linux/usb/r8a66597.h>
#include <linux/pm_clock.h>
#include <linux/dma-mapping.h>

#include <media/sh_mobile_ceu.h>
#include <media/sh_mobile_csi2.h>
#include <media/soc_camera.h>

#include <sound/sh_fsi.h>

#include <video/sh_mobile_hdmi.h>
#include <video/sh_mobile_lcdc.h>
#include <video/sh_mipi_dsi.h>

#include <mach/common.h>
#include <mach/irqs.h>
#include <mach/sh7372.h>

#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/setup.h>









static struct mtd_partition nor_flash_partitions[] = {
	{
		.name		= "loader",
		.offset		= 0x00000000,
		.size		= 512 * 1024,
		.mask_flags	= MTD_WRITEABLE,
	},
	{
		.name		= "bootenv",
		.offset		= MTDPART_OFS_APPEND,
		.size		= 512 * 1024,
		.mask_flags	= MTD_WRITEABLE,
	},
	{
		.name		= "kernel_ro",
		.offset		= MTDPART_OFS_APPEND,
		.size		= 8 * 1024 * 1024,
		.mask_flags	= MTD_WRITEABLE,
	},
	{
		.name		= "kernel",
		.offset		= MTDPART_OFS_APPEND,
		.size		= 8 * 1024 * 1024,
	},
	{
		.name		= "data",
		.offset		= MTDPART_OFS_APPEND,
		.size		= MTDPART_SIZ_FULL,
	},
};

static struct physmap_flash_data nor_flash_data = {
	.width		= 2,
	.parts		= nor_flash_partitions,
	.nr_parts	= ARRAY_SIZE(nor_flash_partitions),
};

static struct resource nor_flash_resources[] = {
	[0]	= {
		.start	= 0x20000000, 
		.end	= 0x28000000 - 1, 
		.flags	= IORESOURCE_MEM,
	}
};

static struct platform_device nor_flash_device = {
	.name		= "physmap-flash",
	.dev		= {
		.platform_data	= &nor_flash_data,
	},
	.num_resources	= ARRAY_SIZE(nor_flash_resources),
	.resource	= nor_flash_resources,
};

static struct resource smc911x_resources[] = {
	{
		.start	= 0x14000000,
		.end	= 0x16000000 - 1,
		.flags	= IORESOURCE_MEM,
	}, {
		.start	= evt2irq(0x02c0) ,
		.flags	= IORESOURCE_IRQ | IORESOURCE_IRQ_LOWLEVEL,
	},
};

static struct smsc911x_platform_config smsc911x_info = {
	.flags		= SMSC911X_USE_16BIT | SMSC911X_SAVE_MAC_ADDRESS,
	.irq_polarity   = SMSC911X_IRQ_POLARITY_ACTIVE_LOW,
	.irq_type       = SMSC911X_IRQ_TYPE_PUSH_PULL,
};

static struct platform_device smc911x_device = {
	.name           = "smsc911x",
	.id             = -1,
	.num_resources  = ARRAY_SIZE(smc911x_resources),
	.resource       = smc911x_resources,
	.dev            = {
		.platform_data = &smsc911x_info,
	},
};

static int slot_cn7_get_cd(struct platform_device *pdev)
{
	return !gpio_get_value(GPIO_PORT41);
}
static struct sh_mobile_meram_info meram_info = {
	.addr_mode      = SH_MOBILE_MERAM_MODE1,
};

static struct resource meram_resources[] = {
	[0] = {
		.name	= "regs",
		.start	= 0xe8000000,
		.end	= 0xe807ffff,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.name	= "meram",
		.start	= 0xe8080000,
		.end	= 0xe81fffff,
		.flags	= IORESOURCE_MEM,
	},
};

static struct platform_device meram_device = {
	.name           = "sh_mobile_meram",
	.id             = 0,
	.num_resources  = ARRAY_SIZE(meram_resources),
	.resource       = meram_resources,
	.dev            = {
		.platform_data = &meram_info,
	},
};

static struct resource sh_mmcif_resources[] = {
	[0] = {
		.name	= "MMCIF",
		.start	= 0xE6BD0000,
		.end	= 0xE6BD00FF,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		
		.start	= evt2irq(0x1ac0),
		.flags	= IORESOURCE_IRQ,
	},
	[2] = {
		
		.start	= evt2irq(0x1ae0),
		.flags	= IORESOURCE_IRQ,
	},
};

static struct sh_mmcif_plat_data sh_mmcif_plat = {
	.sup_pclk	= 0,
	.ocr		= MMC_VDD_165_195 | MMC_VDD_32_33 | MMC_VDD_33_34,
	.caps		= MMC_CAP_4_BIT_DATA |
			  MMC_CAP_8_BIT_DATA |
			  MMC_CAP_NEEDS_POLL,
	.get_cd		= slot_cn7_get_cd,
	.slave_id_tx	= SHDMA_SLAVE_MMCIF_TX,
	.slave_id_rx	= SHDMA_SLAVE_MMCIF_RX,
};

static struct platform_device sh_mmcif_device = {
	.name		= "sh_mmcif",
	.id		= 0,
	.dev		= {
		.dma_mask		= NULL,
		.coherent_dma_mask	= 0xffffffff,
		.platform_data		= &sh_mmcif_plat,
	},
	.num_resources	= ARRAY_SIZE(sh_mmcif_resources),
	.resource	= sh_mmcif_resources,
};

static struct sh_mobile_sdhi_info sdhi0_info = {
	.dma_slave_tx	= SHDMA_SLAVE_SDHI0_TX,
	.dma_slave_rx	= SHDMA_SLAVE_SDHI0_RX,
	.tmio_caps	= MMC_CAP_SDIO_IRQ,
};

static struct resource sdhi0_resources[] = {
	[0] = {
		.name	= "SDHI0",
		.start  = 0xe6850000,
		.end    = 0xe68500ff,
		.flags  = IORESOURCE_MEM,
	},
	[1] = {
		.start	= evt2irq(0x0e00) ,
		.flags	= IORESOURCE_IRQ,
	},
	[2] = {
		.start	= evt2irq(0x0e20) ,
		.flags	= IORESOURCE_IRQ,
	},
	[3] = {
		.start	= evt2irq(0x0e40) ,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device sdhi0_device = {
	.name           = "sh_mobile_sdhi",
	.num_resources  = ARRAY_SIZE(sdhi0_resources),
	.resource       = sdhi0_resources,
	.id             = 0,
	.dev	= {
		.platform_data	= &sdhi0_info,
	},
};

static struct sh_mobile_sdhi_info sdhi1_info = {
	.dma_slave_tx	= SHDMA_SLAVE_SDHI1_TX,
	.dma_slave_rx	= SHDMA_SLAVE_SDHI1_RX,
	.tmio_ocr_mask	= MMC_VDD_165_195,
	.tmio_flags	= TMIO_MMC_WRPROTECT_DISABLE,
	.tmio_caps	= MMC_CAP_NEEDS_POLL | MMC_CAP_SDIO_IRQ,
	.get_cd		= slot_cn7_get_cd,
};

static struct resource sdhi1_resources[] = {
	[0] = {
		.name	= "SDHI1",
		.start  = 0xe6860000,
		.end    = 0xe68600ff,
		.flags  = IORESOURCE_MEM,
	},
	[1] = {
		.start	= evt2irq(0x0e80), 
		.flags	= IORESOURCE_IRQ,
	},
	[2] = {
		.start	= evt2irq(0x0ea0), 
		.flags	= IORESOURCE_IRQ,
	},
	[3] = {
		.start	= evt2irq(0x0ec0), 
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device sdhi1_device = {
	.name           = "sh_mobile_sdhi",
	.num_resources  = ARRAY_SIZE(sdhi1_resources),
	.resource       = sdhi1_resources,
	.id             = 1,
	.dev	= {
		.platform_data	= &sdhi1_info,
	},
};

static void usb1_host_port_power(int port, int power)
{
	if (!power) 
		return;

	
	__raw_writew(__raw_readw(0xE68B0008) | 0x600, 0xE68B0008);
}

static struct r8a66597_platdata usb1_host_data = {
	.on_chip	= 1,
	.port_power	= usb1_host_port_power,
};

static struct resource usb1_host_resources[] = {
	[0] = {
		.name	= "USBHS",
		.start	= 0xE68B0000,
		.end	= 0xE68B00E6 - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= evt2irq(0x1ce0) ,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device usb1_host_device = {
	.name	= "r8a66597_hcd",
	.id	= 1,
	.dev = {
		.dma_mask		= NULL,         
		.coherent_dma_mask	= 0xffffffff,
		.platform_data		= &usb1_host_data,
	},
	.num_resources	= ARRAY_SIZE(usb1_host_resources),
	.resource	= usb1_host_resources,
};

#ifdef CONFIG_AP4EVB_QHD

static struct sh_keysc_info keysc_info = {
	.mode		= SH_KEYSC_MODE_1,
	.scan_timing	= 3,
	.delay		= 2500,
	.keycodes = {
		KEY_0, KEY_1, KEY_2, KEY_3, KEY_4,
		KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
		KEY_A, KEY_B, KEY_C, KEY_D, KEY_E,
		KEY_F, KEY_G, KEY_H, KEY_I, KEY_J,
		KEY_K, KEY_L, KEY_M, KEY_N, KEY_O,
	},
};

static struct resource keysc_resources[] = {
	[0] = {
		.name	= "KEYSC",
		.start  = 0xe61b0000,
		.end    = 0xe61b0063,
		.flags  = IORESOURCE_MEM,
	},
	[1] = {
		.start  = evt2irq(0x0be0), 
		.flags  = IORESOURCE_IRQ,
	},
};

static struct platform_device keysc_device = {
	.name           = "sh_keysc",
	.id             = 0, 
	.num_resources  = ARRAY_SIZE(keysc_resources),
	.resource       = keysc_resources,
	.dev	= {
		.platform_data	= &keysc_info,
	},
};

static int sh_mipi_set_dot_clock(struct platform_device *pdev,
				 void __iomem *base,
				 int enable)
{
	struct clk *pck = clk_get(&pdev->dev, "dsip_clk");

	if (IS_ERR(pck))
		return PTR_ERR(pck);

	if (enable) {
		clk_set_rate(pck, clk_round_rate(pck, 24000000));
		clk_enable(pck);
	} else {
		clk_disable(pck);
	}

	clk_put(pck);

	return 0;
}

static struct resource mipidsi0_resources[] = {
	[0] = {
		.start  = 0xffc60000,
		.end    = 0xffc63073,
		.flags  = IORESOURCE_MEM,
	},
	[1] = {
		.start  = 0xffc68000,
		.end    = 0xffc680ef,
		.flags  = IORESOURCE_MEM,
	},
};

static struct sh_mobile_lcdc_info lcdc_info;

static struct sh_mipi_dsi_info mipidsi0_info = {
	.data_format	= MIPI_RGB888,
	.lcd_chan	= &lcdc_info.ch[0],
	.lane		= 2,
	.vsynw_offset	= 17,
	.phyctrl	= 0x6 << 8,
	.flags		= SH_MIPI_DSI_SYNC_PULSES_MODE |
			  SH_MIPI_DSI_HSbyteCLK,
	.set_dot_clock	= sh_mipi_set_dot_clock,
};

static struct platform_device mipidsi0_device = {
	.name           = "sh-mipi-dsi",
	.num_resources  = ARRAY_SIZE(mipidsi0_resources),
	.resource       = mipidsi0_resources,
	.id             = 0,
	.dev	= {
		.platform_data	= &mipidsi0_info,
	},
};

static struct platform_device *qhd_devices[] __initdata = {
	&mipidsi0_device,
	&keysc_device,
};
#endif 

static const struct fb_videomode ap4evb_lcdc_modes[] = {
	{
#ifdef CONFIG_AP4EVB_QHD
		.name		= "R63302(QHD)",
		.xres		= 544,
		.yres		= 961,
		.left_margin	= 72,
		.right_margin	= 600,
		.hsync_len	= 16,
		.upper_margin	= 8,
		.lower_margin	= 8,
		.vsync_len	= 2,
		.sync		= FB_SYNC_VERT_HIGH_ACT | FB_SYNC_HOR_HIGH_ACT,
#else
		.name		= "WVGA Panel",
		.xres		= 800,
		.yres		= 480,
		.left_margin	= 220,
		.right_margin	= 110,
		.hsync_len	= 70,
		.upper_margin	= 20,
		.lower_margin	= 5,
		.vsync_len	= 5,
		.sync		= 0,
#endif
	},
};

static const struct sh_mobile_meram_cfg lcd_meram_cfg = {
	.icb[0] = {
		.meram_size     = 0x40,
	},
	.icb[1] = {
		.meram_size     = 0x40,
	},
};

static struct sh_mobile_lcdc_info lcdc_info = {
	.meram_dev = &meram_info,
	.ch[0] = {
		.chan = LCDC_CHAN_MAINLCD,
		.fourcc = V4L2_PIX_FMT_RGB565,
		.lcd_modes = ap4evb_lcdc_modes,
		.num_modes = ARRAY_SIZE(ap4evb_lcdc_modes),
		.meram_cfg = &lcd_meram_cfg,
#ifdef CONFIG_AP4EVB_QHD
		.tx_dev = &mipidsi0_device,
#endif
	}
};

static struct resource lcdc_resources[] = {
	[0] = {
		.name	= "LCDC",
		.start	= 0xfe940000, 
		.end	= 0xfe943fff,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= intcs_evt2irq(0x580),
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device lcdc_device = {
	.name		= "sh_mobile_lcdc_fb",
	.num_resources	= ARRAY_SIZE(lcdc_resources),
	.resource	= lcdc_resources,
	.dev	= {
		.platform_data	= &lcdc_info,
		.coherent_dma_mask = ~0,
	},
};

#define IRQ_FSI		evt2irq(0x1840)
static int __fsi_set_rate(struct clk *clk, long rate, int enable)
{
	int ret = 0;

	if (rate <= 0)
		return ret;

	if (enable) {
		ret = clk_set_rate(clk, rate);
		if (0 == ret)
			ret = clk_enable(clk);
	} else {
		clk_disable(clk);
	}

	return ret;
}

static int __fsi_set_round_rate(struct clk *clk, long rate, int enable)
{
	return __fsi_set_rate(clk, clk_round_rate(clk, rate), enable);
}

static int fsi_ak4642_set_rate(struct device *dev, int rate, int enable)
{
	struct clk *fsia_ick;
	struct clk *fsiack;
	int ret = -EIO;

	fsia_ick = clk_get(dev, "icka");
	if (IS_ERR(fsia_ick))
		return PTR_ERR(fsia_ick);

	fsiack = clk_get_parent(fsia_ick);
	if (!fsiack)
		goto fsia_ick_out;

	ret = __fsi_set_rate(fsiack, rate, enable);
	if (ret < 0)
		goto fsiack_out;

	ret = __fsi_set_round_rate(fsia_ick, rate, enable);
	if ((ret < 0) && enable)
		__fsi_set_round_rate(fsiack, rate, 0); 

fsiack_out:
	clk_put(fsiack);

fsia_ick_out:
	clk_put(fsia_ick);

	return 0;
}

static int fsi_hdmi_set_rate(struct device *dev, int rate, int enable)
{
	struct clk *fsib_clk;
	struct clk *fdiv_clk = &sh7372_fsidivb_clk;
	long fsib_rate = 0;
	long fdiv_rate = 0;
	int ackmd_bpfmd;
	int ret;

	switch (rate) {
	case 44100:
		fsib_rate	= rate * 256;
		ackmd_bpfmd	= SH_FSI_ACKMD_256 | SH_FSI_BPFMD_64;
		break;
	case 48000:
		fsib_rate	= 85428000; 
		fdiv_rate	= rate * 256;
		ackmd_bpfmd	= SH_FSI_ACKMD_256 | SH_FSI_BPFMD_64;
		break;
	default:
		pr_err("unsupported rate in FSI2 port B\n");
		return -EINVAL;
	}

	
	fsib_clk = clk_get(dev, "ickb");
	if (IS_ERR(fsib_clk))
		return -EIO;

	ret = __fsi_set_round_rate(fsib_clk, fsib_rate, enable);
	if (ret < 0)
		goto fsi_set_rate_end;

	
	ret = __fsi_set_round_rate(fdiv_clk, fdiv_rate, enable);
	if (ret < 0) {
		
		if (enable)
			__fsi_set_round_rate(fsib_clk, fsib_rate, 0);
		goto fsi_set_rate_end;
	}

	ret = ackmd_bpfmd;

fsi_set_rate_end:
	clk_put(fsib_clk);
	return ret;
}

static struct sh_fsi_platform_info fsi_info = {
	.port_a = {
		.flags		= SH_FSI_BRS_INV,
		.set_rate	= fsi_ak4642_set_rate,
	},
	.port_b = {
		.flags		= SH_FSI_BRS_INV |
				  SH_FSI_BRM_INV |
				  SH_FSI_LRS_INV |
				  SH_FSI_FMT_SPDIF,
		.set_rate	= fsi_hdmi_set_rate,
	},
};

static struct resource fsi_resources[] = {
	[0] = {
		.name	= "FSI",
		.start	= 0xFE3C0000,
		.end	= 0xFE3C0400 - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start  = IRQ_FSI,
		.flags  = IORESOURCE_IRQ,
	},
};

static struct platform_device fsi_device = {
	.name		= "sh_fsi2",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(fsi_resources),
	.resource	= fsi_resources,
	.dev	= {
		.platform_data	= &fsi_info,
	},
};

static struct fsi_ak4642_info fsi2_ak4643_info = {
	.name		= "AK4643",
	.card		= "FSI2A-AK4643",
	.cpu_dai	= "fsia-dai",
	.codec		= "ak4642-codec.0-0013",
	.platform	= "sh_fsi2",
	.id		= FSI_PORT_A,
};

static struct platform_device fsi_ak4643_device = {
	.name	= "fsi-ak4642-audio",
	.dev	= {
		.platform_data	= &fsi2_ak4643_info,
	},
};

static long ap4evb_clk_optimize(unsigned long target, unsigned long *best_freq,
				unsigned long *parent_freq);

static struct sh_mobile_hdmi_info hdmi_info = {
	.flags = HDMI_SND_SRC_SPDIF,
	.clk_optimize_parent = ap4evb_clk_optimize,
};

static struct resource hdmi_resources[] = {
	[0] = {
		.name	= "HDMI",
		.start	= 0xe6be0000,
		.end	= 0xe6be00ff,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		
		.start	= evt2irq(0x17e0),
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device hdmi_device = {
	.name		= "sh-mobile-hdmi",
	.num_resources	= ARRAY_SIZE(hdmi_resources),
	.resource	= hdmi_resources,
	.id             = -1,
	.dev	= {
		.platform_data	= &hdmi_info,
	},
};

static long ap4evb_clk_optimize(unsigned long target, unsigned long *best_freq,
				unsigned long *parent_freq)
{
	struct clk *hdmi_ick = clk_get(&hdmi_device.dev, "ick");
	long error;

	if (IS_ERR(hdmi_ick)) {
		int ret = PTR_ERR(hdmi_ick);
		pr_err("Cannot get HDMI ICK: %d\n", ret);
		return ret;
	}

	error = clk_round_parent(hdmi_ick, target, best_freq, parent_freq, 1, 64);

	clk_put(hdmi_ick);

	return error;
}

static const struct sh_mobile_meram_cfg hdmi_meram_cfg = {
	.icb[0] = {
		.meram_size     = 0x100,
	},
	.icb[1] = {
		.meram_size     = 0x100,
	},
};

static struct sh_mobile_lcdc_info sh_mobile_lcdc1_info = {
	.clock_source = LCDC_CLK_EXTERNAL,
	.meram_dev = &meram_info,
	.ch[0] = {
		.chan = LCDC_CHAN_MAINLCD,
		.fourcc = V4L2_PIX_FMT_RGB565,
		.interface_type = RGB24,
		.clock_divider = 1,
		.flags = LCDC_FLAGS_DWPOL,
		.meram_cfg = &hdmi_meram_cfg,
		.tx_dev = &hdmi_device,
	}
};

static struct resource lcdc1_resources[] = {
	[0] = {
		.name	= "LCDC1",
		.start	= 0xfe944000,
		.end	= 0xfe947fff,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= intcs_evt2irq(0x1780),
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device lcdc1_device = {
	.name		= "sh_mobile_lcdc_fb",
	.num_resources	= ARRAY_SIZE(lcdc1_resources),
	.resource	= lcdc1_resources,
	.id             = 1,
	.dev	= {
		.platform_data	= &sh_mobile_lcdc1_info,
		.coherent_dma_mask = ~0,
	},
};

static struct platform_device fsi_hdmi_device = {
	.name		= "sh_fsi2_b_hdmi",
};

static struct gpio_led ap4evb_leds[] = {
	{
		.name			= "led4",
		.gpio			= GPIO_PORT185,
		.default_state	= LEDS_GPIO_DEFSTATE_ON,
	},
	{
		.name			= "led2",
		.gpio			= GPIO_PORT186,
		.default_state	= LEDS_GPIO_DEFSTATE_ON,
	},
	{
		.name			= "led3",
		.gpio			= GPIO_PORT187,
		.default_state	= LEDS_GPIO_DEFSTATE_ON,
	},
	{
		.name			= "led1",
		.gpio			= GPIO_PORT188,
		.default_state	= LEDS_GPIO_DEFSTATE_ON,
	}
};

static struct gpio_led_platform_data ap4evb_leds_pdata = {
	.num_leds = ARRAY_SIZE(ap4evb_leds),
	.leds = ap4evb_leds,
};

static struct platform_device leds_device = {
	.name = "leds-gpio",
	.id = 0,
	.dev = {
		.platform_data  = &ap4evb_leds_pdata,
	},
};

static struct i2c_board_info imx074_info = {
	I2C_BOARD_INFO("imx074", 0x1a),
};

static struct soc_camera_link imx074_link = {
	.bus_id		= 0,
	.board_info	= &imx074_info,
	.i2c_adapter_id	= 0,
	.module_name	= "imx074",
};

static struct platform_device ap4evb_camera = {
	.name   = "soc-camera-pdrv",
	.id     = 0,
	.dev    = {
		.platform_data = &imx074_link,
	},
};

static struct sh_csi2_client_config csi2_clients[] = {
	{
		.phy		= SH_CSI2_PHY_MAIN,
		.lanes		= 0,		
		.channel	= 0,
		.pdev		= &ap4evb_camera,
	},
};

static struct sh_csi2_pdata csi2_info = {
	.type		= SH_CSI2C,
	.clients	= csi2_clients,
	.num_clients	= ARRAY_SIZE(csi2_clients),
	.flags		= SH_CSI2_ECC | SH_CSI2_CRC,
};

static struct resource csi2_resources[] = {
	[0] = {
		.name	= "CSI2",
		.start	= 0xffc90000,
		.end	= 0xffc90fff,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= intcs_evt2irq(0x17a0),
		.flags  = IORESOURCE_IRQ,
	},
};

static struct sh_mobile_ceu_companion csi2 = {
	.id		= 0,
	.num_resources	= ARRAY_SIZE(csi2_resources),
	.resource	= csi2_resources,
	.platform_data	= &csi2_info,
};

static struct sh_mobile_ceu_info sh_mobile_ceu_info = {
	.flags = SH_CEU_FLAG_USE_8BIT_BUS,
	.csi2 = &csi2,
};

static struct resource ceu_resources[] = {
	[0] = {
		.name	= "CEU",
		.start	= 0xfe910000,
		.end	= 0xfe91009f,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= intcs_evt2irq(0x880),
		.flags  = IORESOURCE_IRQ,
	},
	[2] = {
		
	},
};

static struct platform_device ceu_device = {
	.name		= "sh_mobile_ceu",
	.id             = 0, 
	.num_resources	= ARRAY_SIZE(ceu_resources),
	.resource	= ceu_resources,
	.dev	= {
		.platform_data		= &sh_mobile_ceu_info,
		.coherent_dma_mask	= 0xffffffff,
	},
};

static struct platform_device *ap4evb_devices[] __initdata = {
	&leds_device,
	&nor_flash_device,
	&smc911x_device,
	&sdhi0_device,
	&sdhi1_device,
	&usb1_host_device,
	&fsi_device,
	&fsi_ak4643_device,
	&fsi_hdmi_device,
	&sh_mmcif_device,
	&hdmi_device,
	&lcdc_device,
	&lcdc1_device,
	&ceu_device,
	&ap4evb_camera,
	&meram_device,
};

static void __init hdmi_init_pm_clock(void)
{
	struct clk *hdmi_ick = clk_get(&hdmi_device.dev, "ick");
	int ret;
	long rate;

	if (IS_ERR(hdmi_ick)) {
		ret = PTR_ERR(hdmi_ick);
		pr_err("Cannot get HDMI ICK: %d\n", ret);
		goto out;
	}

	ret = clk_set_parent(&sh7372_pllc2_clk, &sh7372_dv_clki_div2_clk);
	if (ret < 0) {
		pr_err("Cannot set PLLC2 parent: %d, %d users\n", ret, sh7372_pllc2_clk.usecount);
		goto out;
	}

	pr_debug("PLLC2 initial frequency %lu\n", clk_get_rate(&sh7372_pllc2_clk));

	rate = clk_round_rate(&sh7372_pllc2_clk, 594000000);
	if (rate < 0) {
		pr_err("Cannot get suitable rate: %ld\n", rate);
		ret = rate;
		goto out;
	}

	ret = clk_set_rate(&sh7372_pllc2_clk, rate);
	if (ret < 0) {
		pr_err("Cannot set rate %ld: %d\n", rate, ret);
		goto out;
	}

	pr_debug("PLLC2 set frequency %lu\n", rate);

	ret = clk_set_parent(hdmi_ick, &sh7372_pllc2_clk);
	if (ret < 0)
		pr_err("Cannot set HDMI parent: %d\n", ret);

out:
	if (!IS_ERR(hdmi_ick))
		clk_put(hdmi_ick);
}

static void __init fsi_init_pm_clock(void)
{
	struct clk *fsia_ick;
	int ret;

	fsia_ick = clk_get(&fsi_device.dev, "icka");
	if (IS_ERR(fsia_ick)) {
		ret = PTR_ERR(fsia_ick);
		pr_err("Cannot get FSI ICK: %d\n", ret);
		return;
	}

	ret = clk_set_parent(fsia_ick, &sh7372_fsiack_clk);
	if (ret < 0)
		pr_err("Cannot set FSI-A parent: %d\n", ret);

	clk_put(fsia_ick);
}

static void __init gpio_no_direction(u32 addr)
{
	__raw_writeb(0x00, addr);
}

#ifdef CONFIG_AP4EVB_QHD
# define GPIO_TSC_IRQ	GPIO_FN_IRQ28_123
# define GPIO_TSC_PORT	GPIO_PORT123
#else 
# define GPIO_TSC_IRQ	GPIO_FN_IRQ7_40
# define GPIO_TSC_PORT	GPIO_PORT40
#endif

#define IRQ28	evt2irq(0x3380) 
#define IRQ7	evt2irq(0x02e0) 
static int ts_get_pendown_state(void)
{
	int val;

	gpio_free(GPIO_TSC_IRQ);

	gpio_request(GPIO_TSC_PORT, NULL);

	gpio_direction_input(GPIO_TSC_PORT);

	val = gpio_get_value(GPIO_TSC_PORT);

	gpio_request(GPIO_TSC_IRQ, NULL);

	return !val;
}

static int ts_init(void)
{
	gpio_request(GPIO_TSC_IRQ, NULL);

	return 0;
}

static struct tsc2007_platform_data tsc2007_info = {
	.model			= 2007,
	.x_plate_ohms		= 180,
	.get_pendown_state	= ts_get_pendown_state,
	.init_platform_hw	= ts_init,
};

static struct i2c_board_info tsc_device = {
	I2C_BOARD_INFO("tsc2007", 0x48),
	.type		= "tsc2007",
	.platform_data	= &tsc2007_info,
	
};

static struct i2c_board_info i2c0_devices[] = {
	{
		I2C_BOARD_INFO("ak4643", 0x13),
	},
};

static struct i2c_board_info i2c1_devices[] = {
	{
		I2C_BOARD_INFO("r2025sd", 0x32),
	},
};


#define GPIO_PORT9CR	0xE6051009
#define GPIO_PORT10CR	0xE605100A
#define USCCR1		0xE6058144
static void __init ap4evb_init(void)
{
	u32 srcr4;
	struct clk *clk;

	
	clk_set_rate(&sh7372_dv_clki_clk, 27000000);

	sh7372_pinmux_init();

	
	gpio_request(GPIO_FN_SCIFA0_TXD, NULL);
	gpio_request(GPIO_FN_SCIFA0_RXD, NULL);

	
	gpio_request(GPIO_FN_CS5A,	NULL);
	gpio_request(GPIO_FN_IRQ6_39,	NULL);

	
	gpio_request(GPIO_PORT32, NULL);
	gpio_request(GPIO_PORT33, NULL);
	gpio_request(GPIO_PORT34, NULL);
	gpio_request(GPIO_PORT35, NULL);
	gpio_direction_input(GPIO_PORT32);
	gpio_direction_input(GPIO_PORT33);
	gpio_direction_input(GPIO_PORT34);
	gpio_direction_input(GPIO_PORT35);
	gpio_export(GPIO_PORT32, 0);
	gpio_export(GPIO_PORT33, 0);
	gpio_export(GPIO_PORT34, 0);
	gpio_export(GPIO_PORT35, 0);

	
	gpio_request(GPIO_FN_SDHICD0, NULL);
	gpio_request(GPIO_FN_SDHIWP0, NULL);
	gpio_request(GPIO_FN_SDHICMD0, NULL);
	gpio_request(GPIO_FN_SDHICLK0, NULL);
	gpio_request(GPIO_FN_SDHID0_3, NULL);
	gpio_request(GPIO_FN_SDHID0_2, NULL);
	gpio_request(GPIO_FN_SDHID0_1, NULL);
	gpio_request(GPIO_FN_SDHID0_0, NULL);

	
	gpio_request(GPIO_FN_SDHICMD1, NULL);
	gpio_request(GPIO_FN_SDHICLK1, NULL);
	gpio_request(GPIO_FN_SDHID1_3, NULL);
	gpio_request(GPIO_FN_SDHID1_2, NULL);
	gpio_request(GPIO_FN_SDHID1_1, NULL);
	gpio_request(GPIO_FN_SDHID1_0, NULL);

	
	gpio_request(GPIO_FN_MMCD0_0, NULL);
	gpio_request(GPIO_FN_MMCD0_1, NULL);
	gpio_request(GPIO_FN_MMCD0_2, NULL);
	gpio_request(GPIO_FN_MMCD0_3, NULL);
	gpio_request(GPIO_FN_MMCD0_4, NULL);
	gpio_request(GPIO_FN_MMCD0_5, NULL);
	gpio_request(GPIO_FN_MMCD0_6, NULL);
	gpio_request(GPIO_FN_MMCD0_7, NULL);
	gpio_request(GPIO_FN_MMCCMD0, NULL);
	gpio_request(GPIO_FN_MMCCLK0, NULL);

	
	gpio_request(GPIO_FN_VBUS0_1,    NULL);
	gpio_request(GPIO_FN_IDIN_1_18,  NULL);
	gpio_request(GPIO_FN_PWEN_1_115, NULL);
	gpio_request(GPIO_FN_OVCN_1_114, NULL);
	gpio_request(GPIO_FN_EXTLP_1,    NULL);
	gpio_request(GPIO_FN_OVCN2_1,    NULL);

	
	__raw_writew(0x8a0a, 0xE6058130);	

	
	gpio_request(GPIO_FN_FSIAIBT,	NULL);
	gpio_request(GPIO_FN_FSIAILR,	NULL);
	gpio_request(GPIO_FN_FSIAISLD,	NULL);
	gpio_request(GPIO_FN_FSIAOSLD,	NULL);
	gpio_request(GPIO_PORT161,	NULL);
	gpio_direction_output(GPIO_PORT161, 0); 

	gpio_request(GPIO_PORT9, NULL);
	gpio_request(GPIO_PORT10, NULL);
	gpio_no_direction(GPIO_PORT9CR);  
	gpio_no_direction(GPIO_PORT10CR); 

	
	gpio_request(GPIO_PORT41, NULL);
	gpio_direction_input(GPIO_PORT41);

	
	gpio_request(GPIO_FN_FSIBCK, NULL);
	__raw_writew(__raw_readw(USCCR1) & ~(1 << 6), USCCR1); 

	
	clk = clk_get(NULL, "spu_clk");
	if (!IS_ERR(clk)) {
		clk_set_rate(clk, clk_round_rate(clk, 119600000));
		clk_put(clk);
	}

	intc_set_priority(IRQ_FSI, 3);

	i2c_register_board_info(0, i2c0_devices,
				ARRAY_SIZE(i2c0_devices));

	i2c_register_board_info(1, i2c1_devices,
				ARRAY_SIZE(i2c1_devices));

#ifdef CONFIG_AP4EVB_QHD


	
	gpio_request(GPIO_FN_KEYOUT0, NULL);
	gpio_request(GPIO_FN_KEYOUT1, NULL);
	gpio_request(GPIO_FN_KEYOUT2, NULL);
	gpio_request(GPIO_FN_KEYOUT3, NULL);
	gpio_request(GPIO_FN_KEYOUT4, NULL);
	gpio_request(GPIO_FN_KEYIN0_136, NULL);
	gpio_request(GPIO_FN_KEYIN1_135, NULL);
	gpio_request(GPIO_FN_KEYIN2_134, NULL);
	gpio_request(GPIO_FN_KEYIN3_133, NULL);
	gpio_request(GPIO_FN_KEYIN4,     NULL);

	
	irq_set_irq_type(IRQ28, IRQ_TYPE_LEVEL_LOW);

	tsc_device.irq = IRQ28;
	i2c_register_board_info(1, &tsc_device, 1);

	
	lcdc_info.clock_source			= LCDC_CLK_PERIPHERAL;
	lcdc_info.ch[0].interface_type		= RGB24;
	lcdc_info.ch[0].clock_divider		= 1;
	lcdc_info.ch[0].flags			= LCDC_FLAGS_DWPOL;
	lcdc_info.ch[0].panel_cfg.width		= 44;
	lcdc_info.ch[0].panel_cfg.height	= 79;

	platform_add_devices(qhd_devices, ARRAY_SIZE(qhd_devices));

#else

	gpio_request(GPIO_FN_LCDD17,   NULL);
	gpio_request(GPIO_FN_LCDD16,   NULL);
	gpio_request(GPIO_FN_LCDD15,   NULL);
	gpio_request(GPIO_FN_LCDD14,   NULL);
	gpio_request(GPIO_FN_LCDD13,   NULL);
	gpio_request(GPIO_FN_LCDD12,   NULL);
	gpio_request(GPIO_FN_LCDD11,   NULL);
	gpio_request(GPIO_FN_LCDD10,   NULL);
	gpio_request(GPIO_FN_LCDD9,    NULL);
	gpio_request(GPIO_FN_LCDD8,    NULL);
	gpio_request(GPIO_FN_LCDD7,    NULL);
	gpio_request(GPIO_FN_LCDD6,    NULL);
	gpio_request(GPIO_FN_LCDD5,    NULL);
	gpio_request(GPIO_FN_LCDD4,    NULL);
	gpio_request(GPIO_FN_LCDD3,    NULL);
	gpio_request(GPIO_FN_LCDD2,    NULL);
	gpio_request(GPIO_FN_LCDD1,    NULL);
	gpio_request(GPIO_FN_LCDD0,    NULL);
	gpio_request(GPIO_FN_LCDDISP,  NULL);
	gpio_request(GPIO_FN_LCDDCK,   NULL);

	gpio_request(GPIO_PORT189, NULL); 
	gpio_direction_output(GPIO_PORT189, 1);

	gpio_request(GPIO_PORT151, NULL); 
	gpio_direction_output(GPIO_PORT151, 1);

	lcdc_info.clock_source			= LCDC_CLK_BUS;
	lcdc_info.ch[0].interface_type		= RGB18;
	lcdc_info.ch[0].clock_divider		= 3;
	lcdc_info.ch[0].flags			= 0;
	lcdc_info.ch[0].panel_cfg.width		= 152;
	lcdc_info.ch[0].panel_cfg.height	= 91;

	
	irq_set_irq_type(IRQ7, IRQ_TYPE_LEVEL_LOW);

	tsc_device.irq = IRQ7;
	i2c_register_board_info(0, &tsc_device, 1);
#endif 

	


	
	gpio_request(GPIO_FN_VIO_CKO, NULL);

	clk = clk_get(NULL, "vck1_clk");
	if (!IS_ERR(clk)) {
		clk_set_rate(clk, clk_round_rate(clk, 13000000));
		clk_enable(clk);
		clk_put(clk);
	}

	sh7372_add_standard_devices();

	
	gpio_request(GPIO_FN_HDMI_HPD, NULL);
	gpio_request(GPIO_FN_HDMI_CEC, NULL);

	
#define SRCR4 0xe61580bc
	srcr4 = __raw_readl(SRCR4);
	__raw_writel(srcr4 | (1 << 13), SRCR4);
	udelay(50);
	__raw_writel(srcr4 & ~(1 << 13), SRCR4);

	platform_add_devices(ap4evb_devices, ARRAY_SIZE(ap4evb_devices));

	sh7372_add_device_to_domain(&sh7372_a4lc, &lcdc1_device);
	sh7372_add_device_to_domain(&sh7372_a4lc, &lcdc_device);
	sh7372_add_device_to_domain(&sh7372_a4mp, &fsi_device);

	sh7372_add_device_to_domain(&sh7372_a3sp, &sh_mmcif_device);
	sh7372_add_device_to_domain(&sh7372_a3sp, &sdhi0_device);
	sh7372_add_device_to_domain(&sh7372_a3sp, &sdhi1_device);
	sh7372_add_device_to_domain(&sh7372_a4r, &ceu_device);

	hdmi_init_pm_clock();
	fsi_init_pm_clock();
	sh7372_pm_init();
	pm_clk_add(&fsi_device.dev, "spu2");
	pm_clk_add(&lcdc1_device.dev, "hdmi");
}

MACHINE_START(AP4EVB, "ap4evb")
	.map_io		= sh7372_map_io,
	.init_early	= sh7372_add_early_devices,
	.init_irq	= sh7372_init_irq,
	.handle_irq	= shmobile_handle_irq_intc,
	.init_machine	= ap4evb_init,
	.timer		= &shmobile_timer,
MACHINE_END
