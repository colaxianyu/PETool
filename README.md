## 简介

PETool是一个PE文件解析工具，用于解析windows系统下PE文件的格式信息

除了支持PE格式的解析功能外，还支持简单的导入表注入功能和加壳功能

PETool基于c++20开发，请确保你的编译器支持c++20 ()

## 功能

|功能|描述|
|--|--|
|PE文件解析|用于解析windows系统下PE文件的格式信息|
|导入表注入|通过修改目标程序的导入表，将自定义的dll添加到目标程序的导入表中，使目标程序运行时将自定义的DLL加载到自身的进程空间中|
|加壳|首先将原始程序的表信息移动到一个新增的Section中，然后按位取反，再将修改后的原始程序信息添加到一个傀儡程序中，傀儡程序会完成解密工作并建立新的进程以运行原始程序|

## 文件介绍

```c++
PETool
├── executable
│   ├── fake.exe          // 一个加壳后的程序，用于演示加壳的功能
│   ├── PETools.exe       // PETool主体
│   ├── Protector.exe     // 傀儡程序，作为壳源
│   └── realProc.exe      // 目标程序    
├── petools-src           // PETool的源代码
└── protector-src         // 傀儡程序的源代码
```


## 开发人员

[colaxianyu](https://github.com/colaxianyu)
