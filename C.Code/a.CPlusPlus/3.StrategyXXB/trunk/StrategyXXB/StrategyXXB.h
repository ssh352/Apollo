// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� STRATEGYDEMO_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// STRATEGYDEMO_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef STRATEGYDEMO_EXPORTS
#define STRATEGYDEMO_API __declspec(dllexport)
#else
#define STRATEGYDEMO_API __declspec(dllimport)
#endif
// �����Ǵ� StrategyDemo.dll ������
#include "AbstractStrategy.h"
using namespace Abstract;
extern"C" STRATEGYDEMO_API  AbstractStrategy* GetStrategyInstance();