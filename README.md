# STM32F103 工程集合

基于 **arm-none-eabi-gcc + Makefile** 的 STM32F103C8T6 裸机工程集合。
不依赖 CubeMX、Keil 或任何 IDE，命令行三步编译烧录；库源码全部随仓库分发，克隆即用。

```
STM32-Template/
├── HAL/                            ← 新工程从这里开始
│   ├── Template/                   HAL + LL 工程模板（HAL v1.1.10，Makefile 已验证编译）
│   └── 新手注意事项.md             F1 专属坑 / 中断回调 / DMA / 故障速查
└── SPL/                            ← 标准外设库
    └── Template/                   StdPeriph V3.5.0 工程模板
```

## 特性

- **无 IDE 依赖** —— `make` 编译、`make flash` 烧录（OpenOCD + ST-Link），Linux / macOS / Windows 原生通用
- **零配置起步** —— 库源码（HAL+LL / SPL / CMSIS）已在仓库内，克隆后不需要再装任何 SDK
- **自动搜索源码** —— 在 `User/`、`Hardware/`、`Drivers/` 里新建的 `.c/.h` 无需改 Makefile 自动参与编译
- **构建报告** —— 每次链接打印 Flash/RAM 占用百分比，超限直接报错
- **VSCode 就绪** —— 两个模板都带 IntelliSense 与构建任务配置（`.vscode/`）

## 环境搭建

### Ubuntu / Debian

```bash
# 1. 安装工具链与烧录器 (显式包含 newlib 库，避免 nano.specs/stdint.h 缺失)
sudo apt update && sudo apt install -y gcc-arm-none-eabi libnewlib-arm-none-eabi make openocd

# 2. 配置 ST-Link 免 sudo 烧录权限
sudo usermod -aG plugdev $USER
sudo cp /usr/lib/udev/rules.d/60-openocd.rules /etc/udev/rules.d/ 2>/dev/null || \
echo 'ATTRS{idVendor}=="0483", ATTRS{idProduct}=="3748", MODE="0666"
ATTRS{idVendor}=="0483", ATTRS{idProduct}=="374b", MODE="0666"' | sudo tee /etc/udev/rules.d/99-stlink.rules
sudo udevadm control --reload-rules && sudo udevadm trigger
# 注：配置用户组后重新打开终端生效；临时烧录亦可使用 sudo make flash
```

### macOS

```bash
brew install arm-none-eabi-gcc make openocd
```

### Windows (原生 PowerShell，无需进入 MSYS2 终端)

