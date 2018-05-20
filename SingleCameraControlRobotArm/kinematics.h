#ifndef _KINEMATICS_H_
#define _KINEMATICS_H_


//#include <Fpu87.h>

#ifndef PI
#define PI 3.14159265358979
#endif

#include "math.h"
#define sin_ sin
#define cos_ cos
#define acos_ acos
#define pow_ pow
#define sqrt_ sqrt
#define atan2_ atan2
#define fabs_ fabs


/************************************************
* �˶�ѧ����
* R[3][3]				3��3��ת����
* X[3]					�ѿ����ռ�xyzλ��(��λ��mm)
* joint[3]				�ؽڽǶȣ���λ���㣩
* kps[3]				��Чת��3������ת��
* Kine_Forward			�˶�ѧ���⣨��֪joint[3] -> ���X[3]��R[3][3]��
* Kine_Inverse			�˶�ѧ���⣨��֪X[3]��R[3][3] -> ���joint[3]��
* **********************************************/


void kinematics_forward(double* pos, double* r);
void kinematics_inverse(double x, double y, double phi, double* Ajoint);


#endif         /* KINEMATICS_H_ */