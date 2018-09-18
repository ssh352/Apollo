#include "stdafx.h"
#include "StrategyDeviate.h"


StrategyDeviate::StrategyDeviate()
{
	ReadCSVFile("���������Լ�����Ϣģ����.csv");

	GetPara();
	this->paralist = m_vecPara;
	this->strategyid = 3000;
	this->strategyid_str = "3000";
	this->subscribelist = m_listInstructmentID;
	/*iOrderNum = 30000000;
	iOrderQue = 30000000;*/

	bTest = false;
	dTotalMoney = 100000.00;
	iPosition = 1;

	//................������.....................//
	bTest = false;
	iTestTime = 0;

	m_dWinStop = 0.005;

	m_dDeviateRatio = 0.03;
	m_dBailRatio = 0.12;
	m_dAddRatio = 0.0066;

	this->strategyname = "���Ϲ�ͷ����";
	iCountTime = 0;

}


StrategyDeviate::~StrategyDeviate()
{
}

//��ȡCSV�ļ�
void StrategyDeviate::ReadCSVFile(CString str) {
	CFile SourceFile;   //�����ļ�

	CString SourceData;  //������ʱ��������һ����¼;
						 //ֻ����ʽ���ļ�;						 //BeginWaitCursor();
	SourceFile.Open(str, CFile::modeRead);
	int i = 0;
	//CArchive���������ļ�;
	CArchive ar(&SourceFile, CArchive::load);
	CString temp;

	//ѭ����ȡ�ļ���ֱ���ļ�����
	int iStart;
	CRecordData tmpRecordData;
	CString strInstrucmentID, strLimitPrice, str2Price, strPrice, strLever, strSQS, strRange;
	CString strTmp;
	double dLimitPrice;
	while (NULL != ar.ReadString(SourceData))
	{
		i = i + 1;
		iStart = 0;
		if (i > 1)  //������һ���ļ�ͷ��
		{
			//������Լ���룻
			tmpRecordData = CRecordData::CRecordData();

			//��Լ���ƣ�
			iStart = SourceData.Find(',', iStart);

			//��Լ����
			strInstrucmentID = SourceData.Mid(iStart + 1, SourceData.Find(',', iStart + 1) - iStart - 1);
			iStart = SourceData.Find(',', iStart + 1);

			m_listInstructmentID.push_back(strInstrucmentID.GetBuffer(0));
			strInstrucmentID.ReleaseBuffer();
			//������ͣ�ۣ�
			strLimitPrice = SourceData.Mid(iStart + 1, SourceData.Find(',', iStart + 1) - iStart - 1);
			tmpRecordData.m_dRasingLimit = atof(strLimitPrice.GetBuffer(0));
			iStart = SourceData.Find(',', iStart + 1);
			strLimitPrice.ReleaseBuffer();

			//����
			strTmp = SourceData.Mid(iStart + 1, SourceData.Find(',', iStart + 1) - iStart - 1);
			iStart = SourceData.Find(',', iStart + 1);

			//����
			strTmp = SourceData.Mid(iStart + 1, SourceData.Find(',', iStart + 1) - iStart - 1);
			iStart = SourceData.Find(',', iStart + 1);

			//�����۸�
			str2Price = SourceData.Mid(iStart + 1, SourceData.GetLength());
			tmpRecordData.m_dPrice2T = atof(str2Price.GetBuffer(0));
			iStart = SourceData.Find(',', iStart + 1);
			str2Price.ReleaseBuffer();

			//һ���۸�
			strPrice = SourceData.Mid(iStart + 1, SourceData.GetLength());
			tmpRecordData.m_dPrice1T = atof(strPrice.GetBuffer(0));
			iStart = SourceData.Find(',', iStart + 1);
			strPrice.ReleaseBuffer();

			//�ܸ�
			strLever = SourceData.Mid(iStart + 1, SourceData.GetLength());
			tmpRecordData.m_iLever = atoi(strLever);
			iStart = SourceData.Find(',', iStart + 1);

			//�Ƿ�����������
			strSQS = SourceData.Mid(iStart + 1, SourceData.GetLength());
			tmpRecordData.m_iShQS = atoi(strSQS);
			iStart = SourceData.Find(',', iStart + 1);

			//����������ϵķ��ȣ�
			strRange = SourceData.Mid(iStart + 1, SourceData.GetLength());
			tmpRecordData.m_dDown2UpRange = atof(strRange);
			iStart = SourceData.Find(',', iStart + 1);

			//�������ļӲַ��ȣ�
			strRange = SourceData.Mid(iStart + 1, SourceData.GetLength());
			tmpRecordData.m_dDown2UpAddRatio = atof(strRange);
			iStart = SourceData.Find(',', iStart + 1);

			//��������ֹӯ���ȣ�
			strRange = SourceData.Mid(iStart + 1, SourceData.GetLength());
			tmpRecordData.m_dDown2UpWinStop = atof(strRange);
			iStart = SourceData.Find(',', iStart + 1);

			//����������µķ��ȣ�
			strRange = SourceData.Mid(iStart + 1, SourceData.GetLength());
			tmpRecordData.m_dUp2DownRange = atof(strRange);
			iStart = SourceData.Find(',', iStart + 1);

			//������յļӲַ��ȣ�
			strRange = SourceData.Mid(iStart + 1, SourceData.GetLength());
			tmpRecordData.m_dUp2DownAddRatio = atof(strRange);
			iStart = SourceData.Find(',', iStart + 1);

			//������յ�ֹӯ���ȣ�
			strRange = SourceData.Mid(iStart + 1, SourceData.GetLength());
			tmpRecordData.m_dUp2DownWinStop = atof(strRange);
			iStart = SourceData.Find(',', iStart + 1);

			//��ñ�֤�����
			strRange = SourceData.Mid(iStart + 1, SourceData.GetLength());
			tmpRecordData.m_dTrustRatio = atof(strRange);
			iStart = SourceData.Find(',', iStart + 1);

			//��ñ�����ֵ��
			strRange = SourceData.Mid(iStart + 1, SourceData.GetLength());
			tmpRecordData.m_dAlarmRatio = atof(strRange);
			iStart = SourceData.Find(',', iStart + 1);

			//��ð�ȫ��ֵ��
			strRange = SourceData.Mid(iStart + 1, SourceData.GetLength());
			tmpRecordData.m_dSafeRatio = atof(strRange);

			//��÷��ȣ�
			m_MapRecordData.insert(pair<CString, CRecordData>(strInstrucmentID, tmpRecordData));
		}
	}
	SourceFile.Close();
}

