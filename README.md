# STM32F103 工程集合 🚀

<p align="center">
  <img src="https://img.shields.io/badge/Language-C99-00599C.svg?logo=c&logoColor=white" alt="Language: C99">
  <img src="https://img.shields.io/badge/MCU-STM32F103C8T6-blue.svg?logo=stmicroelectronics&logoColor=white" alt="MCU: STM32F103C8T6">
  <img src="https://img.shields.io/badge/Toolchain-arm--none--eabi--gcc%20%7C%20Makefile-orange.svg" alt="Toolchain">
  <img src="https://img.shields.io/badge/Platform-Linux%20%7C%20macOS%20%7C%20Windows-lightgrey.svg" alt="Platform">
  <img src="https://img.shields.io/badge/Flasher-ST--Link%20%7C%20OpenOCD-brightgreen.svg" alt="Flasher">
</p>

> **基于 arm-none-eabi-gcc + Makefile 的 STM32F103C8T6 裸机工程集合。**  
> 不依赖 CubeMX、Keil 或任何重量级 IDE，命令行三步极速编译烧录；固件库源码随仓库完整内置分发，克隆即用。

---

## 📁 目录结构 (Project Structure)

```text
STM32-Template/
├── HAL/                            ← 新工程从这里开始
│   ├── Template/                   HAL + LL 工程模板（HAL v1.1.10，Makefile 已验证编译）
│   └── 新手注意事项.md             F1 专属坑 / 中断回调 / DMA / 故障速查
└── SPL/                            ← 标准外设库（兼容老旧工程）
    └── Template/                   StdPeriph V3.5.0 工程模板
```

---

## ✨ 核心特性 (Features)

- ⚡ **无 IDE 强依赖**：原生基于 `make` 编译、`make flash` 烧录（OpenOCD + ST-Link），Linux / macOS / Windows 原生跨平台通用。
- 📦 **零配置开箱即用**：全套驱动源码（HAL+LL / SPL / CMSIS）已完整内嵌在仓库内，克隆后无需手动下载配置芯片支持包（DFP）。
- 🔍 **源码动态自发现**：在 `User/`、`Hardware/`、`Drivers/` 里新建的 `.c/.h` 源码文件无需手动修改 Makefile，自动解析加入编译列表。
- 📊 **精准资源构建报告**：每次链接自动统计并打印 Flash/RAM 实际占用与超限预警，超额直接报错拦截。
- 💻 **现代编辑器就绪**：两个模板均随附标准 VSCode 配置（包含 C/C++ 语法补全、智能跳转与构建烧录快捷 Task）。

---

## ⚡ 快速开始 (Quick Start)

### 1. 硬件接线 (ST-Link ↔ STM32F103C8T6)

| ST-Link 烧录器 | 核心板引脚 | 说明 |
|---|---|---|
| **3.3V** | 3.3V | ⚠️ **切勿接 5V**，防止瞬间击穿单片机 |
| **GND** | GND | 系统共地 |
| **SWCLK** | SWCLK / PA14 | 时钟调试信号线 |
| **SWDIO** | SWDIO / PA13 | 双向数据信号线 |

> **BOOT 跳线检查**：核心板上两个黄色跳线帽（BOOT0 / BOOT1）务必都置于 **0** 端（靠 GND 侧），否则程序无法从 Flash 正常引导运行。

### 2. 编译与烧录

```bash
# 1. 克隆仓库
git clone https://github.com/shadowzhz/STM32-Template.git
cd STM32-Template/HAL/Template

# 2. 编译工程（自动生成 build/STM32F103C8T6.hex 并打印内存占用报告）
make

# 3. 一键烧录（通过 ST-Link + SWD 烧录，Linux 配置 udev 后无需 sudo）
make flash
```

*按 **Blue Pill**（STM32F103C8T6）预设：8MHz 晶振 → PLL×9 → 主频 72MHz，烧录后板载 PC13 LED 将开始周期闪烁（低电平点亮）。*

---

## 🛠️ 环境搭建 (Prerequisites)

### 🐧 Ubuntu / Debian

```bash
# 1. 安装交叉编译器与烧录器 (Ubuntu 24.04 及以下包含 newlib；Ubuntu 24.10+ 系统自动切换为 picolibc)
sudo apt update && sudo apt install -y gcc-arm-none-eabi make openocd libnewlib-arm-none-eabi 2>/dev/null || sudo apt install -y gcc-arm-none-eabi make openocd

# 2. 配置 ST-Link 免 sudo 烧录权限 (udev 规则)
sudo usermod -aG plugdev $USER
sudo cp /usr/lib/udev/rules.d/60-openocd.rules /etc/udev/rules.d/ 2>/dev/null || \
echo 'ATTRS{idVendor}=="0483", ATTRS{idProduct}=="3748", MODE="0666"
ATTRS{idVendor}=="0483", ATTRS{idProduct}=="374b", MODE="0666"' | sudo tee /etc/udev/rules.d/99-stlink.rules
sudo udevadm control --reload-rules && sudo udevadm trigger
```

> *注：配置用户组后重新打开终端生效；临时烧录亦可直接使用 `sudo make flash`。*

### 🍎 macOS

```bash
brew install arm-none-eabi-gcc make openocd
```

### 🪟 Windows (原生 PowerShell，无需进入 MSYS2 终端)

