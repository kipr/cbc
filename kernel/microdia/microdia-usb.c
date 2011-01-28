/**
 * @file microdia-usb.c
 * @author Nicolas VIVIEN
 * @date 2008-02-01
 *
 * @brief Driver for Microdia USB video camera
 *
 * @note Copyright (C) Nicolas VIVIEN
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

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/kref.h>
#include <linux/stat.h>

#include <linux/usb.h>
#include <media/v4l2-common.h>

#include "microdia.h"
#include "sn9c20x.h"
#include "mt9vx11.h"
#include "ov965x.h"
#include "ov7660.h"

/**
 * @var fps
 *   Module parameter to set frame per second
 */
static int fps = 25;

/**
 * @var bulk
 *  Module parameter to enable/disable bulk transfers
 */
static int bulk = 1;

/**
 * @var jpeg
 *  Module parameter to enable/disable jpeg transfers
 */
int jpeg;

/**
 * @var max_urbs
 *  Module parameter to maximum number of URBs
 */
int max_urbs;

/**
 * @var max_urbs
 *  Module parameter to maximum number of URBs
 */
int altsetting;

/**
 * @var hflip
 *  Module parameter to enable/disable the horizontal flip process
 */
static int hflip;

/**
 * @var flip_detect
 *  Module parameter to enable/disable vflip detection
 */
static int flip_detect;

/**
 * @var vflip
 *   Module parameter to enable/disable the vertical flip process
 */
static int vflip;

/**
 * @var brightness
 *   Module parameter to set the brightness
 */
static int brightness = MICRODIA_PERCENT(50, 0xFFFF);

/**
 * @var whiteness
 *   Module parameter to set the whiteness
 */
static int whiteness = MICRODIA_PERCENT(20, 0xFFFF);

/**
 * @var contrast
 *   Module parameter to set the contrast
 */
static int contrast = MICRODIA_PERCENT(50, 0xFFFF);

/**
 * @var exposure
 *   Module parameter to set the exposure
 */
static int exposure = MICRODIA_PERCENT(2, 0xFFFF);

/**
 * @var sharpness
 *   Module parameter to set the sharpness
 */
static int sharpness = MICRODIA_PERCENT(50, 0x3F);

/**
 * @var rgb_gain
 *   Module parameter to set the red/green/blue gain
 */
static int rgb_gain = MICRODIA_PERCENT(25, 0x7F) |
		     (MICRODIA_PERCENT(25, 0x7F) << 8) |
		     (MICRODIA_PERCENT(25, 0x7F) << 16);

/**
 * @var min_buffers
 *   Module parameter to set the minimum number of image buffers
 */
static int min_buffers = 2;

/**
 * @var max_buffers
 *   Module parameter to set the maximum number of image buffers
 */
static int max_buffers = 5;

/**
 * @var auto_exposure
 *   Module parameter to set the exposure
 */
static int auto_exposure = 0;

/**
 * @var auto_whitebalance
 *   Module parameter to set the whitebalance
 */
static int auto_whitebalance = 1;

/**
 * @var log_level
 *   Module parameter to set the log level
 */
__u8 log_level = 1;


/**
 * @var microdia_table
 * Define all the hotplug supported devices by this driver
 */
static struct usb_device_id microdia_table[] = {
	/* SN9C201 + MI1300 */
	{ USB_DEVICE(USB_0C45_VID, USB_6240_PID) },
	/* SN9C201 + MI1310 */
	{ USB_DEVICE(USB_0C45_VID, USB_6242_PID) },
	/* SN9C201 + S5K4AAFX */
	{ USB_DEVICE(USB_0C45_VID, USB_6243_PID) },
	/* SN9C201 + OV9655 */
	{ USB_DEVICE(USB_0C45_VID, USB_6248_PID) },
	/* SN9C201 + CX1332 */
	{ USB_DEVICE(USB_0C45_VID, USB_624B_PID) },
	/* SN9C201 + MI1320 */
	{ USB_DEVICE(USB_0C45_VID, USB_624C_PID) },
	/* SN9C201 + SOI968 */
	{ USB_DEVICE(USB_0C45_VID, USB_624E_PID) },
	/* SN9C201 + OV9650 */
	{ USB_DEVICE(USB_0C45_VID, USB_624F_PID) },
	/* SN9C201 + OV9650 */
	{ USB_DEVICE(USB_0C45_VID, USB_6253_PID) },
	/* SN9C201 + OV7670ISP */
	{ USB_DEVICE(USB_0C45_VID, USB_6260_PID) },
	/* SN9C201 + OM6802 */
	{ USB_DEVICE(USB_0C45_VID, USB_6262_PID) },
	/* SN9C201 + MI0360/MT9V111 */
	{ USB_DEVICE(USB_0C45_VID, USB_6270_PID) },
	/* SN9C201 + S5K53BEB */
	{ USB_DEVICE(USB_0C45_VID, USB_627A_PID) },
	/* SN9C201 + OV7660 */
	{ USB_DEVICE(USB_0C45_VID, USB_627B_PID) },
	/* SN9C201 + HV7131R */
	{ USB_DEVICE(USB_0C45_VID, USB_627C_PID) },
	/* EEPROM */
	{ USB_DEVICE(USB_0C45_VID, USB_627F_PID) },
	/* SN9C202 + MI1300 */
	{ USB_DEVICE(USB_0C45_VID, USB_6280_PID) },

        // Botball camera
	/* SN9C202 + MI1310 */
	{ USB_DEVICE(USB_0C45_VID, USB_6282_PID) },
        
	/* SN9C202 + S5K4AAFX */
	{ USB_DEVICE(USB_0C45_VID, USB_6283_PID) },
	/* SN9C202 + OV9655 */
	{ USB_DEVICE(USB_0C45_VID, USB_6288_PID) },
	/* SN9C202 + ICM107 */
	{ USB_DEVICE(USB_0C45_VID, USB_628A_PID) },
	/* SN9C202 + CX1332 */
	{ USB_DEVICE(USB_0C45_VID, USB_628B_PID) },
	/* SN9C202 + MI1320 */
	{ USB_DEVICE(USB_0C45_VID, USB_628C_PID) },
	/* SN9C202 + SOI968 */
	{ USB_DEVICE(USB_0C45_VID, USB_628E_PID) },
	/* SN9C202 + OV9650 */
	{ USB_DEVICE(USB_0C45_VID, USB_628F_PID) },
	/* SN9C202 + OV7670ISP */
	{ USB_DEVICE(USB_0C45_VID, USB_62A0_PID) },
	/* SN9C202 + OM6802 */
	{ USB_DEVICE(USB_0C45_VID, USB_62A2_PID) },
	/* SN9C202 + MI0360/MT9V111 */
	{ USB_DEVICE(USB_0C45_VID, USB_62B0_PID) },
	/* SN9C202 + OV9655 */
	{ USB_DEVICE(USB_0C45_VID, USB_62B3_PID) },
	/* SN9C202 + S5K53BEB */
	{ USB_DEVICE(USB_0C45_VID, USB_62BA_PID) },
	/* SN9C202 + OV7660 */
	{ USB_DEVICE(USB_0C45_VID, USB_62BB_PID) },
	/* SN9C202 + HV7131R */
	{ USB_DEVICE(USB_0C45_VID, USB_62BC_PID) },
	/* SN9C202 + OV7663 */
	{ USB_DEVICE(USB_0C45_VID, USB_62BE_PID) },
	/* => 628f (SN9C202 + OV9650) */
	{ USB_DEVICE(USB_045E_VID, USB_00F4_PID) },
	/* => 627b (SN9C201 + OV7660) */
	{ USB_DEVICE(USB_145F_VID, USB_013D_PID) },
	/* => 62be (SN9C202 + OV7663 + EEPROM) */
	{ USB_DEVICE(USB_04F2_VID, USB_A128_PID) },
	{ }
};


