include $(TOPDIR)/rules.mk

PKG_NAME:=diskmon
PKG_RELEASE:=1
PKG_VERSION:=1.0.0

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/diskmon
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Disk activity LED monitor
  DEPENDS:=+libstdcpp
endef

define Package/diskmon/description
  A lightweight C++ utility to flash specific LEDs based on /proc/diskstats activity.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(TARGET_CXX) $(TARGET_CXXFLAGS) $(TARGET_LDFLAGS) \
		$(PKG_BUILD_DIR)/diskmon.cpp -o $(PKG_BUILD_DIR)/diskmon
endef

define Package/diskmon/install
	# Install the binary executable
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/diskmon $(1)/usr/bin/

	# Install the UCI configuration file
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_CONF) ./files/diskmon.config $(1)/etc/config/diskmon

	# Install the init.d script
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/diskmon.init $(1)/etc/init.d/diskmon
endef

$(eval $(call BuildPackage,diskmon))
