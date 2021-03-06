/* Copyright (c) 2012-2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

/****************************** Revision History ******************************************
 *CH# Product		author		Description				Date
 *-----------------------------------------------------------------------------------------
 *01  Atlantic LTE	nc.chaudhary	Intial file creation			27-Jan-2014
 *	3G
 *02  Atlantic-All	nc.chaudhary	Added the gpiomux settings for		30-Jan-2014
 *	variants			UART for LCIA Test failure
 *02  Atlantic-ATT	nc.chaudhary	Added the gpiomux settings for		12-Feb-2014
 *    Kmini-ATT				NC GPIOs as per new schematics
 ******************************************************************************************
 */

#include <linux/init.h>
#include <linux/ioport.h>
#include <mach/board.h>
#include <mach/gpio.h>
#include <mach/gpiomux.h>
#include <mach/socinfo.h>

/*Adding the GPIOMUX settings for suspend states */
static struct gpiomux_setting gpio_suspend_config[] = {
	{	/* IN/ NP/ L */
		.func = GPIOMUX_FUNC_GPIO,
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_NONE,
		.dir = GPIOMUX_IN,
	},
	{	/* OUT/ NP/ L */
		.func = GPIOMUX_FUNC_GPIO,
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_NONE,
		.dir = GPIOMUX_OUT_LOW,
	},
	{	/* IN/ PD/ L */
		.func = GPIOMUX_FUNC_GPIO,
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_DOWN,
		.dir = GPIOMUX_IN,
	},
	{	/* OUT/ PD/ L */
		.func = GPIOMUX_FUNC_GPIO,
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_DOWN,
		.dir = GPIOMUX_OUT_LOW,
	},
};

#ifdef CONFIG_USB_EHCI_MSM_HSIC
static struct gpiomux_setting hsic_act_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_16MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct msm_gpiomux_config msm_hsic_configs[] = {
	{
		.gpio = 115,               /* HSIC_STROBE */
		.settings = {
			[GPIOMUX_ACTIVE] = &hsic_act_cfg,
			[GPIOMUX_SUSPENDED] = &gpio_suspend_config[3],
		},
	},
	{
		.gpio = 116,               /* HSIC_DATA */
		.settings = {
			[GPIOMUX_ACTIVE] = &hsic_act_cfg,
			[GPIOMUX_SUSPENDED] = &gpio_suspend_config[3],
		},
	},
};
#endif

#define KS8851_IRQ_GPIO 115
#define IN_NP_2MA_CFG(gpio_num) { \
	.gpio = gpio_num, \
	.settings = {\
		[GPIOMUX_ACTIVE] = &gpio_suspend_config[0],\
		[GPIOMUX_SUSPENDED] = &gpio_suspend_config[0],\
	}\
}
#define HW_CHK_BIT_CFG(gpio_num) IN_NP_2MA_CFG(gpio_num)
#define OUT_NP_2MA_CFG(gpio_num) { \
	.gpio = gpio_num, \
	.settings = {\
		[GPIOMUX_ACTIVE] = &gpio_suspend_config[1],\
		[GPIOMUX_SUSPENDED] = &gpio_suspend_config[1],\
	}\
}
#define IN_PD_2MA_CFG(gpio_num) { \
	.gpio = gpio_num, \
	.settings ={\
		[GPIOMUX_ACTIVE] = &gpio_suspend_config[2],\
		[GPIOMUX_SUSPENDED] = &gpio_suspend_config[2],\
	}\
}
#define NC_GPIO_CONFIG(gpio_num) IN_PD_2MA_CFG(gpio_num)
#define OUT_PD_2MA_CFG(gpio_num) { \
	.gpio = gpio_num, \
	.settings = {\
		[GPIOMUX_ACTIVE] = &gpio_suspend_config[3],\
		[GPIOMUX_SUSPENDED] = &gpio_suspend_config[3],\
	}\
}

