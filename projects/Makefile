# Project ##########################################################################################

PROJECT_NAME	:= compute
BASE_DIR		:= ..
BIN_DIR 		:= $(BASE_DIR)/bin
LIB_DIR 		:= $(BASE_DIR)/lib
SRC_DIR 		:= $(BASE_DIR)/source
BUILD_DIR 		:= $(BASE_DIR)/build
TST_DIR 		:= $(BASE_DIR)/tests
INC_DIR			:= $(BASE_DIR)/include
INCLUDE 		:= -I. -I$(INC_DIR) -I$(SRC_DIR)
USE_PTHREADS	:= 1

# Tools ############################################################################################

LD 				:= libtool
CC 				:= gcc
RM				:= rm
MKDIR			:= mkdir

# Platform #########################################################################################

OS_NAME 		:= $(shell uname -s)

ifeq ($(OS_NAME),Darwin)
	DEFINES		+= -DDARWIN -D__APPLE__ -D__apple__
	INCLUDE		+= -I/System/Library/Frameworks/OpenCL.framework/Headers
	LIBRARIES 	+= -framework OpenCL -framework AppKit -framework Foundation
	STATIC_EXT	:= a
	SHARED_EXT	:= dylib
endif

ifeq ($(OS_NAME),Linux)
	DEFINES		+= -DLINUX -D__LINUX__ -D__linux__
	INCLUDE		+= -I/usr/local/include
	LIBRARIES 	+= -libcl
	STATIC_EXT	:= a
	SHARED_EXT	:= so
endif

ifeq ($(USE_PTHREADS),1)
	LIBRARIES	+= -lpthread
	DEFINES		:= -DCE_USE_PTHREADS
endif

ifeq ($(ARCH),)
	ARCH		:= $(shell uname -m)
endif

ifeq ($(ARCH),i386)
	DEFINES 	+= -DCE_32BIT
	ARCH_FLAGS	:= -m32 -msse2 -mfpmath=sse
endif

ifeq ($(ARCH),x86_64)
	DEFINES 	+= -DCE_64BIT
	ARCH_FLAGS	:= -m64
endif

# Target Settings ##################################################################################

ifeq ($(TARGET),)
	TARGET		:= Debug
endif

ifeq ($(TARGET),Release)
	TARGET		:= Release
	OPT_FLAGS	:= -O3
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

# Build Configuration ##############################################################################

# Target
TGT_LIB_DIR		:= $(BASE_DIR)/lib/$(OS_NAME)/$(ARCH)/$(TARGET)
TGT_BUILD_DIR 	:= $(BASE_DIR)/build/$(OS_NAME)/$(ARCH)/$(TARGET)
TGT_STATIC_LIB	:= $(TGT_LIB_DIR)/lib$(PROJECT_NAME).$(STATIC_EXT)
TGT_SHARED_LIB	:= $(TGT_LIB_DIR)/lib$(PROJECT_NAME).$(SHARED_EXT)

