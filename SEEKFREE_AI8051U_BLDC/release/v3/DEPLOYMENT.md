# AI8051U 电调 V3 构建与部署手册

## 1. 版本身份

- 仓库基线：`b5e266090f929a5948bd55cfa0ea689b0af81e7d`
- 实施分支：`codex/ai8051u-esc-v3-response`
- 最终候选：`03_mdu32_final/SEEKFREE.hex`
- V2 回退：`rollback_v2_b5e2660/SEEKFREE.hex`
- IDE：μVision 5.25.3.0
- 工具链：PK251/C251 5.60.0.0
- 烧录工具：AiCube-ISP-v6.96V-plus 或更高版本；本机可用安装为 STC-ISP v6.96。

提交完成后，将三个原子提交号补录到各阶段 `MANIFEST.md`；不得仅凭文件名识别待烧录固件。

## 2. 构建

Keil 目标必须保持：

- Target：`AI8051U_32bit`
- Device：`STC8051U-32Bit Series`，寄存器文件 `AI8051U.H`
- 系统时钟：35 MHz
- C251：XSMALL、`OPTIMIZE(7,SPEED)`、`NOALIAS`
- Global Register Coloring：关闭
- HSPWM、TFPU、PWM DMA：不启用

在 `project/mdk` 下执行全量构建：

```powershell
& "D:/keil5-C251-5.60/UV4/UV4.exe" -r "seekfree.uvproj" -t "AI8051U_32bit" -j0
```

构建日志必须包含 `0 Error(s), 0 Warning(s)`。随后执行：

```powershell
& "../../tests/verify_commutation_math.ps1"
& "../../tests/verify_hotpaths.ps1"
Get-FileHash -Algorithm SHA256 "out_file/SEEKFREE.hex"
```

最终 HEX 的 SHA-256 必须为：

```text
6434ADBD658E3C991DA358A00B2FBC31E3D8B88EAD52C4C40E5954E50714F865
```

## 3. 烧录接线

烧录前拆桨并断开电池。下载口只连接三根线，不接 USB 转 TTL 的 VCC：

- USB 转 TTL GND -> 电调 GND
- USB 转 TTL RX -> 电调 TX
- USB 转 TTL TX -> 电调 RX

在 AiCube-ISP 中选择 `AI8051U`、正确 COM 口、35 MHz，并打开已校验的 HEX。点击“下载/编程”，界面进入等待检测后再接通 3S 电池；显示操作成功后断开电池和串口。

PWM 三针接口为信号、5V 输出和 GND。外部 PWM 控制器只连接信号和共地；板上 5V 是对外输出，禁止从外部向该脚灌入 5V。电机 A/B/C 分别连接三相，换向时只交换任意两相。电池只允许 3S。

## 4. 首次上电顺序

1. 不接电机，确认 HEX 哈希、芯片型号和 35 MHz 选项。
2. 拆桨接电机，在 9.5 V 限流电源下以 1000 us、50 Hz 上电，确认保持停转且 MCU 无复位。
3. 依次执行输入边界、75 ms 失联、冷/热启动、快速升降油和 GPIO ISR 时序测试。
4. 在 9.5、11.1、12.6 V 完成拆桨台架测试后，才允许装桨并进入飞行 A/B。
5. 任一步出现失步、异常重启、空载电流明显升高或门槛失败，立即断电并按阶段 HEX 二分。

## 5. 回退 V2

回退文件固定为 `rollback_v2_b5e2660/SEEKFREE.hex`，SHA-256：

```text
D5C037935CCC8E1A4AA40E9A2B47F71473DB68922C3DD7EF487A6F09AA2E794D
```

按照第 3 节相同接线和断电顺序烧录该文件。回退必须使用归档 HEX，不得现场重新编译 V2。烧录完成后先拆桨验证 1000 us 停转、有效油门启动和失联停车，再恢复后续测试。

## 6. 安全边界

本固件没有相电流、温度、母线过压、高侧 VGS 或硬件过流测量。反电势闭环仅控制换相，油门百分比不对应固定 RPM。离线构建和 ISR 汇编门禁不能替代功率级测量，也不能作为适航证明。
