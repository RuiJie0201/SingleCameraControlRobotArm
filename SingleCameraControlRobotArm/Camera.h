#pragma once                     //��֤ͷ�ļ�������һ��
#pragma warning (disable:4996)   //��4996������ΪʧЧ

#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <memory>
#include "tisudshl.h"

using namespace _DSHOWLIB_NAMESPACE;

bool OpenCamera();
bool StartCamera();
void SuspendCamera();
void CloseCamera();


bool GetImageBuffer(MemBufferCollection::tMemBufferPtr *buffer);

bool openDeviceFromFile(_DSHOWLIB_NAMESPACE::Grabber& gr, const std::string& devStateFilename);