/**
 * @file ov7670.h
 * @author Tiago Madeira
 * @date 2008-03-24
 *
 * @brief Common functions and data for the Omnivision OV7670 sensor.
 *
 * @note Copyright (C) Tiago Madeira
 *
 * @par Licences
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef OV7670_H
#define OV7670_H

#include "microdia.h"

#define OV7670_CTL_GAIN			0x00
#define OV7670_CTL_BLUE			0x01
#define OV7670_CTL_RED			0x02
#define OV7670_CTL_VREF			0x03
#define OV7670_CTL_COM1			0x04
#define OV7670_CTL_BAVE			0x05
#define OV7670_CTL_GbAVE		0x06
#define OV7670_CTL_AECHH		0x07
#define OV7670_CTL_RAVE			0x08
#define OV7670_CTL_COM2			0x09
#define OV7670_CTL_PID			0x0a
#define OV7670_CTL_VER			0x0b
#define OV7670_CTL_COM3			0x0c
#define OV7670_CTL_COM4			0x0d
#define OV7670_CTL_COM5			0x0e
#define OV7670_CTL_COM6			0x0f
#define OV7670_CTL_AECH			0x10
#define OV7670_CTL_CLKRC		0x11
#define OV7670_CTL_COM7			0x12
#define OV7670_CTL_COM8			0x13
#define OV7670_CTL_COM9			0x14
#define OV7670_CTL_COM10		0x15
#define OV7670_CTL_HSTART		0x17
#define OV7670_CTL_HSTOP		0x18
#define OV7670_CTL_VSTRT		0x19
#define OV7670_CTL_VSTOP		0x1a
#define OV7670_CTL_PSHFT		0x1b
#define OV7670_CTL_MIDH			0x1c
#define OV7670_CTL_MIDL			0x1d
#define OV7670_CTL_MVFP			0x1e
#define		OV7670_VFLIP_BIT	0x10
#define OV7670_CTL_LAEC			0x1f
#define OV7670_CTL_ADCCTR0		0x20
#define OV7670_CTL_ADCCTR1		0x21
#define OV7670_CTL_ADCCTR2		0x22
#define OV7670_CTL_ADCCTR3		0x23
#define OV7670_CTL_AEW			0x24
#define OV7670_CTL_AEB			0x25
#define OV7670_CTL_VPT			0x26
#define OV7670_CTL_BBIAS		0x27
#define OV7670_CTL_GbBIAS		0x28
#define OV7670_CTL_EXHCH		0x2a
#define OV7670_CTL_EXHCL		0x2b
#define OV7670_CTL_RBIAS		0x2c
#define OV7670_CTL_ADVFL		0x2d
#define OV7670_CTL_ADVFH		0x2e
#define OV7670_CTL_YAVE			0x2f
#define OV7670_CTL_HSYST		0x30
#define OV7670_CTL_HSYEN		0x31
#define OV7670_CTL_HREF			0x32
#define OV7670_CTL_CHLF			0x33
#define OV7670_CTL_ARBLM		0x34
#define OV7670_CTL_ADC			0x37
#define OV7670_CTL_ACOM			0x38
#define OV7670_CTL_OFON			0x39
#define OV7670_CTL_TSLB			0x3a
#define OV7670_CTL_COM11		0x3b
#define OV7670_CTL_COM12		0x3c
#define OV7670_CTL_COM13		0x3d
#define OV7670_CTL_COM14		0x3e
#define OV7670_CTL_EDGE			0x3f
#define OV7670_CTL_COM15		0x40
#define OV7670_CTL_COM16		0x41
#define OV7670_CTL_COM17		0x42
#define OV7670_CTL_AWBC1		0x43
#define OV7670_CTL_AWBC2		0x44
#define OV7670_CTL_AWBC3		0x45
#define OV7670_CTL_AWBC4		0x46
#define OV7670_CTL_AWBC5		0x47
#define OV7670_CTL_AWBC6		0x48
#define OV7670_CTL_REG4B		0x4b
#define OV7670_CTL_DNSTH		0x4c
#define OV7670_CTL_MTX1			0x4f
#define OV7670_CTL_MTX2			0x50
#define OV7670_CTL_MTX3			0x51
#define OV7670_CTL_MTX4			0x52
#define OV7670_CTL_MTX5			0x53
#define OV7670_CTL_MTX6			0x54
#define OV7670_CTL_BRIGHT		0x55
#define OV7670_CTL_CONTRAS		0x56
#define OV7670_CTL_CONTRAS_CENTER	0x57
#define OV7670_CTL_MTXS			0x58
#define OV7670_CTL_LCC1			0x62
#define OV7670_CTL_LCC2			0x63
#define OV7670_CTL_LCC3			0x64
#define OV7670_CTL_LCC4			0x65
#define OV7670_CTL_LCC5			0x66
#define OV7670_CTL_MANU			0x67
#define OV7670_CTL_MANV			0x68
#define OV7670_CTL_GFIX			0x69
#define OV7670_CTL_GGAIN		0x6a
#define OV7670_CTL_DBLV			0x6b
#define OV7670_CTL_AWBCTR3		0x6c
#define OV7670_CTL_AWBCTR2		0x6d
#define OV7670_CTL_AWBCTR1		0x6e
#define OV7670_CTL_AWBCTR0		0x6f
#define OV7670_CTL_SCALING_XSC		0x70
#define OV7670_CTL_SCALING_YSC		0x71
#define OV7670_CTL_SCALING_DCWCTR	0x72
#define OV7670_CTL_SCALING_PCLK_DIV	0x73
#define OV7670_CTL_REG74		0x74
#define OV7670_CTL_REG75		0x75
#define OV7670_CTL_REG76		0x76
#define OV7670_CTL_REG77		0x77
#define OV7670_CTL_SLOP			0x7a
#define OV7670_CTL_GAM1			0x7b
#define OV7670_CTL_GAM2			0x7c
#define OV7670_CTL_GAM3			0x7d
#define OV7670_CTL_GAM4			0x7e
#define OV7670_CTL_GAM5			0x7f
#define OV7670_CTL_GAM6			0x80
#define OV7670_CTL_GAM7			0x81
#define OV7670_CTL_GAM8			0x82
#define OV7670_CTL_GAM9			0x83
#define OV7670_CTL_GAM10		0x84
#define OV7670_CTL_GAM11		0x85
#define OV7670_CTL_GAM12		0x86
#define OV7670_CTL_GAM13		0x87
#define OV7670_CTL_GAM14		0x88
#define OV7670_CTL_GAM15		0x89
#define OV7670_CTL_RGB444		0x8c
#define OV7670_CTL_DM_LNL		0x92
#define OV7670_CTL_DM_LNH		0x93
#define OV7670_CTL_LCC6			0x94
#define OV7670_CTL_LCC7			0x95
#define OV7670_CTL_BD50ST		0x9d
#define OV7670_CTL_BD60ST		0x9e
#define OV7670_CTL_HAECC1		0x9f
#define OV7670_CTL_HAECC2		0xa0
#define OV7670_CTL_SCALING_PCLK_DELAY	0xa2
#define OV7670_CTL_NT_CTRL		0xa4
#define OV7670_CTL_BD50MAX		0xa5
#define OV7670_CTL_HAECC3		0xa6
#define OV7670_CTL_HAECC4		0xa7
#define OV7670_CTL_HAECC5		0xa8
#define OV7670_CTL_HAECC6		0xa9
#define OV7670_CTL_HAECC7		0xaa
#define OV7670_CTL_BD60MAX		0xab
#define OV7670_CTL_STR_OPT		0xac
#define OV7670_CTL_STR_R		0xad
#define OV7670_CTL_STR_G		0xae
#define OV7670_CTL_STR_B		0xaf
#define OV7670_CTL_ABLC1		0xb1
#define OV7670_CTL_THL_ST		0xb3
#define OV7670_CTL_THL_DLT		0xb5
#define OV7670_CTL_AD_CHB		0xbe
#define OV7670_CTL_AH_CHR		0xbf
#define OV7670_CTL_AD_CHGb		0xc0
#define OV7670_CTL_AD_CHGr		0xc1
#define OV7670_CTL_SATCTR		0xc9

int ov7670_auto_flip(struct usb_microdia *, __u8);
int ov7670_initialise(struct usb_microdia *);
#endif
