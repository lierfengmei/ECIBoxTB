#include "stdafx.h"
#include "TPSStateAlgrithom.h"
#include <assert.h>
#include <map>

using namespace std;

/*
查看电压值是否满足TPS过车状态的需求，因为之前使用了阻抗判断（但是好像阻抗并不能显示TPS过车状态），
这里再写一个使用电压值判断TPS过车状态的函数。
	以真实的数据为例：对于第一路过车信号而言，它的电压值为
	674 675 677 674 676 674(正常值，误差<10)
	642 629 593 549 （过车时的值，与正常电压值差距＞30）

* TPS过车测试算法：
* 1、根据ECIInfo info中的数据，提取出电压数据pVoltageInfo
* 2、根据阻抗数据pVoltageInfo提取出大数（众数）pLargeNum，该数据为正常数据
* 3、遍历每一个pVoltageInfo的点，对于每个点，计算出它与前面点的差值PreDis，与后面点的差值NextDis
* 4、如果与差值PreDis为负，与后面差值NextDis也为负，则进一步判断，否则返回step3
* 5、该点与大数之间的距离应该大于ToleranceBtwLargeAndLittle，否则返回step3
* 6、该点的前面和后面应该各有一个大数，否则返回step3
* 7、存在满足以上条件的点，则认为TPS过车测试通过,IfVoltageMatchTPSState 返回true
*/

