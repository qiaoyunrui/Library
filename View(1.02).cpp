#define _CRT_SECURE_NO_WARNINGS
#define MAX_SIZE 1000//个人最大借书量
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<string.h>
#include<conio.h>
#include"MoudleAndControl(1.1).h"
int main(int argc, char *argv[]) 
{
	system("title 图书馆管理系统 by 周向荣、汤承宗");
	system("color 02");
	//system("mode con cols=60 lines=20");//设置窗口大小
	char index;
	Library library = createLibrary();//创建图书馆
	RecordSet recordSet = createRecordSet();//创建借还书记录卡
	ReaderSet readerSet = createReaderSet();//读者借书证号创建
	char path[3][20] = {"library.txt","records.txt","readers.txt"};//文件名
	printf("加载文件中....\n");
	load(path[0],path[1],path[2],&library,&readerSet,&recordSet);
	system("cls");
	printf("加载文件成功\n");
	system("cls");
	printf("->图书馆目前共有 %d 类书，共有 %d 本书，现有 %d 本。\n",
		library.numOfBookClass,
		library.totalMemory,
		library.currentMemory);
	printf("+------------------欢迎进入图书管理系统-------------------+\n"); 
	printf("|                                                         |\n");
	printf("|                        1.查询                           |\n");
	printf("|                        2.借书                           |\n");
	printf("|                        3.还书                           |\n");
	printf("|                        4.申请借书证                     |\n");
	printf("|                        5.修改个人信息                   |\n");
	printf("+---------------------------------------------------------+\n");
	printf("|                        6.退出                           |\n");
	printf("|                        7.管理员系统                     |\n");
	printf("+---------------------------------------------------------+\n");
	printf("|                    输入序号进入对应功能                 |\n");
	printf("+---------------------------------------------------------+\n");
	printf("->");
	while (scanf("%d", &index) == 1) 
	{
		system("cls");
		if (index == 1)//查询书籍 
		{	//综合查询界面			
			BookClass *bookClasses[MAX_SIZE];
			char info[20];
			printf("->请输入图书的名字、作者或者出版社：\n->");
			scanf("%s", info);
			int length = queryLibrary(library, bookClasses, info);
			if (length == 0) 
			{
				system("cls");
				printf("->图书馆中无此书\n");
			}
			else 
			{
				system("cls");
				printf("");
				printf("书号\t书名\t作者\t出版社\t总库量\t现有量\n");
				for (int i = 0; i < length; i++) {
					printf("%3d%10s%10s%10s%5d%5d\n",
						bookClasses[i]->id,
						bookClasses[i]->name,
						bookClasses[i]->author,
						bookClasses[i]->publisher,
						bookClasses[i]->totalMemory,
						bookClasses[i]->currentMemory);
				}
			}
		}
		if (index == 2)//借书 
		{
			
			int bookId, readerId, amount;
			printf("->请输入你的借书证号：\n->");
			scanf("%d", &readerId);
			system("cls");
			if (searchReader(readerSet, readerId) == NULL) 
			{
				printf("->借书证号错误\n");
			}
			else 
			{
				printf("->请输入书号以及数量：\n->");
				scanf("%d %d", &bookId, &amount);
				amount = Borrow(&library, &readerSet, &recordSet, readerId, bookId, amount);
				system("cls");
				printf("->你借走了 %d 本书。\n", amount);
			}
		}
		if (index == 3)//还书 
		{
			
			int bookId, readerId, amount;
			printf("->请输入你的借书证号、书号和数量。\n->");
			scanf("%d %d %d", &readerId, &bookId, &amount);
			system("cls");
			if (searchReader(readerSet, readerId) == NULL) 
			{
				printf("->借书证号错误。\n");
			}
			else 
			{
				amount = Return(&library, &readerSet, &recordSet, readerId, bookId, amount);
				printf("->你归还了 %d 本书。\n", amount);
			}
		}
		if (index == 4)//申请借书证 
		{
			
			printf("->输入你的名字：\n->");
			char name[20];
			scanf("%s", name);
			system("cls");
			printf("->你的借书证号为：%d \n", addReader(&readerSet, name));
		}
		if (index == 5)//修改个人信息 
		{
			int readerId;
			char name[20];
			printf("->请输入你的借书证号：\n->");
			scanf("%d",&readerId);
			system("cls");
			if (searchReader(readerSet, readerId) == NULL) 
			{
				printf("->借书证号错误\n");
			}
			else 
			{
				printf("->输入你的新姓名\n->");
				scanf("%s",name);
				alterReader(&readerSet,readerId,name);
				system("cls");
				printf("->修改成功\n");
			}
		}
		if (index == 6) 
		{
			save(path[0], path[1], path[2], library, readerSet, recordSet);
			printf("->保存成功。\n");
			system("cls");
			printf("->感谢你的使用。\n");
			Sleep(1000);
			exit(0);
		}
		if(index == 7)//管理员模块
		{
			fflush(stdin);
			char Mima[7];
			char mima[7]="123456";
			save(path[0], path[1], path[2], library, readerSet, recordSet);
			library = createLibrary();//图书馆初始化
			recordSet = createRecordSet();//借还书记录卡初始化
			readerSet = createReaderSet();//读者借书证号初始化
			load(path[0],path[1],path[2],&library,&readerSet,&recordSet);
			printf("请输入管理员密码：");
			scanf("%s",&Mima);
			system("cls");
			if(strcmp(Mima,mima)==0)
			{
				printf("+---------------------------------------------------------+\n");
				printf("|                        1.查看记录                       |\n");
				printf("|                        2.查看读者                       |\n");
				printf("|                        3.查看书库                       |\n");
				printf("|                        4.采编入库                       |\n");
				printf("|                        5.清除库存                       |\n");
				printf("|                        6.删改读者                       |\n");
				printf("|                        7.退出                           |\n");
				printf("+---------------------------------------------------------+\n");
				printf("|                  输入序号进入对应功能                   |\n");
				printf("+---------------------------------------------------------+\n");
				printf("->");
				int index_2;
				while (scanf("%d", &index_2) == 1)
				{
					system("cls");
					if(index_2 == 1)
					{
						showRecord(recordSet);//借还书记录显示
					}
					if(index_2 == 2)
					{
						showReaders(readerSet);//读者信息显示
					}
					if(index_2 == 3)
					{
						showAllBooks(&library);
					}
					if(index_2 == 4)//采编入库
					{
						char name[20];
						char author[20];
						char publisher[20];
						int amount;
						printf("->请输入图书名字、作者、出版社以及册数。\n->");
						scanf("%s %s %s %d",name,author,publisher,&amount);
						addBooks(&library,name,author,publisher,amount);//增加书籍
						system("cls");
						printf("->入库成功\n");					
					}
					if(index_2 == 5)//清空库存
					{
						int id;
						printf("->输入旧书的书号：\n->");
						scanf("%d",&id);
						clearBooks(&library,id);//清空书籍
					}
					if(index_2 == 6)
					{
						int choose;
						showReaders(readerSet);//读者信息显示
						printf("->请输入所要进行操作的相应序号：1、修改读者名字|2、删除读者|3、退出\n->");
						scanf("%d",&choose);
						if(choose == 1)
						{
							int readerId;
							char name[20];
			                printf("->请输入所需修改的读者借书证号：\n->");
			                scanf("%d",&readerId);
			                system("cls");
			                if (searchReader(readerSet, readerId) == NULL) 
							{
								printf("->借书证号错误\n");
							}
							else 
							{
								printf("->输入你的新姓名\n->");
				                scanf("%s",name);
				                alterReader(&readerSet,readerId,name);
				                system("cls");
				                printf("->修改成功\n");
							}
						}
						else if(choose == 2)
						{
							int readerId;
							printf("->请输入所需删除的读者借书证号：\n->");
			                scanf("%d",&readerId);
			                system("cls");
							if (searchReader(readerSet, readerId) == NULL) 
							{
								printf("->借书证号错误\n");
							}
							else
							{
								removeReader(&readerSet,readerId);
							}
						}
						else if(choose == 3)
							;
						else
							printf("您的输入有误，请重新输入!\n");

					}
					if(index_2 == 7)
					{	
						save(path[0], path[1], path[2], library, readerSet, recordSet);
						library = createLibrary();//图书馆初始化
			            recordSet = createRecordSet();//借还书记录卡初始化
			            readerSet = createReaderSet();//读者借书证号初始化
			            load(path[0],path[1],path[2],&library,&readerSet,&recordSet);
						system("cls");
						break;
					}
					int non;
					printf("\n->输入任意数字返回主界面。\n->");
					scanf("%d",&non);
					system("cls");
					printf("+---------------------------------------------------------+\n");
					printf("|                        1.查看记录                       |\n");
					printf("|                        2.查看读者                       |\n");
					printf("|                        3.查看书库                       |\n");
					printf("|                        4.采编入库                       |\n");
					printf("|                        5.清除库存                       |\n");
					printf("|                        6.删改读者                       |\n");
				    printf("|                        7.退出                           |\n");
					printf("+---------------------------------------------------------+\n");
					printf("|                  输入序号进入对应功能                   |\n");
					printf("+---------------------------------------------------------+\n");
					printf("->");
				}
			}
		}
		int non;
		printf("->输入任意数字返回主界面。\n->");
		scanf("%d",&non);
		system("cls");
		printf("->图书馆目前共有 %d 类书，共有 %d 本书，现有 %d 本。\n",
			library.numOfBookClass,
			library.totalMemory,
			library.currentMemory);
		printf("+---------------------------------------------------------+\n");
		printf("|                        1.查询                           |\n");
		printf("|                        2.借书                           |\n");
		printf("|                        3.还书                           |\n");
		printf("|                        4.申请借书证                     |\n");
		printf("|                        5.修改个人信息                   |\n");
		printf("+---------------------------------------------------------+\n");
		printf("|                        6.退出                           |\n");
		printf("|                        7.管理员系统                     |\n");
		printf("+---------------------------------------------------------+\n");
		printf("|                    输入序号进入对应功能                 |\n");
		printf("+---------------------------------------------------------+\n");
		printf("->");
	}
	return 0;
}