1. **安装 MSYS2**：前往 [清华镜像 MSYS2 目录](https://mirrors.tuna.tsinghua.edu.cn/msys2/distrib/x86_64/) 下载日期最新的 `msys2-x86_64-*.exe` 安装（建议保持默认路径 `C:\msys64`）。
2. **安装 make 与 openocd**（以管理员或普通 PowerShell 运行，换清华源加速）：
   ```powershell
   C:\msys64\usr\bin\bash -lc "sed -i 's#https://mirror.msys2.org#https://mirrors.tuna.tsinghua.edu.cn/msys2#g' /etc/pacman.d/mirrorlist.* && pacman -Syu --noconfirm && pacman -S --noconfirm make mingw-w64-x86_64-openocd"
   ```
3. **注入环境变量**：
   ```powershell
   [Environment]::SetEnvironmentVariable("Path", [Environment]::GetEnvironmentVariable("Path","User") + ";C:\msys64\usr\bin;C:\msys64\mingw64\bin", "User")
   ```
4. **安装 Arm GNU Toolchain**：
   - 前往 [清华镜像 arm-gnu-toolchain 目录](https://mirrors.tuna.tsinghua.edu.cn/arm-gnu-toolchain/)，下载最新版 `arm-gnu-toolchain-*-mingw-w64-i686-arm-none-eabi.exe` 安装包（**切勿选 aarch64 或 zip 压缩包**），安装时务必勾选 **Add to PATH**。
5. **烧录器驱动配置**：
   - **官方 ST-Link**：安装官方驱动（官网搜索 `STSW-LINK009`）。
   - **淘宝铝壳 / 兼容型 ST-Link V2**：若 OpenOCD 报找不到设备，下载 [Zadig](https://zadig.akeo.ie/) 将 ST-Link 驱动直接一键切换为 **WinUSB** 即可。

> **终端就绪验证**：新打开终端执行 `arm-none-eabi-gcc --version`、`make --version`、`openocd --version`，均能正常打印版本号即代表环境就绪。

---

### VSCode 编译、补全与清理

- 直接打开 `HAL/Template` 或 `SPL/Template` 文件夹，使其中的 `.vscode` 配置生效。
- 按 `Ctrl+Shift+B` 编译；通过“终端 → 运行任务”选择 `STM32: Flash` 烧录或 `STM32: Clean` 清理。
- 补全配置使用 VSCode 用户设置中的 `C_Cpp.default.compilerPath`。请将其设为本机 `arm-none-eabi-gcc` 的完整路径，避免自动选择 `cl.exe`。例如：

  ```json
  "C_Cpp.default.compilerPath": "C:/path/to/arm-gnu-toolchain/bin/arm-none-eabi-gcc.exe"
  ```

- Windows 任务通过 `PATH` 查找 `make`，并使用默认安装位置 `%ProgramFiles%/Git/bin/sh.exe` 执行构建命令；请安装 Git for Windows，或按实际 shell 位置修改任务的 `SHELL` 参数。任务同时支持放在 `%LOCALAPPDATA%/EmbeddedTools/arm-gnu-toolchain` 和 `%LOCALAPPDATA%/EmbeddedTools/openocd` 的本机工具，其他位置仍通过 `PATH` 查找。
- 修改环境变量后完全退出并重新打开 VSCode。若补全仍显示旧错误，可运行 `C/C++: Reset IntelliSense Database`。
- `make clean` 在 Windows 下调用 PowerShell，在其他系统下调用 `rm`；构建目录不存在时也可以重复执行。

---

## ⚖️ 架构选型：SPL 还是 HAL？

| 评估维度 | SPL（标准外设库） | HAL + LL（现代硬件抽象层） |
|---|---|---|
| **维护状态** | ST 官方已停止维护，仅覆盖 F1 等旧系列 | 官方持续维护更新，跨 STM32 全系列芯片生态通用 |
| **抽象层级** | 寄存器与初级封装 | 结构体句柄 + 中断回调；性能敏感处可混用 LL 原生宏 |
| **推荐建议** | 仅用于维护旧项目或教学老旧例程移植 | **新工程一律推荐使用 HAL**，兼顾模块化与代码可移植性 |

---

## 🛠️ 常见避坑指南 (Troubleshooting)

### 1. `stm32f1xx_hal_conf.h` / `stm32f10x_conf.h` 找不到
- 用户配置文件**固定存放于 `User/` 目录下**。切勿挪入 `Libraries/` 内，亦注意防范编辑器手滑保存为 `.h.h`。

### 2. 链接报错 `ERROR: Flash overflow!`
- STM32F103C8T6 标称 Flash 容量为 64KB。HAL 库若启用了大量未使用的外设驱动，请在 `User/stm32f1xx_hal_conf.h` 中注释掉不用的模块（未启用的模块会在链接期自动裁剔）。

### 3. JTAG 引脚复用导致芯片"锁死"无法二次烧录
- 当把 PB3 / PB4 / PA15 用作普通 GPIO 时，**切勿调用 `SWJ_DISABLE`**（会将 SWD 调试口一同彻底关闭）。务必调用 `__HAL_AFIO_REMAP_SWJ_NOJTAG()` 仅禁用 JTAG。
- *若已被锁*：按住板载 **RESET 键**不放并点击烧录，待 OpenOCD 连接握手的瞬间松开 RESET 即可强制夺回控制权。

---

## 📚 相关参考文档 (Documentation)

- [HAL 模板详细配置与外设启用指引](./HAL/Template/README.md)
- [SPL 模板说明与更换芯片选型指引](./SPL/Template/README.md)
- [HAL 新手常见陷阱清单与编译烧录速查](./HAL/新手注意事项.md)

---

## 📄 许可证与开源协议 (License)

- `Libraries/` 目录内的 ST 官方驱动库（HAL/LL、CMSIS、StdPeriph）版权归 **STMicroelectronics** 所有，遵照 BSD-3-Clause 等官方许可证分发，详见各子目录内的许可文件。
- 本仓库所提供的 Makefile 自动化工程模板、VSCode 规则与封装配置代码遵循开放使用原则，欢迎自由用于学习、竞赛与商业嵌入式工程中。
