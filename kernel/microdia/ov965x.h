/**
 * @file ov965x.h
 * @author Dave Neuer
 * @date 2008-03-02
 *
 * @brief Common functions and data for the Omnivision OV965x sensor series.
 *
 * @note Copyright (C) Dave Neuer
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

#ifndef OV965X_H
#define OV965X_H

#define OV965X_CTL_GAIN		0x00
#define OV965X_CTL_BLUE		0x01
#define OV965X_CTL_RED		0x02
#define OV965X_CTL_VREF		0x03
#define OV965X_CTL_COM1		0x04
#define OV965X_CTL_BAVE		0x05
#define OV965X_CTL_GEAVE	0x06
#define OV965X_CTL_RAVE		0x08
#define OV965X_CTL_COM2		0x09
#define OV965X_CTL_PID		0x0a
#define OV965X_CTL_VER		0x0b
#define OV965X_CTL_COM3		0x0c
#define OV965X_CTL_COM4		0x0d
#define OV965X_CTL_COM5		0x0e
#define OV965X_CTL_COM6		0x0f
#define OV965X_CTL_AECH		0x10
#define OV965X_CTL_CLKRC	0x11
#define OV965X_CTL_COM7		0x12
#define OV965X_CTL_COM8		0x13
#define OV965X_CTL_COM9		0x14
#define OV965X_CTL_COM10	0x15
#define OV965X_CTL_HSTART	0x17
#define OV965X_CTL_HSTOP	0x18
#define OV965X_CTL_VSTRT	0x19
#define OV965X_CTL_VSTOP	0x1a
#define OV965X_CTL_PSHIFT	0x1b
#define OV965X_CTL_MIDH		0x1c
#define OV965X_CTL_MIDL		0x1d
#define OV965X_CTL_MVFP		0x1e
#define OV965X_CTL_LAEC		0x1f
#define OV965X_CTL_BOS		0x20
#define OV965X_CTL_GBOS		0x21
#define OV965X_CTL_GROS		0x22
#define OV965X_CTL_ROS		0x23
#define OV965X_CTL_AEW		0x24
#define OV965X_CTL_AEB		0x25
#define OV965X_CTL_VPT		0x26
#define OV965X_CTL_BBIAS	0x27
#define OV965X_CTL_GbBIAS	0x28
#define OV965X_CTL_Gr_COM	0x29
#define OV965X_CTL_EXHCH	0x2a
#define OV965X_CTL_EXHCL	0x2b
#define OV965X_CTL_RBIAS	0x2c
#define OV965X_CTL_ADVFL	0x2d
#define OV965X_CTL_ADVFH	0x2e
#define OV965X_CTL_YAVE		0x2f
#define OV965X_CTL_HSYST	0x30
#define OV965X_CTL_HSYEN	0x31
#define OV965X_CTL_HREF		0x32
#define OV965X_CTL_CHLF		0x33
#define OV965X_CTL_ARBLM	0x34
#define OV965X_CTL_ADC		0x37
#define OV965X_CTL_ACOM		0x38
#define OV965X_CTL_OFON		0x39
#define OV965X_CTL_TSLB		0x3a
#define OV965X_CTL_COM11	0x3b
#define OV965X_CTL_COM12	0x3c
#define OV965X_CTL_COM13	0x3d
#define OV965X_CTL_COM14	0x3e
#define OV965X_CTL_EDGE		0x3f
#define OV965X_CTL_COM15	0x40
#define OV965X_CTL_COM16	0x41
#define OV965X_CTL_COM17	0x42
#define OV965X_CTL_MTX1		0x4f
#define OV965X_CTL_MTX2		0x50
#define OV965X_CTL_MTX3		0x51
#define OV965X_CTL_MTX4		0x52
#define OV965X_CTL_MTX5		0x53
#define OV965X_CTL_MTX6		0x54
#define OV965X_CTL_MTX7		0x55
#define OV965X_CTL_MTX8		0x56
#define OV965X_CTL_MTX9		0x57
#define OV965X_CTL_LCC1		0x62
#define OV965X_CTL_LCC2		0x63
#define OV965X_CTL_LCC3		0x64
#define OV965X_CTL_LCC4		0x65
#define OV965X_CTL_LCC5		0x66
#define OV965X_CTL_MANU		0x67
#define OV965X_CTL_MANV		0x68
#define OV965X_CTL_HV		0x69
#define OV965X_CTL_MBD		0x6a
#define OV965X_CTL_DBLV		0x6b
#define OV965X_CTL_COM21	0x8b
#define OV965X_CTL_COM22	0x8c
#define OV965X_CTL_COM23	0x8d
#define OV965X_CTL_COM24	0x8e
#define OV965X_CTL_DBLC1	0x8f
#define OV965X_CTL_DBLC_B	0x90
#define OV965X_CTL_DBLC_R	0x91
#define OV965X_CTL_DM_LNL	0x92
#define OV965X_CTL_DM_LNH	0x93
#define OV965X_CTL_LCCFB	0x9d
#define OV965X_CTL_LCCFR	0x9e
#define OV965X_CTL_DBLC_Gb	0x9f
#define OV965X_CTL_DBLC_Gr	0xa0
#define OV965X_CTL_AECHM	0xa1
#define OV965X_CTL_COM25	0xa4 /* all bits reserved */
#define OV965X_CTL_COM26	0xa5 /* all bits reserved */
#define OV965X_CTL_G_GAIN	0xa6 /* reserved */
#define OV965X_CTL_VGA_ST	0xa7 /* reserved */
#define OV965X_CTL_ACOM2	0xa8 /* reserved */

