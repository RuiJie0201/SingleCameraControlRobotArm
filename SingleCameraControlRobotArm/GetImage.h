/*
* ��ȡ���ͼ�񣬲�ת��Ϊopencv��ʽ
* �������ͼƬ��ʽ����ʼ��opencvͼ�����ʽ
* 2018.4.9
* Author��LEO
*/

#pragma warning( disable : 4996)
#ifndef _GETIMAGE_H_
#define _GETIMAGE_H_

#include <opencv2\opencv.hpp>
//#include <cv.h>
#include <Windows.h>
#include <iostream>
#include <vector>
#include "Camera.h"

using namespace cv;

/*
*  ��ȡͼ��
*/

//   ͼ��ɼ���ͼ��Ԥ����
bool GetImage(DShowLib::Grabber::tMemBufferPtr &pRight, Mat& rightGray);   //����ͼ���������Ϣ

//   ���������ķ��䲻���������ȡ,��ȡ��־���Բ����������
void AffInMoment(Mat & image, vector<Point2d> & extrMarks, Point2d &center);

/*
* Ϊ�������ȡͼ������׼��
* ���øú����󣬿���ʹ��GetImage��ȡͼ��
*/
void InitGetImages();

/*
* ����Ҫ����ͼ��֮�󣬵��øú��������Թر����
*
*/
void EndGetImages();


#endif

