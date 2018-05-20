#include <iostream>
#include"GetImage.h"
#include"Calibration.h"
#include"kinematics.h"
#include"EyeToHand.h"


/*
  ���侭�飺
  1����������󣬽��ϵ�����ڱ��䡣������ڴ��������������Ĵ���
  2�����������н�������ϵ�����ڱ������һ�䡣���߼�����
*/
using namespace cv;
using namespace std;

int main()
{
	Mat rview, map1, map2;
	Mat cameraMatrix, distCoeffs;

	double Ajoint[3];


	//EyeToHand(rview, rview);
	//getchar();
	//return 0;
	
	InitGetImages();

	while (1)
	{
		static DShowLib::Grabber::tMemBufferPtr pLeft;
		static DShowLib::Grabber::tMemBufferPtr pRight;
		Mat leftGray, rightGray;
		vector<Point2d> leftExtrMarks;
		vector<Point2d> rightExtrMarks;
		//����֮ǰ
		GetImage(pRight,rightGray);
		//AffInMoment(rightGray, rightExtrMarks);
        //����֮��
		init_undistort(cameraMatrix, distCoeffs, rightGray.size(), map1, map2);

		remap(rightGray, rview, map1, map2, INTER_LINEAR);	//����֮��ͼƬ�ߴ��ԭͼһ��

		cv::imwrite("output.bmp", rview);
	
		Point2d center;
		AffInMoment(rview, rightExtrMarks, center);

		cout << "  ��  " << center.x << " ��   " << center.y << endl;

		Mat hand;
		Mat eye = (Mat_<double>(1, 3) << center.x, center.y, 282.1969697);//����һ��1��3�еľ��󡣵����ڵ��Ե�ʱ�򿴵��ľ�����Ϣ��1*3*1��Mat���������ʾ��?*��*��
		
		EyeToHand(eye, hand);

		kinematics_inverse(hand.at<double>(0, 0), hand.at<double>(0, 1), 0, Ajoint);   //x,y,z
		cout << Ajoint[0] << "," << Ajoint[1] << endl;

	}
	return 0;
}