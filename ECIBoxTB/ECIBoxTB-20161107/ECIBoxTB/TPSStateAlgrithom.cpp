#include "stdafx.h"
#include "TPSStateAlgrithom.h"
#include <assert.h>
#include <map>

using namespace std;

/*
�鿴��ѹֵ�Ƿ�����TPS����״̬��������Ϊ֮ǰʹ�����迹�жϣ����Ǻ����迹��������ʾTPS����״̬����
������дһ��ʹ�õ�ѹֵ�ж�TPS����״̬�ĺ�����
	����ʵ������Ϊ�������ڵ�һ·�����źŶ��ԣ����ĵ�ѹֵΪ
	674 675 677 674 676 674(����ֵ�����<10)
	642 629 593 549 ������ʱ��ֵ����������ѹֵ��ࣾ30��

* TPS���������㷨��
* 1������ECIInfo info�е����ݣ���ȡ����ѹ����pVoltageInfo
* 2�������迹����pVoltageInfo��ȡ��������������pLargeNum��������Ϊ��������
* 3������ÿһ��pVoltageInfo�ĵ㣬����ÿ���㣬���������ǰ���Ĳ�ֵPreDis��������Ĳ�ֵNextDis
* 4��������ֵPreDisΪ����������ֵNextDisҲΪ�������һ���жϣ����򷵻�step3
* 5���õ������֮��ľ���Ӧ�ô���ToleranceBtwLargeAndLittle�����򷵻�step3
* 6���õ��ǰ��ͺ���Ӧ�ø���һ�����������򷵻�step3
* 7�������������������ĵ㣬����ΪTPS��������ͨ��,IfVoltageMatchTPSState ����true
*/

//�鿴��ȡ������Num��info�����Ƿ�����TPS״̬������
//ECIInfo info:��ȡ������״̬��Ϣ����
//InfoNum:	info ����Ŀ
//PortNum:  TPS�����ĵڶ���·��PortNum = 1,2,3,4
bool	IfVoltageMatchTPSState(const ECIInfo info[],const int InfoNum,const int PortNum)		//�Ƿ�����TPS��״̬	
{
	assert(InfoNum>5);
	assert(PortNum>0 && PortNum<5);

	int *pVoltageInfo = new int[InfoNum];		//��ȡ�����ĵ�ѹ��Ϣ
	int *pLargeNum = new int[InfoNum];			//�迹��Ϣ�еĴ���
	int LargeNumTolerance = 10;					//�迹��Ϣ�д���������ֵ		
	int ToleranceBtwLargeAndLittle = 30;		//�迹�д�����С������С����	

	int PreDis,NextDis;
	int dis;
	int LargeNumMin;

	//��ȡ����ѹ��Ϣ��pVoltageInfo��
	FindVoltageInfo(info,InfoNum,PortNum,pVoltageInfo);
	
	//��pVoltageInfo����ȡ������
	LargeNumMin = AbstractLargeNumFromVoltage(InfoNum,pVoltageInfo,LargeNumTolerance,pLargeNum);
	int i = 1;
	for(;i<InfoNum-1;)		//����ÿһ����
	{
		PreDis = pVoltageInfo[i] - pVoltageInfo[i-1];
		NextDis = pVoltageInfo[i] - pVoltageInfo[i+1];

		if(PreDis<0 && NextDis<0)
		{
			//ȷ��pVoltageInfo[i]������pLargeNum֮��ľ������ToleranceBtwLargeAndLittle
			dis = LargeNumMin - pVoltageInfo[i];
			if(dis<ToleranceBtwLargeAndLittle) 
			{
				i++;
				continue;
			}
			//ȷ��pVoltageInfo[i]֮ǰ�и�����,֮���и�����
			bool result = IfLargeNumExist(pVoltageInfo,i,InfoNum,LargeNumMin);
			if(result) break;				//��TPS�����ж���ȷ��
		}
		else i++;
	}

	delete pLargeNum;
	delete pVoltageInfo;
	if(i<InfoNum-1)		
		return true;
	else 
		return false;
}






