#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
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

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xdc658e53, "module_layout" },
	{ 0x37a0cba, "kfree" },
	{ 0xbfeffc40, "device_destroy" },
	{ 0x5f8ff9eb, "kmem_cache_alloc_trace" },
	{ 0xc1f15dee, "kmalloc_caches" },
	{ 0x6f021618, "class_destroy" },
	{ 0xef4c61eb, "device_create" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0xe05148c6, "__class_create" },
	{ 0x741ad6e3, "__register_chrdev" },
	{ 0x7261cd16, "remap_pfn_range" },
	{ 0x7cd8d75e, "page_offset_base" },
	{ 0x4c9d28b0, "phys_base" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x92997ed8, "_printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "EC4F6C0D95DB2A644341CAA");