#if defined(CONFIG_KS8851) || defined(CONFIG_KS8851_MODULE)
static struct gpiomux_setting gpio_eth_config = {
	.pull = GPIOMUX_PULL_UP,
	.drv = GPIOMUX_DRV_2MA,
	.func = GPIOMUX_FUNC_GPIO,
};

static struct msm_gpiomux_config msm_eth_configs[] = {
	{
		.gpio = KS8851_IRQ_GPIO,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_eth_config,
		}
	},
};
#endif

static struct gpiomux_setting gpio_spi_stm_config = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gpio_spi_stm_sus_config[] = {
	{
		.func = GPIOMUX_FUNC_1,
		.drv = GPIOMUX_DRV_8MA,
		.pull = GPIOMUX_PULL_DOWN,
	},
	{
		.func = GPIOMUX_FUNC_1,
		.drv = GPIOMUX_DRV_8MA,
		.pull = GPIOMUX_PULL_UP,
	},
	{
		.func = GPIOMUX_FUNC_2,
		.drv = GPIOMUX_DRV_8MA,
		.pull = GPIOMUX_PULL_DOWN,
	},
};

static struct gpiomux_setting synaptics_int_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting synaptics_int_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting gpio_i2c_tkey_active_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting gpio_i2c_tkey_suspend_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting gpio_keys_active = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting gpio_keys_suspend = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gpio_spi_act_config = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gpio_spi_cs_act_config = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting gpio_spi_susp_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting wcnss_5wire_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv  = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting wcnss_5wire_active_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv  = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_DOWN,
};

#define WCNSS_5WIRE_CFG(gpio_num) { \
	.gpio = gpio_num, \
	.settings = {\
		[GPIOMUX_ACTIVE] = &wcnss_5wire_active_cfg,\
		[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,\
	}\
}

static struct gpiomux_setting gpio_i2c_config = {
	.func = GPIOMUX_FUNC_3,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_IN,
};

#define I2C_GPIO_CONFIG(gpio_num) { \
	.gpio = gpio_num, \
	.settings = {\
		[GPIOMUX_ACTIVE] = &gpio_i2c_config,\
		[GPIOMUX_SUSPENDED] = &gpio_i2c_config,\
	}\
}

static struct msm_gpiomux_config msm_keypad_configs[] __initdata = {
	{
		.gpio = 106,
		.settings = {
			[GPIOMUX_ACTIVE]    = &gpio_keys_active,
			[GPIOMUX_SUSPENDED] = &gpio_keys_suspend,
		},
	},
	{
		.gpio = 107,
		.settings = {
			[GPIOMUX_ACTIVE]    = &gpio_keys_active,
			[GPIOMUX_SUSPENDED] = &gpio_keys_suspend,
		},
	},
	{
		.gpio = 108,
		.settings = {
			[GPIOMUX_ACTIVE]    = &gpio_keys_active,
			[GPIOMUX_SUSPENDED] = &gpio_keys_suspend,
		},
	},
};

static struct gpiomux_setting lcd_rst_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting lcd_rst_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir = GPIOMUX_OUT_LOW,
};

static struct gpiomux_setting lcd_pwr_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting lcd_pwr_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir = GPIOMUX_OUT_LOW,
};

static struct msm_gpiomux_config msm_lcd_configs[] __initdata = {
	{
		.gpio = 25,		/* LCD Reset */
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcd_rst_act_cfg,
			[GPIOMUX_SUSPENDED] = &lcd_rst_sus_cfg,
		},
	},
	{
		.gpio = 60,		/* LCD ON*/
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcd_pwr_act_cfg,
			[GPIOMUX_SUSPENDED] = &lcd_pwr_sus_cfg,
		},
	},
};

static struct gpiomux_setting gpio_uart_config = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE, /*should be pulled None for UART */
	.dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting rx_gpio_uart_config = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_DOWN, /*LCIA Test failure*/
	.dir = GPIOMUX_OUT_HIGH,
};

