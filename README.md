# InertSimulation
Inerting Simulation

## Build
### Paraview
Paraview should be built and added to the path.
Paraview build Link: https://gitlab.kitware.com/paraview/paraview/-/blob/master/Documentation/dev/build.md#windows

### Qt
choose Qt MSVC2019 64bit to configure this project.

### change the path in config.json
This is a temporary file to config the absolute path for the test. It will change soon after the automatic path load function is finished.
* xmlPath: a sph xml file suitable to multiphase & inoutlet, already uploaded to this repository("CasePump_Def.xml").
* xmlSavePath: the path to save(include the name).
* sphBinPath: where the sph core is, the absolute path of "DualSPHysics_v5.0/bin/windows".
* sphWorkPath: current sph task path.
* sphOutPath: current sph task output path.
* sphBatName: windows script file to run sph core. it could change to the CPU version, but some features will be disabled to show.

### wCaseDambreak_win64_GPU.bat
Change this line.
```
echo StateCode-S4 : All done
```


## .gitignore  !important
Exclude any local build files or specific configuration to this repository. Use this command to ignore the config.json
```
git update-index --assume-unchanged SPHResource.qrc
git update-index --assume-unchanged config.json
```


## current work
### 界面
* 出入口界面布局的设置：列表式添加 unfinished
* 出入口位置的定位 unfinished
* 界面部件的命名, 界面水平自适应
* 界面部件与SPH参数实体的显示，回调
* 确保界面所有按钮点击后，成功有变化，不成功有提示.
* 界面部件输入值的限制. unfinished
* 实体变量类型均采用Float，在保存时精度有偏差，改成Double仍存在该问题 .
* STL的导入
### SPH任务
* 合并 启动SPH计算与数据流的显示
* SPH计算的（删除继续、后处理、取消）处理
* CPU源码中 对数据流输出 强制刷新缓存 unfinished
* SPH源码中注入 阶段码，修改匹配方式，减少匹配开销
* SPH源码中注入 异常码，修改匹配方式，减少匹配开销
* SPH计算时按钮状态的设置
* 由工作目录的生成文件导入到渲染模块（主要为路径的设置）
* 异常生成后中止计算 .
### 其他功能
* SPH数据流新增对异常码的匹配
* 渲染模块自动显示（无需点击可见按钮） . unfinished
* Euler angles of XYZ axis rotations(degrees) 相互转换 Axis-angle(degrees)的代码支持
* 对paraview的对象属性进行操作，实现导入物体的旋转 unfinished
* 

