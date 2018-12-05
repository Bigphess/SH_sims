# SH_sims
A demo for OPENCV practice in 2018
## Plan
* bulid a superhero simulator
* 通过手势识别，判断用户手势，让加入不同的超级英雄贴图
## 流程
### 输出图像
* 图像大小 720✖️1280
* 是否存在延迟问题？
### 手势识别
#### 识别类型
* thonas？ 响指 是否可以参考手势数字识别里面的7？
* spidy？ 喷射蛛丝
* ironman？ 手掌 是否可以参考手势数字里面的5
#### 问题
* 是否需要机器学习？
* 哪个手势方便识别
* 如果加入脸的部分的识别的话，是否直接在脸上加个人物贴图？
#### 过程
1.背景分割 （避免光效问题。前一帧和后一帧差别不大的地方直接remove掉
* 计算两frame之间的差，不合适，因为手的运动范围比较小
* 背景差分法，和一个已知的模板进行对比
2.肤色检测（颜色阈值？如何计算阈值）
3.人脸检测 ？Author: Pierfrancesco Soffritti https://github.com/PierfrancescoSoffritti
4.手势检测
5.判断
### 添加贴图
* 去哪找动画
* 是否可动？
* 如何给贴图准确定位