static struct msm_gpiomux_config msm_blsp_configs[] __initdata = {
	{
		.gpio      = 8,		/* UART TX */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_uart_config,
		},
	},
	{
		.gpio      = 9,		/* UART RX */
		.settings = {
			[GPIOMUX_SUSPENDED] = &rx_gpio_uart_config,
		},
	},
	{
		.gpio      = 12,		/* UART TX */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_uart_config,
		},
	},
	{
		.gpio      = 13,		/* UART RX */
		.settings = {
			[GPIOMUX_SUSPENDED] = &rx_gpio_uart_config,
		},
	},
	{
		.gpio      = 16,		/* UART TX */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_uart_config,
		},
	},
	{
		.gpio      = 17,		/* UART RX */
		.settings = {
			[GPIOMUX_SUSPENDED] = &rx_gpio_uart_config,
		},
	},
	{
		.gpio      = 22,		/* BLSP1 QUP1 SPI_DATA_MOSI */
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_i2c_tkey_active_config,
			[GPIOMUX_SUSPENDED] = &gpio_i2c_tkey_suspend_config,
		},
	},
	{
		.gpio      = 23,		/* BLSP1 QUP1 SPI_DATA_MISO */
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_i2c_tkey_active_config,
			[GPIOMUX_SUSPENDED] = &gpio_i2c_tkey_suspend_config,
		},
	},
	{
		.gpio      = 2,		/* BLSP1 QUP1 SPI_CS1 */
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_spi_cs_act_config,
			[GPIOMUX_SUSPENDED] = &gpio_spi_susp_config,
		},
	},
	{
		.gpio      = 3,		/* BLSP1 QUP1 SPI_CLK */
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_spi_act_config,
			[GPIOMUX_SUSPENDED] = &gpio_spi_susp_config,
		},
	},
	{
		.gpio      = 4,	/* BLSP1 QUP1 SPI_DATA_MOSI */
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_spi_stm_config,
			[GPIOMUX_SUSPENDED] = &gpio_spi_stm_sus_config[0],
		},
	},
	{
		.gpio      = 5,		/* BLSP1 QUP1 SPI_DATA_MISO */
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_spi_stm_config,
			[GPIOMUX_SUSPENDED] = &gpio_spi_stm_sus_config[0],
		},
	},
	{
		.gpio      = 6,		/* BLSP1 QUP1 SPI_CS1 */
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_spi_stm_config,
			[GPIOMUX_SUSPENDED] = &gpio_spi_stm_sus_config[1],
		},
	},
	{
		.gpio      = 7,		/* BLSP1 QUP1 SPI_CLK */
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_spi_stm_config,
			[GPIOMUX_SUSPENDED] = &gpio_spi_stm_sus_config[1],
		},
	},
	I2C_GPIO_CONFIG(18),		/* BLSP1 QUP5 I2C_SDA */
	I2C_GPIO_CONFIG(19),		/* BLSP1 QUP5 I2C_SCL */
	I2C_GPIO_CONFIG(10),		/* BLSP3 QUP5 I2C_SDA */
	I2C_GPIO_CONFIG(11),		/* BLSP3 QUP5 I2C_SCL */
};
#define HRM_IRQ_CFG(gpio_num) IN_NP_2MA_CFG(gpio_num)
static struct msm_gpiomux_config msm_hrm_configs[] __initdata = {
	HRM_IRQ_CFG(63),		 /* HRM IRQ */
};

static struct gpiomux_setting ssp_setting[] = {
	{
		.func = GPIOMUX_FUNC_GPIO,
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_NONE,
	},
	{
		.func = GPIOMUX_FUNC_GPIO,
		.drv = GPIOMUX_DRV_8MA,
		.pull = GPIOMUX_PULL_NONE,
		.dir = GPIOMUX_OUT_HIGH,
	},
};

