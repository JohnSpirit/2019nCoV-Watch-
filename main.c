/*作者：14岁醋酸铜 2020.1.22 感谢b站up：迷茫的嘉鸿 提供的灵感*/
/*本程序需要同一目录下的wget下载器，切勿单独编译运行*/
/*编译运行环境：Win10，VS2017*/
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<Windows.h>
int main()
{
	FILE* fp, *fp2;//文件指针
	double speed = 1.0;//更新速度，以秒计
	int infected = 0, suspicious = 0, cured = 0, death = 0;//确诊、疑似、康复和死亡数
	char cmd[100] = "wget >nul 2>nul https://3g.dxy.cn/newh5/view/pneumonia?from=singlemessage",//wget命令
		delf[30] = "del pneumonia*", //每次获取数据后删除文件
		info[20480] = { 0 },*ptr = 0,//用于读取文件与确定提取数据位置
		data[81] = { 0 }, lastdata[81] = { 0 },//存放需要提取的数据
		date[64] = { 0 }, time[64] = { 0 };//时间
	printf("请输入显示循环间隔(s)：\n"); scanf("%lf", &speed); speed *= 1000; system("cls");//读取更新速度并清屏

	while (1)
	{
		system(cmd);//发送请求
		fp = fopen("pneumonia@from=singlemessage", "r");//打开文件准备读
		fp2 = fopen("OUTPUT.txt", "a+");//打开存档文件准备添加
		if (fp == NULL)
		{
			printf("文件访问错误！按任意键退出。\n");
			getchar();	exit(0);
		}

		do//获取人数信息
		{
			fgets(info, 20480, fp);//逐行扫描含有countRemark的行，该行含有人数
			ptr = strstr(info, "countRemark");
		} while (ptr == 0);

		strncpy(data, ptr, 80);//将含有待提取数据字符串复制
		sscanf(data, "%*s%d%*s%*s%d%*s%*s%d%*s%*s%d%*s", &infected, &suspicious, &cured, &death);//提取

		rewind(fp);//指针回溯
		do//获取时间信息
		{
			fgets(info, 20480, fp);
			ptr = strstr(info, "mapTitle");//逐行扫描含有maptitle的行，该行含有时间
		} while (ptr == 0);

		strncpy(data, ptr, 80);
		sscanf(data, "%*s%s%s", date, time);

		if (strcmp(lastdata, data))//当数据有更新时打印并存盘
		{
			printf("\7截至 %s %s,全国 确诊 %d 例 疑似 %d 例 治愈 %d 例 死亡 %d 例\n", date, time, infected, suspicious, cured, death);
			fprintf(fp2, "%s,%s,%d,%d,%d,%d\n", date, time, infected, suspicious, cured, death);
			strcpy(lastdata, data);
		}
		else puts("正在监测中...\r");//否则显示“正在监测中”并不存盘

		_fcloseall();//关闭所有文件
		system(delf);//删除响应文件
		Sleep(speed);//等待下一次运行
	}
}
