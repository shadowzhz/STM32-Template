# STM32F103C8T6 · 标准外设库(SPL)工程模板

裸机 Makefile 工程模板, 不依赖 CubeMX / Keil / STM32CubeIDE。
库为 ST 标准外设库 **STM32F10x StdPeriph V3.5.0**（最终版）, 源码随工程分发, 位于 `Libraries/`。

## 环境要求

**Linux / macOS**:

```bash
sudo apt install gcc-arm-none-eabi make openocd   # Debian/Ubuntu, macOS 用 brew 装同名包
```

**Windows（原生，全程 PowerShell，不需要打开 MSYS2 终端）**:

1. 安装 MSYS2: `winget install --id MSYS2.MSYS2 -e`; 没有 winget 就去 [msys2.org](https://www.msys2.org) 下安装包双击, 路径保持默认 `C:\msys64`
2. PowerShell 直接驱动它装 make 和 openocd（若因运行时自更新中断, 再执行一遍）:

   ```powershell
   C:\msys64\usr\bin\bash -lc "pacman -Syu --noconfirm && pacman -S --noconfirm make mingw-w64-x86_64-openocd"
   ```

3. 把工具加入用户 PATH, 重开终端生效:

   ```powershell
   [Environment]::SetEnvironmentVariable("Path", [Environment]::GetEnvironmentVariable("Path","User") + ";C:\msys64\usr\bin;C:\msys64\mingw64\bin", "User")
   ```

4. 安装 [Arm GNU Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)（选 `arm-none-eabi` 的 Windows mingw-w64 安装包, 安装时勾选 *Add to PATH*）
5. 安装 ST-Link 驱动 STSW-LINK009（st.com 下载）, VSCode + C/C++ 扩展打开工程即可

> 注意: make 必须用 MSYS2 提供的版本（含 POSIX shell, Makefile 配方需要 find/awk/rm）,
> 不要用 scoop/choco 装的原生 make.exe（不带 shell, 编译会失败）。

注意: `.vscode/c_cpp_properties.json` 的 `compilerPath` 填的是不带路径的 `arm-none-eabi-gcc`（从 PATH 查找, 三平台通用）；若 IntelliSense 报找不到头文件, 再改成编译器的完整安装路径。

## 使用

```bash
make          # 编译, 生成 build/STM32F103C8T6.hex, 打印 Flash/RAM 占用并检查溢出
make flash    # ST-Link + SWD 烧录 (Linux 权限不足时: sudo make flash)
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