MODULE_DEVICE_TABLE(usb, microdia_table);		/**< Define the supported devices */

DEFINE_MUTEX(open_lock); /**< Define global mutex */

int microdia_6240_initialize(struct usb_microdia *dev);
int microdia_6240_start_stream(struct usb_microdia *dev);
int microdia_6240_stop_stream(struct usb_microdia *dev);
int microdia_6242_start_stream(struct usb_microdia *dev);
int microdia_6242_stop_stream(struct usb_microdia *dev);
int microdia_6242_set_autofeature(struct usb_microdia *dev);
int microdia_6242_set_exposure(struct usb_microdia *dev);
int microdia_624e_initialize(struct usb_microdia *dev);
int microdia_624e_start_stream(struct usb_microdia *dev);
int microdia_624e_stop_stream(struct usb_microdia *dev);
int microdia_624f_initialize(struct usb_microdia *dev);
int microdia_624f_stop_stream(struct usb_microdia *dev);
int microdia_624f_start_stream(struct usb_microdia *dev);
int microdia_624f_set_exposure(struct usb_microdia *dev);
int microdia_624f_flip_detect(struct usb_microdia *dev);
int microdia_6260_initialize(struct usb_microdia *dev);
int microdia_6260_start_stream(struct usb_microdia *dev);
int microdia_6260_stop_stream(struct usb_microdia *dev);
int microdia_6260_flip_detect(struct usb_microdia *dev);
int microdia_6270_initialize(struct usb_microdia *dev);
int microdia_6270_start_stream(struct usb_microdia *dev);
int microdia_6270_stop_stream(struct usb_microdia *dev);
int microdia_627b_initialize(struct usb_microdia *dev);
int microdia_627b_start_stream(struct usb_microdia *dev);
int microdia_627b_stop_stream(struct usb_microdia *dev);
int microdia_627f_initialize(struct usb_microdia *dev);
int microdia_6288_initialize(struct usb_microdia *dev);
int microdia_6288_start_stream(struct usb_microdia *dev);
int microdia_6288_stop_stream(struct usb_microdia *dev);

/**
 * @var cameras
 *
 * This array assigns all specific functions and settings to each cam.
 *
 */