#define SSPN_CONFIG(gpio_num, n) { \
	.gpio = gpio_num, \
	.settings = {\
		[GPIOMUX_ACTIVE] = &ssp_setting[n],\
		[GPIOMUX_SUSPENDED] = &ssp_setting[n],\
	}\
}
static struct msm_gpiomux_config msm_sensorhub_configs[] __initdata = {
	SSPN_CONFIG(64, 0),			/* MCU INT 1 IRQ */
	SSPN_CONFIG(65, 0),			/* MCU INT 2 IRQ */
	SSPN_CONFIG(118, 1),			/* AP INT IRQ */
	SSPN_CONFIG(104, 0),			/* RESET */
};

#define NFC_IRQ_CFG(gpio_num) IN_NP_2MA_CFG(gpio_num)
#define NFC_FW_CFG(gpio_num) OUT_NP_2MA_CFG(gpio_num)
#define NFC_EN_CFG(gpio_num) OUT_PD_2MA_CFG(gpio_num)
static struct msm_gpiomux_config msm_nfc_configs[] __initdata = {
	NFC_IRQ_CFG(21),	/* NFC IRQ */
	NFC_FW_CFG(62),		/* NFC FIRMWARE */
	NFC_EN_CFG(20),		/* NFC VEN */
};
static struct msm_gpiomux_config msm_synaptics_configs[] __initdata = {
	{
		.gpio = 55,
		.settings = {
			[GPIOMUX_ACTIVE] = &synaptics_int_act_cfg,
			[GPIOMUX_SUSPENDED] = &synaptics_int_sus_cfg,
		},
	},
};

static struct gpiomux_setting goodix_ldo_en_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting goodix_int_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct msm_gpiomux_config msm_skuf_blsp_configs[] __initdata = {
	NC_GPIO_CONFIG(2),
	NC_GPIO_CONFIG(3),
	NC_GPIO_CONFIG(4),
};

static struct msm_gpiomux_config msm_skuf_goodix_configs[] __initdata = {
	{
		.gpio = 15,		/* LDO EN */
		.settings = {
			[GPIOMUX_ACTIVE] = &goodix_ldo_en_act_cfg,
			[GPIOMUX_SUSPENDED] = &gpio_suspend_config[2],
		},
	},
	{
		.gpio = 17,		/* INT */
		.settings = {
			[GPIOMUX_ACTIVE] = &goodix_int_act_cfg,
			[GPIOMUX_SUSPENDED] = &gpio_suspend_config[2],
		},
	},
	I2C_GPIO_CONFIG(18),		/* BLSP1 QUP5 I2C_SDA */
	I2C_GPIO_CONFIG(19),		/* BLSP1 QUP5 I2C_SCL */
};

static struct gpiomux_setting nfc_wake_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct msm_gpiomux_config msm_skuf_nfc_configs[] __initdata = {
	{					/*  NFC   WAKE */
		.gpio      = 5,
		.settings = {
			[GPIOMUX_ACTIVE] = &nfc_wake_act_cfg,
			[GPIOMUX_SUSPENDED] = &gpio_suspend_config[1],
		},
	},
};

static struct msm_gpiomux_config wcnss_5wire_interface[] = {
	WCNSS_5WIRE_CFG(40),
	WCNSS_5WIRE_CFG(41),
	WCNSS_5WIRE_CFG(42),
	WCNSS_5WIRE_CFG(43),
	WCNSS_5WIRE_CFG(44),
};

static struct gpiomux_setting cam_settings[] = {
	{
		.func = GPIOMUX_FUNC_1, /*active 1*/ /* 0 */
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_NONE,
	},

	{
		.func = GPIOMUX_FUNC_1, /*suspend*/ /* 1 */
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_DOWN,
	},

	{
		.func = GPIOMUX_FUNC_1, /*i2c suspend*/ /* 2 */
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_KEEPER,
	},

	{
		.func = GPIOMUX_FUNC_GPIO, /*active 0*/ /* 3 */
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_NONE,
	},

