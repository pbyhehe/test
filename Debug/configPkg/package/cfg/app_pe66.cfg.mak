# invoke SourceDir generated makefile for app.pe66
app.pe66: .libraries,app.pe66
.libraries,app.pe66: package/cfg/app_pe66.xdl
	$(MAKE) -f D:\BEIFEN\LTE_DL_GJ/src/makefile.libs

clean::
	$(MAKE) -f D:\BEIFEN\LTE_DL_GJ/src/makefile.libs clean

