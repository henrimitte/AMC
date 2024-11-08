.PHONY: all clean install run uninstall

# AMC Version
AMC_VERSION_MAJOR = 0
AMC_VERSION_MINOR = 3
AMC_VERSION_PATCH = 1
AMC_VERSION = "$(AMC_VERSION_MAJOR).$(AMC_VERSION_MINOR).$(AMC_VERSION_PATCH)"

# Default paths
SOURCE_DIR = ./src
BUILD_DIR = ./build
INSTALLATION_DIR = /usr/local/bin

# The default C Compiler
CC = gcc

# Compiler flags
CFLAGS = -lm -lraylib

# User performing the action
USER = $(shell whoami)


all: AMC

AMC: $(BUILD_DIR)/AMC.o
	mv --verbose $(BUILD_DIR)/AMC.o $(BUILD_DIR)/AMC

$(BUILD_DIR)/AMC.o: $(SOURCE_DIR)/AMC.c ${SOURCE_DIR}/AMC.h ${SOURCE_DIR}/cell.h ${SOURCE_DIR}/game_state.h ${SOURCE_DIR}/level.h
	@echo "# Making sure $(BUILD_DIR) directory exists..."
	mkdir --parents --verbose $(BUILD_DIR)

	@echo "# Building the game: AMC (v$(AMC_VERSION))..."
	$(CC) $(SOURCE_DIR)/AMC.c -o $(BUILD_DIR)/AMC.o $(CFLAGS) 

install: AMC
ifeq ($(USER),root)
	@echo "# Making sure $(INSTALLATION_DIR) directory exists..."
	mkdir --parents --verbose $(INSTALLATION_DIR)

	@echo "# Copying binary to $(INSTALLATION_DIR)..."
	cp --update=all --verbose $(BUILD_DIR)/AMC $(INSTALLATION_DIR)/AMC

	@echo Game installed with success. Type AMC to run.
else
	@echo It was NOT possible to install the game. Try running with sudo.
endif

uninstall:
ifeq ($(USER), root)
	rm --force --verbose $(INSTALLATION_DIR)/AMC
	@echo Game uninstalled with success.
else
	@echo It was NOT possible to uninstall the game. Try running with sudo.
endif

clean:
	@echo "# Removing the build directory..."
	rm --recursive --force --verbose $(BUILD_DIR)

run:
	$(BUILD_DIR)/AMC
