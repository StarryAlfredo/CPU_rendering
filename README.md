## CPU_RENDER

这是用C++写的一个软渲染器，目前仅支持在windows上运行。

## 特性 V1.1

- **支持背面剔除**
- **支持齐次裁剪**
- **支持模型的读入和纹理的读入**
- **可编程的渲染管线**
- **Bllin-phong 反射模型**
- **正确的透视(三角形重心插值)**
- **支持线框模式**
- **颜色混合操作，透明物体的实现**
- **深度测试**
- **天空盒的实现**
- **支持双线性插值**（更平滑的纹理，解决纹理过小的问题）
- **支持法线贴图(模型更逼真的效果)**
- **支持阴影贴图**

### 后续待加入的效果

动画

ui界面

### 运行效果

天空盒：

![skybox](https://github.com/StarryAlfredo/CPU_rendering/blob/master/RunningEffectOfRender/skybox.PNG)

阴影贴图：

![shadowMap](https://github.com/StarryAlfredo/CPU_rendering/blob/master/RunningEffectOfRender/shadow.png)

双线性插值效果：

​	![Bilinear interpolation](https://github.com/StarryAlfredo/CPU_rendering/blob/master/RunningEffectOfRender/double.png)

齐次裁剪：

![clip](https://github.com/StarryAlfredo/CPU_rendering/blob/master/RunningEffectOfRender/clip.png)

材质:

![material](https://github.com/StarryAlfredo/CPU_rendering/blob/master/RunningEffectOfRender/material.png)







