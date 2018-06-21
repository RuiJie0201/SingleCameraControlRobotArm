#include <iostream>
#include"GetImage.h"
#include"Calibration.h"
#include"kinematics.h"
#include"EyeToHand.h"
#include <windows.h>
#include "C:\TwinCAT\ADSApi\TcAdsDll\Include\TcAdsDef.h" //�ṹ��ͳ���������
#include "C:\TwinCAT\ADSApi\TcAdsDll\Include\TcAdsAPI.h" // ADS����������

/*
  ���侭�飺
  1����������󣬽��ϵ�����ڱ��䡣������ڴ��������������Ĵ���
  2�����������н�������ϵ�����ڱ������һ�䡣���߼�����
*/

/*
 ��Сʦ�ܴ����޸ģ�
  1���˶�ѧ�������
  2���ؽڿռ�PTP�����ζ���ʽ��double s
  3���ѿ����ռ䣺ֱ�ߡ�Բ����double s)
  4��������Сʦ��ѧϰ����GitHub��ѧ��ʹ��git
  5������TCP����������ϵ��
*/
using namespace cv;
using namespace std;

long nErr, nPort;				//����˿ڱ���
AmsAddr Addr;			//����AMS��ַ����

struct CartesianCoordinates
{
	double x;
	double y;
	double z;
	double a;		//��z����ת�Ƕ�
	double b;		//��y��
	double c;		//��x��
};

int Connect()
{
	nPort = AdsPortOpen();//��ADSͨѶ�˿�
	if (nPort == 0)
	{
		return -1;
	}
	AmsNetId AmsId = { 5, 39, 221, 128, 1, 1 };
	Addr.netId = AmsId;
	Addr.port = 0x8888;

	return 0;
}

int main()
{
	Mat rview, map1, map2;
	Mat cameraMatrix, distCoeffs;
	CartesianCoordinates target_cartesian;

	//double Ajoint[3];
	
	if (!Connect())
	{
		cout << "Connect success!" << endl;
	};

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
		Mat eye = (Mat_<double>(1, 3) << center.x, center.y, 196.97);//����һ��1��3�еľ��󡣵����ڵ��Ե�ʱ�򿴵��ľ�����Ϣ��1*3*1��Mat���������ʾ��?*��*��
		
		EyeToHand(eye, hand);
		
		cout << hand.at<double>(0, 0) << " " <<hand.at<double>(0, 1) << endl;

		//kinematics_inverse(hand.at<double>(0, 0), hand.at<double>(0, 1), 0, Ajoint);   //x,y,z
		//cout << Ajoint[0] << "," << Ajoint[1] << endl;
		target_cartesian.x = hand.at<double>(0, 0);
		target_cartesian.y = hand.at<double>(0, 1);
		AdsSyncReadWriteReq(&Addr, 0x02, 0x01, 0, NULL, sizeof(CartesianCoordinates), &target_cartesian);

	}
	return 0;
}