struct microdia_camera cameras[] = {
	{
		.model = CAMERA_MODEL(USB_0C45_VID, USB_6240_PID),
		.type = MICRODIA_VGA,
		.sensor_slave_address = 0x5d,
		.sensor_flags = SN9C20X_I2C_2WIRE,
		.supported_fmts = 0x03,
		.initialize = microdia_6240_initialize,
		.start_stream = microdia_6240_start_stream,
		.stop_stream = microdia_6240_stop_stream,
	},
	{
		.model = CAMERA_MODEL(USB_0C45_VID, USB_6242_PID),
		.type = MICRODIA_VGA,
		.sensor_slave_address = 0x5d,
		.sensor_flags = SN9C20X_I2C_2WIRE,
		.supported_fmts = 0x03,
		.initialize = microdia_624e_initialize,
		.start_stream = microdia_6242_start_stream,
		.stop_stream = microdia_6242_stop_stream,
	},
	{
          // Botball camera
          // Has Micro MT9M111 sensor
		.model = CAMERA_MODEL(USB_0C45_VID, USB_6282_PID),
		.type = MICRODIA_VGA,
		.sensor_slave_address = 0x5d,
		.sensor_flags = SN9C20X_I2C_2WIRE,
                .supported_fmts = 0x17,
		.initialize = microdia_624e_initialize,
		.start_stream = microdia_6242_start_stream,
		.stop_stream = microdia_6242_stop_stream,
                .set_auto_exposure = microdia_6242_set_autofeature,
                .set_auto_whitebalance = microdia_6242_set_autofeature,
	},
	{
		.model = CAMERA_MODEL(USB_0C45_VID, USB_624E_PID),
		.type = MICRODIA_VGA,
		.sensor_slave_address = 0x30,
		.sensor_flags = SN9C20X_I2C_2WIRE,
		.supported_fmts = 0x03,
		.initialize = microdia_624e_initialize,
		.sensor_init = soi968_initialize,
		.start_stream = microdia_624e_start_stream,
		.stop_stream = microdia_624e_stop_stream,
		.set_auto_exposure = ov965x_set_autoexposure,
		/* .set_exposure = soi968_set_exposure, */
	},
	{
		.model = CAMERA_MODEL(USB_0C45_VID, USB_624F_PID),
		.type = MICRODIA_VGA,
		.sensor_slave_address = 0x30,
		.sensor_flags = SN9C20X_I2C_2WIRE,
		.supported_fmts = 0x13,
		.initialize = microdia_624f_initialize,
		.sensor_init = ov965x_initialize,
		.start_stream = microdia_624f_start_stream,
		.stop_stream = microdia_624f_stop_stream,
		.flip_detect = microdia_624f_flip_detect,
		.set_auto_exposure = ov965x_set_autoexposure,
		.set_exposure = ov965x_set_exposure,
		.set_hvflip = ov965x_set_hvflip,
	},
	{
		.model = CAMERA_MODEL(USB_0C45_VID, USB_6253_PID),
		.type = MICRODIA_VGA,
		.sensor_slave_address = 0x30,
		.sensor_flags = SN9C20X_I2C_2WIRE,
		.supported_fmts = 0x03,
		.initialize = microdia_624f_initialize,
		.sensor_init = ov965x_initialize,
		.start_stream = microdia_624f_start_stream,
		.stop_stream = microdia_624f_stop_stream,
		.flip_detect = microdia_624f_flip_detect,
		.set_auto_exposure = ov965x_set_autoexposure,
		.set_exposure = ov965x_set_exposure,
		.set_hvflip = ov965x_set_hvflip,
	},
	{
		.model = CAMERA_MODEL(USB_0C45_VID, USB_6260_PID),
		.type = MICRODIA_VGA,
		.sensor_slave_address = 0x21,
		.sensor_flags = SN9C20X_I2C_2WIRE,
		.supported_fmts = 0x08,
		.initialize = microdia_6260_initialize,
		.start_stream = microdia_6260_start_stream,
		.stop_stream = microdia_6260_stop_stream,
		.flip_detect = microdia_6260_flip_detect,
	},
	{
		.model = CAMERA_MODEL(USB_0C45_VID, USB_6270_PID),
		.type = MICRODIA_VGA,
		.sensor_slave_address = 0,
		.sensor_flags = SN9C20X_I2C_2WIRE,
		.supported_fmts = 0x07,
		.initialize = microdia_6270_initialize,
		.start_stream = microdia_6270_start_stream,
		.stop_stream = microdia_6270_stop_stream,
		.set_exposure = mt9vx11_set_exposure,
		.set_hvflip = mt9vx11_set_hvflip,
		.set_auto_exposure = mt9v111_set_autoexposure,
		.set_auto_whitebalance = mt9v111_set_autowhitebalance,
	},
	{
		.model = CAMERA_MODEL(USB_0C45_VID, USB_627B_PID),
		.type = MICRODIA_VGA,
		.sensor_slave_address = 0x21,
		.sensor_flags = SN9C20X_I2C_2WIRE,
		.supported_fmts = 0x07,
		.initialize = microdia_627b_initialize,
		.sensor_init = ov7660_initialize,
		.start_stream = microdia_627b_start_stream,
		.stop_stream = microdia_627b_stop_stream,
		.set_auto_exposure = ov7660_set_autoexposure,
		.set_exposure = ov7660_set_exposure,
	},
	{
		.model = CAMERA_MODEL(USB_0C45_VID, USB_627F_PID),
		.type = MICRODIA_VGA,
		.sensor_slave_address = 0x30,
		.sensor_flags = SN9C20X_I2C_2WIRE,
		.supported_fmts = 0x03,
		.initialize = microdia_624f_initialize,
		.sensor_init = ov965x_initialize,
		.start_stream = microdia_624f_start_stream,
		.stop_stream = microdia_624f_stop_stream,
		.flip_detect = microdia_624f_flip_detect,
		.set_exposure = ov965x_set_exposure,
		.set_hvflip = ov965x_set_hvflip,
	},
	{
		.model = CAMERA_MODEL(USB_0C45_VID, USB_6288_PID),
		.type = MICRODIA_VGA,
		.sensor_slave_address = 0x30,
		.sensor_flags = SN9C20X_I2C_2WIRE,
		.supported_fmts = 0x03,
		.initialize = microdia_6288_initialize,
		.start_stream = microdia_6288_start_stream,
		.stop_stream = microdia_6288_stop_stream,
		.set_auto_exposure = ov965x_set_autoexposure,
	},
	{
		.model = CAMERA_MODEL(USB_0C45_VID, USB_628F_PID),
		.type = MICRODIA_VGA,
		.sensor_slave_address = 0x30,
		.sensor_flags = SN9C20X_I2C_2WIRE,
		.supported_fmts = 0x03,
		.initialize = microdia_624f_initialize,
		.sensor_init = ov965x_initialize,
		.start_stream = microdia_624f_start_stream,
		.stop_stream = microdia_624f_stop_stream,
		.flip_detect = microdia_624f_flip_detect,
		.set_auto_exposure = ov965x_set_autoexposure,
		.set_exposure = ov965x_set_exposure,
		.set_hvflip = ov965x_set_hvflip,
	},
	{
		.model = CAMERA_MODEL(USB_0C45_VID, USB_62B3_PID),
		.type = MICRODIA_VGA,
		.sensor_slave_address = 0x30,
		.sensor_flags = SN9C20X_I2C_2WIRE,
		.supported_fmts = 0x03,
		.initialize = microdia_6288_initialize,
		.start_stream = microdia_6288_start_stream,
		.stop_stream = microdia_6288_stop_stream,
	},
	{
		.model = CAMERA_MODEL(USB_0C45_VID, USB_62BB_PID),
		.type = MICRODIA_VGA,
		.sensor_slave_address = 0x21,
		.sensor_flags = SN9C20X_I2C_2WIRE,
		.supported_fmts = 0x07,
		.initialize = microdia_627b_initialize,
		.start_stream = microdia_627b_start_stream,
		.stop_stream = microdia_627b_stop_stream,
	},
	{
		.model = CAMERA_MODEL(USB_145F_VID, USB_013D_PID),
		.type = MICRODIA_VGA,
		.sensor_slave_address = 0x21,
		.sensor_flags = SN9C20X_I2C_2WIRE,
		.supported_fmts = 0x07,
		.initialize = microdia_627b_initialize,
		.start_stream = microdia_627b_start_stream,
		.stop_stream = microdia_627b_stop_stream,
	},
	{
		.model = CAMERA_MODEL(USB_045E_VID, USB_00F4_PID),
		.type = MICRODIA_VGA,
		.sensor_slave_address = 0x30,
		.sensor_flags = SN9C20X_I2C_2WIRE,
		.supported_fmts = 0x03,
		.initialize = microdia_624f_initialize,
		.sensor_init = ov965x_initialize,
		.start_stream = microdia_624f_start_stream,
		.stop_stream = microdia_624f_stop_stream,
		.flip_detect = microdia_624f_flip_detect,
		.set_auto_exposure = ov965x_set_autoexposure,
		.set_exposure = ov965x_set_exposure,
		.set_hvflip = ov965x_set_hvflip,
	},
};


/**
 * @brief Find device specific microdia_camera structure for model
 *
 * @param model Model we are looking for
 *
 * @returns Pointer to device specific microdia_camera structure or NULL
 *
 */
static inline struct microdia_camera *find_camera(__u32 model)
{
	int i;
	for (i = 0; i < ARRAY_SIZE(cameras); i++) {
		if (cameras[i].model == model)
			return &cameras[i];
	}
	return NULL;
}

struct usb_endpoint_descriptor *find_endpoint(struct usb_host_interface *alts, __u8 epaddr)
{
	unsigned long i;
	struct usb_endpoint_descriptor *ep;

	for (i = 0; i < alts->desc.bNumEndpoints; ++i) {
		ep = &alts->endpoint[i].desc;
		if ((ep->bEndpointAddress & 0xf) == epaddr) {
			UDIA_INFO("Found Endpoint %02x\n", ep->bEndpointAddress);
			return ep;
		}
	}

	return NULL;
}
/**
 * @param dev Device structure
 * @param ep Usb endpoint structure
 *
 * @returns 0 if all is OK
 *
 * @brief Initilize an isochronous pipe.
 *
 * This function permits to initialize an URB transfert (or isochronous pipe).
 */