//��ȡ����
void StrategyDeviate::GetPara() {
	int iSize = m_MapRecordData.size();
	string strPara;

	double m_dPrice1T;             //1���ļ۸�
	int m_iLever;                  //Ʒ�ָܸ˱�����
	int m_iShQS;                   //�ж��Ƿ�����������Ʒ�֣�0��ʾ���ǣ�1��ʾ�ǣ�
	double m_dDown2UpRange;        //�������ϵķ��ȣ�
	double m_dUp2DownRange;        //�������µķ��ȣ�
	double m_dPreSettlement;       //����Ľ���ۣ�   
	bool bStop;                    //��������ֹͣ�����֣�
	CTime m_tUpperTime;            //��¼��ͣʱ�䣻
	CTime m_tLowerTime;            //��¼��ͣʱ�䣻
	double m_dTrustRatio;          //��֤�������
	int m_iAddTimes;               //�Ӳִ�����ÿ��Ʒ�ּӲִ������ô���2��



	for (map<string, CRecordData>::iterator i = m_MapRecordData.begin(); i != m_MapRecordData.end(); i++)
	{

		char str[1024];
		memset(str, 0, 1024);
		strPara = i->first.c_str();
		sprintf(str, "%.3f", i->second.m_dPrice1T);
		strPara = "��Լ���룺" + strPara + ";" + "1���۸�" + str + ";";
		memset(str, 0, 1024);
		sprintf(str, "%d", i->second.m_iLever);
		strPara = strPara + "�ܸ˱�����" + str + ";";
		memset(str, 0, 1024);
		sprintf(str, "%.3f", i->second.m_dUp2DownRange);
		strPara = strPara + "�������µķ��ȣ�" + str + ";";
		memset(str, 0, 1024);
		sprintf(str, "%.3f", i->second.m_dUp2DownAddRatio);
		strPara = strPara + "�Ӳַ��ȣ�" + str + ";";
		memset(str, 0, 1024);
		sprintf(str, "%.3f", i->second.m_dUp2DownWinStop);
		strPara = strPara + "ֹӯ������" + str + ";";
		memset(str, 0, 1024);
		sprintf(str, "%.3f", i->second.m_dTrustRatio);
		strPara = strPara + "��֤�������" + str + ";";
		memset(str, 0, 1024);
		sprintf(str, "%.3f", i->second.m_dAlarmRatio);
		strPara = strPara + "ƫ�ֵ��" + str + ";";
		memset(str, 0, 1024);
		sprintf(str, "%.3f", i->second.m_dSafeRatio);
		strPara = strPara + "��ȫ�棺" + str + ";";

		m_vecPara.push_back(strPara);
	}
}

