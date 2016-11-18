#include "stdafx.h"

struct ECIInfo
{
	UINT	WorkingHours;

	CString Port1Status;
	UINT	Port1Voltage;
	UINT	Port1Current;
	UINT	Port1Resistant;
	UINT	Port1Phase;
	UINT	Port1Distance;

	CString Port2Status;
	UINT	Port2Voltage;
	UINT	Port2Current;
	UINT	Port2Resistant;
	UINT	Port2Phase;
	UINT	Port2Distance;

	CString Port3Status;
	UINT	Port3Voltage;
	UINT	Port3Current;
	UINT	Port3Resistant;
	UINT	Port3Phase;
	UINT	Port3Distance;

	CString Port4Status;
	UINT	Port4Voltage;
	UINT	Port4Current;
	UINT	Port4Resistant;
	UINT	Port4Phase;
	UINT	Port4Distance;
};

//查看阻抗是否满足TPS过车状态
/*
bool	IfMatchTPSState(const ECIInfo info[],const int InfoNum,const int PortNum);
void	FindResistantInfo(	const ECIInfo info[],
							const int InfoNum,
							const int PortNum,
							int *pResistantInfo);
int		AbstractLargeNumFromResistant(	const int InfoNum,
										const int *pResistantInfo,
										const int LargeNumTolerance,
										const int *pLargeNum);
*/

bool	IfLargeNumExist(const int *pInfo,const int i,const int InfoNum,const int LargeNumMin);

//查看电压是否满足TPS过车状态
bool	IfVoltageMatchTPSState(const ECIInfo info[],const int InfoNum,const int PortNum);
void	FindVoltageInfo(const ECIInfo info[],
						const int InfoNum,
						const int PortNum,
						int *pVoltageInfo);
//返回LargeNumMin
int		AbstractLargeNumFromVoltage(const int InfoNum,
									const int *pVoltageInfo,
									const int LargeNumTolerance,
									const int *pLargeNum);