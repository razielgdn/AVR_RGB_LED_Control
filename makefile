# Makefile for building and flashing ATMEGA328p

# Variables
MCU         = atmega328p
F_CPU       = 8000000
SRC         = src
BUILD_DIR   = bin
BINARY_NAME = RGB_control
PROGRAMMER  = usbasp
PORT        = usb
LFUSE 		= 0xD2
HFUSE 		= 
EFUSE 		=

# Compiler and linker flags
CFLAGS=-mmcu=$(MCU) -Wall -Os -DF_CPU=$(F_CPU)
LDFLAGS= -p $(MCU) -c $(PROGRAMMER) -P $(PORT)


# Rules
all: $(BUILD_DIR)/$(BINARY_NAME).hex

$(BUILD_DIR)/$(BINARY_NAME).elf: $(SRC)
	@mkdir -p $(BUILD_DIR)
	avr-gcc $(CFLAGS)  $(SRC)/*.c $(SRC)/LED_control/*.c $(SRC)/timer_control/*.c -o $(BUILD_DIR)/$(BINARY_NAME).elf

$(BUILD_DIR)/$(BINARY_NAME).hex: $(BUILD_DIR)/$(BINARY_NAME).elf
	avr-objcopy -j .text -j .data -O ihex $(BUILD_DIR)/$(BINARY_NAME).elf $(BUILD_DIR)/$(BINARY_NAME).hex -v

flash: $(BUILD_DIR)/$(BINARY_NAME).hex
	avrdude $(LDFLAGS) -U flash:w:$(BUILD_DIR)/$(BINARY_NAME).hex:i 
readFuses: 
	avrdude $(LDFLAGS) -U efuse:r:-:h  -U hfuse:r:-:h  -U lfuse:r:-:h

writeLowFuse:
	avrdude $(LDFLAGS) -U lfuse:w:0xD2:m
clean:
	rm -rf $(BUILD_DIR) 

.PHONY: all flash clean 
