/**
 * @file microdia.h
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

#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/usb.h>
#include <linux/videodev.h>

#ifndef MICRODIA_H
#define MICRODIA_H

#define list_first_entry(ptr, type, member) \
        list_entry((ptr)->next, type, member)

#define DRIVER_NAME		"microdia"			/**< Name of this driver */
#define DRIVER_VERSION		"v2008.10"			/**< Version of this driver */
#define DRIVER_VERSION_NUM	0x000000			/**< Version numerical of this driver */
#define DRIVER_DESC		"Microdia USB 2.0 Webcam Driver"/**< Short description of this driver */
#define DRIVER_AUTHOR		"Nicolas VIVIEN"		/**< Author of this driver */
#define PREFIX			DRIVER_NAME ": "		/**< Prefix use for the MICRODIA "printk" */

#define USB_0C45_VID		0x0c45	/**< Vendor ID of MICRODIA */
#define USB_045E_VID		0x045e	/**< Vendor ID of MICROSOFT */
#define USB_04F2_VID		0x04f2	/**< Vendor ID of CHICONY */
#define USB_145F_VID		0x145f	/**< Vendor ID of TRUST */

/* SN9C201: */
#define USB_6240_PID	0x6240			/**< SN9C201 + MI1300 */
#define USB_6242_PID	0x6242			/**< SN9C201 + MI1310 */
#define USB_6243_PID	0x6243			/**< SN9C201 + S5K4AAFX */
#define USB_6248_PID	0x6248			/**< SN9C201 + OV9655 */
#define USB_624B_PID	0x624b			/**< SN9C201 + CX1332 */
#define USB_624C_PID	0x624c			/**< SN9C201 + MI1320 */
#define USB_624E_PID	0x624e			/**< SN9C201 + SOI968 */
#define USB_624F_PID	0x624f			/**< SN9C201 + OV9650 */
#define USB_6253_PID	0x6253			/**< SN9C201 + OV9650 */
#define USB_6260_PID	0x6260			/**< SN9C201 + OV7670ISP */
#define USB_6262_PID	0x6262			/**< SN9C201 + OM6802 */
#define USB_6270_PID	0x6270			/**< SN9C201 + MI0360/MT9V111 */
#define USB_627A_PID	0x627a			/**< SN9C201 + S5K53BEB */
#define USB_627B_PID	0x627b			/**< SN9C201 + OV7660 */
#define USB_627C_PID	0x627c			/**< SN9C201 + HV7131R */
#define USB_627F_PID	0x627f			/**< EEPROM */
/* SN9C202:*/
#define USB_6280_PID	0x6280			/**< SN9C202 + MI1300 */
#define USB_6282_PID	0x6282			/**< SN9C202 + MI1310 */
#define USB_6283_PID	0x6283			/**< SN9C202 + S5K4AAFX */
#define USB_6288_PID	0x6288			/**< SN9C202 + OV9655 */
#define USB_628A_PID	0x628a			/**< SN9C202 + ICM107 */
#define USB_628B_PID	0x628b			/**< SN9C202 + CX1332 */
#define USB_628C_PID	0x628c			/**< SN9C202 + MI1320 */
#define USB_628E_PID	0x628e			/**< SN9C202 + SOI968 */
#define USB_628F_PID	0x628f			/**< SN9C202 + OV9650 */
#define USB_62A0_PID	0x62a0			/**< SN9C202 + OV7670ISP */
#define USB_62A2_PID	0x62a2			/**< SN9C202 + OM6802 */
#define USB_62B0_PID	0x62b0			/**< SN9C202 + MI0360/MT9V111 */
#define USB_62B3_PID	0x62b3			/**< SN9C202 + OV9655 */
#define USB_62BA_PID	0x62ba			/**< SN9C202 + S5K53BEB */
#define USB_62BB_PID	0x62bb			/**< SN9C202 + OV7660 */
#define USB_62BC_PID	0x62bc			/**< SN9C202 + HV7131R */
#define USB_62BE_PID	0x62be			/**< SN9C202 + OV7663 */
/* SN9C325: */
#define USB_6128_PID	0x6128			/**< SN9C325 + OM6802 */
#define USB_612A_PID	0x612a			/**< SN9C325 + OV7648 + POx1030xC + SOI768 + PO2030N + OV7660 + OV7670 + HV7131R */
/* *** "Clone"-devices ***: */

