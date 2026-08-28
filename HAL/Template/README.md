# STM32F103C8T6 · HAL + LL 工程模板

裸机 Makefile 工程模板, 不依赖 CubeMX / Keil / STM32CubeIDE。
驱动源码取自 ST 官方仓库并随工程分发: `stm32f1xx-hal-driver` **v1.1.10**（HAL+LL）、`cmsis_device_f1`、CMSIS 内核头, 许可证见各目录内 LICENSE。

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
├── User/          用户代码: main.c、时钟配置、stm32f1xx_it.c(中断)、
│                  stm32f1xx_hal_conf.h(HAL 模块开关)、启动文件、system
├── Hardware/      自己写的外设驱动模块 (xxx.c/xxx.h)
├── Drivers/       备用, 与 Hardware 同样会被自动搜索
├── Libraries/     STM32 HAL+LL 驱动、CMSIS —— 不要改动
├── Makefile       自动搜索 User/Drivers/Hardware 下所有源码, 新增文件无需改动
├── STM32F103C8T6.ld
└── .vscode/       IntelliSense 与构建任务
```

## 启用新外设

编辑 `User/stm32f1xx_hal_conf.h`, 取消对应 `HAL_XXX_MODULE_ENABLED` 的注释即可,
源文件已在 Makefile 全量编译, 不需要动 Makefile（未启用的模块会被链接器 `--gc-sections` 裁掉, 不占 Flash）。

LL 用法示例见 `User/main.c` 的注释（LL 大部分函数内联在头文件里, 直接 include 对应 `stm32f1xx_ll_xxx.h` 即可）。

## 板子假设

按 Blue Pill（STM32F103C8T6）预设: 8MHz 外部晶振 → PLL×9 → 主频 72MHz;
演示程序翻转 PC13 上的 LED（低电平点亮）。换板子主要改 `User/main.c` 的引脚和 `stm32f1xx_hal_conf.h` 的 `HSE_VALUE`。

## 更多

F1 专属坑（AFIO/重映射/JTAG 引脚）、中断回调规则、DMA 注意事项、编译烧录故障速查,
见仓库 `STM32/HAL/新手注意事项.md`（随 Template 复制时不自动携带, 建议工程目录里保留一份）。