//查看提取出来的Num组info数据是否满足TPS状态的需求
//ECIInfo info:提取出来的状态信息数据
//InfoNum:	info 的数目
//PortNum:  TPS测量的第多少路，PortNum = 1,2,3,4
bool	IfVoltageMatchTPSState(const ECIInfo info[],const int InfoNum,const int PortNum)		//是否满足TPS的状态	
{
	assert(InfoNum>5);
	assert(PortNum>0 && PortNum<5);

	int *pVoltageInfo = new int[InfoNum];		//提取出来的电压信息
	int *pLargeNum = new int[InfoNum];			//阻抗信息中的大数
	int LargeNumTolerance = 10;					//阻抗信息中大数的容忍值		
	int ToleranceBtwLargeAndLittle = 30;		//阻抗中大数和小数的最小距离	

	int PreDis,NextDis;
	int dis;
	int LargeNumMin;

	//提取出电压信息到pVoltageInfo中
	FindVoltageInfo(info,InfoNum,PortNum,pVoltageInfo);
	
	//从pVoltageInfo中提取出大数
	LargeNumMin = AbstractLargeNumFromVoltage(InfoNum,pVoltageInfo,LargeNumTolerance,pLargeNum);
	int i = 1;
	for(;i<InfoNum-1;)		//对于每一个数
	{
		PreDis = pVoltageInfo[i] - pVoltageInfo[i-1];
		NextDis = pVoltageInfo[i] - pVoltageInfo[i+1];

		if(PreDis<0 && NextDis<0)
		{
			//确认pVoltageInfo[i]到大数pLargeNum之间的距离大于ToleranceBtwLargeAndLittle
			dis = LargeNumMin - pVoltageInfo[i];
			if(dis<ToleranceBtwLargeAndLittle) 
			{
				i++;
				continue;
			}
			//确认pVoltageInfo[i]之前有个大数,之后还有个大数
			bool result = IfLargeNumExist(pVoltageInfo,i,InfoNum,LargeNumMin);
			if(result) break;				//该TPS过车判断正确！
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
* TPS过车测试算法：
* 1、根据ECIInfo info中的数据，提取出阻抗数据pResistantInfo
* 2、根据阻抗数据pResistantInfo提取出大数（众数）pLargeNum，该数据为正常数据
* 3、遍历每一个pResistantInfo的点，对于每个点，计算出它与前面点的差值PreDis，与后面点的差值NextDis
* 4、如果与差值PreDis为负，与后面差值NextDis，则进一步判断，否则返回step3
* 5、该点与大数之间的距离应该大于ToleranceBtwLargeAndLittle，否则返回step3
* 6、该点的前面和后面应该各有一个大数，否则返回step3
* 7、存在满足以上条件的点，则认为TPS过车测试通过,IfMatchTPSState 返回true
*/

//查看提取出来的Num组阻抗的info数据是否满足TPS状态的需求
//ECIInfo info:提取出来的状态信息数据
//InfoNum:	info 的数目
//PortNum:  TPS测量的第多少路，PortNum = 1,2,3,4
/*
bool	IfMatchTPSState(const ECIInfo info[],const int InfoNum,const int PortNum)		//是否满足TPS的状态	
{
	assert(InfoNum>5);
	assert(PortNum>0 && PortNum<5);

	int *pResistantInfo = new int[InfoNum];		//提取出来的阻抗信息
	int *pLargeNum = new int[InfoNum];			//阻抗信息中的大数
	int LargeNumTolerance = 3;					//阻抗信息中大数的容忍值		//todo:数值待议
	int ToleranceBtwLargeAndLittle = 30;		//阻抗中大数和小数的最小距离	//todo:数值待议

	int PreDis,NextDis;
	int dis;
	int LargeNumMin;

	//提取出阻抗信息到pResistanceInfo中
	FindResistantInfo(info,InfoNum,PortNum,pResistantInfo);
	//从pResistanceInfo中提取出大数
	LargeNumMin = AbstractLargeNumFromResistant(InfoNum,pResistantInfo,LargeNumTolerance,pLargeNum);
	int i = 1;
	for(;i<InfoNum-1;)		//对于每一个数
	{
		PreDis = pResistantInfo[i] - pResistantInfo[i-1];
		NextDis = pResistantInfo[i] - pResistantInfo[i+1];

		if(PreDis<0 && NextDis<0)
		{
			//确认pResistantInfo[i]到大数pLargeNum之间的距离大于ToleranceBtwLargeAndLittle
			dis = LargeNumMin - pResistantInfo[i];
			if(dis<ToleranceBtwLargeAndLittle) 
			{
				i++;
				continue;
			}
			//确认pResistantInfo[i]之前有个大数,之后还有个大数
			bool result = IfLargeNumExist(pResistantInfo,i,InfoNum,LargeNumMin);
			if(result) break;				//该TPS过车判断正确！
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
//提取出4路端口的电压信息
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

//提取出4路端口的阻抗信息
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
//返回电压的LargeNumMin
int AbstractLargeNumFromVoltage(  const int InfoNum,
								  const int *pVoltageInfo,
								  const int LargeNumTolerance,
								  const int *pLargeNum)
{
	map<int, int> mapVoltageInfoCount;
	int ModeValue;		//众数
	int LargeNumMin;	//众数中能接受的最小的数

	for(int i=0;i<InfoNum;i++)
		++mapVoltageInfoCount[pVoltageInfo[i]];

	map<int,int>::const_iterator it = mapVoltageInfoCount.begin();
	ModeValue = it->first;

	//寻找众数
	for(it = mapVoltageInfoCount.begin(); it!=mapVoltageInfoCount.end();it++)
	{
		if ((it->second)>ModeValue)
			ModeValue = it->first;
	}

	//寻找大数中较小的那个数LargeNumMin
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


//返回LargeNumMin
/*
int AbstractLargeNumFromResistant(const int InfoNum,
								  const int *pResistantInfo,
								  const int LargeNumTolerance,
								  const int *pLargeNum)
{
	map<int, int> mapResistantInfoCount;
	int ModeValue;		//众数
	int LargeNumMin;	//众数中能接受的最小的数

	for(int i=0;i<InfoNum;i++)
		++mapResistantInfoCount[pResistantInfo[i]];

	map<int,int>::const_iterator it = mapResistantInfoCount.begin();
	ModeValue = it->first;

	//寻找众数
	for(it = mapResistantInfoCount.begin(); it!=mapResistantInfoCount.end();it++)
	{
		if ((it->second)>ModeValue)
			ModeValue = it->first;
	}

	//寻找大数中较小的那个数LargeNumMin
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
//判断点pInfo[i]的前面和后面应该各有一个大数(即都存在一个数，>=LargeNumMin),正确返回true,错误返回false
bool	IfLargeNumExist(const int *pInfo,const int i,const int InfoNum,const int LargeNumMin)
{
	int min = InfoNum-1, max = 0;
	int j;

	assert(i>0 && i<InfoNum-1);

	//前面有个大数
	for(j=0;j<i;j++)
	{
		if (pInfo[j]>=LargeNumMin)	
		{
			min = j;
			break;
		}
	}

	//后面有个大数
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