/*
* TPS���������㷨��
* 1������ECIInfo info�е����ݣ���ȡ���迹����pResistantInfo
* 2�������迹����pResistantInfo��ȡ��������������pLargeNum��������Ϊ��������
* 3������ÿһ��pResistantInfo�ĵ㣬����ÿ���㣬���������ǰ���Ĳ�ֵPreDis��������Ĳ�ֵNextDis
* 4��������ֵPreDisΪ����������ֵNextDis�����һ���жϣ����򷵻�step3
* 5���õ������֮��ľ���Ӧ�ô���ToleranceBtwLargeAndLittle�����򷵻�step3
* 6���õ��ǰ��ͺ���Ӧ�ø���һ�����������򷵻�step3
* 7�������������������ĵ㣬����ΪTPS��������ͨ��,IfMatchTPSState ����true
*/

//�鿴��ȡ������Num���迹��info�����Ƿ�����TPS״̬������
//ECIInfo info:��ȡ������״̬��Ϣ����
//InfoNum:	info ����Ŀ
//PortNum:  TPS�����ĵڶ���·��PortNum = 1,2,3,4
/*
bool	IfMatchTPSState(const ECIInfo info[],const int InfoNum,const int PortNum)		//�Ƿ�����TPS��״̬	
{
	assert(InfoNum>5);
	assert(PortNum>0 && PortNum<5);

	int *pResistantInfo = new int[InfoNum];		//��ȡ�������迹��Ϣ
	int *pLargeNum = new int[InfoNum];			//�迹��Ϣ�еĴ���
	int LargeNumTolerance = 3;					//�迹��Ϣ�д���������ֵ		//todo:��ֵ����
	int ToleranceBtwLargeAndLittle = 30;		//�迹�д�����С������С����	//todo:��ֵ����

	int PreDis,NextDis;
	int dis;
	int LargeNumMin;

	//��ȡ���迹��Ϣ��pResistanceInfo��
	FindResistantInfo(info,InfoNum,PortNum,pResistantInfo);
	//��pResistanceInfo����ȡ������
	LargeNumMin = AbstractLargeNumFromResistant(InfoNum,pResistantInfo,LargeNumTolerance,pLargeNum);
	int i = 1;
	for(;i<InfoNum-1;)		//����ÿһ����
	{
		PreDis = pResistantInfo[i] - pResistantInfo[i-1];
		NextDis = pResistantInfo[i] - pResistantInfo[i+1];

		if(PreDis<0 && NextDis<0)
		{
			//ȷ��pResistantInfo[i]������pLargeNum֮��ľ������ToleranceBtwLargeAndLittle
			dis = LargeNumMin - pResistantInfo[i];
			if(dis<ToleranceBtwLargeAndLittle) 
			{
				i++;
				continue;
			}
			//ȷ��pResistantInfo[i]֮ǰ�и�����,֮���и�����
			bool result = IfLargeNumExist(pResistantInfo,i,InfoNum,LargeNumMin);
			if(result) break;				//��TPS�����ж���ȷ��
		}
		else i++;
	}

	delete pLargeNum;
	delete pResistantInfo;
	if(i<InfoNum-1)		
		return true;
	else 
		return false;
}
*/
//��ȡ��4·�˿ڵĵ�ѹ��Ϣ
void	FindVoltageInfo(const ECIInfo info[],const int InfoNum,const int PortNum,int *pVoltageInfo)
{
		for(int i=0;i<InfoNum;i++)
	{
		switch(PortNum)
		{
		case 1:pVoltageInfo[i] = info[i].Port1Voltage;break;
		case 2:pVoltageInfo[i] = info[i].Port2Voltage;break;
		case 3:pVoltageInfo[i] = info[i].Port3Voltage;break;
		case 4:pVoltageInfo[i] = info[i].Port4Voltage;break;
		default:break;
		}
	}

}

//��ȡ��4·�˿ڵ��迹��Ϣ
/*
void FindResistantInfo(const ECIInfo info[],const int InfoNum,const int PortNum,int *pResistantInfo)
{
	for(int i=0;i<InfoNum;i++)
	{
		switch(PortNum)
		{
		case 1:pResistantInfo[i] = info[i].Port1Resistant;break;
		case 2:pResistantInfo[i] = info[i].Port2Resistant;break;
		case 3:pResistantInfo[i] = info[i].Port3Resistant;break;
		case 4:pResistantInfo[i] = info[i].Port4Resistant;break;
		default:break;
		}
	}
}
*/
//���ص�ѹ��LargeNumMin
int AbstractLargeNumFromVoltage(  const int InfoNum,
								  const int *pVoltageInfo,
								  const int LargeNumTolerance,
								  const int *pLargeNum)
{
	map<int, int> mapVoltageInfoCount;
	int ModeValue;		//����
	int LargeNumMin;	//�������ܽ��ܵ���С����

	for(int i=0;i<InfoNum;i++)
		++mapVoltageInfoCount[pVoltageInfo[i]];

	map<int,int>::const_iterator it = mapVoltageInfoCount.begin();
	ModeValue = it->first;

	//Ѱ������
	for(it = mapVoltageInfoCount.begin(); it!=mapVoltageInfoCount.end();it++)
	{
		if ((it->second)>ModeValue)
			ModeValue = it->first;
	}

	//Ѱ�Ҵ����н�С���Ǹ���LargeNumMin
	LargeNumMin = ModeValue;
	for(it = mapVoltageInfoCount.begin(); it!=mapVoltageInfoCount.end();it++)
	{
		int delta;
		delta  = ModeValue-(it->first);
		if(delta<=LargeNumTolerance && delta>0)
		{
			if(it->first<LargeNumMin)
				LargeNumMin = it->first;
		}
	}

	return LargeNumMin;
}


//����LargeNumMin
/*
int AbstractLargeNumFromResistant(const int InfoNum,
								  const int *pResistantInfo,
								  const int LargeNumTolerance,
								  const int *pLargeNum)
{
	map<int, int> mapResistantInfoCount;
	int ModeValue;		//����
	int LargeNumMin;	//�������ܽ��ܵ���С����

	for(int i=0;i<InfoNum;i++)
		++mapResistantInfoCount[pResistantInfo[i]];

	map<int,int>::const_iterator it = mapResistantInfoCount.begin();
	ModeValue = it->first;

	//Ѱ������
	for(it = mapResistantInfoCount.begin(); it!=mapResistantInfoCount.end();it++)
	{
		if ((it->second)>ModeValue)
			ModeValue = it->first;
	}

	//Ѱ�Ҵ����н�С���Ǹ���LargeNumMin
	LargeNumMin = ModeValue;
	for(it = mapResistantInfoCount.begin(); it!=mapResistantInfoCount.end();it++)
	{
		int delta;
		delta  = ModeValue-(it->first);
		if(delta<=LargeNumTolerance && delta>0)
		{
			if(it->first<LargeNumMin)
				LargeNumMin = it->first;
		}
	}

	return LargeNumMin;
}

*/
//�жϵ�pInfo[i]��ǰ��ͺ���Ӧ�ø���һ������(��������һ������>=LargeNumMin),��ȷ����true,���󷵻�false
bool	IfLargeNumExist(const int *pInfo,const int i,const int InfoNum,const int LargeNumMin)
{
	int min = InfoNum-1, max = 0;
	int j;

	assert(i>0 && i<InfoNum-1);

	//ǰ���и�����
	for(j=0;j<i;j++)
	{
		if (pInfo[j]>=LargeNumMin)	
		{
			min = j;
			break;
		}
	}

	//�����и�����
	for(j=i+1;j<InfoNum;j++)
	{
		if (pInfo[j] >= LargeNumMin)	
		{
			max = j;
			break;
		}
	}
	if(min<i && max>i)		return true;
	else					return false;
}