	{
		.func = GPIOMUX_FUNC_GPIO, /*suspend 0*/ /* 4 */
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_DOWN,
	},
	{
		.func = GPIOMUX_FUNC_GPIO, /*active 1*/  /* 5 */
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_NONE,
		.dir = GPIOMUX_OUT_LOW,
	},
};

#define CAM_COMMON_CONFIG(gpio_num) { \
	.gpio = gpio_num, \
	.settings = {\
		[GPIOMUX_ACTIVE] = &cam_settings[3],\
		[GPIOMUX_SUSPENDED] = &cam_settings[4],\
	}\
}
static struct msm_gpiomux_config msm_sensor_configs[] __initdata = {
	{
		.gpio = 26, /* CAM_MCLK0 */
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[0],
			[GPIOMUX_SUSPENDED] = &cam_settings[1],
		},
	},
	{
		.gpio = 27, /* CAM_MCLK1 */
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[0],
			[GPIOMUX_SUSPENDED] = &cam_settings[1],
		},

	},
	{
		.gpio = 29, /* CCI_I2C_SDA0 */
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[0],
			[GPIOMUX_SUSPENDED] = &gpio_suspend_config[0],
		},
	},
	{
		.gpio = 30, /* CCI_I2C_SCL0 */
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[0],
			[GPIOMUX_SUSPENDED] = &gpio_suspend_config[0],
		},
	},
	CAM_COMMON_CONFIG(2),	/* CCI_I2C_SDA_EEPROM */
	CAM_COMMON_CONFIG(3),	/* CCI_I2C_SCL_EEPROM */
	CAM_COMMON_CONFIG(33),	/* CCI_I2C_SDA_AF */
	CAM_COMMON_CONFIG(34),	/* CCI_I2C_SCL_AF */
	CAM_COMMON_CONFIG(35),  /* CAM2_STANDBY_N */
	CAM_COMMON_CONFIG(36),	/* CAM1_STANDBY_N */
	CAM_COMMON_CONFIG(37),	/* CAM1_RST_N */
	CAM_COMMON_CONFIG(28),	/* CAM2_RST_N */
	{
		.gpio = 113, /* CAM_ANALOG_EN */
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[3],
			[GPIOMUX_SUSPENDED] = &cam_settings[6],
		},
	},
};

static struct msm_gpiomux_config msm_sensor_configs_skuf_plus[] __initdata = {
	CAM_COMMON_CONFIG(34),	/* CAM1 VCM_PWDN */
};


static struct gpiomux_setting auxpcm_act_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting auxpcm_sus_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct msm_gpiomux_config msm_auxpcm_configs[] __initdata = {
	{
		.gpio = 64,
		.settings = {
			[GPIOMUX_SUSPENDED] = &auxpcm_sus_cfg,
			[GPIOMUX_ACTIVE] = &auxpcm_act_cfg,
		},
	},
};

#if defined (CONFIG_SAMSUNG_JACK)
#define EAR_SWITCH_CFG(gpio_num) IN_PD_2MA_CFG(gpio_num)
static struct msm_gpiomux_config msm_earjack_gpio_configs[] __initdata = {
	EAR_SWITCH_CFG(35),	/* EAR_SWITCH */
};
#endif

#ifdef CONFIG_SND_SOC_MAX98504
static struct gpiomux_setting  pri_mi2s_act_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting  pri_mi2s_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
#define TERTIARY_MI2S_CONFIG(gpio_num) { \
	.gpio = gpio_num, \
	.settings = {\
		[GPIOMUX_ACTIVE] = &pri_mi2s_act_cfg,\
		[GPIOMUX_SUSPENDED] = &pri_mi2s_sus_cfg,\
	}\
}
static struct msm_gpiomux_config msm8226_tertiary_mi2s_configs[] __initdata = {
	TERTIARY_MI2S_CONFIG(49),	/* qua mi2s sck */
	TERTIARY_MI2S_CONFIG(50),
	TERTIARY_MI2S_CONFIG(51),
	TERTIARY_MI2S_CONFIG(52),
};
#endif /* CONFIG_SND_SOC_MAX98504 */

