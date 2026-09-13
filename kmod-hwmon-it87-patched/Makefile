include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=hwmon-it87-patched
PKG_VERSION:=2024-09-22
PKG_RELEASE:=1

# Pulls from shauno8's IT8613E-patched driver repository
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/shauno8/it87.git
PKG_SOURCE_VERSION:=81ae4a610ea763fa7f8cd195e712c41f33207264
PKG_MIRROR_HASH:=skip

include $(INCLUDE_DIR)/package.mk

define KernelPackage/hwmon-it87-patched
  SUBMENU:=Hardware Monitoring Support
  TITLE:=ITE IT87xx driver (Shauno8 IT8613E Patch)
  FILES:=$(PKG_BUILD_DIR)/it87.ko
  AUTOLOAD:=$(call AutoLoad,60,it87)
  DEPENDS:=@TARGET_x86_64 +kmod-hwmon-core
endef

define KernelPackage/hwmon-it87-patched/description
  Kernel module for ITE IT87xx Super I/O chips. 
  This specific variant includes the Shauno8 patches enabling support for 
  the ITE IT8613E found on platforms like the UGREEN DXP2800 NAS.
endef

# Overriding the build prepare to replace upstream desktop Makefile
define Build/Prepare
	$(call Build/Prepare/Default)
	echo "obj-m += it87.o" > $(PKG_BUILD_DIR)/Makefile
endef

define Build/Compile
	$(KERNEL_MAKE) M="$(PKG_BUILD_DIR)" modules
endef

$(eval $(call KernelPackage,hwmon-it87-patched))