void StrategyDeviate::OnTime(tm t) {
	m_listOrder = this->portfoliomanager->GetOrderFormStrategy(this->strategyid_str);
	vector<CThostFtdcInputOrderField> OrderInputList = this->portfoliomanager->orderinputlist;
	now = t;
	if (OrderInputList.size() > 0) {
		for (int i = 0; i < OrderInputList.size(); i++) {
			if (m_MapOrderInput.count(OrderInputList[i].InstrumentID) == 0)
				m_MapOrderInput[OrderInputList[i].InstrumentID] = mktime(&t);
			else {
				if (m_MapOrderInput[OrderInputList[i].InstrumentID] - mktime(&t) >= 30) {
					sndPlaySound("ָ�����.wav", SND_ASYNC);
					LOG("��Լ%sOrderRef��%s�޷�����,����", m_listOrder[i].InstrumentID, m_listOrder[i].OrderRef);
				}
			}
		}
	}

	if (t.tm_hour >= 14 && t.tm_min >= 30 && t.tm_hour < 15) {
		if (!bCloseAll)
			bCloseAll = true;
	}
	else {
		bCloseAll = false;
		for (int i = 0; i < m_listOrder.size(); i++) {
			///LOG("ϵͳʱ��%ld,��������%s��Լ,�ύʱ��%ld", mktime(&t),OrderList[i].InstrumentID, GetTimeStamp(OrderList[i].InsertDate, OrderList[i].InsertTime));
			if (*m_listOrder[i].CombOffsetFlag == THOST_FTDC_OFEN_Open) {
				if (mktime(&t) - GetTimeStamp(m_listOrder[i].InsertDate, m_listOrder[i].InsertTime) >= 120) {
					CThostFtdcInputOrderActionField inputOrderActionField;
					inputOrderActionField = MakeCancelOrder(m_listOrder[i]);
					int result = this->tdapi->ReqOrderAction(&inputOrderActionField, this->requestid++);
				}
			}
		}
	}

	bTime1015 = false;

	if (t.tm_hour == 10 && t.tm_min > 7 && t.tm_min<30)
	{
		bTime1015 = true;
	}

	bTime1130 = false;

	if (t.tm_hour == 11 && t.tm_min > 22 && t.tm_min<30)
	{
		bTime1130 = true;
	}

	bTimeControlDown2Up = false;
	if (t.tm_hour == 9 && t.tm_min <= 30)
	{
		bTimeControlDown2Up = true;
	}

	if (t.tm_hour == 14 && t.tm_min >= 28)
	{
		bTimeControlDown2Up = true;
	}

	if (t.tm_hour == 15)
	{
		bTimeControlDown2Up = true;
	}

	if (t.tm_hour == 23 && t.tm_min == 29)
	{
		bTimeControlDown2Up = true;
	}

}
//OnTick
void StrategyDeviate::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {
	//Step0:��ú�Լ��Ʒ�ֵ������Ϣ��
	//LOG("123");
	m_MapTick[pDepthMarketData->InstrumentID] = *pDepthMarketData;
	string strInstrumentID = pDepthMarketData->InstrumentID;
	string strDay = pDepthMarketData->TradingDay;
	double dLastPrice = pDepthMarketData->LastPrice;
	double dPreSettlementPrice = pDepthMarketData->PreSettlementPrice;
	string strUpdateTime = pDepthMarketData->UpdateTime;
	double dBidPrice1 = pDepthMarketData->BidPrice1;
	double dBidVolume1 = pDepthMarketData->BidVolume1;
	double dAskPrice1 = pDepthMarketData->AskPrice1;
	double dAskVolume1 = pDepthMarketData->AskVolume1;
	double dVolume = pDepthMarketData->Volume;
	double dTurnover = pDepthMarketData->Turnover;
	double dUpperLimitPrice = pDepthMarketData->UpperLimitPrice;
	double dLowerLimitPrice = pDepthMarketData->LowerLimitPrice;
	double dSettlementPrice;
	CTime tNow;
	CTimeSpan tTimeSpan;
	tNow = GetTime(strDay, strUpdateTime);
	//bool bCloseAll = false;     //�ּ�ƽ�����гֲֵ���

	int iHour = atoi(strUpdateTime.substr(0, 2).c_str());
	int iMiniute = atoi(strUpdateTime.substr(2, 2).c_str());
	int iSecond = atoi(strUpdateTime.substr(5, 2).c_str());

	long  m1, m2 = 0;

	if (m_MapRecordData[strInstrumentID].m_iShQS == 0)
	{
		dSettlementPrice = pDepthMarketData->AveragePrice;    //֣�ݵĲ��ó��Ըܸˣ�
	}
	else
	{
		dSettlementPrice = pDepthMarketData->AveragePrice / m_MapRecordData[strInstrumentID].m_iLever;   //������������Ҫ���Ըܸ˵õ�
	}
	m_MapRecordData[strInstrumentID].m_dPreSettlement = dPreSettlementPrice;
	double dDeviateRatio = (dLastPrice - dSettlementPrice) / dPreSettlementPrice * 100;

	if (bCloseAll)  //14��29����ʼ���е���ֶ�����
	{
		//����ʣ��Ŀ��ֳɽ�����
		m_listPosition = this->portfoliomanager->GetPostionDetail(pDepthMarketData->InstrumentID, THOST_FTDC_D_Buy);
		int iSize = m_listPosition.size();
		for (int i = 0; i < iSize; i++)
		{
			if (strInstrumentID == m_listPosition[i].InstrumentID)
			{
				/*if (m_vecOpenTradeOrderData[i].m_bMustClose)
				{*/
				//��ɷ���ƽ�ֶ�����
				CThostFtdcInputOrderField inputOrderField;
				if (m_listPosition[i].Direction == THOST_FTDC_D_Buy) {
					inputOrderField = MakeOrder(m_listPosition[i].InstrumentID, dBidPrice1, m_listPosition[i].Volume, THOST_FTDC_D_Sell, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD);
				}
				else {
					inputOrderField = MakeOrder(m_listPosition[i].InstrumentID, dAskPrice1, m_listPosition[i].Volume, THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD);
				}
				int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
				//}
			}
		}

		//����δ�ɽ���ƽ�ֶ��У�
		m_listOrder = this->portfoliomanager->GetOrder();
		iSize = m_listOrder.size();
		for (int i = 0; i < iSize; i++)
		{
			if (strInstrumentID == m_listOrder[i].InstrumentID && (*m_listOrder[i].CombOffsetFlag == THOST_FTDC_OFEN_Close || *m_listOrder[i].CombOffsetFlag == THOST_FTDC_OFEN_CloseToday))
			{
				double dTmp = m_listOrder[i].LimitPrice - dAskPrice1;
				if (dTmp / dPreSettlementPrice <-0.02)
				{
					sndPlaySound("ָ�����.wav", SND_ASYNC);
				}
				else {
					CThostFtdcInputOrderActionField inputOrderActionField;
					inputOrderActionField = MakeCancelOrder(m_listOrder[i]);
					int result = this->tdapi->ReqOrderAction(&inputOrderActionField, this->requestid++);
				}
			}
		}
	}
	else
	{
		//Step1�����ƫ��Ƚϴ�ĺ�Լ�����Ϣ��
		OutPutDeviateData(strInstrumentID, strDay + " " + strUpdateTime, dSettlementPrice, dLastPrice, dDeviateRatio, m_MapRecordData[strInstrumentID].m_dAlarmRatio * 100);

		//Step2���������У�
		Deviate(pDepthMarketData, dDeviateRatio / 100, dSettlementPrice);
	}
}

