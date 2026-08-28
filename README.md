# STM32F103 工程集合

基于 **arm-none-eabi-gcc + Makefile** 的 STM32F103C8T6 裸机工程集合。
不依赖 CubeMX、Keil 或任何 IDE，命令行三步编译烧录；库源码全部随仓库分发，克隆即用。

```
STM32/
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

## 环境要求

**Linux / macOS**: 装 `arm-none-eabi-gcc`、`make`、`openocd` 三个包即可:

```bash
sudo apt install gcc-arm-none-eabi make openocd   # Debian/Ubuntu
```

**Windows（原生，无需 WSL）**, 四步:

1. 安装 [MSYS2](https://www.msys2.org)，在 MSYS2 终端执行:
   `pacman -S make mingw-w64-x86_64-openocd`
2. 把 `C:\msys64\usr\bin`（make/sh/find/awk）和 `C:\msys64\mingw64\bin`（openocd）加入系统 PATH
3. 安装 [Arm GNU Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)
   （选 `arm-none-eabi` 的 Windows mingw-w64 安装包，安装时勾选 *Add to PATH*）
4. 装好 ST-Link 驱动（STSW-LINK009，st.com 下载），VSCode + C/C++ 扩展打开工程即可

## 快速开始

```bash
git clone https://github.com/shadowzhz/STM32.git
cd STM32/HAL/Template
make          # 生成 build/STM32F103C8T6.hex 并打印内存占用
make flash    # ST-Link + SWD 烧录（Linux 权限不足时: sudo make flash）
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
