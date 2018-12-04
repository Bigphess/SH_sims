# input
## intput from vedio
VideoCampure cap(0) -> 打开照相机然后捕捉图像
while(1) 里面处理这个图像 cap>>frame。这个frame的type是Mat，然后建议在输入之后直接写新的函数处理这个mat
  

在while里面设置特殊条件的方法：waitKey(1) != 'q'
  


#### CalcCentroid：计算图形的中心（像素值）

	//Calculates all of the moments up to the third order of a polygon or rasterized shape.
		cv::Moments moment = cv::moments(binarizedImage, true);