//�����ַ�������CTime
CTime StrategyDeviate::GetTime(string strDate, string strTime) {

	int   iYear, iMonth, iDate, iHour, iMin, iSec;
	iYear = atoi(strDate.substr(0, 4).c_str());
	iMonth = atoi(strDate.substr(4, 2).c_str());
	iDate = atoi(strDate.substr(6, 2).c_str());
	iHour = atoi(strTime.substr(0, 2).c_str());
	iMin = atoi(strTime.substr(3, 2).c_str());
	iSec = atoi(strTime.substr(6, 2).c_str());
	CTime   t(iYear, iMonth, iDate, iHour, iMin, iSec);
	return t;
}

//��װ��������
CThostFtdcInputOrderField StrategyDeviate::MakeOrder(char* instrumentid, double price, int volume, char direction, char ordertype, char timecondition) {
	CThostFtdcInputOrderField order;
	(void)memset(&order, 0, sizeof(order));
	strcpy(order.BrokerID, this->brokerid.c_str());
	strcpy(order.InvestorID, this->investid.c_str());
	strcpy(order.InstrumentID, instrumentid);
	sprintf(order.OrderRef, "%s", this->GetNewOrderRef().c_str());
	///ǿƽԭ��: ��ǿƽ
	order.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///�Զ������־: ��
	order.IsAutoSuspend = 0;
	///�û�ǿ����־: ��
	order.UserForceClose = 0;
	///Ͷ���ױ���ʶ��Ͷ��
	order.CombHedgeFlag[0] = THOST_FTDC_HFEN_Speculation;
	///��������
	order.ContingentCondition = THOST_FTDC_CC_Immediately;
	///�۸�
	order.LimitPrice = price;
	///����
	order.VolumeTotalOriginal = volume;
	///�ɽ�������
	order.VolumeCondition = THOST_FTDC_VC_AV;
	///��С�ɽ���
	order.MinVolume = 1;
	///��������
	order.Direction = direction;
	///�෴�ķ���	
	char opdir;
	if (direction == THOST_FTDC_D_Sell)
		opdir = THOST_FTDC_D_Buy;
	if (direction == THOST_FTDC_D_Buy)
		opdir = THOST_FTDC_D_Sell;
	///��Ч������
	order.TimeCondition = timecondition;
	///��������
	order.OrderPriceType = ordertype;
	///��������
	order.CombOffsetFlag[0] = THOST_FTDC_OFEN_Open;
	///����в�λ��Ϊƽ�ֵ�
	int allposition = this->portfoliomanager->GetPostionDetailVolume(instrumentid, opdir);
	int todayposition = this->portfoliomanager->GetPostionDetailVolumeToday(instrumentid, opdir, this->TradingDay);
	if (allposition != todayposition)
		int index = 0;
	if (allposition > 0) {
		/*if (opdir == THOST_FTDC_D_Buy)
		//LOG("��Լ%s�ж��", instrumentid);
		if (opdir == THOST_FTDC_D_Sell)
		//LOG("��Լ%s�пղ�", instrumentid);
		//LOG("TradingDay:%s", this->TradingDay);*/
		if (allposition - todayposition > 0)
			order.CombOffsetFlag[0] = THOST_FTDC_OFEN_CloseYesterday;
		else
			order.CombOffsetFlag[0] = THOST_FTDC_OFEN_CloseToday;
	}
	return order;
}

