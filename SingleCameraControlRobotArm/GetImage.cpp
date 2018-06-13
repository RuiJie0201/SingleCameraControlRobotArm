#include <boost/timer.hpp>
#include <opencv2/opencv.hpp> 
//#include <highgui.h>
#include "GetImage.h"

using namespace cv;
using namespace std;

/*
*  Ϊ�������ȡͼ������׼��
*  ���øú����󣬿���ʹ��GetImage��ȡͼ��
*/
void InitGetImages()
{
	OpenCamera();
	StartCamera();
	Sleep(500);
}

/*
*  ����Ҫ����ͼ��֮�󣬵��øú��������Թر����
*/
void EndGetImages()
{
	CloseCamera();
}


/*
*   ͼ��ɼ���ͼ��Ԥ����
*/
bool GetImage( DShowLib::Grabber::tMemBufferPtr &pRight, Mat& rightGray)  //����ͼ���������Ϣ
{

	if (!GetImageBuffer(&pRight))
	{
		printf("error in file:GetImageFromCamera.cpp  function:InitOpenCVmats\n");
		return false;
	}

	Size size(pRight->getBitmapInfoHeader()->biWidth, pRight->getBitmapInfoHeader()->biHeight);


	/*
	//��1��CV_8UC1---����Դ���----8λ�޷��ŵĵ�ͨ��---�Ҷ�ͼƬ------grayImg
           #define CV_8UC1 CV_MAKETYPE(CV_8U,1)
           #define CV_8UC2 CV_MAKETYPE(CV_8U,2)
	//��2��CV_8UC3---����Դ���----8λ�޷��ŵ���ͨ��---RGB��ɫͼ��---colorImg 
           #define CV_8UC3 CV_MAKETYPE(CV_8U,3)
	//��3��CV_8UC4--����Դ���-----8λ�޷��ŵ���ͨ��---��͸��ɫ��RGBͼ�� 
           #define CV_8UC4 CV_MAKETYPE(CV_8U,4)
    */
      
	Mat rightRGB = Mat(size, CV_8UC3);

	rightRGB.data = (uchar*)(pRight->getPtr());

    //  //����ͼ���С
	//Size ResImgSiz = Size(1280*0.5, 960*0.5);
	//resize(rightRGB, rightRGB, ResImgSiz, CV_INTER_CUBIC);

	//imshow("right", rightRGB);
	//waitKey(0);
	
	cvtColor(rightRGB, rightGray, CV_RGB2GRAY);
	
	//����ͼ���С
	//Size ResImgSiz = Size(1280*0.8, 960*0.8);
	//resize(rightGray, rightGray, ResImgSiz, CV_INTER_CUBIC);
	
	
	GaussianBlur(rightGray, rightGray, Size(25, 25), 0, 0);	//��˹�˲�
   
	threshold(rightGray, rightGray, 40, 255, THRESH_BINARY); //��ֵ��

	Canny(rightGray, rightGray, 80, 40, 3);		//canny��Ե ��ֵ��
	
	//imshow("left", leftGray);

	Mat rightsize;
	Size ResImgSiz = Size(1280*0.8, 960*0.8);
	resize(rightGray, rightsize, ResImgSiz, CV_INTER_CUBIC);
	imshow("right", rightsize);
	waitKey(0);
	
	return true;

}

/*
*   ���������ķ��䲻���������ȡ,��ȡ��־���Բ����������
*/