static struct gpiomux_setting usb_otg_sw_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.dir = GPIOMUX_OUT_LOW,
};

static struct msm_gpiomux_config usb_otg_sw_configs[] __initdata = {
	{
		.gpio = 67,
		.settings = {
			[GPIOMUX_SUSPENDED] = &usb_otg_sw_cfg,
		},
	},
};

#ifdef CONFIG_MMC_MSM_SDC3_SUPPORT
static struct gpiomux_setting sdc3_clk_actv_cfg = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting sdc3_cmd_data_0_3_actv_cfg = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting sdc3_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting sdc3_data_1_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

#define SDC3_COMMON_CONFIG(gpio_num) { \
	.gpio = gpio_num, \
	.settings = {\
		[GPIOMUX_ACTIVE] = &sdc3_cmd_data_0_3_actv_cfg,\
		[GPIOMUX_SUSPENDED] = &sdc3_suspend_cfg,\
	}\
}
static struct msm_gpiomux_config msm8226_sdc3_configs[] __initdata = {
	SDC3_COMMON_CONFIG(39),		/* DAT3 */
	SDC3_COMMON_CONFIG(40),		/* DAT2 */
	{
		/* DAT1 */
		.gpio      = 41,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdc3_cmd_data_0_3_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdc3_data_1_suspend_cfg,
		},
	},
	SDC3_COMMON_CONFIG(42),		/* DAT0 */
	SDC3_COMMON_CONFIG(43),		/* cmd */
	{
		/* CLK */
		.gpio      = 44,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdc3_clk_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdc3_suspend_cfg,
		},
	},
};

static void msm_gpiomux_sdc3_install(void)
{
	msm_gpiomux_install(msm8226_sdc3_configs,
			    ARRAY_SIZE(msm8226_sdc3_configs));
}
#else
static void msm_gpiomux_sdc3_install(void) {}
#endif /* CONFIG_MMC_MSM_SDC3_SUPPORT */

/* Battery charging and BMS GPIO configuration */
#define TA_CHG_CFG(gpio_num) IN_NP_2MA_CFG(gpio_num)
static struct msm_gpiomux_config msm_ta_nchg_configs[] = {
	TA_CHG_CFG(54),		/* TA_NCHG */
};

extern int system_rev;

/*NC GPIOs configuration*/
static struct msm_gpiomux_config nc_gpio_cfgs[] __initdata = {
	NC_GPIO_CONFIG(75),
	NC_GPIO_CONFIG(76),
	NC_GPIO_CONFIG(77),
	NC_GPIO_CONFIG(78),
	NC_GPIO_CONFIG(79),
	NC_GPIO_CONFIG(80),
	NC_GPIO_CONFIG(81),
	NC_GPIO_CONFIG(82),
	NC_GPIO_CONFIG(83),
	NC_GPIO_CONFIG(84),
	NC_GPIO_CONFIG(85),
	NC_GPIO_CONFIG(86),
	NC_GPIO_CONFIG(88),
	NC_GPIO_CONFIG(90),
	NC_GPIO_CONFIG(91),
	NC_GPIO_CONFIG(92),
	NC_GPIO_CONFIG(93),
	NC_GPIO_CONFIG(94),
	NC_GPIO_CONFIG(97),
	NC_GPIO_CONFIG(98),
	NC_GPIO_CONFIG(110),
	NC_GPIO_CONFIG(111),
	NC_GPIO_CONFIG(113),
};

