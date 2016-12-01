#
_XDCBUILDCOUNT = 0
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/bios_6_34_02_18/packages;C:/ti/ccsv5/ccs_base;C:/ti/dsplib_c66x_3_4_0_0/packages;D:/BEIFEN/LTE_DL_GJ/.config
override XDCROOT = C:/ti/xdctools_3_23_04_60
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/bios_6_34_02_18/packages;C:/ti/ccsv5/ccs_base;C:/ti/dsplib_c66x_3_4_0_0/packages;D:/BEIFEN/LTE_DL_GJ/.config;C:/ti/xdctools_3_23_04_60/packages;..
HOSTOS = Windows
endif