int usb_microdia_isoc_init(struct usb_microdia *dev, struct usb_endpoint_descriptor *ep)
{
	int i, j;
	__u16 iso_max_frame_size;
	struct urb *urb;
	struct usb_device *udev;

	udev = dev->udev;

	UDIA_DEBUG("usb_microdia_isoc_init()\n");

	iso_max_frame_size =
		max_packet_sz(le16_to_cpu(le16_to_cpu(ep->wMaxPacketSize))) *
		hb_multiplier(le16_to_cpu(le16_to_cpu(ep->wMaxPacketSize)));

	for (i = 0; i < max_urbs; i++) {
		urb = usb_alloc_urb(ISO_FRAMES_PER_DESC, GFP_KERNEL);

		if (urb == NULL) {
			UDIA_ERROR("Failed to allocate URB %d\n", i);
			usb_microdia_uninit_urbs(dev);
			return -ENOMEM;
		}

		urb->interval = 1;
		urb->dev = udev;
		urb->pipe = usb_rcvisocpipe(udev, ep->bEndpointAddress);
		urb->transfer_flags = URB_ISO_ASAP;
		urb->transfer_buffer_length = iso_max_frame_size * ISO_FRAMES_PER_DESC;
		urb->complete = usb_microdia_completion_handler;
		urb->context = dev;
		urb->start_frame = 0;
		urb->number_of_packets = ISO_FRAMES_PER_DESC;

		for (j = 0; j < ISO_FRAMES_PER_DESC; j++) {
			urb->iso_frame_desc[j].offset = j * iso_max_frame_size;
			urb->iso_frame_desc[j].length = iso_max_frame_size;
		}

		dev->urbs[i].data = kzalloc(urb->transfer_buffer_length,
					    GFP_KERNEL);
		if (dev->urbs[i].data == NULL) {
			usb_microdia_uninit_urbs(dev);
			return -ENOMEM;
		}

		urb->transfer_buffer = dev->urbs[i].data;
		dev->urbs[i].urb = urb;
	}

	return 0;
}

void usb_microdia_bulk_completion_handler(struct urb *urb, struct pt_regs *regs);

// WARNING:  driver is hardcoded for 160x120 operation
#define FRAME_HEADER_SIZE 64
#define FRAME_BODY_SIZE 28800
#define FRAME_SIZE (FRAME_HEADER_SIZE + FRAME_BODY_SIZE)

int usb_microdia_bulk_init(struct usb_microdia *dev, struct usb_endpoint_descriptor *ep)
{
  struct urb *urb;
  unsigned int pipe, i;
  __u16 psize;
  __u32 size;
  psize = max_packet_sz(le16_to_cpu(le16_to_cpu(ep->wMaxPacketSize)));
  size = psize * ISO_FRAMES_PER_DESC;
  pipe = usb_rcvbulkpipe(dev->udev, ep->bEndpointAddress);

  size=FRAME_SIZE;
  
  UDIA_INFO("Creating %d urbs of %d bytes each\n", max_urbs, size);
  for (i = 0; i < max_urbs; ++i) {
    urb = usb_alloc_urb(0, GFP_KERNEL);
    if (urb == NULL) {
      usb_microdia_uninit_urbs(dev);
      return -ENOMEM;
    }
    
    dev->urbs[i].data = kzalloc(size, GFP_KERNEL);
    
    usb_fill_bulk_urb(urb, dev->udev, pipe,
                      dev->urbs[i].data, size,
                      usb_microdia_bulk_completion_handler,
                      dev);
    
    //urb->transfer_flags |= URB_SHORT_NOT_OK;
    
    dev->urbs[i].urb = urb;
  }
  
  return 0;
}

int usb_microdia_init_urbs(struct usb_microdia *dev)
{
	int ret, i;
	__u8 value;
	struct usb_endpoint_descriptor *ep;
	struct usb_interface *intf = dev->interface;

	ret = usb_microdia_control_read(dev, 0x1061, &value, 1);
	if (ret < 0)
		return ret;

	if (!bulk) {
		ep = find_endpoint(usb_altnum_to_altsetting(intf, altsetting), MICRODIA_VID_ISOC);

		if (ep == NULL)
			return -EIO;

		ret = usb_set_interface(dev->udev, 0, 8);
		if (ret < 0)
			return ret;

		value |= 0x01;
		ret = usb_microdia_control_write(dev, 0x1061, &value, 1);
		if (ret < 0)
			return ret;

		ret = usb_microdia_isoc_init(dev, ep);
        }
        else {
		ep = find_endpoint(usb_altnum_to_altsetting(intf, 0), MICRODIA_BULK);
		if (ep == NULL)
			return -EIO;

                UDIA_INFO("Picked enpoint with max packet size %u\n", ep->wMaxPacketSize);

		ret = usb_set_interface(dev->udev, 0, 0);
		if (ret < 0)
			return ret;

		value &= ~0x01;
		ret = usb_microdia_control_write(dev, 0x1061, &value, 1);
		if (ret < 0)
			return ret;

		ret = usb_microdia_bulk_init(dev, ep);
	}

	if (ret < 0)
		return ret;

	for (i = 0; i < max_urbs; i++) {
		ret = usb_submit_urb(dev->urbs[i].urb, GFP_KERNEL);
		if (ret)
			UDIA_ERROR("isoc_init() submit_urb %d failed with error %d\n", i, ret);
	}

	return 0;
}

/**
 * @param dev Device structure
 *
 * @brief Clean-up all the ISOC buffers
 *
 * This function permits to clean-up all the ISOC buffers.
 */
void usb_microdia_uninit_urbs(struct usb_microdia *dev)
{
	int i;
	struct urb *urb;

	UDIA_DEBUG("Isoc cleanup\n");

	if (dev == NULL)
		return;

	for (i = 0; i < max_urbs; i++) {
		urb = dev->urbs[i].urb;
		if (urb == NULL)
			continue;
		usb_kill_urb(urb);
		kfree(dev->urbs[i].data);
		usb_free_urb(urb);
		dev->urbs[i].urb = NULL;
	}
}

int usb_microdia_detect_frame(unsigned char *buf, unsigned int buf_length)
{
	unsigned char frame_header[] = {0xff, 0xff, 0x00, 0xc4, 0xc4, 0x96};
	int index;

	if (buf_length < 64)
		return -1;
	if (buf_length == 64) {
		if(memcmp(buf, frame_header, 6) == 0)
			return 0;
		else
			return -1;
	}
	for (index = 0; index < buf_length - 63; index++) {
		if (memcmp(buf + index, frame_header, 6) == 0) {
			UDIA_INFO("Found Header at %d\n", index);
			return index;
		}
	}
	return -1;
}

void usb_microdia_assemble_video(struct usb_microdia *dev,
	unsigned char *transfer, unsigned int transfer_length,
	struct microdia_buffer **buffer)
{
	void *mem = NULL;
    unsigned char format;
    const char *format_desc;
	int header_index;
	struct microdia_buffer *buf = *buffer;
	struct microdia_video_queue *queue = &dev->queue;

