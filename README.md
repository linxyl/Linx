### 测试环境：
VisualStudio 2022
Qt 5.14.2 MinGW 64-bit
Linux

123
342342
234234

### 工程配置：
##### Qt：
在.pro文件下添加

```
CONFIG += c++17
DEFINES -= UNICODE
LIBS += -lws2_32
```

