/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2008 Maxime Bizon <mbizon@freebox.fr>
 */

#include <linux/types.h>
#include <linux/pci.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <asm/bootinfo.h>

#include "pci-bcm63xx.h"

int bcm63xx_pci_enabled;

static struct resource bcm_pci_mem_resource = {
	.name   = "bcm63xx PCI memory space",
	.start  = BCM_PCI_MEM_BASE_PA,
	.end    = BCM_PCI_MEM_END_PA,
	.flags  = IORESOURCE_MEM
};

static struct resource bcm_pci_io_resource = {
	.name   = "bcm63xx PCI IO space",
	.start  = BCM_PCI_IO_BASE_PA,
#ifdef CONFIG_CARDBUS
	.end    = BCM_PCI_IO_HALF_PA,
#else
	.end    = BCM_PCI_IO_END_PA,
#endif
	.flags  = IORESOURCE_IO
};

struct pci_controller bcm63xx_controller = {
	.pci_ops	= &bcm63xx_pci_ops,
	.io_resource	= &bcm_pci_io_resource,
	.mem_resource	= &bcm_pci_mem_resource,
};

#ifdef CONFIG_CARDBUS
static struct resource bcm_cb_mem_resource = {
	.name   = "bcm63xx Cardbus memory space",
	.start  = BCM_CB_MEM_BASE_PA,
	.end    = BCM_CB_MEM_END_PA,
	.flags  = IORESOURCE_MEM
};

static struct resource bcm_cb_io_resource = {
	.name   = "bcm63xx Cardbus IO space",
	.start  = BCM_PCI_IO_HALF_PA + 1,
	.end    = BCM_PCI_IO_END_PA,
	.flags  = IORESOURCE_IO
};

struct pci_controller bcm63xx_cb_controller = {
	.pci_ops	= &bcm63xx_cb_ops,
	.io_resource	= &bcm_cb_io_resource,
	.mem_resource	= &bcm_cb_mem_resource,
};
#endif

static u32 bcm63xx_int_cfg_readl(u32 reg)
{
	u32 tmp;

	tmp = reg & MPI_PCICFGCTL_CFGADDR_MASK;
	tmp |= MPI_PCICFGCTL_WRITEEN_MASK;
	bcm_mpi_writel(tmp, MPI_PCICFGCTL_REG);
	iob();
	return bcm_mpi_readl(MPI_PCICFGDATA_REG);
}

static void bcm63xx_int_cfg_writel(u32 val, u32 reg)
{
	u32 tmp;

	tmp = reg & MPI_PCICFGCTL_CFGADDR_MASK;
	tmp |=  MPI_PCICFGCTL_WRITEEN_MASK;
	bcm_mpi_writel(tmp, MPI_PCICFGCTL_REG);
	bcm_mpi_writel(val, MPI_PCICFGDATA_REG);
}

void __iomem *pci_iospace_start;