//���ݽ�������date��time����ʱ���
time_t StrategyDeviate::GetTimeStamp(string date, string time) {
	tm tm;
	int year, month, day, hour, minute, second;
	year = atoi((date.substr(0, 4)).c_str());
	month = atoi((date.substr(4, 2)).c_str());
	day = atoi((date.substr(6, 2)).c_str());
	hour = atoi((time.substr(0, 2)).c_str());
	minute = atoi((time.substr(3, 2)).c_str());
	second = atoi((time.substr(6, 2)).c_str());
	tm.tm_year = year - 1900;
	tm.tm_mon = month - 1;
	tm.tm_mday = day;
	tm.tm_hour = hour;
	tm.tm_min = minute;
	tm.tm_sec = second;
	tm.tm_isdst = 0;
	time_t t = mktime(&tm); ///�Ѿ�����8��ʱ��  
	return t; ///��ʱ�� 	
}

//��װ��������
CThostFtdcInputOrderActionField StrategyDeviate::MakeCancelOrder(CThostFtdcOrderField order) {
	CThostFtdcInputOrderActionField inputOrderActionField;
	(void)memset(&inputOrderActionField, 0, sizeof(inputOrderActionField));
	strcpy(inputOrderActionField.BrokerID, this->brokerid.c_str());
	strcpy(inputOrderActionField.InvestorID, this->investid.c_str());
	strcpy(inputOrderActionField.OrderRef, order.OrderRef);
	strcpy(inputOrderActionField.InstrumentID, order.InstrumentID);
	inputOrderActionField.FrontID = order.FrontID;
	inputOrderActionField.SessionID = order.SessionID;
	inputOrderActionField.ActionFlag = THOST_FTDC_AF_Delete;
	return inputOrderActionField;
}