	if (buf->state != MICRODIA_BUF_STATE_ACTIVE)
		buf->state = MICRODIA_BUF_STATE_ACTIVE;

	header_index = usb_microdia_detect_frame(transfer, transfer_length);
	if (header_index >= 0) {
		if (header_index + buf->buf.bytesused > buf->buf.length) {
			UDIA_WARNING("Frame Buffer overflow!\n");
			dev->vframes_overflow++;
			buf->state = MICRODIA_BUF_STATE_DONE;
		}
		UDIA_INFO("Frame Resolution: %dx%d\n",
			   transfer[0x3a+header_index] << 4, transfer[0x3b+header_index] << 3);
        format = 0x03 & transfer[0x06+header_index];
        switch(format)
        {
            case 0:
                format_desc = "JPEG 422";
                break;
            case 1:
                format_desc = "Raw sensor data";
                break;
            case 2:
                format_desc = "YUV 422";
                break;
            case 3:
                format_desc = "YUV 420";
                break;
            default:
                format_desc = "unknown";
        }
		UDIA_INFO("Frame Format: %s (%2x)\n", format_desc, format);
		header_index = min(buf->buf.length - buf->buf.bytesused,
				      (unsigned int)header_index);
		mem = queue->mem + buf->buf.m.offset + buf->buf.bytesused;
		memcpy(mem, transfer, header_index);
		buf->buf.bytesused += header_index;
		UDIA_INFO("Frame Size: %d\n", buf->buf.bytesused);
		if (buf->buf.bytesused != 0)
			buf->state = MICRODIA_BUF_STATE_DONE;
	} else {
		if (transfer_length + buf->buf.bytesused > buf->buf.length) {
			UDIA_WARNING("Frame Buffer overflow!\n");
			dev->vframes_overflow++;
			buf->state = MICRODIA_BUF_STATE_DONE;
		}
		transfer_length = min(buf->buf.length - buf->buf.bytesused,
				      transfer_length);
		mem = queue->mem + buf->buf.m.offset + buf->buf.bytesused;
		memcpy(mem, transfer, transfer_length);
		buf->buf.bytesused += transfer_length;
	}
	if (buf->state == MICRODIA_BUF_STATE_DONE ||
	    buf->state == MICRODIA_BUF_STATE_ERROR) {
		buf = microdia_queue_next_buffer(queue, buf);
		*buffer = buf;
		if (buf == NULL) {
			dev->vframes_dropped++;
		} else {
			if (header_index + 64 < transfer_length) {
				memcpy(queue->mem + buf->buf.m.offset,
				       transfer + header_index + 64,
				       transfer_length - (header_index + 64));
				buf->buf.bytesused +=
				       transfer_length - (header_index + 64);
			}
		}
	}
}

void usb_microdia_completion_handler(struct urb *urb, struct pt_regs *regs)
{
}


/**
 * @param urb URB structure
 *
 * @brief ISOC handler
 *
 * This function is called as an URB transfert is complete (Isochronous pipe).
 * So, the traitement is done in interrupt time, so it has be fast, not crash,
 * and not stall. Neat.
 */

void usb_microdia_receive_bytes(const char *data, size_t len,
                                struct microdia_video_queue *queue);

void usb_microdia_bulk_completion_handler(struct urb *urb, struct pt_regs *regs)
{
  int ret;
  
  struct usb_microdia *dev = urb->context;
  struct microdia_video_queue *queue = &dev->queue;

  UDIA_STREAM("Isoc handler\n");
  
  switch (urb->status) {
  case 0:
    break;
    
  default:
    UDIA_WARNING("Non-zero status (%d) in video "
                 "completion handler.\n", urb->status);
    
  case -ENOENT:		/* usb_kill_urb() called. */
    if (queue->frozen)
      return;
    
  case -ECONNRESET:	/* usb_unlink_urb() called. */
  case -ESHUTDOWN:	/* The endpoint is being disabled. */
    microdia_queue_cancel(queue);
    return;
  }

  //UDIA_INFO("bulk: len=%d\n", urb->actual_length);

  if (urb->actual_length) {
    usb_microdia_receive_bytes(urb->transfer_buffer, urb->actual_length,
                               queue);
  }
  
  ret = usb_submit_urb(urb, GFP_ATOMIC);
  
  if (ret != 0) {
    UDIA_INFO("Error (%d) re-submitting urb in "
              "microdia_isoc_handler.\n", ret);
  }
}

// WARNING:  this should all be in a per-device struct
//           currently the driver can't cope with more than one device
static int bytes_so_far = 0;
static int bytes_expected = FRAME_SIZE;
static int waiting_for_header = 0;
static int bytes_dropped = 0;
static unsigned char frame_header[] = {0xff, 0xff, 0x00, 0xc4, 0xc4, 0x96};
static unsigned char frame_buf[FRAME_SIZE];

void usb_microdia_receive_frame(struct microdia_video_queue *queue);

void usb_microdia_receive_bytes(const char *data, size_t len,
                                struct microdia_video_queue *queue)
{
    const char *end = data+len;
    while (data < end) {
        if (waiting_for_header >= 0) {
            while (1) {
                if (*data == frame_header[waiting_for_header]) {
                    data++;
                    waiting_for_header++;
                    if (waiting_for_header == sizeof(frame_header)) {
                        bytes_so_far = sizeof(frame_header);
                        memcpy(frame_buf, frame_header, sizeof(frame_header));
                        waiting_for_header = -1;
                        break;
                    }
                }
                else {
                    data++;
                    waiting_for_header = 0;
                    bytes_dropped++;
                }
                if (data == end) return;
            }
        }

        {
            int bytes_to_use = end - data;
            int bytes_to_finish_frame= bytes_expected - bytes_so_far;
            if (bytes_to_use > bytes_to_finish_frame) bytes_to_use = bytes_to_finish_frame;
            memcpy(frame_buf+bytes_so_far, data, bytes_to_use);
            bytes_so_far += bytes_to_use;
            data += bytes_to_use;

            if (bytes_expected == bytes_so_far) {
                // Finish frame
                waiting_for_header = 0;
                //UDIA_INFO("Finish frame, bytes = %d (dropped %d)\n", bytes_expected, bytes_dropped);
                bytes_dropped = 0;
                bytes_so_far = 0;

                usb_microdia_receive_frame(queue);

            }
        }
    }
}

