BASE_DIR	= ..
SRC_DIR 	= ../source
INCLUDE 	= -I. -I$(BASE_DIR) -I$(BASE_DIR)/include

LIBRARY_NAME:= wcl
OS_NAME 	:= $(shell uname -s)

ifeq ($(TARGET),)
	TARGET		= Release
endif

ifeq ($(TARGET),Release)
	TARGET		= Release
	OPT_FLAGS	= -O3
	DEFINES		:= -DCE_RELEASE_BUILD
endif

ifeq ($(TARGET),Debug)
	TARGET		:= Debug
	DBG_FLAGS	:= -gstabs+ -feliminate-unused-debug-symbols
	DEFINES		:= -DDEBUG -DCE_DEBUG_BUILD
endif

ifeq ($(LIB_CONFIG),)
	LIB_CONFIG	:= static
endif

ifeq ($(OS_NAME),Darwin)
	DEFINES		+= -DDARWIN -D__APPLE__ -D__apple__
	INCLUDE		+= -I/System/Library/Frameworks/OpenCL.framework/Headers
	LIBRARIES 	+= -framework OpenCL -framework AppKit -framework Foundation 
	STATIC_EXT	:= a
	SHARED_EXT	:= dylib
endif
ifeq ($OS_NAME),Linux)
	DEFINES		+= -DLINUX -D__LINUX__ -D__linux__
	INCLUDE		+= -I/usr/local/include
	LIBRARIES 	+= -libcl
	STATIC_EXT	:= a
	SHARED_EXT	:= so
endif

BUILD_DIR	:= $(BASE_DIR)/build/$(OS_NAME)
LIB_DIR		:= $(BASE_DIR)/lib/$(OS_NAME)
OBJ_DIR 	:= $(BUILD_DIR)/$(TARGET)
TARGET_DIR 	:= $(LIB_DIR)/$(TARGET)
STATIC_LIB	:= $(TARGET_DIR)/lib$(LIBRARY_NAME).$(STATIC_EXT)
SHARED_LIB	:= $(TARGET_DIR)/lib$(LIBRARY_NAME).$(SHARED_EXT)
SRC_FILES	:= $(shell ls $(SRC_DIR)/*.c)
OBJ_FILES	:= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
HDR_FILES	:= $(shell ls $(BASE_DIR)/include/*.h $(SRC_DIR)/*.h)
LIBPATH 	+= -L$(TARGET_DIR)

LD = libtool
CC = gcc


BUILD_OPTIONS = -c -Wall -std=c99
CFLAGS = $(BUILD_OPTIONS) $(DEFINES) $(OPT_FLAGS) $(DBG_FLAGS) $(INCLUDE) 
LDFLAGS = -static

ifeq ($(LIB_CONFIG),shared)
	LIB_FILE	:= $(SHARED_LIB)
	DEFINES		+= -DCE_SHARED_LIB_TARGET -DCE_SHARED_LIB_EXPORT
else
	LIB_FILE	:= $(STATIC_LIB)
	DEFINES		+= -DCE_STATIC_LIB_TARGET
endif

default: $(LIB_FILE)

Release:
	$(MAKE) $(MAKEFILE) TARGET=Release

release:
	$(MAKE) $(MAKEFILE) TARGET=Release

Debug:
	$(MAKE) $(MAKEFILE) TARGET=Debug 

debug:
	$(MAKE) $(MAKEFILE) TARGET=Debug 

shared:
 	$(MAKE) $(MAKEFILE) LIB_CONFIG=shared

shared-debug:
	$(MAKE) $(MAKEFILE) LIB_CONFIG=shared TARGET=Debug 

shared-Release:
	$(MAKE) $(MAKEFILE) LIB_CONFIG=shared TARGET=Release

static:
 	$(MAKE) $(MAKEFILE) LIB_CONFIG=static

static-debug:
	$(MAKE) $(MAKEFILE) LIB_CONFIG=static TARGET=Debug 

static-Release:
	$(MAKE) $(MAKEFILE) LIB_CONFIG=static TARGET=Release

$(LIB_FILE): $(BUILD_DIR) $(LIB_DIR) $(TARGET_DIR) $(OBJ_DIR)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	
$(TARGET_DIR):
	mkdir -p $(TARGET_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<
	
$(SHARED_LIB): $(OBJ_FILES) $(OBJ_DIR) $(TARGET_DIR)
	$(CC) -dynamiclib -o $@ -g $(OBJ_FILES) $(LIBPATH) $(LIBRARIES)

$(STATIC_LIB): $(OBJ_DIR) $(OBJ_FILES) $(TARGET_DIR)
	$(LD) -o $@  $(OBJ_FILES) $(LDFLAGS)

depend: dep

dep:
	makedepend -- $(CFLAGS) -- $(SRC_FILES)
	
clobber: clean
	rm -rf $(BUILD_DIR)
	rm -rf $(LIB_DIR)

clean: 
	rm -f $(BUILD_DIR)/*/*.o
	rm -f $(LIB_DIR)/*/*.$(STATIC_EXT)
	rm -f $(LIB_DIR)/*/*.$(SHARED_EXT)


.DEFAULT:
	@echo The target \"$@\" does not exist in this Makefile.	
	
# DO NOT DELETE


