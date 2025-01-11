# DeskBuddy（知饮知座）

基于ESP32的物联网智能桌面伴侣。

> [!NOTE]
> 这是一个正在开发的仓库，具体情况请查看下方的开发计划和完成情况。
> 
> 也可在[Notion笔记](https://systemannounce.notion.site/DeskBuddy-1423a31c61c5808896e3cd2a04a4a249)界面查看实时开发状况（此进度比GitHub仓库同步进度更细更清晰）

> | Key    | Value                                        |
> | ------ | -------------------------------------------- |
> | Board  | ESP32-DevKitC                                |
> | Module | ESP32-WROOM-32E                              |
> | Chip   | ESP32-D0WD-V3                                |
> | IDE    | Platform IO <br />Espressif ESP32 Dev Module |

# 介绍

### 作者的话

因为各种各种的原因，导致了这个项目的诞生，因为本人经常废、寝、忘、食，所以为了抓住消费者（我花钱买设备做项目）的钱包，我们眼中有目标，心中有立项。

也为了某些真的有需要的人（其实是为了做个能拿得出手的项目），做了一个桌面伴侣，具体功能可以看下面。

总之，我会把这个项目给做出来，不知道会遇到多大的阻力，不过，如果真的阻力很大，我会好好躺平（写代码）的！！请大家放心。

### 功能

> 简单来说，就是一个物联网的桌面上久坐和喝水提醒的桌面小摆件。

1. **HC-SR501**：人体红外感应电子模块；功能嘛，就是久坐的人体检测啦。
2. **HX711**：电子秤压力传感器；唔，简单来说，是个……杯垫。
3. **彩色LCD/OLED显示屏**：显示系统的各种状态；考虑接入个天气API和NTP服务器顺带显示天气和当前时间。
4. **三色LED模块**：状态提示LED；我已经在做最大的精简了，别再说我的成品臃肿了…………
5. **光敏电阻**：检测周围的环境光；防止低亮度用眼。
6. **ESP32-WROOM-32E**：ESP32单片机主控
   * 实现最主要的逻辑和程序处理
   * NVS持久的数据存储
   * RTOS系统
   * 接入Wi-Fi、蓝牙的连接实现手机控制
     * 实现OTA升级，多固件云端切换和远程更新
     * 微信小程序连接控制（暂时不打算开源）
     * 远程监视，手动发送提醒统计，久坐次数，喝水次数等。

---

7. 语音控制（评估中，尚未得知是否支持）

8. 语音播报（评估中）
9. 语音留言（评估中）
10. 低头提醒（探索中，暂时不知道要怎么检测头部离桌面或者屏幕过近）
11. 每天喝水量(ml)统计（探索中，因为喝水和倒水情况实在太多了，不知道怎么处理好）
12. 更多功能正在思考中……

### PINS

![](https://images.systemannounce.com/i/1/2024/11/esp32-devkitC-v4-pinout.webp)

<table>
    <tr>
    	<td>Module/Sensor</td>
        <td>FUNCTION</td>
        <td>PIN NAME</td>
    </tr>
    <tr>
    	<td>HC-SR501</td>
        <td>OUT</td>
        <td>34</td>
    </tr>
    <tr>
    	<td rowspan="2">HX711</td>
        <td>DT</td>
        <td>32</td>
    </tr>
    <tr>
    	<td>SCK</td>
        <td>33</td>
    </tr>
    <tr>
    	<td rowspan="6">LCD/OLED</td>
        <td>SCL</td>
        <td>18</td>
    </tr>
    <tr>
        <td>SDA</td>
        <td>23</td>
    </tr>
        <td>RES</td>
        <td>15</td>
    </tr>
        <td>DC</td>
        <td>2</td>
    </tr>
        <td>CS</td>
        <td>5</td>
    </tr>
        <td>BL</td>
        <td>17</td>
    </tr>
    <tr>
    	<td rowspan="3">LED</td>
        <td>R</td>
        <td>12</td>
    </tr>
    <tr>
        <td>G</td>
        <td>14</td>
    </tr>
    <tr>
        <td>B</td>
        <td>27</td>
    </tr>
    <tr>
        <td rowspan="2">光敏电阻</td>
        <td>AO</td>
        <td>36 (VP)</td>
    </tr>
    <tr>
        <td>DO</td>
        <td>39 (VN)</td>
    </tr>
</table>

### 微信小程序

> 开发中，暂时不打算开源

![](https://images.systemannounce.com/i/1/2024/11/image-20241124163102796.webp)

# 开发计划（完成情况）

> [!WARNING]
> 因为基本上所有的模块和传感器都没有提供ESP32的代码，得做移植，不知道到时候的情况怎么样。

- [x] ~~HC-SR501适配~~
- [x] ~~HX711适配~~
- [x] ~~光敏电阻适配~~
- [x] ~~彩色LCD/OLED显示屏的选型~~
- [x] ~~彩色LCD/OLED显示屏的代码移植~~
- [ ] 显示屏UI制作
- [x] ~~(Canceled)显示屏天气图标取模制作~~
- [ ] ESP32-WROOM-32E物联网部分程序
  - [x] ~~Wi-Fi和MQTT服务器~~
  - [ ] HTTP客户端
  - [ ] 蓝牙部分
  - [x] ~~NVS数据存储~~
  - [ ] RTOS适配➕重构
  - [ ] OTA升级
    - [x] ~~HTTP网络升级~~
    - [ ] 多固件远程切换
    - [x] ~~(Canceled)蓝牙固件升级~~
- [ ] 微信小程序整体代码（暂不开源）
  - [ ] 远程监视
  - [ ] 数据统计
  - [ ] 手机提醒
  - [ ] 远程发送提醒
  - [ ] 多固件控制切换

---

#### 评估中

> [!IMPORTANT]
> 如果下方的内容有你熟悉的项目希望可以[帮助作者](mailto:felixsana@systemannounce.com)，谢谢了。

- [ ] 语音控制（暂时不知道选用什么模块，芯片，怎么做语音识别）
- [ ] 语音播报（暂时不知道选用什么模块，芯片，怎么做）
- [ ] 语音留言（同上，如果知道怎么播报什么的就好办了）
- [ ] 低头提醒（不知道怎么检测低头/离屏幕过近）
- [ ] 每天喝水量(ml)统计（因为不同的人有不同的喝水情况，很难量化）