#define OV965X_COM7_SCCB_RESET		0x80
#define OV965X_COM7_OUTPUT_VGA		0x40
#define OV965X_COM7_OUTPUT_CIF		0x20
#define OV965X_COM7_OUTPUT_QVGA		0x10
#define OV965X_COM7_OUTPUT_QCIF		0x08
#define OV965X_COM7_OUTPUT_RGB		0x04
#define OV965X_COM7_OUTPUT_RAW_RGB	0x05

#define OV965X_VREF_ADC_HIGH2(x) 	((x) << 6)
#define OV965X_VREF_VSTOP_LOW3(x) 	(0x38 & ((x) << 3))
#define OV965X_VREF_VSTART_LOW3(x) 	(0x07 & (x))

#define OV965X_COM2_OUTPUT_DRIVE_CAP_1X		0x00
#define OV965X_COM2_OUTPUT_DRIVE_CAP_2X		0x01
#define OV965X_COM2_OUTPUT_DRIVE_CAP_3X		0x02
#define OV965X_COM2_OUTPUT_DRIVE_CAP_4X		0x03
#define OV965X_COM2_SOFT_SLEEP			0x10

#define OV965X_COM5_15FPS_48MHZ_RGB		0x80
#define OV965X_COM5_SLAM_MODE			0x10

#define OV965X_COM6_BLACK_LINE_HREF_ENABLE	0x80
#define OV965X_COM6_ADBLC_BIAS_ENABLE		0x08
#define OV965X_COM6_ADBLC_OFFSET_2_CHANNEL	0x04 /* else 4-channel */
#define OV965X_COM6_TIMING_RESET_ON_FMT_CHANGE	0x02
#define OV965X_COM6_ADBLC_ENABLE		0x01

#define OV965X_CLKRC_DBL_CLK_ENABLE		0x80
#define OV965X_CLKRC_INPUT_CLK_NO_SCALE		0x40

#define OV965X_COM8_FAST_AGC_AEC		0x80
#define OV965X_COM8_AEC_STEP_SIZE_NOLIMIT	0x40 /* otherwise VSYNC */
#define OV965X_COM8_BANDING_FILTER_ON		0x20
#define OV965X_COM8_AGC_ENABLE			0x04
#define OV965X_COM8_AWB_ENABLE			0x02
#define OV965X_COM8_AEC_ENABLE			0x01