void usb_microdia_receive_frame(struct microdia_video_queue *queue)
{
  struct microdia_buffer *buf = NULL;
  unsigned long flags;
  spin_lock_irqsave(&queue->irqlock, flags);
  if (!list_empty(&queue->irqqueue))
    buf = list_first_entry(&queue->irqqueue, struct microdia_buffer,
                           queue);
  spin_unlock_irqrestore(&queue->irqlock, flags);
  if (buf) {
    int header_size = 64;
    buf->state = MICRODIA_BUF_STATE_ACTIVE;
    memcpy(queue->mem + buf->buf.m.offset,
           frame_buf+FRAME_HEADER_SIZE,
           FRAME_BODY_SIZE);
    buf->buf.bytesused = FRAME_BODY_SIZE;
    buf->state = MICRODIA_BUF_STATE_DONE;
    microdia_queue_next_buffer(queue, buf);
  }
}

/**
 * @param dev Device structure
 *
 * @returns 0 if all is OK
 *
 * @brief Send the message SET_CONFIGURATION
 *
 * This function permits to send the message SET_CONFIGURATION on the USB bus.
 */
int usb_microdia_set_configuration(struct usb_microdia *dev)
{
	int result;
	struct usb_device *udev = dev->udev;

	result = usb_control_msg(udev, usb_sndctrlpipe(udev, 0),
			USB_REQ_SET_CONFIGURATION,
			USB_TYPE_STANDARD | USB_DIR_OUT | USB_RECIP_DEVICE,
			0,
			udev->config[0].desc.bConfigurationValue,
			NULL,
			0,
			500);

	if (result < 0)
		UDIA_ERROR("SET CONFIGURATION fail !\n");
	else
		UDIA_DEBUG("SET CONFIGURATION %d\n", udev->config[0].desc.bConfigurationValue);

	return result;
}


/**
 * @param dev Device structure
 * @param value register to write to
 * @param data
 * @param length number of bytes
 *
 * @returns 0 if all is OK
 *
 * @brief Write a 16-bit value to a 16-bit register
 *
 * This function permits to write a 16-bit value to a 16-bit register on the USB bus.
 */
int usb_microdia_control_write(struct usb_microdia *dev, __u16 value, __u8 *data, __u16 length)
{
	int result;
	struct usb_device *udev = dev->udev;

	result = usb_control_msg(udev, usb_sndctrlpipe(udev, 0),
			0x08,
			USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_INTERFACE,
			value,
			0x00,
			data,
			length,
			500);

	if (result < 0)
		UDIA_ERROR("Write register failed index = 0x%02X\n", value);

	return result;
}

/**
 * @param dev Device structure
 * @param index register to read from
 * @param data
 * @param length number of bytes
 *
 * @returns 0 if all is OK
 *
 * @brief Read a 16-bit value from a 16-bit register
 *
 * This function permits to read a 16-bit value from a 16-bit register on the USB bus.
 */
int usb_microdia_control_read(struct usb_microdia *dev, __u16 index, __u8 *data, __u16 length)
{
	int result;

	struct usb_device *udev = dev->udev;

	*data = 0;

	result = usb_control_msg(udev, usb_rcvctrlpipe(udev, 0),
			0x00,
			USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_INTERFACE,
			index,
			0x00,
			data,
			length,
			500);

	if (result < 0)
		UDIA_ERROR("Read register failed 0x%02X\n", index);

	return result;
}


/**
 * @param dev
 *
 * @returns 0 if all is OK
 *
 * @brief Set the default value about the video settings.
 *
 * This function permits to set the video settings for each video camera model.
 *
 */
static int usb_microdia_default_settings(struct usb_microdia *dev)
{
	struct v4l2_pix_format *def_fmt;

	dev->vframes_overflow = 0;
	dev->vframes_incomplete = 0;
	dev->vframes_dropped = 0;

	dev->queue.min_buffers = min_buffers;
	dev->queue.max_buffers = max_buffers;

        def_fmt = v4l2_enum_supported_formats(dev, 0);

	switch (dev->webcam_model) {
	default:
		dev->vsettings.fps = fps;
		dev->vsettings.vflip = vflip;
		dev->vsettings.hflip = hflip;
		dev->vsettings.brightness = brightness & 0xffff;
		dev->vsettings.contrast = contrast & 0xffff;
		dev->vsettings.whiteness = whiteness & 0xffff;
		dev->vsettings.exposure = exposure & 0xffff;
		dev->vsettings.sharpness = sharpness & 0x3f;
		dev->vsettings.rgb_gain[0] = (rgb_gain >> 16) & 0x7f;
		dev->vsettings.rgb_gain[1] = (rgb_gain >> 8) & 0x7f;
		dev->vsettings.rgb_gain[2] = (rgb_gain >> 8) & 0x7f;
		dev->vsettings.rgb_gain[3] = rgb_gain & 0x7f;
		dev->vsettings.auto_exposure = auto_exposure & 1;
		dev->vsettings.auto_whitebalance = auto_whitebalance & 1;
		dev->vsettings.hue = 0xffff;

		if (def_fmt) {
                    UDIA_INFO("V4L2_PIX_FMT_YUV420 = %x\n",V4L2_PIX_FMT_YUV420);
                    UDIA_INFO("V4L2_PIX_FMT_BGR24 = %x\n",V4L2_PIX_FMT_BGR24);
                    UDIA_INFO("V4L2_PIX_FMT_RGB24 = %x\n",V4L2_PIX_FMT_RGB24);
                    UDIA_INFO("PIX format=%x\n", def_fmt->pixelformat);

                    memcpy(&(dev->vsettings.format), def_fmt, sizeof(struct v4l2_pix_format));
		}

                sn9c20x_set_resolution(dev, 160, 120);
                //sn9c20x_set_resolution(dev, 640, 480);
		break;
	}
	return 0;
}

/**
 * @brief Load the driver
 *
 * @param interface
 * @param id
 *
 * @returns 0 if all is OK
 *
 * This function detects the device and allocate the buffers for the device
 * and the video interface.
 */
