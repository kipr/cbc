#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
};

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=videodev";

MODULE_ALIAS("usb:v0C45p6240d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p6242d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p6243d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p6248d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p624Bd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p624Cd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p624Ed*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p624Fd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p6253d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p6260d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p6262d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p6270d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p627Ad*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p627Bd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p627Cd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p627Fd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p6280d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p6282d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p6283d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p6288d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p628Ad*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p628Bd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p628Cd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p628Ed*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p628Fd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p62A0d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p62A2d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p62B0d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p62B3d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p62BAd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p62BBd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p62BCd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0C45p62BEd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v045Ep00F4d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v145Fp013Dd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v04F2pA128d*dc*dsc*dp*ic*isc*ip*");
