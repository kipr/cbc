/**
 * @file mt9vx11.h
 * @author Comer352l
 * @date 2008-04-25
 *
 * @brief Common functions and data for the Micron MT9Vx11 sensors.
 *
 * @note Copyright (C) Comer352l
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

#ifndef MT9VX11_H
#define MT9VX11_H


#define MT9V111_I2C_SLAVE_ADDRESS	0x5c
#define MT9V011_I2C_SLAVE_ADDRESS	0x5d


/* Registers: */
#define MT9V111_IFPREG_OPMODECTL		0x06	/* Operating mode control */

#define MT9V111_IFPREG_AE_HWINBOUNDARY		0x26	/* Horizontal boundaries of AE measurement window */
#define MT9V111_IFPREG_AE_VWINBOUNDARY		0x27	/* Vertical boundaries of AE measurement window */
#define MT9V111_IFPREG_AE_HWINBOUNDARY_BLC	0x2b	/* H. bound. of AE meas. win. for back light compensation */
#define MT9V111_IFPREG_AE_VWINBOUNDARY_BLC	0x2c	/* V. bound. of AE meas. win. for back light compensation */
#define MT9V111_IFPREG_AE_TARGETLUMCTL		0x2e	/* AE target luminance and tracking accuracy control */
#define MT9V111_IFPREG_AE_SPEEDSENSCTL		0x2f	/* AE speed and sensitivity control */
#define MT9V111_IFPREG_AE_DGAINSLIMITS		0x67	/* AE digital gains limits */

#define MT9V111_IFPREG_AWB_TINT			0x21	/* AWB tint */
#define MT9V111_IFPREG_AWB_SPEEDCOLORSATCTL	0x25	/* AWB speed and color saturation control */
#define MT9V111_IFPREG_AWB_WINBOUNDARY		0x2d	/* Boundaries of AWB measurement window */


/* Values: */
#define MT9V111_ADDRESSSPACE_IFP			0x01			/* IFP-register address space */
#define MT9V111_ADDRESSSPACE_SENSOR			0x04			/* sensor-(core-)register address space */

#define MT9V111_IFP_OPMODE_AUTOEXPOSURE			0x4000			/* enable auto exposure */
#define MT9V111_IFP_OPMODE_AUTOWHITEBALANCE		0x0002			/* enable auto exposure */
#define MT9V111_IFP_OPMODE_BYPASSCOLORCORR		0x0010			/* bypass color correction matrix */
#define MT9V111_IFP_OPMODE_APERTURECORR			0x1000			/* enable aperture knee correction */
#define MT9V111_IFP_OPMODE_ONTHEFLYDEFECTCORR		0x2000			/* enable on-the-fly color correction */

#define MT9V111_IFP_AE_TARGETLUMINANCE(x)		(x & 0xff)		/* AE target luminace */
#define MT9V111_IFP_AE_TRACKINGACCURACY(x)		((x & 0xff) << 8)	/* AE tracking accuracy */
#define MT9V111_IFP_AE_DELAY(x)				(x & 0x7)		/* AE reaction delay */
#define MT9V111_IFP_AE_SPEED(x)				((x & 0x7) << 3)	/* AE speed */
#define MT9V111_IFP_AE_STEPSIZE_MEDIUMDOWNSLOWUP	0x0000			/* AE step size: medium speed when going down, slow when going up */
#define MT9V111_IFP_AE_STEPSIZE_MEDIUM			0x0040			/* AE step size: medium speed */
#define MT9V111_IFP_AE_STEPSIZE_FAST			0x0080			/* AE step size: fast speed*/
#define MT9V111_IFP_AE_STEPSIZE_FASTDOWNMEDIUMUP	0x00c0			/* AE step size: fast when going down, medium when going up */
#define MT9V111_IFP_AE_WINBOUNDARY_LEFT(x)		((x/4) & 0xff)		/* left boundary of AE meas. window */
#define MT9V111_IFP_AE_WINBOUNDARY_RIGHT(x)		(((x/4) & 0xff) << 8)	/* right boundary of AE meas. window */
#define MT9V111_IFP_AE_WINBOUNDARY_TOP(x)		((x/2) & 0xff)		/* top boundary of AE meas. window */
#define MT9V111_IFP_AE_WINBOUNDARY_BOTTOM(x)		(((x/2) & 0xff) << 8)	/* bottom boundary of AE meas. window */
#define MT9V111_IFP_AE_WINBOUNDARY_LEFT_BLC(x)		((x/4) & 0xff)		/* left boundary of AE meas. window (backlight compensation) */
#define MT9V111_IFP_AE_WINBOUNDARY_RIGHT_BLC(x)		(((x/4) & 0xff) << 8)	/* right boundary of AE meas. window (backlight compensation)*/
#define MT9V111_IFP_AE_WINBOUNDARY_TOP_BLC(x)		((x/2) & 0xff)		/* top boundary of AE meas. window (backlight compensation)*/
#define MT9V111_IFP_AE_WINBOUNDARY_BOTTOM_BLC(x)	(((x/2) & 0xff) << 8)	/* bottom boundary of AE meas. window (backlight compensation)*/
#define MT9V111_IFP_AE_WIN_LARGE			0x0000			/* large AE measurement window */
#define MT9V111_IFP_AE_WIN_MEDIUM			0x0004			/* medium AE measurement window (backlight compensation) */
#define MT9V111_IFP_AE_WIN_COMBINED			0x0008			/* combined AE meas. win: (large win + (2 * medium win))/3 */
#define MT9V111_IFP_AE_MAXGAIN_PRELS(x)			(x & 0xff)		/* max. digital gain applied before lens shading correction */
#define MT9V111_IFP_AE_MAXGAIN_POSTLS(x)		((x & 0xff) << 8)	/* max. digital gain applied during lens shading correction */

