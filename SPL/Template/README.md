# STM32F103C8T6 · 标准外设库(SPL)工程模板

裸机 Makefile 工程模板, 不依赖 CubeMX / Keil / STM32CubeIDE。
库为 ST 标准外设库 **STM32F10x StdPeriph V3.5.0**（最终版）, 源码随工程分发, 位于 `Libraries/`。

## 环境要求

类 Unix 环境（Linux / macOS / WSL / MSYS2）, 已安装:

| 工具 | 用途 | Debian/Ubuntu 安装 |
|---|---|---|
| `arm-none-eabi-gcc` | 交叉编译 | `sudo apt install gcc-arm-none-eabi` |
| `make` | 构建 | `sudo apt install make` |
| `openocd` | ST-Link 烧录 | `sudo apt install openocd` |

注意: `.vscode/c_cpp_properties.json` 里 `compilerPath` 写的是 `/usr/bin/arm-none-eabi-gcc`, 非 Linux 环境请改成你机器上的实际路径（或直接写 `arm-none-eabi-gcc` 让它从 PATH 查找）。

## 使用

```bash
make          # 编译, 生成 build/STM32F103C8T6.hex, 打印 Flash/RAM 占用并检查溢出
make flash    # ST-Link + SWD 烧录 (sudo openocd)
make bin      # 需要时额外生成 .bin
make clean    # 清理
```

新建工程: 复制整个 `Template` 目录改名即可, 源码路径是相对的, 位置随意。

## 目录结构

```
Template/
├── User/          用户代码: main.c、stm32f10x_conf.h(外设头开关)、
│                  startup_stm32f10x_md.s(启动文件)、system_stm32f10x.c、stm32f10x.h
├── Hardware/      自己写的外设驱动模块 (xxx.c/xxx.h)
├── Drivers/       备用, 与 Hardware 同样会被自动搜索
├── Libraries/     CMSIS + STM32F10x_StdPeriph_Driver —— 不要改动
├── Makefile       自动搜索 User/Drivers/Hardware 下所有源码, 新增文件无需改动
├── STM32F103C8T6.ld
└── .vscode/       IntelliSense 与构建任务
```

## 启用新外设

编辑 `User/stm32f10x_conf.h`, 取消对应 `#include "stm32f10x_xxx.h"` 的注释。
标准库源文件已在 Makefile 全量编译, 不需要动 Makefile。

## 换芯片（同系列）

三处一起改:
1. Makefile 里 `-DSTM32F10X_MD` 宏、`ASRC` 启动文件（如大容量换 `-DSTM32F10X_HD` + `startup_stm32f10x_hd.s`）, `.vscode` 的 defines 同步改;
2. `STM32F103C8T6.ld` 的 FLASH/RAM `LENGTH`;
3. Makefile 顶部的 `FLASH_SIZE`/`RAM_SIZE`。

## 板子假设

按 Blue Pill（STM32F103C8T6）预设: 8MHz 外部晶振, 72MHz 主频;
演示程序翻转 PC13 上的 LED（低电平点亮）。
