include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=kmod-ledtrig-ugreen
PKG_VERSION:=0.3
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/miskcoo/ugreen_leds_controller
PKG_SOURCE_DATE:=2025-03-20
PKG_SOURCE_VERSION:=v$(PKG_VERSION)
PKG_MIRROR_HASH:=ca7ec88e0d9df5b9660fd89455df5b4f7e5eb165048ef97197671fbe102e48e8

include $(INCLUDE_DIR)/package.mk

define KernelPackage/ledtrig-ugreen
  SUBMENU:=LED Modules
  TITLE:=UGREEN NAS Front Panel LED Driver
  FILES:=$(PKG_BUILD_DIR)/kmod/led-ugreen.ko
  AUTOLOAD:=$(call AutoLoad,60,led-ugreen)
  DEPENDS:=+kmod-i2c-core
endef

define KernelPackage/ledtrig-ugreen/description
  Kernel module to control the front panel disk and power LEDs on UGREEN DX/DXP series NAS devices.
endef

MAKE_OPTS:= \
	ARCH="$(LINUX_KARCH)" \
	CROSS_COMPILE="$(TARGET_CROSS)" \
	M="$(PKG_BUILD_DIR)/kmod"

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" $(MAKE_OPTS) modules
endef

$(eval $(call KernelPackage,ledtrig-ugreen))
