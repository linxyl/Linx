### 测试环境：

VisualStudio 2022

Qt 5.14.2 MinGW 64-bit

Linux g++



### 工程配置：
##### Qt：
在.pro文件下添加

```
CONFIG += c++17
DEFINES -= UNICODE
LIBS += -lws2_32
```

##### Linux:

添加选项

```shell
-std=c++17
-lpthread
-latomic
```

