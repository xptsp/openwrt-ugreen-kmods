include $(TOPDIR)/rules.mk

PKG_NAME:=hdd-led-monitor
PKG_VERSION:=1.0.0
PKG_RELEASE:=1

PKG_MAINTAINER:=OpenWrt Developer <dev@openwrt.org>
PKG_LICENSE:=GPL-2.0-or-later

include $(INCLUDE_DIR)/package.mk

define Package/hdd-led-monitor
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Multi-instance Hex/SMART HDD LED Flasher
  DEPENDS:=+libstdcpp +smartmontools
endef

define Package/hdd-led-monitor/description
  A multi-instance C++ background daemon that flashes target LEDs via sysfs on hard drive access.
  Integrates smartctl to switch the idle baseline color if a drive health assessment flags failures.
endef

# Setup build commands to compile the C++ source file
define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/hdd_led_monitor.cpp $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(TARGET_CXX) $(TARGET_CXXFLAGS) $(TARGET_CPPFLAGS) \
		-O3 -std=c++11 $(PKG_BUILD_DIR)/hdd_led_monitor.cpp \
		-o $(PKG_BUILD_DIR)/hdd_led_monitor $(TARGET_LDFLAGS) -lpthread
endef

# Install built binaries and system configurations directly to target image footprint
define Package/hdd-led-monitor/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/hdd_led_monitor $(1)/usr/bin/

	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_CONF) ./files/hdd_led.config $(1)/etc/config/hdd_led

	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/hdd_led.init $(1)/etc/init.d/hdd_led
endef

$(eval $(call BuildPackage,hdd-led-monitor))
