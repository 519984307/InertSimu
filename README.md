# InertSimulation
Inerting Simulation

## Build
### Paraview
Paraview should be built and added to the path.
Paraview build Link: https://gitlab.kitware.com/paraview/paraview/-/blob/master/Documentation/dev/build.md#windows
不使用cmake -GNinja 命令，直接利用cmake gui
使用vs2019打开paraview.sln，分别在debug和release模式下build，将生成的路径添加到环境变量中


### Qt
choose Qt MSVC2019 64bit to configure this project.

### change the path in config.json
Configure your own path in config.json, add it if not exist
This is a temporary file to config the absolute path for the test. It will change soon after the automatic path load function is finished.
* xmlPath: a sph xml file suitable to multiphase & inoutlet, already uploaded to this repository("CasePump_Def.xml").
* xmlSavePath: the path to save(include the name).
* Tank_config: 4 tank cases.
* sphBinPath: where the sph core is, the absolute path of "DualSPHysics_v5.0/bin/windows".
* sphWorkPath: current sph task path.
* sphOutPath: current sph task output path.
* sphBatName: windows script file to run sph core. it could change to the CPU version, but some features will be disabled to show.

### wCaseDambreak_win64_GPU.bat
Change this line.
```
echo StateCode-S4 : All done
```


## current work
### 界面
* 出入口界面布局的设置：列表式添加
* 出入口位置的定位
* 界面部件输入值的限制
* 汉化工作
### SPH任务
* CPU源码中 对数据流输出 强制刷新缓存
* 由工作目录的生成文件导入到渲染模块（主要为路径的设置）
* 异常生成后中止计算 
### 渲染视图
* 渲染模块自动显示（无需点击可见按钮）
* 对paraview的对象属性进行操作，实现导入物体的旋转
* 出入口位置的可视化
* 渲染对象的标签化

