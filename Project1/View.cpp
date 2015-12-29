#define _CRT_SECURE_NO_WARNINGS
#define MAX_SIZE 1000
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<string.h>
#include"MoudleAndControl.h"
int main() {
	system("title Library");
	system("color 03");
	int index;
	Library library = createLibrary();
	RecordSet recordSet = createRecordSet();
	ReaderSet readerSet = createReaderSet();
	char path[3][20] = {"library.txt","records.txt","readers.txt"};
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
	printf("|                        6.采编入库                       |\n");
	printf("|                        7.清除库存                       |\n");
	printf("|                        8.查看记录                       |\n");
	printf("|                        9.查看读者                       |\n");
	printf("|                        10.退出                          |\n");
	printf("+---------------------------------------------------------+\n");
	printf("|                  输入序号进入对应功能                   |\n");
	printf("+---------------------------------------------------------+\n");
	printf("->");
	while (scanf("%d", &index) == 1) {
		system("cls");
		if (index == 1) {	//综合查询界面
			
			BookClass *bookClasses[MAX_SIZE];
			char info[20];
			printf("->请输入图书的名字或者作者：\n->");
			scanf("%s", info);
			int length = queryLibrary(library, bookClasses, info);
			//printf("%d\n",length);
			if (length == 0) {
				system("cls");
				printf("->图书馆中无此书\n");
			}
			else {
				system("cls");
				printf("");
				printf("书号\t书名\t作者\t总库量\t现有量\n");
				for (int i = 0; i < length; i++) {
					printf("%d\t%s\t%s\t%d\t%d\n",
						bookClasses[i]->id,
						bookClasses[i]->name,
						bookClasses[i]->author,
						bookClasses[i]->totalMemory,
						bookClasses[i]->currentMemory);
				}
			}
		}
		if (index == 2) {
			
			int bookId, readerId, amount;
			printf("->请输入你的借书证号：\n->");
			scanf("%d", &readerId);
			system("cls");
			if (searchReader(readerSet, readerId) == NULL) {
				printf("->借书证号错误\n");
			}
			else {
				printf("->请输入书号以及数量：\n->");
				scanf("%d %d", &bookId, &amount);
				amount = Borrow(&library, &readerSet, &recordSet, readerId, bookId, amount);
				system("cls");
				printf("->你借走了 %d 本书。\n", amount);
			}
		}
		if (index == 3) {
			
			int bookId, readerId, amount;
			printf("->请输入你的借书证号、书号和数量。\n->");
			scanf("%d %d %d", &readerId, &bookId, &amount);
			system("cls");
			if (searchReader(readerSet, readerId) == NULL) {
				printf("->借书证号错误。\n");
			}
			else {
				amount = Return(&library, &readerSet, &recordSet, readerId, bookId, amount);
				printf("->你归还了 %d 本书。\n", amount);
			}
		}
		if (index == 4) {
			
			printf("->输入你的名字：\n->");
			char name[20];
			scanf("%s", name);
			system("cls");
			printf("->你的借书证号为：%d \n", addReader(&readerSet, name));
		}
		if (index == 5) {
			int readerId;
			char name[20];
			printf("->请输入你的借书证号：\n->");
			scanf("%d",&readerId);
			system("cls");
			if (searchReader(readerSet, readerId) == NULL) {
				printf("->借书证号错误\n");
			}
			else {
				printf("->输入你的新姓名\n->");
				scanf("%s",name);
				alterReader(&readerSet,readerId,name);
				system("cls");
				printf("->修改成功\n");
			}
		}
		if (index == 6) {	//采编入库
			char name[20];
			char author[20];
			int amount;
			printf("->请输入图书名字、作者以及册数。->\n");
			scanf("%s %s %d",name,author,&amount);
			addBooks(&library,name,author,amount);
			system("cls");
			printf("->入库成功\n");
		} 
		if (index == 7) {	//清除库存	
			int id;
			printf("->输入旧书的书号：\n->");
			scanf("%d",&id);
			clearBooks(&library,id);
		}
		if (index == 8) {
			showRecord(recordSet);
		}
		if (index == 9) {
			showReaders(readerSet);
		}
		if (index == 10) {
			save(path[0], path[1], path[2], library, readerSet, recordSet);
			printf("->保存成功。\n");
			system("cls");
			printf("->感谢你的使用。\n");
			printf("->by 非鱼");
			Sleep(1000);
			exit(0);
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
		printf("|                        6.采编入库                       |\n");
		printf("|                        7.清除库存                       |\n");
		printf("|                        8.查看记录                       |\n");
		printf("|                        9.查看读者                       |\n");
		printf("|                        10.退出                          |\n");
		printf("+---------------------------------------------------------+\n");
		printf("|                  输入序号进入对应功能                   |\n");
		printf("+---------------------------------------------------------+\n");
		printf("->");
	}
	
	return 0;
}