static int __init bcm63xx_pci_init(void)
{
	unsigned int mem_size;
	u32 val;

	if (!BCMCPU_IS_6348() && !BCMCPU_IS_6358() && !BCMCPU_IS_6368())
		return -ENODEV;

	if (!bcm63xx_pci_enabled)
		return -ENODEV;

	pci_iospace_start = ioremap_nocache(BCM_PCI_IO_BASE_PA, 4);
	if (!pci_iospace_start)
		return -ENOMEM;

	
	val = BCM_PCI_MEM_BASE_PA & MPI_L2P_BASE_MASK;
	bcm_mpi_writel(val, MPI_L2PMEMBASE1_REG);
	bcm_mpi_writel(~(BCM_PCI_MEM_SIZE - 1), MPI_L2PMEMRANGE1_REG);
	bcm_mpi_writel(val | MPI_L2PREMAP_ENABLED_MASK, MPI_L2PMEMREMAP1_REG);

	val = bcm_pcmcia_readl(PCMCIA_C1_REG);
	val &= ~PCMCIA_C1_CBIDSEL_MASK;
	val |= (CARDBUS_PCI_IDSEL << PCMCIA_C1_CBIDSEL_SHIFT);
	bcm_pcmcia_writel(val, PCMCIA_C1_REG);

#ifdef CONFIG_CARDBUS
	
	val = BCM_CB_MEM_BASE_PA & MPI_L2P_BASE_MASK;
	bcm_mpi_writel(val, MPI_L2PMEMBASE2_REG);
	bcm_mpi_writel(~(BCM_CB_MEM_SIZE - 1), MPI_L2PMEMRANGE2_REG);
	val |= MPI_L2PREMAP_ENABLED_MASK | MPI_L2PREMAP_IS_CARDBUS_MASK;
	bcm_mpi_writel(val, MPI_L2PMEMREMAP2_REG);
#else
	
	bcm_mpi_writel(0, MPI_L2PMEMREMAP2_REG);
#endif

	val = BCM_PCI_IO_BASE_PA & MPI_L2P_BASE_MASK;
	bcm_mpi_writel(val, MPI_L2PIOBASE_REG);
	bcm_mpi_writel(~(BCM_PCI_IO_SIZE - 1), MPI_L2PIORANGE_REG);
	bcm_mpi_writel(val | MPI_L2PREMAP_ENABLED_MASK, MPI_L2PIOREMAP_REG);

	
	bcm_mpi_writel(MPI_LOCBUSCTL_EN_PCI_GPIO_MASK, MPI_LOCBUSCTL_REG);

	bcm63xx_int_cfg_writel(0, PCI_BASE_ADDRESS_3);
	if (BCMCPU_IS_6358() || BCMCPU_IS_6368())
		val = MPI_SP0_REMAP_ENABLE_MASK;
	else
		val = 0;
	bcm_mpi_writel(val, MPI_SP0_REMAP_REG);

	bcm63xx_int_cfg_writel(0x0, PCI_BASE_ADDRESS_4);
	bcm_mpi_writel(0, MPI_SP1_REMAP_REG);

	mem_size = bcm63xx_get_memory_size();

	if (BCMCPU_IS_6348() && (bcm63xx_get_cpu_rev() & 0xf0) == 0xa0) {
		if (mem_size > (16 * 1024 * 1024))
			printk(KERN_WARNING "bcm63xx: this CPU "
			       "revision cannot handle more than 16MB "
			       "of RAM for PCI bus mastering\n");
	} else {
		
		bcm_mpi_writel(~(mem_size - 1), MPI_SP0_RANGE_REG);
		bcm_mpi_writel(0, MPI_SP1_RANGE_REG);
	}

	val = bcm63xx_int_cfg_readl(BCMPCI_REG_TIMERS);
	val &= ~REG_TIMER_RETRY_MASK;
	bcm63xx_int_cfg_writel(val, BCMPCI_REG_TIMERS);

	
	val = bcm63xx_int_cfg_readl(PCI_COMMAND);
	val |= (PCI_COMMAND_MEMORY | PCI_COMMAND_MASTER);
	bcm63xx_int_cfg_writel(val, PCI_COMMAND);

	val = bcm_mpi_readl(MPI_PCIMODESEL_REG);
	val &= ~MPI_PCIMODESEL_BAR1_NOSWAP_MASK;
	val &= ~MPI_PCIMODESEL_BAR2_NOSWAP_MASK;
	val &= ~MPI_PCIMODESEL_PREFETCH_MASK;
	val |= (8 << MPI_PCIMODESEL_PREFETCH_SHIFT);
	bcm_mpi_writel(val, MPI_PCIMODESEL_REG);

	
	val = bcm_mpi_readl(MPI_LOCINT_REG);
	val |= MPI_LOCINT_MASK(MPI_LOCINT_EXT_PCI_INT);
	bcm_mpi_writel(val, MPI_LOCINT_REG);

	register_pci_controller(&bcm63xx_controller);

#ifdef CONFIG_CARDBUS
	register_pci_controller(&bcm63xx_cb_controller);
#endif

	
	request_mem_region(BCM_PCI_IO_BASE_PA, BCM_PCI_IO_SIZE,
			   "bcm63xx PCI IO space");
	return 0;
}

arch_initcall(bcm63xx_pci_init);
