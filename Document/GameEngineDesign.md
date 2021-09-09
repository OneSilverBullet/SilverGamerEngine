# 引擎设计指引

## Shader Effect处理
### 概念定义
* Render Pass: Render Pass是一组渲染处理命令。这组命令用于渲染拥有相同贴图的一组几何体，同时GPU程序在一个特定rendertarget上执行。
* Local Multipassing: Local Multipassing是一个过程，是使用不同的Render Pass对相同几何体进行多次渲染的过程。
* Global Multipassing：Global Multipassing也是一个过程，是使用不同Render Pass对场景中所有几何体进行多次渲染的过程。

### shader部分处理需求
* 快速原型：一种新的图形方案能够快速集成。
* 接口清晰：需要清晰的调用接口。

### shader管理方式
* Individual program：vertex和fragment都分开进行管理。这种方式简洁清晰，但是复用性极差。
* Effect Frameworks:vertex和fragment shader都被整合到同一个effect文件当中。非常适合以render pass为单位的渲染。
* Uber Shader:将所有shader文件写入一个文件中，用宏与流控制来调整最终shader的渲染效果。其渲染效率极高。
* Micro Shader:与Uber Shader相反的解决方案。其将shader分为一个个细微的代码片段，然后程序将其进行整合。
* Abstract Shader Tree: 类似虚幻的材质图系统。
