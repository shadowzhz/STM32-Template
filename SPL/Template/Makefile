TARGET = STM32F103C8T6

CC = arm-none-eabi-gcc
AS = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

CPU = -mcpu=cortex-m3
MCU = $(CPU) -mthumb


# STM32F103C8T6
FLASH_SIZE = 65536
RAM_SIZE   = 20480


CFLAGS = $(MCU) \
         -std=gnu11 \
         -Wall \
         -ffunction-sections \
         -fdata-sections \
         -O0 \
         -g3 \
         -DUSE_STDPERIPH_DRIVER \
         -DSTM32F10X_MD \
         -ILibraries/CMSIS/CM3/CoreSupport \
         -ILibraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x \
         -ILibraries/STM32F10x_StdPeriph_Driver/inc


BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj


LDFLAGS = $(MCU) \
          -TSTM32F103C8T6.ld \
          --specs=nano.specs \
          -Wl,--gc-sections \
          -Wl,-Map=$(BUILD_DIR)/$(TARGET).map



# ============================================================
# 自动搜索源码
# ============================================================

SRC = $(shell find User Drivers Hardware -type f -name '*.c' 2>/dev/null)

# 自动搜索头文件目录
INC_DIRS = $(shell find User Drivers Hardware -type d 2>/dev/null)

CFLAGS += $(addprefix -I,$(INC_DIRS))


# STM32标准外设库
SRC += $(wildcard Libraries/STM32F10x_StdPeriph_Driver/src/*.c)


# 启动文件
ASRC = User/startup_stm32f10x_md.s



# ============================================================
# OBJ
# ============================================================

OBJ = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC))
OBJ += $(patsubst %.s,$(OBJ_DIR)/%.o,$(ASRC))



# ============================================================
# 默认只生成 HEX
# ============================================================

all: $(BUILD_DIR)/$(TARGET).hex



# ============================================================
# ELF
# ============================================================

$(BUILD_DIR)/$(TARGET).elf: $(OBJ)

	@mkdir -p $(dir $@)

	$(CC) $(OBJ) $(LDFLAGS) -o $@

	@echo ""
	@echo "========== Memory Usage =========="

	@TEXT=$$($(SIZE) $@ | tail -n 1 | awk '{print $$1}'); \
	DATA=$$($(SIZE) $@ | tail -n 1 | awk '{print $$2}'); \
	BSS=$$($(SIZE) $@ | tail -n 1 | awk '{print $$3}'); \
	FLASH=$$((TEXT + DATA)); \
	RAM=$$((DATA + BSS)); \
	FLASH_PCT=$$(awk "BEGIN {printf \"%.2f\", $$FLASH / $(FLASH_SIZE) * 100}"); \
	RAM_PCT=$$(awk "BEGIN {printf \"%.2f\", $$RAM / $(RAM_SIZE) * 100}"); \
	echo "Flash: $$FLASH / $(FLASH_SIZE) bytes ($$FLASH_PCT%)"; \
	echo "RAM:   $$RAM / $(RAM_SIZE) bytes ($$RAM_PCT%)"

	@echo "================================="

	@$(SIZE) $@

	@TEXT=$$($(SIZE) $@ | tail -n 1 | awk '{print $$1}'); \
	DATA=$$($(SIZE) $@ | tail -n 1 | awk '{print $$2}'); \
	BSS=$$($(SIZE) $@ | tail -n 1 | awk '{print $$3}'); \
	FLASH=$$((TEXT + DATA)); \
	RAM=$$((DATA + BSS)); \
	if [ $$FLASH -gt $(FLASH_SIZE) ]; then \
		echo "ERROR: Flash overflow!"; \
		exit 1; \
	fi; \
	if [ $$RAM -gt $(RAM_SIZE) ]; then \
		echo "ERROR: RAM overflow!"; \
		exit 1; \
	fi

# ============================================================
# C编译
# ============================================================

$(OBJ_DIR)/%.o: %.c

	@mkdir -p $(dir $@)

	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@



# ============================================================
# 汇编
# ============================================================

$(OBJ_DIR)/%.o: %.s

	@mkdir -p $(dir $@)

	$(CC) $(CFLAGS) -c $< -o $@



# ============================================================
# HEX
# ============================================================

$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf

	$(OBJCOPY) -O ihex $< $@



# ============================================================
# BIN（需要时 make bin）
# ============================================================

bin: $(BUILD_DIR)/$(TARGET).bin


$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf

	$(OBJCOPY) -O binary $< $@



# ============================================================
# 烧录
# ============================================================

flash:

	sudo openocd \
		-f interface/stlink.cfg \
		-f target/stm32f1x.cfg \
		-c "program $(BUILD_DIR)/$(TARGET).elf verify reset exit"



# ============================================================
# 清理
# ============================================================

clean:

	rm -rf $(BUILD_DIR)



# 自动依赖

-include $(OBJ:.o=.d)



.PHONY: all clean flash bin