//���߼�
void StrategyDeviate::Deviate(CThostFtdcDepthMarketDataField *pDepthMarketData, double dDeviateRatio, double dSettlementPrice)
{
	//......................................................//
	int iStatus = 1000;     //��ƽ�ֵ�״̬������Ϊ0��
							//COrderData tmpOrderData = COrderData::COrderData();

	double dUpperLimitPrice = pDepthMarketData->UpperLimitPrice;
	double dLowerLimitPrice = pDepthMarketData->LowerLimitPrice;

	m_dDeviateRatio = m_MapRecordData[pDepthMarketData->InstrumentID].m_dDown2UpRange;
	m_dBailRatio = m_MapRecordData[pDepthMarketData->InstrumentID].m_dTrustRatio;
	m_dAddRatio = m_MapRecordData[pDepthMarketData->InstrumentID].m_dDown2UpAddRatio;

	CString strTmppp = pDepthMarketData->InstrumentID;

	int iOpenLots = ((double)dTotalMoney) / (((double)iPosition)*m_MapRecordData[pDepthMarketData->InstrumentID].m_iLever*m_dBailRatio*pDepthMarketData->AskPrice1);
	if (iOpenLots == 0)
	{
		iOpenLots = 1;
	}

	//Step1:�������²���1,�����жϣ�
	bool bTimeControlDown2Up = false;     //ʱ����ƣ�9��00��9��30�����֣�14��28�Ժ󲻿��֣� �ڴӵ����ϵ�ʱ������߿�2%,����9��00��9��30�����ƣ�����Ϳ�2%,�������ƣ�  
	CTime tOpen;                        //��¼����ʱ�̣�
	CTime tNow;                         //��¼��ǰʱ�̣�
	CTimeSpan tTimeSpan;                //ʱ������


	string strDay = pDepthMarketData->TradingDay;
	string strUpdateTime = pDepthMarketData->UpdateTime;
	int iHour = atoi(strUpdateTime.substr(0, 2).c_str());
	int iMiniute = atoi(strUpdateTime.substr(2, 2).c_str());
	int iSecond = atoi(strUpdateTime.substr(5, 2).c_str());
	tNow = GetTime(strDay, strUpdateTime);


	//�����жϣ����Ϳ����߸߿���ʱ����ȥ09��30��׼���������ƣ�
	if (!m_MapRecordData[pDepthMarketData->InstrumentID].bOneTime)
	{
		if (iHour == 9 && iMiniute == 0)
		{
			m_MapRecordData[pDepthMarketData->InstrumentID].bOpenHigh = false;
			m_MapRecordData[pDepthMarketData->InstrumentID].bOpenLow = false;
			m_MapRecordData[pDepthMarketData->InstrumentID].bOpenHigh = ConditionOpenHigh(pDepthMarketData->PreSettlementPrice, pDepthMarketData->LastPrice, 0.02);
			m_MapRecordData[pDepthMarketData->InstrumentID].bOneTime = true;
		}
	}

	//ʱ�����;09��00��09��30�� 15��00��23��29�ֵȿ������ƣ�
	if (m_MapRecordData[pDepthMarketData->InstrumentID].bOpenHigh)
		bTimeControlDown2Up = false;
	else
		bTimeControlDown2Up = TimeControl(tNow);

	if (strcmp(strTmppp, "RM709") == 0)
		int index = 0;


	//-------------------------------09:30------------------------------//
	if (abs(pDepthMarketData->AskPrice1 - dLowerLimitPrice) < 0.1)
	{
		m_MapRecordData[pDepthMarketData->InstrumentID].m_bOpen = false;   //��ͣ�����֣�
		LOG("��ͣƷ��:%s����ͣ�۸�%d", pDepthMarketData->InstrumentID, pDepthMarketData->AskPrice1);

		if (m_MapRecordData[pDepthMarketData->InstrumentID].iCountLimitTimes == 0)
		{
			m_MapRecordData[pDepthMarketData->InstrumentID].iCountLimitTimes = m_MapRecordData[pDepthMarketData->InstrumentID].iCountLimitTimes + 1;
			//��¼ʱ�䣻
			m_MapRecordData[pDepthMarketData->InstrumentID].m_tLowerTime = tNow;
		}
		else
		{
			m_MapRecordData[pDepthMarketData->InstrumentID].iCountLimitTimes = m_MapRecordData[pDepthMarketData->InstrumentID].iCountLimitTimes + 1;

			//.........................//
			CTimeSpan timeSpan1;
			timeSpan1 = tNow - m_MapRecordData[pDepthMarketData->InstrumentID].m_tLowerTime;
			if (m_MapRecordData[pDepthMarketData->InstrumentID].iCountLimitTimes > 1 && timeSpan1.GetTotalSeconds() >= 120)
			{
				m_MapRecordData[pDepthMarketData->InstrumentID].bStop = true;
				LOG("%sƷ��״̬:������ͣ��ʱ�������֣�", pDepthMarketData->InstrumentID);
			}
		}
	}
	else
	{
		if (!m_MapRecordData[pDepthMarketData->InstrumentID].bStop)
		{
			m_MapRecordData[pDepthMarketData->InstrumentID].iCountLimitTimes = 0;
		}
		m_MapRecordData[pDepthMarketData->InstrumentID].m_bOpen = true;
	}


	//������ڽ���۵��ڰ�ȫ�棬��ôƫ���Ӧ��������ȣ�
	double dTmp;
	dTmp = (dSettlementPrice - pDepthMarketData->PreSettlementPrice) / pDepthMarketData->PreSettlementPrice;  //����۵�ƫ����ȣ�
	if (dTmp > m_MapRecordData[pDepthMarketData->InstrumentID].m_dSafeRatio)
	{
		m_dDeviateRatio = m_dDeviateRatio + dTmp;
	}

	//ͳ��7�����ڣ��µ�����0.7%����Ӳ֣�
	if (m_MapRecordData[pDepthMarketData->InstrumentID].bHasOrder && (!bTimeControlDown2Up) && m_MapRecordData[pDepthMarketData->InstrumentID].m_bOpen && !m_MapRecordData[pDepthMarketData->InstrumentID].bStop && m_MapRecordData[pDepthMarketData->InstrumentID].m_iAddTimes<3)
	{
		tTimeSpan = tNow - m_MapRecordData[pDepthMarketData->InstrumentID].m_tLastOpen;
		int iSecondCount = tTimeSpan.GetTotalSeconds();

		int iTimeJian = 420;

		if (bTime1015)
		{
			iTimeJian = 1320;
		}

		if (bTime1130)
		{
			iTimeJian = 7620;
		}

		if (iSecondCount <= iTimeJian)
		{
			//�жϳɽ���
			if ((pDepthMarketData->LastPrice - m_MapRecordData[pDepthMarketData->InstrumentID].m_dOpenPrice) / pDepthMarketData->PreSettlementPrice < -m_dAddRatio)
			{
				//�ж�Ask1�ļ۸��Ƿ�������֮�ڣ�
				if (abs(pDepthMarketData->LastPrice - pDepthMarketData->AskPrice1) <= m_MapRecordData[pDepthMarketData->InstrumentID].m_dPrice2T)
				{
					//.......���Ͷ���......//
					while (iOpenTime < 6)
					{


						/*char strOrderRef[1024];
						memset(strOrderRef, 0, 1024);*/
						iOrderNum = iOrderNum + 1;
						/*sprintf_s(strOrderRef, "%d", iOrderNum);*/
						CThostFtdcInputOrderField inputOrderField;
						inputOrderField = MakeOrder(pDepthMarketData->InstrumentID, pDepthMarketData->AskPrice1, iOpenLots, THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD);
						iStatus = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
						if (iStatus == 0) {
							LOG("�Ӳֺ�Լ:%s���ּ۸�:%f��������:%dƫ���:%.3f", pDepthMarketData->InstrumentID, pDepthMarketData->BidPrice1, iOpenLots, dDeviateRatio);
							////.............��¼�����Ϣ............//
							m_MapRecordData[pDepthMarketData->InstrumentID].m_tLastOpen = tNow;              //�µ�ʱ�䣻
							m_MapRecordData[pDepthMarketData->InstrumentID].m_dOpenPrice = pDepthMarketData->AskPrice1;       //�µ��۸�
							m_MapRecordData[pDepthMarketData->InstrumentID].bHasOrder = true;                //�Ѿ��е��ӣ�
							m_MapRecordData[pDepthMarketData->InstrumentID].m_dPrice = pDepthMarketData->LastPrice;           //�µ�ʱ�̵����¼ۣ�
							m_MapRecordData[pDepthMarketData->InstrumentID].iDirection = 1;
							break;
						}

					}
				}
			}
		}
	}

	//�ж�����㣬�Ƿ��������֣�
	if (m_MapRecordData[pDepthMarketData->InstrumentID].m_bOpen && !m_MapRecordData[pDepthMarketData->InstrumentID].bStop)
	{
		if (dDeviateRatio <-m_dDeviateRatio && (!bTimeControlDown2Up) && (!m_MapRecordData[pDepthMarketData->InstrumentID].bHasOrder))
		{
			//���������Ƿ�>3%����ô������
			if (ConditionMeanPriceDown(pDepthMarketData->PreSettlementPrice, dSettlementPrice, 0.03))   //����۵�������>3%,��ô������
			{
				//�ж�Ask1�ļ۸��Ƿ�������֮�ڣ�
				if (abs(pDepthMarketData->LastPrice - pDepthMarketData->AskPrice1) <= m_MapRecordData[pDepthMarketData->InstrumentID].m_dPrice2T)
				{
					//.......���Ͷ���......//
					while (iOpenTime < 6)
					{
						iOrderNum = iOrderNum + 1;
						if (pDepthMarketData->InstrumentID == "jm1701")
						{
							int pp = 0;
						}
						CThostFtdcInputOrderField inputOrderField;
						inputOrderField = MakeOrder(pDepthMarketData->InstrumentID, pDepthMarketData->AskPrice1, iOpenLots, THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD);
						iStatus = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
						if (iStatus == 0) {
							LOG("���ֺ�Լ:%s���ּ۸�:%f��������:%dƫ���:%.3f", pDepthMarketData->InstrumentID, pDepthMarketData->BidPrice1, iOpenLots, dDeviateRatio);
							////.............��¼�����Ϣ............//
							m_MapRecordData[pDepthMarketData->InstrumentID].m_tLastOpen = tNow;              //�µ�ʱ�䣻
							m_MapRecordData[pDepthMarketData->InstrumentID].m_dOpenPrice = pDepthMarketData->AskPrice1;       //�µ��۸�
							m_MapRecordData[pDepthMarketData->InstrumentID].bHasOrder = true;                //�Ѿ��е��ӣ�
							m_MapRecordData[pDepthMarketData->InstrumentID].m_dPrice = pDepthMarketData->LastPrice;           //�µ�ʱ�̵����¼ۣ�
							m_MapRecordData[pDepthMarketData->InstrumentID].iDirection = 1;
							break;
						}
						else {
							Sleep(100);   //100���룻
							iOpenTime = iOpenTime + 1;
						}
					}
				}
			}
		}
	}
}

