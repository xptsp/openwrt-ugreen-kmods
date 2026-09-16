include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=led-ugreen
PKG_VERSION:=0.1
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/miskcoo/ugreen_leds_controller.git
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz

include $(INCLUDE_DIR)/package.mk

define KernelPackage/led-ugreen
  SUBMENU:=LED modules
  TITLE:=Kernel module for UGREEN NAS front panel LEDs
  DEPENDS:=+kmod-i2c-i801 +kmod-leds-gpio
  FILES:=$(PKG_BUILD_DIR)/kmod/led-ugreen.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,60,led-ugreen)
endef

define Build/Compile
	$(KERNEL_MAKE) M="$(PKG_BUILD_DIR)/kmod" modules
endef

$(eval $(call KernelPackage,led-ugreen))