1. **安装 MSYS2**：前往 [清华镜像 MSYS2 目录](https://mirrors.tuna.tsinghua.edu.cn/msys2/distrib/x86_64/) 下载日期最新的 `msys2-x86_64-*.exe` 安装（路径保持默认 `C:\msys64`）。
   - *(若尚未安装 Git，可在 [清华镜像 Git 目录](https://mirrors.tuna.tsinghua.edu.cn/github-release/git-for-windows/git/) 下载安装)*
2. **安装 make 与 openocd**（PowerShell 运行，自动换清华源加速）：
   ```powershell
   C:\msys64\usr\bin\bash -lc "sed -i 's#https://mirror.msys2.org#https://mirrors.tuna.tsinghua.edu.cn/msys2#g' /etc/pacman.d/mirrorlist.* && pacman -Syu --noconfirm && pacman -S --noconfirm make mingw-w64-x86_64-openocd"
   ```
   > **提示**：若 MSYS2 首次更新提示需强制关闭终端重启核心，属于正常机制，**直接重新执行一次步骤 2 的命令**即可继续完成安装。
3. **注入环境变量**：
   ```powershell
   [Environment]::SetEnvironmentVariable("Path", [Environment]::GetEnvironmentVariable("Path","User") + ";C:\msys64\usr\bin;C:\msys64\mingw64\bin", "User")
   ```
4. **安装 Arm GNU Toolchain**：
   - 前往 [清华镜像 arm-gnu-toolchain 目录](https://mirrors.tuna.tsinghua.edu.cn/arm-gnu-toolchain/)，进入最新版本号文件夹，下载形如 `arm-gnu-toolchain-*-mingw-w64-i686-arm-none-eabi.exe` 的安装包（**切勿选 aarch64 或 zip 压缩包**），安装时勾选 **Add to PATH**。
   - *（若安装时漏选 Add to PATH，在 PowerShell 执行以下命令自动补全）*：
     ```powershell
     $arm = (Get-ChildItem -Path "C:\Program Files", "C:\Program Files (x86)" -Filter "arm-none-eabi-gcc.exe" -Recurse -ErrorAction SilentlyContinue | Select-Object -First 1).DirectoryName; if ($arm) { [Environment]::SetEnvironmentVariable("Path", [Environment]::GetEnvironmentVariable("Path","User") + ";$arm", "User"); Write-Host "已补全 Arm PATH: $arm" -ForegroundColor Green }
     ```
5. **驱动**：
   - 官方 ST-Link：安装 ST 官方驱动（官网搜 **STSW-LINK009**）。
   - 淘宝山寨 / 铝壳 ST-Link V2：若 OpenOCD 报找不到设备，下载 [Zadig](https://zadig.akeo.ie/) 将 ST-Link 设备驱动切换为 **WinUSB** 即可。

> **重启 PowerShell 终端验证**：依次执行 `arm-none-eabi-gcc --version`、`make --version`、`openocd --version`，均有版本号即就绪。

---

## 避坑速查 (必看)

1. **`stm32f1xx_hal_conf.h` / `stm32f10x_conf.h` 找不到**：
   - 用户配置文件**固定存放在 `User/` 目录下**。不要放在 `Libraries/` 里，也不要手滑保存为 `.h.h`。
2. **链接报错 `ERROR: Flash overflow!`**：
   - F103C8T6 标称 64KB Flash。HAL 库若启用了过多外设，请在 `User/stm32f1xx_hal_conf.h` 中注释掉不用的模块（未启用的会被链接器裁掉）。
3. **JTAG 引脚复用导致芯片"锁死"无法烧录**：
   - PB3/PB4/PA15 当普通 IO 时，切勿使用 `SWJ_DISABLE`（会连 SWD 一起关掉）。必须使用 `__HAL_AFIO_REMAP_SWJ_NOJTAG()`。
   - 若已被锁，按住板载 **RESET 键**不放点击烧录，连接瞬间松开。

---

## 快速开始

### 1. 硬件接线 (ST-Link ↔ STM32F103C8T6)

| ST-Link 烧录器 | 核心板引脚 | 说明 |
|---|---|---|
| **3.3V** | 3.3V | ⚠️ **切勿接 5V**，防止烧毁单片机 |
| **GND** | GND | 共地 |
| **SWCLK** | SWCLK / PA14 | 时钟线 |
| **SWDIO** | SWDIO / PA13 | 数据线 |

> **BOOT 跳线检查**：核心板上两个黄色跳线帽（BOOT0 / BOOT1）务必都置于 **0** 端（靠 GND 侧），否则程序无法从 Flash 正常引导。

### 2. 编译与烧录

```bash
git clone https://github.com/shadowzhz/STM32-Template.git
cd STM32-Template/HAL/Template
make          # 生成 build/STM32F103C8T6.hex 并打印内存占用
make flash    # ST-Link + SWD 烧录（Linux 配置好 udev 后无需 sudo）
```

按 **Blue Pill**（STM32F103C8T6）预设：8MHz 晶振 → PLL×9 → 主频 72MHz，
演示程序闪烁 PC13 LED（低电平点亮）。

## SPL 还是 HAL？

| | SPL（标准库） | HAL + LL |
|---|---|---|
| 状态 | ST 已停更，仅 F1 系列 | 持续维护，跨全系列芯片通用 |
| 抽象层级 | 外设寄存器级封装 | 句柄 + 回调；LL 提供寄存器级宏 |
| 建议 | 维护旧工程 | **新工程一律用 HAL**，性能敏感处混用 LL |

## 相关文档

- [HAL/Template/README.md](./HAL/Template/README.md) —— HAL 模板详细说明与启用外设方法
- [SPL/Template/README.md](./SPL/Template/README.md) —— SPL 模板说明与换芯片指引
- [HAL/新手注意事项.md](./HAL/新手注意事项.md) —— 时钟/中断/DMA 陷阱清单与编译烧录速查表

## 第三方许可

`Libraries/` 内的 ST 官方驱动（HAL/LL、CMSIS、StdPeriph）版权归 STMicroelectronics，
BSD-3-Clause 等许可原文见各目录内的 LICENSE 文件；其余代码未单独声明许可。