//�ж��Ƿ�߿�
bool StrategyDeviate::ConditionOpenHigh(double dLastMeanPrice, double dNowMeanPrice, double dRatio)
{
	if (((dNowMeanPrice - dLastMeanPrice) / dLastMeanPrice) > dRatio)
		return true;
	else return false;
}

bool StrategyDeviate::ConditionMeanPriceDown(double dLastMeanPrice, double dNowMeanPrice, double dRatio)
{
	bool bConditionControl = true;

	if ((dNowMeanPrice - dLastMeanPrice) / dLastMeanPrice <-dRatio)
	{
		bConditionControl = false;
	}
	return bConditionControl;
}


//�ж��Ƿ�����Ӧʱ���
bool StrategyDeviate::TimeControl(CTime tTime)
{
	bool bTimeControl = false;

	int iHour = tTime.GetHour();
	int iMiniute = tTime.GetMinute();
	if (iHour == 9 && iMiniute <= 30)
	{
		bTimeControl = true;
	}

	if (iHour == 14 && iMiniute >= 28)
	{
		bTimeControl = true;
	}

	if (iHour == 15)
	{
		bTimeControl = true;
	}

	if (iHour == 23 && iMiniute == 29)
	{
		bTimeControl = true;
	}

	return bTimeControl;
}

