
//提取生产信息SN码

// 输入：SN_original1 ( 250byte),SN_original2(26byte)
// 输出：SN_create(20byte)
void ExtractSN(void)
{
	int i,j;
	int length1,length2;
	char * pszValue1 = NULL;
	char * pszValue2 = NULL;
	char szValue[300];
	pszValue1 = GetTagChar("SN_original1");
	pszValue2 = GetTagChar("SN_original2");
	length1 = strlen(pszValue1);
	length2 = strlen(pszValue2);
	length1 = length1+length2;
	if(280!=length1)
	{
		printf("错误0：包含SN码的信息长度不对！\n");
		return;
	}
	
	if((pszValue1[4]!='4')&&(pszValue1[5]=='0'))					//如果第2字节不是40，返回2		
	{
		printf("错误1：提取SN码信息错误！\n");
		return;
	}
	
	if((pszValue1[10]!='0')&&(pszValue1[11]=='1'))					//如果第5字节不是01，返回3
	{
		printf("错误2：提取SN码信息错误！\n");
		return;
	}

	j=80;
	for(i=0;i<5;)
	{
	     if(pszValue1[j]!='3')
	       {
	             printf("错误3：SN码包含不是数字的字符，错误！\n");
	             return;
	       }
	       i++;
	       j = j+2;
	}
	
	j=81;
	for( i=0;i<5;)												//直接-30转换成ASCII嘛
	{
	       szValue[i]=pszValue1[j];
	       i++;
	       j = j+2;
	}
	szValue[i] = '\0';
	
	SetTagChar("SN_create",szValue);
	printf("SN码输出！\n");
	printf("%s",szValue);
	printf("\n");
}


//将SN码存到txt文件中
void SaveSN(void)
{
	FILE *fp = fopen("telegram.txt","rw");		//打开telegram.txt文件
	if(fp==NULL)
	{
			printf("存储报文的txt文件不存在！\n");
			return;
	} 
	
	
	
}

//将报文文件提取出来存储到TXT里面中
//输入：FilePath
//生成TXT文件
void SaveTxtFile(void)
{
	
	
	
}