#define MT9V111_IFP_AWB_AUTOSATLOWLIGHT_ENABLE		0x4000			/* enable automatic color saturation control in low light */
#define MT9V111_IFP_AWB_ADDON_BLUE(x)			(x & 0xff)		/* AWB blue channel add-on */
#define MT9V111_IFP_AWB_ADDON_RED(x)			((x & 0xff) << 8)	/* AWB red channel add-on */
#define MT9V111_IFP_AWB_DELAY(x)			(x & 0x7)		/* AWB reaction delay */
#define MT9V111_IFP_AWB_SPEED(x)			((x & 0x7) << 3)	/* AWB speed */
#define MT9V111_IFP_AWB_SATURATION_BLACKWHITE		0x3000			/* black and white */
#define MT9V111_IFP_AWB_SATURATION_25			0x2000			/* 25% of full saturation */
#define MT9V111_IFP_AWB_SATURATION_37p5			0x1800			/* 37.5% of full saturation */
#define MT9V111_IFP_AWB_SATURATION_50			0x1000			/* 50% of full saturation */
#define MT9V111_IFP_AWB_SATURATION_75			0x0800			/* 75% of full saturation */
#define MT9V111_IFP_AWB_SATURATION_FULL			0x0000			/* full color saturation */
#define MT9V111_IFP_AWB_SATURATION_150			0x2800			/* 150% of full saturation */
#define MT9V111_IFP_AWB_WINBOUNDARY_LEFT(x)		((x/64) & 0xf)		/* left boundary of AWB meas. window */
#define MT9V111_IFP_AWB_WINBOUNDARY_RIGHT(x)		(((x/64) & 0xf) << 4)	/* right boundary of AWB meas. window */
#define MT9V111_IFP_AWB_WINBOUNDARY_TOP(x)		(((x/32) & 0xf) << 8)	/* top boundary of AWB meas. window */
#define MT9V111_IFP_AWB_WINBOUNDARY_BOTTOM(x)		(((x/32) & 0xf) << 12)	/* bottom boundary of AWB meas. window */


int mt9v111_select_address_space(struct usb_microdia *dev, __u8 address_space);

int mt9vx11_sensor_probe(struct usb_microdia *dev);

int mt9v111_setup_autoexposure(struct usb_microdia *dev);

int mt9v111_setup_autowhitebalance(struct usb_microdia *dev);

int mt9vx11_set_exposure(struct usb_microdia *dev);

int mt9vx11_set_hvflip(struct usb_microdia *dev);

int mt9v111_set_autoexposure(struct usb_microdia *dev);

int mt9v111_set_autowhitebalance(struct usb_microdia *dev);

int mt9v111_set_autocorrections(struct usb_microdia *dev, int enable);


#endif
