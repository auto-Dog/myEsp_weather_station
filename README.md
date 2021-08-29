# myEsp_weather_station


__/docs saves Static page for boardcasting display contents__<br>
Using esp8266 series board（e.g. esp01s）

_本项目参考太极创客团队的天气时钟项目，原作者[gitee地址](https://gitee.com/taijichuangke/bilibili_weather_clock)_

## features
#### 1. 更新信息后自动进入省电模式，增强续航
#### 2. 在第四页可以显示128x52图片，图片可以从个人的网页提供的json文件中获取
#### 3. 夜间低亮度模式
#### 4. 奈何没有粉丝≧ ﹏ ≦  所以把显示粉丝的地方改为显示生活建议：运动建议和晾衣服建议，数据同样从和风天气中获取，以图标方式展示适合、不适合

--------------------------------------------------------------------------
关于图片显示：

>为图片预留接口类似天气API，本质上是获取json文件
>
>该json文件中储存标签和图片取模后字符串（宽128\*高52），取模方式： __阴码，逆序，逐行__ 。该字符串必须是累计1664个 __十六进制数字的对应字符__ 。json文件中`status`标签决定是否显示该图，`meof`校验esp8266端接收完整性，保持`true`即可
>
>_[json示例](https://github.com/auto-Dog/myEsp_weather_station/blob/main/samplebmp.json)_
>
>.ino文件中建议直接配置json文件地址，或者能直接向esp8266提供json内容的地址，记得__关闭gzip__
> 
> 接收失败的情况下，第四页将显示一副X\~\~\~X的图片
>
> ~~图片大小问题：涉及到ArduinoJson中 `DynamicJsonDocument`空间分配问题. 64x32图片需要6144字节空间。当我尝试将空间进一步扩大来接收更大的图片数组，调试发现屡屡接收失败（一般是出现空间不足，初始化分配空间失败的问题）这个问题在多次接收较小尺寸（比如这里的64x32）图片也有出现。从json文件中获得的字符串是完整的，但在`DynamicJsonDocument`分配空间处开始出现bug，显示该对象的`capacity`为0~~
> 
