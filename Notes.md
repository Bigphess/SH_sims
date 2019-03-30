# input
## intput from vedio
VideoCampure cap(0) -> 打开照相机然后捕捉图像
while(1) 里面处理这个图像 cap>>frame。这个frame的type是Mat，然后建议在输入之后直接写新的函数处理这个mat
  

在while里面设置特殊条件的方法：waitKey(1) != 'q'
  


#### CalcCentroid：计算图形的中心（像素值）

	//Calculates all of the moments up to the third order of a polygon or rasterized shape.
		cv::Moments moment = cv::moments(binarizedImage, true);



# SH_sims
A demo for OPENCV practice in 2018
## Plan
* bulid a superhero simulator
* 通过手势识别，判断用户手势，让加入不同的~超级英雄~ smash bro贴图
## 流程
### 输出图像
* 图像大小 720✖️1280
* ~是否存在延迟问题？~ （只显示一张图的话是没问题的）
### 手势识别
#### 识别类型
* ~thonas？ 响指 是否可以参考手势数字识别里面的7？~
* ~spidy？ 喷射蛛丝~
* ~ironman？ 手掌 是否可以参考手势数字里面的5~
* 直接往手上加图片的难度太大，改成往指尖上面加图片
#### 问题
* ~是否需要机器学习？~
* 哪个手势方便识别 -> 可以识别正面的手，需要打强光，手背面不是很好识别
* ~如果加入脸的部分的识别的话，是否直接在脸上加个人物贴图？~
#### 过程
1.背景分割 （避免光效问题。前一帧和后一帧差别不大的地方直接remove掉
* 计算两frame之间的差，不合适，因为手的运动范围比较小
* ~背景差分法，和一个已知的模板进行对比~ **已完成**

~2.肤色检测（颜色阈值？如何计算阈值）~ **已完成，取手上两块，计算颜色阈值**

~3.人脸检测 ？Author: Pierfrancesco Soffritti https://github.com/PierfrancescoSoffritti~  **已完成，在脸上加上了一个黑块，或者可以考虑不检测脸的部分**

~4.手势检测~ **已完成，根据手的convex的far点来判断手指的根数**

~5.判断~ **可判断不同的手指的数量**
### 添加贴图
* 去哪找动画 -> 截取超级英雄（smash bro怎么样？）里面的人物Q版（五个？更多？ -> 使用png图片，定位到手指尖的位置
* ~是否可动？~
* ~如何给贴图准确定位 -> 定位到手指尖的位置~

### 优化问题
* 代码速度优化 -> 只输出一张图就没事了
* 代码书写方法优化
* 分割手的过程中近一步优化 -> 换个环境就好了
* 找到相应的png动画 完成
* 对应不同手势显示不同动画 -> 随机数实现
* 推迟显示时间的问题