/* Microsoft LifeCam VX-6000: */
#define USB_00F4_PID	0x00f4			/**< => 628f (SN9C202 + OV9650) */
/*  Trust WB-3600R: */
#define USB_013D_PID	0x013d			/**< => 627b (SN9C201 + OV7660) */
/*  Chicony Panda 7, Trust WB-3450P: */
#define USB_A128_PID	0xa128			/**< => 62be (SN9C202 + OV7663) */

#define CAMERA_MODEL(vid, pid) ((vid << 16) | pid)
#define CAMERA_VID(model) ((model >> 16) & 0xFFFF)
#define CAMERA_PID(model) (model & 0xFFFF)

/**
 * @def VID_HARDWARE_MICRODIA
 *
 * This value must be inserted into the kernel headers linux/videodev.h
 * It's useful only for the support of V4L v1
 */
#define VID_HARDWARE_MICRODIA		88
/* FIXME: Can the above be removed ? */

/**
 * @def MAX_URBS
 *   Number maximal of URBS
 *
 * @def ISO_FRAMES_PER_DESC
 *   Number frames per ISOC descriptor
 */
#define MAX_URBS				200
#define ISO_FRAMES_PER_DESC			20

/**
 * @def hb_multiplier(wMaxPacketSize)
 *   USB endpoint high bandwidth multiplier
 *
 * @def max_packet_sz(wMaxPacketSize)
 *   USB endpoint maximum packet size
 *
 * These values are both encoded within the wMaxPacketSize field of the usb_endpoint_descriptor structure.
 * The 11(10:0) lowest bits hold the MaxPacketSize(according to the usb specs a value over 1024 is undefined),
 * and the bits 12:11 will give the high bandwidth multiplier(this should be a value of 0-2).
 */

#define hb_multiplier(wMaxPacketSize) (((wMaxPacketSize >> 11) & 0x03) + 1)
#define max_packet_sz(wMaxPacketSize) ((wMaxPacketSize) & 0x07ff)

/**
 * @def MICRODIA_FRAME_SIZE
 *   Maximum size after decompression
 */
#define MICRODIA_FRAME_SIZE	(1280 * 1024 * 3)

/**
 * @def MICRODIA_FRAME_SIZE_DIVISOR_DEFAULT
 * 	Most models have a Frame size of image_size + image_size/2
 *
 * @def MICRODIA_FRAME_SIZE_DIVISOR_6260
 * 	The 6260 has a Frame size of image_size + image_size
 */
#define MICRODIA_FRAME_SIZE_DIVISOR_DEFAULT	2
#define MICRODIA_FRAME_SIZE_DIVISOR_6260	1

/**
 * @def DRIVER_SUPPORT
 *
 * List of supported devices
 */
#define DRIVER_SUPPORT				"Microdia USB Camera"

#define MICRODIA_INFO		(1 << 0)
#define MICRODIA_WARNING	(1 << 1)
#define MICRODIA_ERROR		(1 << 2)
#define MICRODIA_DEBUG		(1 << 3)
#define MICRODIA_STREAM		(1 << 4)

/**
 * @def UDIA_INFO(str, args...)
 *   Print information message.
 *   @a Use this function like the function printf.
 *
 * @def UDIA_ERROR(str, args...)
 *   Print error message.
 *   @a Use this function like the function printf.
 *
 * @def UDIA_WARNING(str, args...)
 *   Print warning message.
 *   @a Use this function like the function printf.
 *
 * @def UDIA_DEBUG(str, args...)
 *   Print debug message.
 *   @a Use this function like the function printf.
 *
 * @def UDIA_STREAM(str, args...)
 *   Print stream debug message.
 *   @a Use this function like the function printf.
 */