//���ƫ��Ƚϴ�ĺ�Լ�����Ϣ
void StrategyDeviate::OutPutDeviateData(string strInstrumentID, string strUpdateTime, double dSettlementPrice, double dLastPrice, double dDeviateRatio, double dThresld)
{
	//Step1�����ƫ��Ƚϴ�ĺ�Լ�����Ϣ��
	if (iCountTime > 100)
	{
		if (dDeviateRatio < -dThresld)
		{
			//LOG("Ʒ��:%s;ʱ��:%s;�����:%.3f;�ּ�:%.2f;ƫ���:%.3f", strInstrumentID, strUpdateTime, dSettlementPrice, dLastPrice, dDeviateRatio);
			sndPlaySound("��Ϣ����.wav", SND_ASYNC);
		}

		if (dDeviateRatio < -2.3)
		{
			LOG("Ʒ��:%s;ʱ��:%s;�����:%.3f;�ּ�:%.2f;ƫ���:%.3f", strInstrumentID, strUpdateTime, dSettlementPrice, dLastPrice, dDeviateRatio);
			//sndPlaySound("��Ϣ����.wav", SND_ASYNC);
		}

		iCountTime = 0;

		//�����������״̬��״̬��

		/*int iSize = m_vecOpenQueueOrderData.size();
		if (iSize > 0)
		{
		LOG("���Կ���δ�ɽ�������:%d", iSize);
		}

		iSize = m_vecOpenTradeOrderData.size();
		if (iSize > 0)
		{
		LOG("���Կ��ֳɽ�������:%d", iSize);
		}

		iSize = m_vecCloseQueueOrderData.size();
		if (iSize > 0)
		{
		LOG("����ƽ��δ�ɽ�������:%d", iSize);
		}

		iSize = m_vecCloseTradeOrderData.size();
		if (iSize > 0)
		{
		LOG("����ƽ�ֳɽ�������:%d", iSize);
		}*/
	}
	else
	{
		iCountTime = iCountTime + 1;
	}
}

//�ɽ��ر�
void StrategyDeviate::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	int iOpenTime = 0;      //���������Ĵ�����
	bool bFindOrder = false;   // �ж��Ƿ�������еĶ�����
	int iFlagNum;              //������ĸ�����

							   //�ж�Ʒ�֣�����ǳɽ�����//���򿪳�ֹӯ����
	string strOrderRef = pTrade->OrderRef;                        //����ѷ��͵���
	char strDirection = pTrade->Direction;
	TThostFtdcPriceType	Price = pTrade->Price;
	TThostFtdcVolumeType Volume = pTrade->Volume;
	string strInstrumentID = pTrade->InstrumentID;
	string strDate = pTrade->TradeDate;
	string strTime = pTrade->TradeTime;
	CTime tTime = GetTime(strDate, strTime);
	string strTmp1 = (strDirection == '0') ? "��" : "��";
	int iDirection = (strDirection == '0') ? 1 : -1;
	//LOG("�ɽ���Լ��%s �ɽ��۸�%.3f �ɽ�ʱ�䣺%s �ɽ�������%d ����%s", strInstrumentID, Price, pTrade->TradeTime, Volume, strTmp1);

	if (Volume > 0 && strDirection == '0')   //��������Ƕ�'0'����ô�����ѿ��ֶ����У�
	{

		LOG("���ֳɽ���Լ��%s�ɽ��۸�%.3f�ɽ�ʱ�䣺%s�ɽ�������%d����%s", strInstrumentID, Price, pTrade->TradeTime, Volume, strTmp1);
		m_MapRecordData[strInstrumentID].m_iAddTimes = m_MapRecordData[strInstrumentID].m_iAddTimes + 1;
		m_dWinStop = m_MapRecordData[strInstrumentID].m_dDown2UpWinStop;
		//����Ӳ����Σ���ô�޸ĵ�һ����ֹӯ�۸�
		if (m_MapRecordData[strInstrumentID].m_iAddTimes == 3)
		{
			//................����ֹ��........................//
			/*	this->orderlist;*/


		}

		// �Ա�����Ϊ��С����������������ƽ�֣�
		CRecordData tmpData;
		tmpData = m_MapRecordData[strInstrumentID];
		int iSQS = tmpData.m_iShQS;
		int iStatus;

		double dPrice1T = tmpData.m_dPrice1T;
		double dPrice = Price + int(tmpData.m_dPreSettlement*m_dWinStop / dPrice1T)*dPrice1T;

		//����ƽ�ֵ���
		//FinishOrderClose(strInstrumentID, dPrice, Volume, THOST_FTDC_D_Sell, tTime, strOrderRef, 0);
		CThostFtdcInputOrderField inputOrderField;
		inputOrderField = MakeOrder(pTrade->InstrumentID, dPrice, Volume, THOST_FTDC_D_Sell, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD);
		iStatus = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);

	}

	if (Volume > 0 && strDirection == '1')   //��������ǿյ�����ô�����ѿ��ֶ����У�ֹӯ���ɽ���
	{
		LOG("ƽ�ֳɽ���Լ��%s�ɽ��۸�%.3f�ɽ�ʱ�䣺%s�ɽ�������%d����%s", strInstrumentID, Price, pTrade->TradeTime, Volume, strTmp1);
	}
};

void StrategyDeviate::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
}
void StrategyDeviate::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
}
void StrategyDeviate::OnRtnOrder(CThostFtdcOrderField *pOrder) {

}
void StrategyDeviate::OnInit() {
}