# Compilation
SRC_FILES		:= $(shell ls $(SRC_DIR)/*.c)
OBJ_FILES		:= $(patsubst $(SRC_DIR)/%.c,$(TGT_BUILD_DIR)/%.o,$(SRC_FILES))
HDR_FILES		:= $(shell ls $(BASE_DIR)/include/*.h)

# Tests
TST_BIN_DIR 	:= $(BASE_DIR)/bin/$(OS_NAME)/$(ARCH)/$(TARGET)
TST_BUILD_DIR 	:= $(BASE_DIR)/build/$(OS_NAME)/$(ARCH)/Tests
TST_DIR_LIST	:= $(shell ls -d $(TST_DIR)/*)
TST_SRC_FILES	:= $(shell ls $(TST_DIR)/*/*.c)
TST_OBJ_FILES	:= $(patsubst %.c,%.o,$(TST_SRC_FILES))
TST_NAMES		:= $(notdir $(wildcard $(TST_DIR)/*))
TST_BINS		:= $(addprefix bin/, $(TST_NAMES))

LIBPATH 		+= -L$(TGT_LIB_DIR)

ifeq ($(LIB_CONFIG),static)
	LIB_FILE	:= $(TGT_STATIC_LIB)
	DEFINES		+= -DCE_STATIC_LIB_TARGET
else
	LIB_FILE	:= $(TGT_SHARED_LIB)	
	DEFINES		+= -DCE_SHARED_LIB_TARGET -DCE_SHARED_LIB_EXPORT 
	ifeq ($(OS_NAME),Darwin)
		LDFLAGS		+= -flat_namespace -undefined suppress -bundle -noprebind
	endif
endif

BUILD_OPTIONS 	:= -c -Wall -std=c99
CFLAGS 			:= $(BUILD_OPTIONS) $(DEFINES) $(ARCH_FLAGS) $(OPT_FLAGS) $(DBG_FLAGS) $(INCLUDE) 

# Target Rules #####################################################################################

default: $(LIB_FILE)

Release:
	$(MAKE) $(MAKEFILE) TARGET=Release

release:
	$(MAKE) $(MAKEFILE) TARGET=Release

Debug:
	$(MAKE) $(MAKEFILE) TARGET=Debug 

debug:
	$(MAKE) $(MAKEFILE) TARGET=Debug 

shared: $(TGT_SHARED_LIB)

shared-debug:
	$(MAKE) $(MAKEFILE) LIB_CONFIG=shared TARGET=Debug 

shared-release:
	$(MAKE) $(MAKEFILE) LIB_CONFIG=shared TARGET=Release

static: $(TGT_STATIC_LIB)

static-debug:
	$(MAKE) $(MAKEFILE) LIB_CONFIG=static TARGET=Debug 

static-release:
	$(MAKE) $(MAKEFILE) LIB_CONFIG=static TARGET=Release

$(LIB_FILE): $(TGT_BUILD_DIR) $(TGT_LIB_DIR)  

$(TGT_LIB_DIR):
	@echo "Creating target directory '$(TGT_LIB_DIR)'"
	@$(MKDIR) -p $(TGT_LIB_DIR)

$(TGT_BUILD_DIR):
	@echo "Creating build directory '$(TGT_BUILD_DIR)'"
	@$(MKDIR) -p $(TGT_BUILD_DIR)

$(TST_BIN_DIR):
	@echo "Creating test bin directory '$(TST_BIN_DIR)'"
	@$(MKDIR) -p $(TST_BIN_DIR)
	
$(TGT_BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Building object '$@'"
	@$(CC) $(CFLAGS) -o $@ -c $<

$(TGT_SHARED_LIB): $(TGT_LIB_DIR) $(TGT_BUILD_DIR) $(OBJ_FILES) 
	@echo "Linking library '$@'"
	@$(LD) -o $@  $(OBJ_FILES) -dynamic $(LDFLAGS) $(LIBPATH) $(LIBRARIES)

$(TGT_STATIC_LIB): $(TGT_LIB_DIR) $(TGT_BUILD_DIR) $(OBJ_FILES)
	@echo "Linking library '$@'"
	@$(LD) -o $@  $(OBJ_FILES) -static $(LDFLAGS)

depend: dep

dep:
	makedepend -- $(CFLAGS) -- $(SRC_FILES)
	
clobber: clean
	@echo "Clobbering..."
	@$(RM) -rf $(BUILD_DIR)/*
	@$(RM) -rf $(LIB_DIR)/*
	@$(RM) -rf $(BIN_DIR)/*

clean: 
	@echo "Cleaning..."
	@$(RM) -f $(BUILD_DIR)/*/*.o
	@$(RM) -f $(TST_DIR)/*/*/*.o
	@$(RM) -f $(LIB_DIR)/*/*/*.*
	@$(RM) -f $(BIN_DIR)/*/*/*/*

.DEFAULT:
	@echo The target \"$@\" does not exist in this Makefile.	

# Test Rules #######################################################################################

tests: $(LIB_FILE) $(TST_BIN_DIR) $(TST_BIN_DIR)/options

$(TST_BIN_DIR)/options: $(TST_DIR)/options/main.o
	@echo "Linking $@"
	@$(CC) $(ARCH_FLAGS) $< -o $@  $(LDFLAGS) $(LIBPATH) $(LIBRARIES) $(LIB_FILE) $(LIBS) 

$(TST_DIR)/options/main.o: $(TST_DIR)/options/main.c
	@echo "Building object '$@'"
	@$(CC) $(CFLAGS) -o $@ -c $<


	
# DO NOT DELETE


