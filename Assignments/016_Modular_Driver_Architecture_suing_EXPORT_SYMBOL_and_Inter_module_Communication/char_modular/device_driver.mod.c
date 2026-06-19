#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xb6c08e4c, "class_create" },
	{ 0xf350d701, "device_create" },
	{ 0xe8213e80, "_printk" },
	{ 0x2f1de159, "core_version" },
	{ 0x546c19d9, "validate_usercopy_range" },
	{ 0xa61fd7aa, "__check_object_size" },
	{ 0x092a35a2, "_copy_from_user" },
	{ 0x51905738, "core_log_message" },
	{ 0x90a48d82, "__ubsan_handle_out_of_bounds" },
	{ 0x30a11079, "device_destroy" },
	{ 0xfd4b4a36, "class_destroy" },
	{ 0x0c92f06e, "cdev_del" },
	{ 0x0bc5fb0d, "unregister_chrdev_region" },
	{ 0xd272d446, "__fentry__" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0x9f222e1e, "alloc_chrdev_region" },
	{ 0x4c075f7d, "cdev_init" },
	{ 0x6459621a, "cdev_add" },
	{ 0x814e12e5, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0xb6c08e4c,
	0xf350d701,
	0xe8213e80,
	0x2f1de159,
	0x546c19d9,
	0xa61fd7aa,
	0x092a35a2,
	0x51905738,
	0x90a48d82,
	0x30a11079,
	0xfd4b4a36,
	0x0c92f06e,
	0x0bc5fb0d,
	0xd272d446,
	0xd272d446,
	0x9f222e1e,
	0x4c075f7d,
	0x6459621a,
	0x814e12e5,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"class_create\0"
	"device_create\0"
	"_printk\0"
	"core_version\0"
	"validate_usercopy_range\0"
	"__check_object_size\0"
	"_copy_from_user\0"
	"core_log_message\0"
	"__ubsan_handle_out_of_bounds\0"
	"device_destroy\0"
	"class_destroy\0"
	"cdev_del\0"
	"unregister_chrdev_region\0"
	"__fentry__\0"
	"__x86_return_thunk\0"
	"alloc_chrdev_region\0"
	"cdev_init\0"
	"cdev_add\0"
	"module_layout\0"
;

MODULE_INFO(depends, "core_driver");


MODULE_INFO(srcversion, "339D10629332F62B2B826B0");
