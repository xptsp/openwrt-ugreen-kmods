include $(TOPDIR)/rules.mk

PKG_NAME:=luci-app-hdd-led-monitor
PKG_VERSION:=1.0.0
PKG_RELEASE:=1

PKG_MAINTAINER:=OpenWrt Developer <dev@openwrt.org>
PKG_LICENSE:=Apache-2.0

include $(INCLUDE_DIR)/package.mk

define Package/luci-app-hdd-led-monitor
  SECTION:=luci
  CATEGORY:=LuCI
  SUBMENU:=3. Applications
  TITLE:=LuCI Support for Multi-instance HDD LED Monitor
  DEPENDS:=+luci-base +hdd-led-monitor
  PKGARCH:=all
endef

define Build/Compile
	# Nothing to compile since this consists entirely of static web files
endef

define Package/luci-app-hdd-led-monitor/install
	$(CP) ./root/* $(1)/
endef

$(eval $(call BuildPackage,luci-app-hdd-led-monitor))
