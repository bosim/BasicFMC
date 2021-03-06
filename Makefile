# Makefile for BasicFMC (based on ExampleGauge code from Sandy Barbour 

BUILDDIR	:=	./build
SRC_BASE	:=	./src
SDK_BASE	:=	./SDK
TARGET		:= BasicFMC

SOURCES = \
        Bitmap.cpp \
	Page.cpp \
	Page_Init.cpp \
	Page_Legs.cpp \
	Page_Airport.cpp \
	Page_Progress.cpp \
	Page_Route.cpp \
	Page_Navaid.cpp \
	Pages.cpp \
	InputHandler.cpp \
	Utils.cpp \
	ProcedureReader.cpp \
	FreeNavProcedureReader.cpp \
	GNSProcedureReader.cpp \
	Main.cpp

LIBS = 

INCLUDES = \
	-I$(SDK_BASE)/CHeaders/XPLM \
	-I$(SDK_BASE)/CHeaders/Widgets

DEFINES = -DAPL=0 -DIBM=0 -DLIN=1 -DXPLM200 -ggdb

VPATH = $(SRC_BASE)
CSOURCES	:= $(filter %.c, $(SOURCES))
CXXSOURCES	:= $(filter %.cpp, $(SOURCES))

CDEPS		:= $(patsubst %.c, $(BUILDDIR)/obj32/%.cdep, $(CSOURCES))
CXXDEPS		:= $(patsubst %.cpp, $(BUILDDIR)/obj32/%.cppdep, $(CXXSOURCES))
COBJECTS	:= $(patsubst %.c, $(BUILDDIR)/obj32/%.o, $(CSOURCES))
CXXOBJECTS	:= $(patsubst %.cpp, $(BUILDDIR)/obj32/%.o, $(CXXSOURCES))
ALL_DEPS	:= $(sort $(CDEPS) $(CXXDEPS))
ALL_OBJECTS	:= $(sort $(COBJECTS) $(CXXOBJECTS))

CDEPS64			:= $(patsubst %.c, $(BUILDDIR)/obj64/%.cdep, $(CSOURCES))
CXXDEPS64		:= $(patsubst %.cpp, $(BUILDDIR)/obj64/%.cppdep, $(CXXSOURCES))
COBJECTS64		:= $(patsubst %.c, $(BUILDDIR)/obj64/%.o, $(CSOURCES))
CXXOBJECTS64	:= $(patsubst %.cpp, $(BUILDDIR)/obj64/%.o, $(CXXSOURCES))
ALL_DEPS64		:= $(sort $(CDEPS64) $(CXXDEPS64))
ALL_OBJECTS64	:= $(sort $(COBJECTS64) $(CXXOBJECTS64))

CFLAGS := $(DEFINES) $(INCLUDES) -fPIC -fvisibility=hidden -Wall

.PHONY: all clean $(TARGET)
.SECONDARY: $(ALL_OBJECTS) $(ALL_OBJECTS64) $(ALL_DEPS)

$(TARGET): $(BUILDDIR)/$(TARGET)/32/lin.xpl $(BUILDDIR)/$(TARGET)/64/lin.xpl

$(BUILDDIR)/$(TARGET)/64/lin.xpl: $(ALL_OBJECTS64)
	@echo Linking $@
	mkdir -p $(dir $@)
	gcc -m64 -static-libgcc -shared -Wl,--version-script=exports.txt -o $@ $(ALL_OBJECTS64) $(LIBS)

$(BUILDDIR)/$(TARGET)/32/lin.xpl: $(ALL_OBJECTS)
	@echo Linking $@
	mkdir -p $(dir $@)
	gcc -m32 -static-libgcc -shared -Wl,--version-script=exports.txt -o $@ $(ALL_OBJECTS) $(LIBS)

$(BUILDDIR)/obj32/%.o : %.c
	mkdir -p $(dir $@)
	g++ $(CFLAGS) -m32 -c $< -o $@
	g++ $(CFLAGS) -MM -MT $@ -o $(@:.o=.cdep) $<

$(BUILDDIR)/obj32/%.o : %.cpp
	mkdir -p $(dir $@)
	g++ $(CFLAGS) -m32 -c $< -o $@
	g++ $(CFLAGS) -MM -MT $@ -o $(@:.o=.cppdep) $<

$(BUILDDIR)/obj64/%.o : %.c
	mkdir -p $(dir $@)
	g++ $(CFLAGS) -m64 -c $< -o $@
	g++ $(CFLAGS) -MM -MT $@ -o $(@:.o=.cdep) $<

$(BUILDDIR)/obj64/%.o : %.cpp
	mkdir -p $(dir $@)
	g++ $(CFLAGS) -m64 -c $< -o $@
	g++ $(CFLAGS) -MM -MT $@ -o $(@:.o=.cppdep) $<

clean:
	@echo Cleaning out everything.
	rm -rf $(BUILDDIR)

install:
	cp -r $(BUILDDIR)/$(TARGET)/32 /data/bo/X-Plane\ 10/Resources/plugins/$(TARGET)/
	cp -r $(BUILDDIR)/$(TARGET)/64 /data/bo/X-Plane\ 10/Resources/plugins/$(TARGET)/

# Include any dependency turds, but don't error out if they don't exist.
# On the first build, every .c is dirty anyway.  On future builds, if the
# .c changes, it is rebuilt (as is its dep) so who cares if dependencies
# are stale.  If the .c is the same but a header has changed, this 
# declares the header to be changed.  If a primary header includes a 
# utility header and the primary header is changed, the dependency
# needs a rebuild because EVERY header is included.  And if the secondary
# header is changed, the primary header had it before (and is unchanged)
# so that is in the dependency file too.
-include $(ALL_DEPS)
-include $(ALL_DEPS64)