#define OV965X_COM9_MAX_AGC_128X		(0x06 << 4)
#define OV965X_COM9_MAX_AGC_64X			(0x05 << 4)
#define OV965X_COM9_MAX_AGC_32X			(0x04 << 4)
#define OV965X_COM9_MAX_AGC_16X			(0x03 << 4)
#define OV965X_COM9_MAX_AGC_8X			(0x02 << 4)
#define OV965X_COM9_MAX_AGC_4X			(0x01 << 4)
#define OV965X_COM9_RELAX_EXPOSURE_TIMING	0x08
#define OV965X_COM9_DROP_VSYNC_ON_FRAME_DROP	0x04
#define OV965X_COM9_DROP_FRAME_ON_BIG_AEC	0x02
#define OV965X_COM9_FREEZE_AGC_AEC		0x01

#define OV965X_MVFP_MIRROR			0x20
#define OV965X_MVFP_VFLIP			0x10

#define OV965X_Gr_COM_BYPASS_ANALOG_BLC		0x20
#define OV965X_Gr_COM_BYPASS_REGULATOR		0x10

#define OV965X_BIAS_SUBTRACT			0x80
#define OV965X_BIAS_VALUE(x)			((~0x80) & (x))

#define OV965X_HREF_EDGE_OFT_TO_DATA_OUT(x)	((x) << 6)
#define OV965X_HREF_HSTOP_LOW3(x)		((~0xc0) & ((x) << 3))
#define OV965X_HREF_HSTART_LOW3(x)		((~0xf8) & (x))

#define OV965X_TSLB_BITWISE_REVERSE		0x20
#define OV965X_TSLB_FIXED_UV_OUTPUT		0x10
#define OV965X_TSLB_OUTPUT_SEQ_YUYV		0x00
#define OV965X_TSLB_OUTPUT_SEQ_YVYU		0x04
#define OV965X_TSLB_OUTPUT_SEQ_VYUY		0x08
#define OV965X_TSLB_OUTPUT_SEQ_UYVY		0x0c
#define OV965X_TSLB_DIGITAL_BLC_ENABLE		0x01

#define OV965X_COM11_NIGHT_MODE_ENABLE		0x80
#define OV965X_COM11_NM_NORMAL_FRAME_RATE	0x00
#define OV965X_COM11_NM_HALF_FRAME_RATE		0x20
#define OV965X_COM11_NM_QUARTER_FRAME_RATE	0x40
#define OV965X_COM11_NM_EIGHTH_FRAME_RATE	0x60
#define OV965X_COM11_FULL_FRAME_CALC_WIND	0x00
#define OV965X_COM11_HALF_FRAME_CALC_WIND	0x08
#define OV965X_COM11_QUARTER_FRAME_CALC_WIND	0x10
#define OV965X_COM11_MANUAL_BANDING_FILTER	0x01

#define OV965X_COM13_ONLY_Y_CHANNEL_GAMMA	0x40
#define OV965X_COM13_GAMMA_RAW_PRE_INTERP	0x80
#define OV965X_COM13_ENABLE_COLOR_MATRIX	0x10
#define OV965X_COM13_DELAY_UV_CHANNEL		0x00
#define OV965X_COM13_DELAY_Y_CHANNEL		0x08
#define OV965X_COM13_OUTPUT_DELAY(x)		((~0xf8) & (x))

#define OV965X_COM14_YUV_EDGE_ENHANCE		0x02
#define OV965X_COM14_EDGE_ENHANCE_FACTOR_DBL	0x01

#define OV965X_EDGE_EDGE_ENHANCE_LOW4(x)	((x) << 4)
#define OV965X_EDGE_EDGE_ENHANCE_FACTOR(x)	(0xf0 & (x))

#define OV965X_COM15_OUTPUT_RANGE_10_TO_F0	0x00
#define OV965X_COM15_OUTPUT_RANGE_O1_TO_FE	0x80
#define OV965X_COM15_OUTPUT_RANGE_O0_TO_FF	0xc0
#define OV965X_COM15_OUTPUT_RGB_565		0x10
#define OV965X_COM15_OUTPUT_RGB_555		0x30
#define OV965X_COM15_SWAP_R_AND_B		0x08


int soi968_set_exposure(struct usb_microdia *dev);
int soi968_initialize(struct usb_microdia *dev);
int ov965x_initialize(struct usb_microdia *);
int ov965x_set_hvflip(struct usb_microdia *);
int ov965x_set_exposure(struct usb_microdia *);
int ov965x_set_autoexposure(struct usb_microdia *dev);
#endif
