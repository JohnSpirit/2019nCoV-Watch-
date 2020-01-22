#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<Windows.h>
int main()
{
	FILE* fp, *fp2;
	double speed = 1.0;
	int infected = 0, suspicious = 0, cured = 0, death = 0;
	char cmd[100] = "wget >nul 2>nul https://3g.dxy.cn/newh5/view/pneumonia?from=singlemessage",
		delf[30] = "del pneumonia", info[20480] = { 0 }, 
		data[81] = { 0 }, lastdata[81] = { 0 },
		date[64] = { 0 }, time[64] = { 0 }, *ptr = 0;
	printf("请输入显示循环间隔(s)：\n");scanf("%lf", &speed);speed *= 1000;
	
	strcat(delf, "*");
	system("cls");
	
	while (1)
	{
		system(cmd);
		fp = fopen("pneumonia@from=singlemessage", "r");
		fp2 = fopen("data2.txt", "a+");
		if (fp == NULL)
		{
			printf("文件访问错误！按任意键退出。\n");
			getchar();
			exit(0);
		}

		do
		{
			fgets(info, 20480, fp);
			ptr = strstr(info, "countRemark");
		} while (ptr == 0);

		strncpy(data, ptr, 80);
		sscanf(data, "%*s%d%*s%*s%d%*s%*s%d%*s%*s%d%*s", &infected, &suspicious, &cured, &death);

		rewind(fp);
		do
		{
			fgets(info, 20480, fp);
			ptr = strstr(info, "mapTitle");
		} while (ptr == 0);
		
		strncpy(data, ptr, 80);
		sscanf(data, "%*s%s%s", date,time);

		if (strcmp(lastdata, data))
		{
			printf("截至 %s %s,全国 确诊 %d 例 疑似 %d 例 治愈 %d 例 死亡 %d 例\n", date, time, infected, suspicious, cured, death);
			fprintf(fp2, "%s,%s,%d,%d,%d,%d\n", date, time, infected, suspicious, cured, death);
			strcpy(lastdata, data);
		}
		else puts("正在监测中...\r");
		_fcloseall();
		system(delf);
		Sleep(speed);
	}
}