static struct msm_gpiomux_config hw_chkbits_cfg[] __initdata = {
	HW_CHK_BIT_CFG(24),
	HW_CHK_BIT_CFG(67),
	HW_CHK_BIT_CFG(116),
	HW_CHK_BIT_CFG(117),
};
void __init msm8226_init_gpiomux(void)
{
	int rc;

	rc = msm_gpiomux_init_dt();
	if (rc) {
		pr_err("%s failed %d\n", __func__, rc);
		return;
	}

/* Battery charging and BMS GPIO */
	msm_gpiomux_install(msm_ta_nchg_configs, ARRAY_SIZE(msm_ta_nchg_configs));

	msm_gpiomux_install(msm_hrm_configs,
		ARRAY_SIZE(msm_hrm_configs));

	msm_gpiomux_install(msm_sensorhub_configs,
		ARRAY_SIZE(msm_sensorhub_configs));

	msm_gpiomux_install(msm_nfc_configs,
		ARRAY_SIZE(msm_nfc_configs));

#if defined(CONFIG_KS8851) || defined(CONFIG_KS8851_MODULE)
	msm_gpiomux_install(msm_eth_configs, ARRAY_SIZE(msm_eth_configs));
#endif
	msm_gpiomux_install(msm_keypad_configs,
			ARRAY_SIZE(msm_keypad_configs));

	if (of_board_is_skuf())
		msm_gpiomux_install(msm_skuf_blsp_configs,
			ARRAY_SIZE(msm_skuf_blsp_configs));
	else
		msm_gpiomux_install(msm_blsp_configs,
			ARRAY_SIZE(msm_blsp_configs));

	msm_gpiomux_install(wcnss_5wire_interface,
				ARRAY_SIZE(wcnss_5wire_interface));

	if (of_board_is_skuf())
		msm_gpiomux_install(msm_skuf_goodix_configs,
				ARRAY_SIZE(msm_skuf_goodix_configs));

		msm_gpiomux_install(msm_synaptics_configs,
				ARRAY_SIZE(msm_synaptics_configs));

	if (of_board_is_skuf())
		msm_gpiomux_install(msm_skuf_nfc_configs,
				ARRAY_SIZE(msm_skuf_nfc_configs));

	msm_gpiomux_install_nowrite(msm_lcd_configs,
			ARRAY_SIZE(msm_lcd_configs));

	msm_gpiomux_install(msm_sensor_configs, ARRAY_SIZE(msm_sensor_configs));

	if (of_board_is_skuf())
		msm_gpiomux_install(msm_sensor_configs_skuf_plus,
			ARRAY_SIZE(msm_sensor_configs_skuf_plus));

	msm_gpiomux_install(msm_auxpcm_configs,
			ARRAY_SIZE(msm_auxpcm_configs));

	if (of_board_is_cdp() || of_board_is_mtp() || of_board_is_xpm())
		msm_gpiomux_install(usb_otg_sw_configs,
					ARRAY_SIZE(usb_otg_sw_configs));

	/*
	 * gpio mux settings for the NC GPIOs
	 */
	msm_gpiomux_install(nc_gpio_cfgs,
			ARRAY_SIZE(nc_gpio_cfgs));

	msm_gpiomux_install(hw_chkbits_cfg, ARRAY_SIZE(hw_chkbits_cfg));
	msm_gpiomux_sdc3_install();

	/*
	 * HSIC STROBE gpio is also used by the ethernet. Install HSIC
	 * gpio mux config only when HSIC is enabled. HSIC config will
	 * be disabled when ethernet config is enabled.
	 */
#ifdef CONFIG_USB_EHCI_MSM_HSIC
	if (machine_is_msm8926()) {
		msm_hsic_configs[0].gpio = 119; /* STROBE */
		msm_hsic_configs[1].gpio = 120; /* DATA */
	}
	msm_gpiomux_install(msm_hsic_configs, ARRAY_SIZE(msm_hsic_configs));
#endif
#if defined(CONFIG_SAMSUNG_JACK)
	msm_gpiomux_install(msm_earjack_gpio_configs, ARRAY_SIZE(msm_earjack_gpio_configs));
#endif
#ifdef CONFIG_SND_SOC_MAX98504
		msm_gpiomux_install(msm8226_tertiary_mi2s_configs,ARRAY_SIZE(msm8226_tertiary_mi2s_configs));
#endif
}
