#pragma once
#include "ThostFtdcUserApiStruct.h"
#include <string>
#include <vector>
#include "AbstractStoreObject.h"
#include <thread>
using namespace std;
//����Ĳ�������
typedef unsigned char byte;
namespace Abstract {
	/*����Ĳ����־û��࣬ÿ�������Լ�ʵ��*/
	class AbstractStrategy;
	class BASELIB_API AbstractParameterPersistencer {
	private:
		friend class Abstract::AbstractStrategy; //��Ԫ
		//int strategyid;
		void SetStrategyID(int strategyid);
	protected:
		 int strategyid;
	 public:
		virtual void AddOrUpdateParameter(string name, double value) = 0; //�����ַ����͵Ĳ���
		virtual double GetParameter(string name) = 0;
		virtual void AddOrUpdataParameter(string name, shared_ptr<byte>,int sizeofobject)=0;
		virtual shared_ptr<byte> GetObjectParameter(string name,int sizeofobject)=0;
		virtual void AddOrUpdateArrayElement(string arrayname,string elementid, shared_ptr<byte> element,int sizeofobject) = 0;
		virtual vector<pair<string, shared_ptr<byte>>> GetArray(string name,int sizeofelementobject) = 0;
		virtual void DeleteArrayElement(string arrayname,string elementid) = 0;
		//virtual void AddOrUpdateParameter(string name, string value) = 0;//����ַ����͵Ĳ���
		//virtual string GetSParameter(string name) = 0;   //�����ַ����͵Ĳ���
	};

}