static int usb_microdia_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	int ret;

	int vendor_id;
	int product_id;
	int bNumInterfaces;

	struct usb_microdia *dev = NULL;
	struct usb_device *udev = interface_to_usbdev(interface);
	struct microdia_camera *camera = NULL;

	/* Get USB VendorID and ProductID */
	vendor_id = le16_to_cpu(udev->descriptor.idVendor);
	product_id = le16_to_cpu(udev->descriptor.idProduct);

	/* Check if we can handle this device */
	UDIA_DEBUG("Probe function called with VendorID=%04X, ProductID=%04X and InterfaceNumber=%d\n",
			vendor_id, product_id, interface->cur_altsetting->desc.bInterfaceNumber);

	/*
	//The interface are probed one by one.
	// We are interested in the video interface (always the interface '0')
	// The interfaces '1' or '2' (if presents) are the audio control.
	*/
	if (interface->cur_altsetting->desc.bInterfaceNumber > 0) {
		ret = -ENODEV;
		goto error;
	}

	/* Detect device */
	camera = find_camera(CAMERA_MODEL(vendor_id, product_id));
	if (camera == NULL) {
		UDIA_INFO("Camera %04X:%04X not supported.\n",
			  vendor_id, product_id);
		ret = -ENODEV;
		goto error;
	}

	UDIA_INFO("Microdia USB 2.0 Webcam - %04X:%04X plugged-in.\n",
		  vendor_id, product_id);


	/*
	// Allocate structure, initialize pointers, mutexes, etc.
	// and link it to the usb_device
	*/
	dev = kzalloc(sizeof(struct usb_microdia), GFP_KERNEL);

	if (dev == NULL) {
		UDIA_ERROR("Out of memory !\n");
		ret = -ENOMEM;
		goto error;
	}

	/* Init mutexes, spinlock, etc. */
	mutex_init(&dev->mutex);
	kref_init(&dev->vopen);

	/* Save pointers */
	dev->webcam_model = camera->model;
	dev->webcam_type = camera->type;
	if (dev->webcam_model == CAMERA_MODEL(USB_0C45_VID, USB_6260_PID))
		dev->frame_size_divisor = MICRODIA_FRAME_SIZE_DIVISOR_6260;
	else
		dev->frame_size_divisor = MICRODIA_FRAME_SIZE_DIVISOR_DEFAULT;
	dev->initialize = camera->initialize;
	dev->sensor_init = camera->sensor_init;
	dev->stop_stream = camera->stop_stream;
	dev->start_stream = camera->start_stream;
        dev->set_contrast = camera->set_contrast == NULL ? sn9c20x_set_contrast : camera->set_contrast;
        dev->set_brightness = camera->set_brightness == NULL ? sn9c20x_set_brightness : camera->set_brightness;
        dev->set_gamma = camera->set_gamma == NULL ? sn9c20x_set_gamma : camera->set_gamma;
        dev->set_sharpness = camera->set_sharpness == NULL ? sn9c20x_set_sharpness : camera->set_sharpness;
        dev->set_rgb_gain = camera->set_rgb_gain == NULL ? sn9c20x_set_rgb_gain : camera->set_rgb_gain;
        dev->set_exposure = camera->set_exposure == NULL ? sn9c20x_set_exposure : camera->set_exposure;
	if (flip_detect) {
		UDIA_INFO("Rotate detection enabled\n");
		dev->flip_detect = camera->flip_detect;
	}
	dev->set_hvflip = camera->set_hvflip;
	dev->set_auto_exposure = camera->set_auto_exposure;
	dev->set_auto_whitebalance = camera->set_auto_whitebalance;
	dev->udev = udev;
	dev->interface = interface;

	dev->sensor_slave_address = camera->sensor_slave_address;
	dev->sensor_flags = camera->sensor_flags;
	dev->supported_fmts = camera->supported_fmts;

	/* Read the product release  */
	dev->release = le16_to_cpu(udev->descriptor.bcdDevice);
	UDIA_DEBUG("Release: %04x\n", dev->release);

	/* How many interfaces (1 or 3) ? */
	bNumInterfaces = udev->config->desc.bNumInterfaces;
	UDIA_DEBUG("Number of interfaces : %d\n", bNumInterfaces);

	usb_microdia_default_settings(dev);

	/* Initialize the video device */
	dev->vdev = video_device_alloc();

	if (!dev->vdev) {
		ret = -ENOMEM;
		goto free_dev;
	}

	/* Initialize the camera */
	dev_microdia_initialize_device(dev);

	/* Register the video device */
	ret = v4l_microdia_register_video_device(dev);

	if (ret)
		goto free_dev;

	/* Create the entries in the sys filesystem */
	microdia_create_sysfs_files(dev->vdev);

#ifdef CONFIG_MICRODIA_DEBUGFS
	microdia_create_debugfs_files(dev);
#endif

	/* Save our data pointer in this interface device */
	usb_set_intfdata(interface, dev);

	return 0;

free_dev:
	kref_put(&dev->vopen, usb_microdia_delete);
error:
	return ret;
}

void usb_microdia_delete(struct kref *kref)
{
	struct usb_microdia *dev;
	dev = container_of(kref, struct usb_microdia, vopen);

	if (dev->vdev != NULL) {
		microdia_remove_sysfs_files(dev->vdev);
#ifdef CONFIG_MICRODIA_DEBUGFS
		microdia_remove_debugfs_files(dev);
#endif
		v4l_microdia_unregister_video_device(dev);
	}
	kfree(dev);
}

/**
 * @param interface
 *
 * @brief This function is called when the device is disconnected
 *   or when the kernel module is unloaded.
 */
static void usb_microdia_disconnect(struct usb_interface *interface)
{
	struct usb_microdia *dev = usb_get_intfdata(interface);

	UDIA_INFO("Microdia USB 2.0 Webcam unplugged\n");

	usb_set_intfdata(interface, NULL);

	mutex_lock(&open_lock);
	kref_put(&dev->vopen, usb_microdia_delete);
	mutex_unlock(&open_lock);
}


/**
 * @var usb_microdia_driver
 *
 * This variable contains some callback
 */
static struct usb_driver usb_microdia_driver = {
	.name = "usb_microdia_driver",
	.probe = usb_microdia_probe,
	.disconnect = usb_microdia_disconnect,
	.id_table = microdia_table,
};

module_param(fps, int, 0444);			/**< @brief Module parameter frames per second */
module_param(bulk, int, 0444);
module_param(jpeg, int, 0444);
module_param(max_urbs, int, 0444);
module_param(altsetting, int, 0444);
module_param(hflip, int, 0444);			/**< @brief Module parameter horizontal flip process */
module_param(vflip, int, 0444);			/**< @brief Module parameter vertical flip process */
module_param(flip_detect, int, 0444);		/**< @brief Module parameter flip detect */
module_param(auto_exposure, int, 0444);		/**< @brief Module parameter automatic exposure control */
module_param(auto_whitebalance, int, 0444);	/**< @brief Module parameter automatic whitebalance control */
module_param(brightness, int, 0444);		/**< @brief Module parameter brightness */
module_param(whiteness, int, 0444);		/**< @brief Module parameter whiteness */
module_param(contrast, int, 0444);		/**< @brief Module parameter contrast */
module_param(exposure, int, 0444);		/**< @brief Module parameter exposure */
module_param(sharpness, int, 0444);		/**< @brief Module parameter sharpness */
module_param(rgb_gain, int, 0444);		/**< @brief Module parameter red/green/blue gain */

module_param(min_buffers, int, 0444);
module_param(max_buffers, int, 0444);

module_param(log_level, byte, 0444);

/**
 * @returns 0 if all is OK
 *
 * @brief Initialize the driver.
 *
 * This function is called at first.
 * This function permits to define the default values from the command line.
 */
