# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=release_x64
endif

ifndef verbose
  SILENT = @
endif

.PHONY: clean prebuild

SHELLTYPE := posix
ifeq (.exe,$(findstring .exe,$(ComSpec)))
	SHELLTYPE := msdos
endif

# Configurations
# #############################################

RESCOMP = windres
INCLUDES += -Iinclude
FORCE_INCLUDE +=
ALL_CPPFLAGS += $(CPPFLAGS) -MD -MP $(DEFINES) $(INCLUDES)
ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
LDDEPS +=
LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
define PREBUILDCMDS
endef
define PRELINKCMDS
endef
define POSTBUILDCMDS
endef

ifeq ($(config),release_x64)
TARGETDIR = bin
TARGET = $(TARGETDIR)/UnitTests_x86-64
OBJDIR = obj/x64/release/UnitTests
DEFINES += -DBS_BUILD_OPENGL -DBS_BUILD_LINUX -DBS_BUILD_64BIT -DUT_BUILD_LINUX
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -O3 -Wall -Wextra -pthread -Wno-unknown-pragmas
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -O3 -Wall -Wextra -std=c++20 -pthread -Wno-unknown-pragmas
LIBS += -lBrimstone_x86-64 -lluajit-5.1_x64 -lgll_x86-64 -lGL -ldl -lX11 -lpng
ALL_LDFLAGS += $(LDFLAGS) -Llib -L/usr/lib64 -m64 -s -pthread

else ifeq ($(config),release_x32)
TARGETDIR = bin
TARGET = $(TARGETDIR)/UnitTests_x86
OBJDIR = obj/x32/release/UnitTests
DEFINES += -DBS_BUILD_OPENGL -DBS_BUILD_LINUX -DUT_BUILD_LINUX
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -O3 -Wall -Wextra -pthread -Wno-unknown-pragmas
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -O3 -Wall -Wextra -std=c++20 -pthread -Wno-unknown-pragmas
LIBS += -lBrimstone_x86 -lluajit-5.1_x86 -lgll_x86 -lGL -ldl -lX11 -lpng
ALL_LDFLAGS += $(LDFLAGS) -Llib -L/usr/lib32 -m32 -s -pthread

else ifeq ($(config),debug_x64)
TARGETDIR = bin
TARGET = $(TARGETDIR)/UnitTests_x86-64d
OBJDIR = obj/x64/debug/UnitTests
DEFINES += -DBS_BUILD_OPENGL -DBS_BUILD_DEBUG -DBS_ZERO -DBS_CHECK_NULLPTR -DBS_CHECK_SIZE -DBS_CHECK_INDEX -DBS_CHECK_DIVBYZERO -DBS_CHECK_DOMAIN -DBS_BUILD_LINUX -DBS_BUILD_64BIT -DUT_BUILD_LINUX
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -g -Wall -Wextra -pthread -Wno-unknown-pragmas
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -g -Wall -Wextra -std=c++20 -pthread -Wno-unknown-pragmas
LIBS += -lBrimstone_x86-64d -lluajit-5.1_x64 -lgll_x86-64 -lGL -ldl -lX11 -lpng
ALL_LDFLAGS += $(LDFLAGS) -Llib -L/usr/lib64 -m64 -pthread

else ifeq ($(config),debug_x32)
TARGETDIR = bin
TARGET = $(TARGETDIR)/UnitTests_x86d
OBJDIR = obj/x32/debug/UnitTests
DEFINES += -DBS_BUILD_OPENGL -DBS_BUILD_DEBUG -DBS_ZERO -DBS_CHECK_NULLPTR -DBS_CHECK_SIZE -DBS_CHECK_INDEX -DBS_CHECK_DIVBYZERO -DBS_CHECK_DOMAIN -DBS_BUILD_LINUX -DUT_BUILD_LINUX
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -g -Wall -Wextra -pthread -Wno-unknown-pragmas
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -g -Wall -Wextra -std=c++20 -pthread -Wno-unknown-pragmas
LIBS += -lBrimstone_x86d -lluajit-5.1_x86 -lgll_x86 -lGL -ldl -lX11 -lpng
ALL_LDFLAGS += $(LDFLAGS) -Llib -L/usr/lib32 -m32 -pthread

endif

# Per File Configurations
# #############################################


# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/Array.o
GENERATED += $(OBJDIR)/Bounds2.o
GENERATED += $(OBJDIR)/Bounds3.o
GENERATED += $(OBJDIR)/Bounds4.o
GENERATED += $(OBJDIR)/BoundsN.o
GENERATED += $(OBJDIR)/Exception.o
GENERATED += $(OBJDIR)/Matrix2x2.o
GENERATED += $(OBJDIR)/Matrix3x3.o
GENERATED += $(OBJDIR)/Matrix4x4.o
GENERATED += $(OBJDIR)/MatrixNxN.o
GENERATED += $(OBJDIR)/MatrixRxC.o
GENERATED += $(OBJDIR)/Menu.o
GENERATED += $(OBJDIR)/Point2.o
GENERATED += $(OBJDIR)/Point3.o
GENERATED += $(OBJDIR)/Point4.o
GENERATED += $(OBJDIR)/PointN.o
GENERATED += $(OBJDIR)/Range.o
GENERATED += $(OBJDIR)/Size2.o
GENERATED += $(OBJDIR)/Size3.o
GENERATED += $(OBJDIR)/Size4.o
GENERATED += $(OBJDIR)/SizeN.o
GENERATED += $(OBJDIR)/Test.o
GENERATED += $(OBJDIR)/TextColor.o
GENERATED += $(OBJDIR)/Vector2.o
GENERATED += $(OBJDIR)/Vector3.o
GENERATED += $(OBJDIR)/Vector4.o
GENERATED += $(OBJDIR)/VectorN.o
GENERATED += $(OBJDIR)/main.o
GENERATED += $(OBJDIR)/types.o
GENERATED += $(OBJDIR)/utils.o
OBJECTS += $(OBJDIR)/Array.o
OBJECTS += $(OBJDIR)/Bounds2.o
OBJECTS += $(OBJDIR)/Bounds3.o
OBJECTS += $(OBJDIR)/Bounds4.o
OBJECTS += $(OBJDIR)/BoundsN.o
OBJECTS += $(OBJDIR)/Exception.o
OBJECTS += $(OBJDIR)/Matrix2x2.o
OBJECTS += $(OBJDIR)/Matrix3x3.o
OBJECTS += $(OBJDIR)/Matrix4x4.o
OBJECTS += $(OBJDIR)/MatrixNxN.o
OBJECTS += $(OBJDIR)/MatrixRxC.o
OBJECTS += $(OBJDIR)/Menu.o
OBJECTS += $(OBJDIR)/Point2.o
OBJECTS += $(OBJDIR)/Point3.o
OBJECTS += $(OBJDIR)/Point4.o
OBJECTS += $(OBJDIR)/PointN.o
OBJECTS += $(OBJDIR)/Range.o
OBJECTS += $(OBJDIR)/Size2.o
OBJECTS += $(OBJDIR)/Size3.o
OBJECTS += $(OBJDIR)/Size4.o
OBJECTS += $(OBJDIR)/SizeN.o
OBJECTS += $(OBJDIR)/Test.o
OBJECTS += $(OBJDIR)/TextColor.o
OBJECTS += $(OBJDIR)/Vector2.o
OBJECTS += $(OBJDIR)/Vector3.o
OBJECTS += $(OBJDIR)/Vector4.o
OBJECTS += $(OBJDIR)/VectorN.o
OBJECTS += $(OBJDIR)/main.o
OBJECTS += $(OBJDIR)/types.o
OBJECTS += $(OBJDIR)/utils.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking UnitTests
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning UnitTests
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(GENERATED)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(GENERATED)) del /s /q $(subst /,\\,$(GENERATED))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild: | $(OBJDIR)
	$(PREBUILDCMDS)

ifneq (,$(PCH))
$(OBJECTS): $(GCH) | $(PCH_PLACEHOLDER)
$(GCH): $(PCH) | prebuild
	@echo $(notdir $<)
	$(SILENT) $(CXX) -x c++-header $(ALL_CXXFLAGS) -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
$(PCH_PLACEHOLDER): $(GCH) | $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) touch "$@"
else
	$(SILENT) echo $null >> "$@"
endif
else
$(OBJECTS): | prebuild
endif


# File Rules
# #############################################

$(OBJDIR)/Exception.o: src/tests/Exception.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Test.o: src/tests/Test.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Menu.o: src/tests/console/Menu.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/TextColor.o: src/tests/console/TextColor.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/main.o: src/tests/main.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Array.o: src/tests/test/Array.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Bounds2.o: src/tests/test/Bounds2.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Bounds3.o: src/tests/test/Bounds3.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Bounds4.o: src/tests/test/Bounds4.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/BoundsN.o: src/tests/test/BoundsN.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Matrix2x2.o: src/tests/test/Matrix2x2.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Matrix3x3.o: src/tests/test/Matrix3x3.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Matrix4x4.o: src/tests/test/Matrix4x4.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/MatrixNxN.o: src/tests/test/MatrixNxN.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/MatrixRxC.o: src/tests/test/MatrixRxC.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Point2.o: src/tests/test/Point2.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Point3.o: src/tests/test/Point3.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Point4.o: src/tests/test/Point4.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/PointN.o: src/tests/test/PointN.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Range.o: src/tests/test/Range.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Size2.o: src/tests/test/Size2.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Size3.o: src/tests/test/Size3.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Size4.o: src/tests/test/Size4.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/SizeN.o: src/tests/test/SizeN.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Vector2.o: src/tests/test/Vector2.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Vector3.o: src/tests/test/Vector3.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Vector4.o: src/tests/test/Vector4.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/VectorN.o: src/tests/test/VectorN.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/types.o: src/tests/test/types.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/utils.o: src/tests/utils.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif