/*
 * Copyright (C) 2014-2017 Broadcom
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _USB_BRCM_COMMON_INIT_H
#define _USB_BRCM_COMMON_INIT_H

#include <linux/regmap.h>

#define USB_CTLR_DEVICE_OFF 0
#define USB_CTLR_DEVICE_ON 1
#define USB_CTLR_DEVICE_DUAL 2
#define USB_CTLR_DEVICE_TYPEC_PD 3


enum brcmusb_reg_sel {
	BRCM_REGS_CTRL = 0,
	BRCM_REGS_XHCI_EC,
	BRCM_REGS_XHCI_GBL,
	BRCM_REGS_USB_PHY,
	BRCM_REGS_USB_MDIO,
	BRCM_REGS_MAX
};

#define USB_CTRL_REG(base, reg)	((void *)base + USB_CTRL_##reg)
#define USB_XHCI_EC_REG(base, reg) ((void *)base + USB_XHCI_EC_##reg)
#define USB_CTRL_MASK(reg, field) \
	USB_CTRL_##reg##_##field##_MASK
#define USB_CTRL_SET(base, reg, field)	\
	brcm_usb_ctrl_set(USB_CTRL_REG(base, reg),	\
			  USB_CTRL_##reg##_##field##_MASK)
#define USB_CTRL_UNSET(base, reg, field)	\
	brcm_usb_ctrl_unset(USB_CTRL_REG(base, reg),		\
			    USB_CTRL_##reg##_##field##_MASK)

struct  brcm_usb_init_params;

struct brcm_usb_init_ops {
	void (*init_ipp)(struct brcm_usb_init_params *params);
	void (*init_common)(struct brcm_usb_init_params *params);
	void (*init_eohci)(struct brcm_usb_init_params *params);
	void (*init_xhci)(struct brcm_usb_init_params *params);
	void (*uninit_common)(struct brcm_usb_init_params *params);
	void (*uninit_eohci)(struct brcm_usb_init_params *params);
	void (*uninit_xhci)(struct brcm_usb_init_params *params);
	int  (*get_dual_select)(struct brcm_usb_init_params *params);
	void (*set_dual_select)(struct brcm_usb_init_params *params, int mode);
	void (*wake_enable)(struct brcm_usb_init_params *params,
			    int enable);
};

struct  brcm_usb_init_params {
	void __iomem *regs[BRCM_REGS_MAX];
	int ioc;
	int ipp;
	int device_mode;
	uint32_t family_id;
	uint32_t product_id;
	int selected_family;
	const char *family_name;
	const uint32_t *usb_reg_bits_map;
	const struct brcm_usb_init_ops *ops;
	struct regmap *syscon_piarbctl;
};

void brcm_usb_dvr_init_7445(struct brcm_usb_init_params *params);
void brcm_usb_dvr_init_7216(struct brcm_usb_init_params *params);
void brcm_usb_dvr_init_7211b0(struct brcm_usb_init_params *params);

static inline u32 brcm_usb_readl(void __iomem *addr)
{
	/*
	 * MIPS endianness is configured by boot strap, which also reverses all
	 * bus endianness (i.e., big-endian CPU + big endian bus ==> native
	 * endian I/O).
	 *
	 * Other architectures (e.g., ARM) either do not support big endian, or
	 * else leave I/O in little endian mode.
	 */
	if (IS_ENABLED(CONFIG_MIPS) && IS_ENABLED(__BIG_ENDIAN))
		return __raw_readl(addr);
	else
		return readl_relaxed(addr);
}

static inline void brcm_usb_writel(u32 val, void __iomem *addr)
{
	/* See brcmnand_readl() comments */
	if (IS_ENABLED(CONFIG_MIPS) && IS_ENABLED(__BIG_ENDIAN))
		__raw_writel(val, addr);
	else
		writel_relaxed(val, addr);
}

static inline void brcm_usb_ctrl_unset(void __iomem *reg, u32 mask)
{
	brcm_usb_writel(brcm_usb_readl(reg) & ~(mask), reg);
};

static inline void brcm_usb_ctrl_set(void __iomem *reg, u32 mask)
{
	brcm_usb_writel(brcm_usb_readl(reg) | (mask), reg);
};

static inline void brcm_usb_init_ipp(struct brcm_usb_init_params *ini)
{
	if (ini->ops->init_ipp)
		ini->ops->init_ipp(ini);
}

static inline void brcm_usb_init_common(struct brcm_usb_init_params *ini)
{
	if (ini->ops->init_common)
		ini->ops->init_common(ini);
}

static inline void brcm_usb_init_eohci(struct brcm_usb_init_params *ini)
{
	if (ini->ops->init_eohci)
		ini->ops->init_eohci(ini);
}

static inline void brcm_usb_init_xhci(struct brcm_usb_init_params *ini)
{
	if (ini->ops->init_xhci)
		ini->ops->init_xhci(ini);
}

static inline void brcm_usb_uninit_common(struct brcm_usb_init_params *ini)
{
	if (ini->ops->uninit_common)
		ini->ops->uninit_common(ini);
}

static inline void brcm_usb_uninit_eohci(struct brcm_usb_init_params *ini)
{
	if (ini->ops->uninit_eohci)
		ini->ops->uninit_eohci(ini);
}

static inline void brcm_usb_uninit_xhci(struct brcm_usb_init_params *ini)
{
	if (ini->ops->uninit_xhci)
		ini->ops->uninit_xhci(ini);
}

static inline void brcm_usb_wake_enable(struct brcm_usb_init_params *ini,
	int enable)
{
	if (ini->ops->wake_enable)
		ini->ops->wake_enable(ini, enable);
}

static inline int brcm_usb_get_dual_select(struct brcm_usb_init_params *ini)
{
	if (ini->ops->get_dual_select)
		return ini->ops->get_dual_select(ini);
	return 0;
}

static inline void brcm_usb_set_dual_select(struct brcm_usb_init_params *ini,
	int mode)
{
	if (ini->ops->set_dual_select)
		ini->ops->set_dual_select(ini, mode);
}

#endif /* _USB_BRCM_COMMON_INIT_H */