extern __u8 log_level;

#define UDIA_INFO(str, args...)	\
do { \
	if (log_level & MICRODIA_INFO) \
		printk(KERN_INFO PREFIX str, ##args); \
} while (0)

#define UDIA_WARNING(str, args...) \
do { \
	if (log_level & MICRODIA_WARNING) \
		printk(KERN_WARNING PREFIX str, ##args); \
} while (0)

#define UDIA_ERROR(str, args...) \
do { \
	if (log_level & MICRODIA_ERROR) \
		printk(KERN_ERR PREFIX str, ##args); \
} while (0)

#define UDIA_DEBUG(str, args...) \
do { \
	if (log_level & MICRODIA_DEBUG) \
		printk(KERN_DEBUG PREFIX str, ##args); \
} while (0)

#define UDIA_STREAM(str, args...) \
do { \
	if (log_level & MICRODIA_STREAM) \
		printk(KERN_DEBUG PREFIX str, ##args); \
} while (0)

/**
 * @enum T_MICRODIA_VIDEOMODE Video feature supported by camera
 */
typedef enum {
	MICRODIA_VGA,						/**< For VGA video camera */
	MICRODIA_SXGA,						/**< For SXGA video camera 1.3M */
	MICRODIA_UXGA						/**< For UXGA video camera 2M */
} T_MICRODIA_VIDEOMODE;

enum microdia_buffer_state {
	MICRODIA_BUF_STATE_IDLE       = 0,
	MICRODIA_BUF_STATE_QUEUED     = 1,
	MICRODIA_BUF_STATE_ACTIVE     = 2,
	MICRODIA_BUF_STATE_DONE       = 3,
	MICRODIA_BUF_STATE_ERROR      = 4,
};

enum microdia_mode {
	MICRODIA_MODE_IDLE       = 0,
	MICRODIA_MODE_READ       = 1,
	MICRODIA_MODE_STREAM     = 2,
};

struct microdia_buffer {
	unsigned long vma_use_count;
	struct list_head stream;

	/* Touched by interrupt handler. */
	struct v4l2_buffer buf;
	struct list_head queue;
	wait_queue_head_t wait;
	enum microdia_buffer_state state;
};

struct microdia_video_queue {
	void *mem;
	void *scratch;
	unsigned int streaming:1,
		     frozen:1,
		     drop_incomplete:1;
	__u32 sequence;

	unsigned int count;
	unsigned int min_buffers;
	unsigned int max_buffers;
	unsigned int buf_size;
	unsigned int frame_size; /*current frame size */
	struct microdia_buffer *buffer;
	struct microdia_buffer *read_buffer;
	struct mutex mutex;	/* protects buffers and mainqueue */
	spinlock_t irqlock;	/* protects irqqueue */

	struct list_head mainqueue;
	struct list_head irqqueue;
};

/**
 * @struct microdia_urb
 */
struct microdia_urb {
	void *data;
	struct urb *urb;
};

/**
 * @struct microdia_video
 */
struct microdia_video {
	struct v4l2_pix_format format;
	int fps;				/**< FPS setting */
	int brightness;				/**< Brightness setting */
	int contrast;				/**< Contrast setting */
	int whiteness;				/**< Whiteness setting */
	int colour;				/**< Colour setting */
	int hue;				/**< Hue setting */
	int hflip;				/**< Horizontal flip */
	int vflip;				/**< Vertical flip */
	int exposure;				/**< Exposure */
	int sharpness;				/**< Sharpness */
	__u8 rgb_gain[4];			/**< RGB Gain (RGGB) */
	int auto_exposure;			/**< Automatic exposure */
	int auto_whitebalance;			/**< Automatic whitebalance */
};

/**
 * @struct microdia_debugfs
 */
struct microdia_debugfs {
	struct dentry *dent_device;
	struct dentry *dent_bridge_addr;
	struct dentry *dent_bridge_val;
	struct dentry *dent_bridge_dump;
	struct dentry *dent_sensor_addr;
	struct dentry *dent_sensor_val8;
	struct dentry *dent_sensor_val16;
	struct dentry *dent_sensor_val32;

	__u16 bridge_addr; /**< Current bridge register address */
	__u8 sensor_addr; /**< Current sensor register address */
};

/**
 * @struct usb_microdia
 */
struct usb_microdia {
	struct video_device *vdev; 			/**< Pointer on a V4L2 video device */
	struct usb_device *udev;			/**< Pointer on a USB device */
	struct usb_interface *interface;	/**< Pointer on a USB interface */

	int release;						/**< Release of the device (bcdDevice) */
	__u32 webcam_model;					/**< Model of video camera device */
	__u8 webcam_type;					/**< Type of camera : VGA, SXGA (1.3M), UXGA (2M) */

	int frame_size_divisor;			/**> Used to calculate frame_size	 */

	__u8 sensor_slave_address;		/**< I2C/SCCB slave address of the image sensor */
	__u8 sensor_flags;			/**< I2C/SCCB flags of the image sensor */

	struct microdia_video vsettings;		/**< Video settings (brightness, whiteness...) */
	struct microdia_debugfs debug; /**< debugfs information structure */

	struct kref vopen;	/**< Video status (Opened or Closed) */
	struct file *owner;	/**< file handler of stream owner */
	enum microdia_mode mode;/**< camera mode */

	int vframes_overflow;	/**< Buffer overflow frames */
	int vframes_incomplete;	/**< Incomplete frames */
	int vframes_dropped;	/**< Dropped frames */

	struct mutex mutex;				/**< Mutex */

	struct microdia_urb urbs[MAX_URBS];

	int resolution;

	struct microdia_video_queue queue;

	__u8 supported_fmts;

	int (*initialize) (struct usb_microdia *dev);
	int (*sensor_init) (struct usb_microdia *dev);
	int (*start_stream) (struct usb_microdia *dev);
	int (*stop_stream) (struct usb_microdia *dev);
	int (*set_contrast) (struct usb_microdia *dev);
	int (*set_brightness) (struct usb_microdia *dev);
	int (*set_gamma) (struct usb_microdia *dev);
	int (*set_exposure) (struct usb_microdia *dev);
	int (*flip_detect) (struct usb_microdia *dev);
	int (*set_hvflip) (struct usb_microdia *dev);
	int (*set_sharpness) (struct usb_microdia *dev);
	int (*set_rgb_gain) (struct usb_microdia *dev);
	int (*set_auto_exposure) (struct usb_microdia *dev);
	int (*set_auto_whitebalance) (struct usb_microdia *dev);
};

struct microdia_camera {

	__u32 model;	/**< Model of video camera device */
	__u8  type;	/**< Type of camera : VGA, SXGA, UXGA */
	__u8  supported_fmts;
	__u8 sensor_slave_address;
	__u8 sensor_flags;

	int (*initialize) (struct usb_microdia *dev);
	int (*sensor_init) (struct usb_microdia *dev);
	int (*start_stream) (struct usb_microdia *dev);
	int (*stop_stream) (struct usb_microdia *dev);
	int (*set_contrast) (struct usb_microdia *dev);
	int (*set_brightness) (struct usb_microdia *dev);
	int (*set_gamma) (struct usb_microdia *dev);
	int (*set_exposure) (struct usb_microdia *dev);
	int (*flip_detect) (struct usb_microdia *dev);
	int (*set_hvflip) (struct usb_microdia *dev);
	int (*set_sharpness) (struct usb_microdia *dev);
	int (*set_rgb_gain) (struct usb_microdia *dev);
	int (*set_auto_exposure) (struct usb_microdia *dev);
	int (*set_auto_whitebalance) (struct usb_microdia *dev);
};

/**
 * @def MICRODIA_PERCENT
 *   Calculate a value from a percent
 */
#define MICRODIA_PERCENT(x, y) (((int)x * (int)y) / 100)

enum microdia_endpoints {
	MICRODIA_VID_ISOC = 1,
	MICRODIA_BULK = 2,
	MICRODIA_INT = 3,
	MICRODIA_AUD_ISOC = 4
};

/**
 * @def to_microdia_dev(d)
 * Cast a member of a structure out to the containing structure
 */
#define to_microdia_dev(d) container_of(d, struct usb_microdia, kref)

extern struct mutex open_lock;

int usb_microdia_control_write(struct usb_microdia *, __u16, __u8 *, __u16);
int usb_microdia_control_read(struct usb_microdia *, __u16, __u8 *, __u16);
int usb_microdia_isoc_init(struct usb_microdia *,
	struct usb_endpoint_descriptor *);
void usb_microdia_completion_handler(struct urb *, struct pt_regs *);
int usb_microdia_init_urbs(struct usb_microdia *);
void usb_microdia_uninit_urbs(struct usb_microdia *);
void usb_microdia_isoc_cleanup(struct usb_microdia *);
void usb_microdia_delete(struct kref *);


int dev_microdia_initialize_device(struct usb_microdia *);
int dev_microdia_start_stream(struct usb_microdia *);
int dev_microdia_stop_stream(struct usb_microdia *);
int dev_microdia_check_device(struct usb_microdia *, int);
int dev_microdia_camera_asleep(struct usb_microdia *);
int dev_microdia_init_camera(struct usb_microdia *);
int dev_microdia_reconf_camera(struct usb_microdia *);
int dev_microdia_camera_settings(struct usb_microdia *);
int dev_microdia_camera_set_contrast(struct usb_microdia *);
int dev_microdia_camera_set_brightness(struct usb_microdia *);
int dev_microdia_camera_set_gamma(struct usb_microdia *);
int dev_microdia_camera_set_exposure(struct usb_microdia *);
int dev_microdia_camera_set_hvflip(struct usb_microdia *);
int dev_microdia_camera_set_sharpness(struct usb_microdia *dev);
int dev_microdia_camera_set_rgb_gain(struct usb_microdia *dev);
int dev_microdia_camera_set_auto_exposure(struct usb_microdia *dev);
int dev_microdia_camera_set_auto_whitebalance(struct usb_microdia *dev);
/* int dev_microdia_set_camera_quality(struct usb_microdia *);*/
int dev_microdia_set_camera_fps(struct usb_microdia *);

struct v4l2_pix_format *v4l2_enum_supported_formats(struct usb_microdia *,
	__u8);
int v4l_microdia_select_video_mode(struct usb_microdia *, int);
int v4l_microdia_register_video_device(struct usb_microdia *);
int v4l_microdia_unregister_video_device(struct usb_microdia *);

int microdia_create_sysfs_files(struct video_device *);
void microdia_remove_sysfs_files(struct video_device *);

void microdia_init_debugfs(void);
void microdia_uninit_debugfs(void);
int microdia_create_debugfs_files(struct usb_microdia *);
int microdia_remove_debugfs_files(struct usb_microdia *);

void microdia_queue_init(struct microdia_video_queue *);
int microdia_alloc_buffers(struct microdia_video_queue *,
	unsigned int, unsigned int);
int microdia_free_buffers(struct microdia_video_queue *);
int microdia_queue_enable(struct microdia_video_queue *, int);
void microdia_queue_cancel(struct microdia_video_queue *);
unsigned int microdia_queue_poll(struct microdia_video_queue *,
	struct file *, poll_table *);
int microdia_query_buffer(struct microdia_video_queue *, struct v4l2_buffer *);
int microdia_queue_buffer(struct microdia_video_queue *, struct v4l2_buffer *);
int microdia_dequeue_buffer(struct microdia_video_queue *,
	struct v4l2_buffer *, int);
struct microdia_buffer *microdia_queue_next_buffer(
	struct microdia_video_queue *, struct microdia_buffer *);

int microdia_decompress(struct usb_microdia *, struct v4l2_buffer *);
#endif
