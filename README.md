# SUSTech CS301 Project

**Our Project Final Grade: (110/100)**

## 目录结构

此目录分为两个部分，`CUBE_IDE`和`CMAKE`，顾名思义为对应编译器的文件夹。

- `Docs`： 包含了本次项目的项目需求和报告，此文件夹下 `Readme.md` 文件就是本次项目 Markdown 格式的报告。**若需快速了解本项目，推荐首先阅览 [Docs 文件夹](Docs/)。**

* `CMAKE`使用VSCode打开，建议安装插件`clangd`, `CMake`, `CMake Tools`, `STM32 VS Code EXtension`, 参考url: [CSDN连接](https://blog.csdn.net/weixin_62701327/article/details/131744874)，我已经成功import了项目，因此只需要右键`CmakeList.txt` 选择Configure All project，会为你生成你目录下的`build`（大家的`build/`不通用，因此被ignore掉了）
* `CUBE_IDE`是双生STM32 Cube IDE的版本，不包含混编代码，当我们需要模块的时候，可以从CUBE IDE打开，然后复制粘贴/重构代码以获取新模块
* P.S. 所有的模块均可通过非Cube IDE方式生成并启用，正点原子官方啊HAL库就是这么做的
* `.clang-format`为cpp语言规范，clangd会自动识别
* `.vscode/`包含了VSCode可以成功编译/处理项目的配置（主要为Clangd）

## 框架

已有功能：自动LED（未修改为Timer），Framework框架，LCD和触控，IIC...

### Framework

所有的框架调用均在`frameworks/framework.cpp`它提供了基础的功能模块：按钮，可输入文本框， 键盘，静态文字，场景组件（需要自己编写）

* 组件之间存在树状结构，`dpo::sub_object`即为子组件vector
* 组件仅当`need_render`为true时刷新，刷新时会覆盖原来位置组件，因此刷新策略为刷新组件及其子组件，刷新后会自动置false
* 仅当`isVisible`为True时，组件拥有功能
* 组件之间的pos为相对其父组中心的位置，我们显示屏的大小为`(240,360)`

* 初始屏幕组建为`canvas`，已经实例化在main里，是组件树的根

### 场景组件

场景均由Class组成，基于dpo（display_object）我们需要重写其`update`来编写组件操作逻辑，并且在`framework.cpp`中的update部分增加其实例化的update方法（参考以前）