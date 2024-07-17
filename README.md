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
├── example                       // 一些展示图片
├── executable
│   ├── example_protect.exe       // 用于演示加壳功能的程序，资源管理器中的进程名称为fake.exe，但运行的内容为realProc.exe 
│   ├── fake.exe                  // 傀儡程序，可独立运行，Protector.exe默认指定该程序为傀儡进程，即目标程序会套上fake.exe的名称运行
│   ├── PETools.exe               // PETool主体
│   ├── Protector.exe             // 加壳程序，用于实现具体的加壳功能，无法独立运行
│   └── realProc.exe              // 目标程序    
├── petools-src                   // PETool的源代码
└── protector-src                 // 加壳程序的源代码
```

## 效果展示

### PETool主界面
![PETool主界面](https://github.com/colaxianyu/PETool/blob/master/example/PETool_Main.png)

### PETool Section解析
![PETool Section解析](https://github.com/colaxianyu/PETool/blob/master/example/PETool_Section.png)

### PETool加壳界面
![PETool protect](https://github.com/colaxianyu/PETool/blob/master/example/PETool_Protect.png)

### 傀儡进程展示
![fake_process](https://github.com/colaxianyu/PETool/blob/master/example/fake.png)

## 开发人员

[colaxianyu](https://github.com/colaxianyu)

## 使用许可

[MIT](LICENSE) © Richard Littauer
