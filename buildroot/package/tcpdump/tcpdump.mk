################################################################################
#
# tcpdump
#
################################################################################

TCPDUMP_VERSION = dbd70580fa548c39813c3e1b45455a3ac4acf25a
TCPDUMP_SITE = $(call github,the-tcpdump-group,tcpdump,$(TCPDUMP_VERSION))
TCPDUMP_LICENSE = BSD-3-Clause
TCPDUMP_LICENSE_FILES = LICENSE
TCPDUMP_CONF_ENV = \
	ac_cv_linux_vers=2 \
	td_cv_buggygetaddrinfo=no \
	PCAP_CONFIG=$(STAGING_DIR)/usr/bin/pcap-config
TCPDUMP_CONF_OPTS = \
	--without-crypto \
	--with-system-libpcap \
	$(if $(BR2_PACKAGE_TCPDUMP_SMB),--enable-smb,--disable-smb)
TCPDUMP_DEPENDENCIES = libpcap

# 0001-PPP-When-un-escaping-don-t-allocate-a-too-large-buffer.patch
TCPDUMP_IGNORE_CVES += CVE-2020-8037

ifeq ($(BR2_STATIC_LIBS),y)
TCPDUMP_CONF_OPTS += LIBS="`$(STAGING_DIR)/usr/bin/pcap-config --static --additional-libs`"
endif

# make install installs an unneeded extra copy of the tcpdump binary
define TCPDUMP_REMOVE_DUPLICATED_BINARY
	rm -f $(TARGET_DIR)/usr/sbin/tcpdump.$(TCPDUMP_VERSION)
endef

TCPDUMP_POST_INSTALL_TARGET_HOOKS += TCPDUMP_REMOVE_DUPLICATED_BINARY

$(eval $(autotools-package))
