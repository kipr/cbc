/**
 * @file ov7660.h
 * @author Neekhil
 * @date 2008-03-25
 *
 * @brief Register names & Hex Addresses for the Omnivision OV7660 Image Sensor chips.
 *
 * @note Copyright (C) Neekhil
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

#ifndef OV7660_H
#define OV7660_H

#define OV7660_CTL_GAIN			0x00
#define OV7660_CTL_BLUE			0x01
#define OV7660_CTL_RED			0x02
#define OV7660_CTL_VREF			0x03
#define OV7660_CTL_COM1			0x04
#define OV7660_CTL_BAVE			0x05
#define OV7660_CTL_GEAVE			0x06
#define OV7660_CTL_AECHH			0x07
#define OV7660_CTL_RAVE			0x08
#define OV7660_CTL_COM2			0x09
#define OV7660_CTL_PID				0x0a
#define OV7660_CTL_VER			0x0b
#define OV7660_CTL_COM3			0x0c
#define OV7660_CTL_COM4			0x0d
#define OV7660_CTL_COM5			0x0e
#define OV7660_CTL_COM6			0x0f
#define OV7660_CTL_AECH			0x10
#define OV7660_CTL_CLKRC			0x11
#define OV7660_CTL_COM7			0x12
#define OV7660_CTL_COM8			0x13
#define OV7660_CTL_COM9			0x14
#define OV7660_CTL_COM10			0x15
/* RSVD 0x16 is Reserved */
#define OV7660_CTL_HSTART			0x17
#define OV7660_CTL_HSTOP			0x18
#define OV7660_CTL_VSTRT			0x19
#define OV7660_CTL_VSTOP			0x1a
#define OV7660_CTL_PSHFT			0x1b
#define OV7660_CTL_MIDH			0x1c
#define OV7660_CTL_MIDL			0x1d
#define OV7660_CTL_MVFP			0x1e
#define OV7660_CTL_LAEC			0x1f
#define OV7660_CTL_BOS			0x20
#define OV7660_CTL_GBOS			0x21
#define OV7660_CTL_GROS			0x22
#define OV7660_CTL_ROS			0x23
#define OV7660_CTL_AEW			0x24
#define OV7660_CTL_AEB			0x25
#define OV7660_CTL_VPT			0x26
#define OV7660_CTL_BBIAS			0x27
#define OV7660_CTL_GbBIAS			0x28
/* RSVD 0x29 is Reserved */
#define OV7660_CTL_EXHCH			0x2a
#define OV7660_CTL_EXHCL			0x2b
#define OV7660_CTL_RBIAS			0x2c
#define OV7660_CTL_ADVFL			0x2d
#define OV7660_CTL_ADVFH			0x2e
#define OV7660_CTL_YAVE			0x2f
#define OV7660_CTL_HSYST			0x30
#define OV7660_CTL_HSYEN			0x31
#define OV7660_CTL_HREF			0x32
#define OV7660_CTL_CHLF			0x33
#define OV7660_CTL_ARBLM			0x34
/* RSVD 0x35 is Reserved */
/* RSVD 0x36 is Reserved */
#define OV7660_CTL_ADC			0x37
#define OV7660_CTL_ACOM			0x38
#define OV7660_CTL_OFON			0x39
#define OV7660_CTL_TSLB			0x3a
#define OV7660_CTL_COM11			0x3b
#define OV7660_CTL_COM12			0x3c
#define OV7660_CTL_COM13			0x3d
#define OV7660_CTL_COM14			0x3e
#define OV7660_CTL_EDGE			0x3f
#define OV7660_CTL_COM15			0x40
#define OV7660_CTL_COM16			0x41
#define OV7660_CTL_COM17			0x42
/* RSVD 0x43 is Reserved */
/* RSVD 0x44 is Reserved */
/* RSVD 0x45 is Reserved */
/* RSVD 0x46 is Reserved */
/* RSVD 0x47 is Reserved */
/* RSVD 0x48 is Reserved */
/* RSVD 0x49 is Reserved */
/* RSVD 0x4a is Reserved */
/* RSVD 0x4b is Reserved */
/* RSVD 0x4c is Reserved */
/* RSVD 0x4d is Reserved */
/* RSVD 0x4e is Reserved */
#define OV7660_CTL_MTX1			0x4f
#define OV7660_CTL_MTX2			0x50
#define OV7660_CTL_MTX3			0x51
#define OV7660_CTL_MTX4			0x52
#define OV7660_CTL_MTX5			0x53
#define OV7660_CTL_MTX6			0x54
#define OV7660_CTL_MTX7			0x55
#define OV7660_CTL_MTX8			0x56
#define OV7660_CTL_MTX9			0x57
#define OV7660_CTL_MTXS			0x58
/* RSVD 0x59 is Reserved */
/* RSVD 0x60 is Reserved */
/* RSVD 0x61 is Reserved */
#define OV7660_CTL_LCC1			0x62
#define OV7660_CTL_LCC2			0x63
#define OV7660_CTL_LCC3			0x64
#define OV7660_CTL_LCC4			0x65
#define OV7660_CTL_LCC5			0x66
#define OV7660_CTL_MANU			0x67
#define OV7660_CTL_MANV			0x68
#define OV7660_CTL_HV				0x69
#define OV7660_CTL_GGAIN			0x6a
#define OV7660_CTL_DBLV			0x6b
/* 6c-7b GSP */
/* 7c-8a GST */
/* RSVD 0x8b is Reserved */
/* RSVD 0x8c is Reserved */
/* RSVD 0x8d is Reserved */
/* RSVD 0x8e is Reserved */
/* RSVD 0x8f is Reserved */
/* RSVD 0x90 is Reserved */
/* RSVD 0x91 is Reserved */
#define OV7660_CTL_DM_LNL			0x92
#define OV7660_CTL_DM_LNH			0x93
/* RSVD 0x94 is Reserved */
/* RSVD 0x95 is Reserved */
/* RSVD 0x96 is Reserved */
/* RSVD 0x97 is Reserved */
/* RSVD 0x98 is Reserved */
/* RSVD 0x99 is Reserved */
/* RSVD 0x9a is Reserved */
/* RSVD 0x9b is Reserved */
/* RSVD 0x9c is Reserved */
#define OV7660_CTL_BD50ST			0x9d
#define OV7660_CTL_BD60ST			0x9e
/* RSVD 0x9f is Reserved */
/* a0 DSPC2 */
/* RSVD 0xa1 is Reserved */
/* RSVD 0xa2 is Reserved */
/* RSVD 0xa3 is Reserved */
/* RSVD 0xa4 is Reserved */
/* RSVD 0xa5 is Reserved */
/* All other are factory reserved ! */

int ov7660_initialize(struct usb_microdia *);
int ov7660_set_exposure(struct usb_microdia *);
int ov7660_set_autoexposure(struct usb_microdia *dev);

#endif