void AffInMoment(Mat & image, vector<Point2d> & extrMarks, Point2d & center)
{
	vector< vector<Point> >contours; 
	vector<Vec4i>hierarchy;     //ģ��
	CvBox2D box;
	unsigned int i;
	
	/*
	* Opencv��ͨ��ʹ��findContours�������򵥼�������Ϳ��Լ���������������ܷ��㡣
	
	* ��һ��������image����ͨ��ͼ����󣬿����ǻҶ�ͼ���������õ��Ƕ�ֵͼ��һ���Ǿ���Canny��������˹�ȱ�Ե������Ӵ�����Ķ�ֵͼ��
	
	* �ڶ���������contours������Ϊ��vector<vector<Point>> contours������һ��������������һ��˫��������������ÿ��Ԫ�ر�����һ����������Point�㹹�ɵĵ�ļ��ϵ�������
	              ÿһ��Point�㼯����һ���������ж�������������contours���ж���Ԫ�ء�
    
	* ������������hierarchy������Ϊ��vector<Vec4i> hierarchy����������һ��Vec4i�Ķ��壺
                  typedef    Vec<int, 4>   Vec4i;                                                                                                                                       
                  Vec4i��Vec<int,4>�ı�����������һ����������ÿһ��Ԫ�ذ�����4��int�ͱ�������������
                  ���ԴӶ����Ͽ���hierarchyҲ��һ��������������ÿ��Ԫ�ر�����һ������4��int���͵����顣
                  ����hiararchy�ڵ�Ԫ�غ���������contours�ڵ�Ԫ����һһ��Ӧ�ģ�������������ͬ��
                  hierarchy������ÿһ��Ԫ�ص�4��int�ͱ�������hierarchy[i][0] ~hierarchy[i][3]��
				  �ֱ��ʾ��i�������ĺ�һ��������ǰһ������������������Ƕ������������š�
				  �����ǰ����û�ж�Ӧ�ĺ�һ��������ǰһ������������������Ƕ�����Ļ�����hierarchy[i][0] ~hierarchy[i][3]����Ӧλ������ΪĬ��ֵ-1��
	
	*���ĸ������� int�͵�mode�����������ļ���ģʽ��
                  ȡֵһ��CV_RETR_EXTERNAL ֻ�������Χ��������������Χ�����ڵ���Χ����������
                  
				  ȡֵ����CV_RETR_LIST     ������е�������������Χ����Χ���������Ǽ�⵽�������������ȼ���ϵ���˴�֮�������û�еȼ���ϵ��
				                           �����ζ���������ģʽ�²����ڸ���������Ƕ����������hierarchy����������Ԫ�صĵ�3����4���������ᱻ��Ϊ-1���������Ļὲ��
                  
				  ȡֵ����CV_RETR_CCOMP    ������е�����������������ֻ���������ȼ���ϵ����ΧΪ���㣬����Χ�ڵ���Χ������������������������Ϣ������Χ�ڵ����������������ڶ���

                  ȡֵ�ģ�CV_RETR_TREE��   ������е�������������������һ���ȼ����ṹ��������������ڲ��������ڲ����������Լ���������Ƕ������
	*����������� int�͵�method�����������Ľ��Ʒ�����
                  ȡֵһ��CV_CHAIN_APPROX_NONE   ��������߽������������������㵽contours������
                  ȡֵ����CV_CHAIN_APPROX_SIMPLE �����������Ĺյ���Ϣ�������������յ㴦�ĵ㱣����contours�����ڣ��յ���յ�֮��ֱ�߶��ϵ���Ϣ�㲻�豣��
                  ȡֵ�����ģ�CV_CHAIN_APPROX_TC89_L1��CV_CHAIN_APPROX_TC89_KCOSʹ��teh-Chinl chain �����㷨

    *������������ Pointƫ���������е�������Ϣ�����ԭʼͼ���Ӧ���ƫ�������൱����ÿһ���������������ϼ��ϸ�ƫ����������Point�������Ǹ�ֵ��
	*/

	findContours(image, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);  
	for (i = 0; i < contours.size(); i++)           //contours.size()��ʾ������
	{
		Moments moment;
		moment = moments(contours[i], false);       //���������Ĳ����
		double u00 = moment.m00; double u01 = moment.m01; double u10 = moment.m10;
		double u20 = moment.mu20; double u11 = moment.mu11; double u02 = moment.mu02;
		double u30 = moment.mu30; double u21 = moment.mu21; double u12 = moment.mu12; double u03 = moment.mu03;  //ԭ�������ľ�
		
		double I1 = (u02*u20 - u11*u11) / pow(u00, 4);		//һ�׾�
		double I2 = (u30*u30*u03*u03 - 6 * u30*u12*u21*u03 + 4 * u30*pow(u12, 3) + 4 * u03*pow(u21, 3) - 3 * u21*u21*u12*u12) / pow(u00, 10);
		double I3 = (u20*(u21*u03 - u12*u12) - u11*(u30*u03 - u21*u12) + u02*(u30*u12 - u21*u21)) / pow(u00, 7);
		
		double Ia = abs(I1 - 0.006332) - 0.0003;   //�������ֵ�ж� ���׼�����ֵ�Ƚ�
		double Ib = abs(I2) - 0.00001;
		double Ic = abs(I3) - 0.00001;

		if (Ia <= 0 && Ib<0 && Ic<0)
		{
			box = fitEllipse(contours[i]);	//��Բ�������
			if (box.size.width>10 && box.size.height>10)
			{
				//Point2d center;
				center.x = int(box.center.x);		//ͼ�����ţ�ȡ����
				center.y = int(box.center.y);
				//cout << "  ��  " << center.x << " ��   " << center.y << endl;
				cout << "Find the center." << endl; //�ж��Ƿ���Բ��
				extrMarks.push_back(center);
				ellipse(image, box, Scalar(0, 250, 0), 0.2);	//������Բ
				circle(image, box.center, 1, Scalar(250, 0, 0), 0.01, 8);	//������Բ��
			}
		}
	}

}
