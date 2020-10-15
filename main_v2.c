/*作者：14岁醋酸铜 2020.1.22 感谢b站up：迷茫的嘉鸿 提供的灵感*/
/*本程序需要同一目录下的wget下载器，切勿单独编译运行*/
/*编译运行环境：Win10，VS2017*/
#define _CRT_SECURE_NO_WARNINGS
#define TIMESPAN (time(0)-start)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<Windows.h>
#include<time.h>
int main()
{
	FILE* fp, *fp2;//文件指针
	double speed = 1.0;//更新速度，以秒计
	time_t start = 1579691340;//2020-01-22 22:09(UTC+8)
	int infected = 0, suspicious = 0, cured = 0, death = 0;//确诊、疑似、康复和死亡数
	char cmd[74] = "wget >nul 2>nul https://3g.dxy.cn/newh5/view/pneumonia?from=singlemessage",//wget命令
		delf[15] = "del pneumonia*", //每次获取数据后删除文件
		info[16384] = { 0 }, *ptr = 0,//用于读取文件与确定提取数据位置
		data[2048] = { 0 },//存放需要提取的数据
		Date[64] = { 0 }, Time[64] = { 0 }, Lasttime[64] = { 0 },//时间
		inf_str[64] = { 0 }, sus_str[64] = { 0 }, cur_str[64] = { 0 }, dea_str[64] = { 0 };//人数

	printf("请输入显示循环间隔(s) = "); scanf("%lf", &speed); speed *= 1000; system("cls");//读取更新速度并清屏

	fp2 = fopen("OUTPUT.txt", "r");//打开存档文件,输出全部内容
	char delim[2] = ",", *date_ptr = 0, *time_ptr = 0;
	while (!feof(fp2))//读取CSV文件
	{
		char _info[64] = { 0 };
		fgets(_info, 64, fp2);
		if (!_info[0])break;
		date_ptr = strtok(_info, delim);
		time_ptr = strtok(NULL, delim);
		sscanf(_info + 17, "%d,%d,%d,%d\n", &infected, &suspicious, &cured, &death);//提取
		printf("截至 %s %s,全国 确诊 %4d 例 疑似 %4d 例 治愈 %4d 例 死亡 %4d 例\n", date_ptr, time_ptr, infected, suspicious, cured, death);
		strcpy(Lasttime, time_ptr);
	}
	fclose(fp2);

	while (1)
	{
		system(cmd);//发送请求
		fp = fopen("pneumonia@from=singlemessage", "r");//打开文件准备读
		fp2 = fopen("OUTPUT.txt", "a+");//打开存档文件准备添加
		if (fp == NULL || fp2 == NULL)
		{
			printf("文件访问错误！按任意键退出。\n");
			getchar();	exit(0);
		}

		do//获取信息
		{
			fgets(info, 16383, fp);
			ptr = strstr(info, "mapTitle");//逐行扫描含有maptitle的行，该行含有所需提取信息
		} while (ptr == 0 && !feof(fp));
		if (feof(fp))break;

		strncpy(data, ptr, 2048);
		sscanf(data, "%*s%s%s%*s%*s%*s%*s%*s%s%*s%*s%*s%*s%s%*s%*s%*s%*s%s%*s%*s%*s%*s%s", Date, Time, inf_str, sus_str, cur_str, dea_str);
		//#4169e2\">%d</span>
		Time[5] = 0;
		sscanf(inf_str + 9, "%d", &infected);
		sscanf(sus_str + 9, "%d", &suspicious);
		sscanf(cur_str + 9, "%d", &cured);
		sscanf(dea_str + 9, "%d", &death);

		if (strcmp(Lasttime, Time))//当数据有更新时打印并存盘
		{
			printf("截至 %s %s,全国 确诊 %4d 例 疑似 %4d 例 治愈 %4d 例 死亡 %4d 例\n", Date, Time, infected, suspicious, cured, death);
			fprintf(fp2, "%s,%s,%d,%d,%d,%d\n", Date, Time, infected, suspicious, cured, death);
			strcpy(Lasttime, Time);
		}

		printf("正在监测中，已监测 %lldh %lldm %llds                       \r",
			(((TIMESPAN - TIMESPAN % 60) / 60 - ((TIMESPAN - TIMESPAN % 60) / 60) % 60) / 60),
			(((TIMESPAN - TIMESPAN % 60) / 60) % 60),
			TIMESPAN % 60);//否则显示“正在监测中”并不存盘

		_fcloseall();//关闭所有文件
		system(delf);//删除响应文件
		Sleep(speed);//等待下一次运行
	}
}
