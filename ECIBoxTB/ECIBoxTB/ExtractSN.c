
//��ȡ������ϢSN��

// ���룺SN_original1 ( 250byte),SN_original2(26byte)
// �����SN_create(20byte)
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
		printf("����0������SN�����Ϣ���Ȳ��ԣ�\n");
		return;
	}
	
	if((pszValue1[4]!='4')&&(pszValue1[5]=='0'))					//�����2�ֽڲ���40������2		
	{
		printf("����1����ȡSN����Ϣ����\n");
		return;
	}
	
	if((pszValue1[10]!='0')&&(pszValue1[11]=='1'))					//�����5�ֽڲ���01������3
	{
		printf("����2����ȡSN����Ϣ����\n");
		return;
	}

	j=80;
	for(i=0;i<5;)
	{
	     if(pszValue1[j]!='3')
	       {
	             printf("����3��SN������������ֵ��ַ�������\n");
	             return;
	       }
	       i++;
	       j = j+2;
	}
	
	j=81;
	for( i=0;i<5;)												//ֱ��-30ת����ASCII��
	{
	       szValue[i]=pszValue1[j];
	       i++;
	       j = j+2;
	}
	szValue[i] = '\0';
	
	SetTagChar("SN_create",szValue);
	printf("SN�������\n");
	printf("%s",szValue);
	printf("\n");
}


//��SN��浽txt�ļ���
void SaveSN(void)
{
	FILE *fp = fopen("telegram.txt","rw");		//��telegram.txt�ļ�
	if(fp==NULL)
	{
			printf("�洢���ĵ�txt�ļ������ڣ�\n");
			return;
	} 
	
	
	
}

//�������ļ���ȡ�����洢��TXT������
//���룺FilePath
//����TXT�ļ�
void SaveTxtFile(void)
{
	
	
	
}