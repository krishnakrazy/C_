#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

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
	{ 0x6eac0e5c, "module_layout" },
	{ 0x37a0cba, "kfree" },
	{ 0x6eaaf525, "nf_unregister_net_hook" },
	{ 0xc5850110, "printk" },
	{ 0x462deff2, "nf_register_net_hook" },
	{ 0x2e969bb1, "init_net" },
	{ 0xc4d15c59, "kmem_cache_alloc_trace" },
	{ 0x681207f1, "kmalloc_caches" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "946B6C1005AF84BF8807022");
