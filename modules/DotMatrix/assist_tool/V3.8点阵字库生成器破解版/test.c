#include <stdio.h>  

int main(void)  
{  
	char chs[32];  
	FILE *fp;  
	int i,j;  
	unsigned long offset;  
	unsigned char s[]="毛";  
	offset=((s[0]-0xa1)*94+(s[1]-0xa1))*32;//根据内码找出汉字在HZK16中的偏移位置  
	if ((fp=fopen("HZK16","rb"))==NULL) return 1;  
	fseek(fp,offset,SEEK_SET);  
	fread(chs,32,1,fp);  
	for (i=0;i<32;i++)  
	{  
		if (i%2==0) printf("\n");   //每行两字节,16X16点阵  
		for (j=7;j>=0;j--)  
		{  
			if (chs[i]&(0x1<<j))printf("*"); //由高到低,为1则输出'O',反之输出'-';  
			else  
				printf("-");  
		}  
	}  
	fclose(fp);  
	return 0;  
}  