static int __init usb_microdia_init(void)
{
	int result;

	UDIA_INFO("Microdia USB 2.0 webcam driver loaded\n");

#ifdef CONFIG_MICRODIA_DEBUGFS
	microdia_init_debugfs();
#endif

	if (fps < 10 || fps > 30) {
		UDIA_WARNING("Framerate out of bounds [10-30]! Defaulting to 25\n");
		fps = 25;
	}

	if (max_urbs < 10 || max_urbs > 200) {
        UDIA_WARNING("Max URBs out of bounds [10-200]! Defaulting to %d\n", MAX_URBS);
		max_urbs = MAX_URBS;
	}

	if (bulk != 0 && bulk != 1) {
		UDIA_WARNING("Bulk transfer should be 0 or 1! Defaulting to 0\n");
		bulk = 0;
	}

	if (altsetting < 1 || altsetting > 8) {
                UDIA_WARNING("Altsetting out of bounds [1-8]! Defaulting to 8\n");
		altsetting = 8;
	}

	if (jpeg != 0 && jpeg != 1) {
		UDIA_WARNING("JPEG enabled should be 0 or 1! Defaulting to 0\n");
		jpeg = 0;
	}

	if (vflip != 0 && vflip != 1) {
		UDIA_WARNING("Vertical flip should be 0 or 1! Defaulting to 0\n");
		vflip = 0;
	}

	if (hflip != 0 && hflip != 1) {
		UDIA_WARNING("Horizontal flip should be 0 or 1! Defaulting to 0\n");
		hflip = 0;
	}

	if (sharpness < 0 || sharpness > 0x3f) {
		UDIA_WARNING("Sharpness should be 0 to 63 ! Defaulting to 31\n");
		sharpness = 0x1f;
	}

	if ((rgb_gain >> 16) < 0 || (rgb_gain >> 16) > 0x7f) {
		UDIA_WARNING("Red Gain should be 0 to 127 ! Defaulting to 32\n");
		rgb_gain = (rgb_gain & 0x0000ffff) | 0x200000;
	}

	if (((rgb_gain >> 8) & 0xFF) < 0 || ((rgb_gain >> 8) & 0xFF) > 0x7f) {
		UDIA_WARNING("Green Gain should be 0 to 127 ! Defaulting to 32\n");
		rgb_gain = (rgb_gain & 0x00ff00ff) | 0x002000;
	}

	if ((rgb_gain & 0xFF) < 0 || (rgb_gain & 0xFF) > 0x7f) {
		UDIA_WARNING("Blue Gain should be 0 to 127 ! Defaulting to 32\n");
		rgb_gain = (rgb_gain & 0x00ffff00) | 0x20;
	}

	if (auto_exposure != 0 && auto_exposure != 1) {
		UDIA_WARNING("Automatic exposure should be 0 or 1! "
			     "Defaulting to 1\n");
		auto_exposure = 1;
	}

	if (auto_whitebalance != 0 && auto_whitebalance != 1) {
		UDIA_WARNING("Automatic whitebalance should be 0 or 1! "
			     "Defaulting to 1\n");
		auto_whitebalance = 1;
	}

	if (min_buffers < 2) {
		UDIA_WARNING("Minimum buffers can't be less then 2! "
			     "Defaulting to 2\n");
		min_buffers = 2;
	}


	if (min_buffers > max_buffers) {
		UDIA_WARNING("Minimum buffers must be less then or equal to "
                             "max buffers! Defaulting to 2\n");
		min_buffers = 2;
                max_buffers = 2;
	}

	/* Register the driver with the USB subsystem */
	result = usb_register(&usb_microdia_driver);

	if (result)
		UDIA_ERROR("usb_register failed ! Error number %d\n", result);

	UDIA_INFO(DRIVER_VERSION " : " DRIVER_DESC "\n");

	return result;
}


/**
 * @brief Close the driver
 *
 * This function is called at last when you unload the driver.
 */
static void __exit usb_microdia_exit(void)
{
	UDIA_INFO("usb_microdia_exit: Microdia USB 2.0 webcam driver unloaded\n");

#ifdef CONFIG_MICRODIA_DEBUGFS
	microdia_uninit_debugfs();
#endif

	/* Deregister this driver with the USB subsystem */
	usb_deregister(&usb_microdia_driver);
}


module_init(usb_microdia_init);				/**< @brief Module initialize */
module_exit(usb_microdia_exit);				/**< @brief Module exit */


MODULE_PARM_DESC(fps, "Frames per second [10-30]");	/**< @brief Description of 'fps' parameter */
MODULE_PARM_DESC(bulk, "Enable Bulk transfer (default is to use ISOC)");
MODULE_PARM_DESC(jpeg, "Enable JPEG in bridge (default is disabled)");
MODULE_PARM_DESC(max_urbs, "Max number of URBs");
MODULE_PARM_DESC(altsetting, "Which alternate setting on USB video interface");
MODULE_PARM_DESC(hflip, "Horizontal image flip");	/**< @brief Description of 'hflip' parameter */
MODULE_PARM_DESC(vflip, "Vertical image flip");		/**< @brief Description of 'vflip' parameter */
MODULE_PARM_DESC(flip_detect, "Image flip detection");		/**< @brief Description of 'vflip_detect' parameter */
MODULE_PARM_DESC(auto_exposure, "Automatic exposure control");	/**< @brief Description of 'auto_exposure' parameter */
MODULE_PARM_DESC(auto_whitebalance, "Automatic whitebalance");	/**< @brief Description of 'auto_whitebalance' parameter */
MODULE_PARM_DESC(brightness, "Brightness setting");	/**< @brief Description of 'brightness' parameter */
MODULE_PARM_DESC(whiteness, "Whiteness setting");	/**< @brief Description of 'whiteness' parameter */
MODULE_PARM_DESC(exposure, "Exposure setting");		/**< @brief Description of 'exposure' parameter */
MODULE_PARM_DESC(contrast, "Contrast setting");		/**< @brief Description of 'contrast' parameter */
MODULE_PARM_DESC(sharpness, "Sharpness setting");	/**< @brief Description of 'sharpness' parameter */
MODULE_PARM_DESC(rgb_gain, "Red/Green/Blue Gain setting"); /**< @brief Description of 'RGB Gain' parameter */

MODULE_PARM_DESC(min_buffers, "Minimum number of image buffers");
MODULE_PARM_DESC(max_buffers, "Maximum number of image buffers");
MODULE_PARM_DESC(log_level, " <n>\n"
			    "Driver log level\n"
			    "1  = info (default)\n"
			    "2  = warning\n"
			    "4  = error\n"
			    "8  = debug\n"
			    "16 = stream\n");

MODULE_LICENSE("GPL");					/**< @brief Driver is under licence GPL */
MODULE_AUTHOR(DRIVER_AUTHOR);				/**< @brief Driver is written by Nicolas VIVIEN */
MODULE_DESCRIPTION(DRIVER_DESC);			/**< @brief Define the description of the driver */
MODULE_SUPPORTED_DEVICE(DRIVER_SUPPORT);		/**< @brief List of supported device */


