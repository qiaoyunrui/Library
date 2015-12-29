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
	printf("图书馆目前共有%d类书，共有%d本书，现有%d本。\n",library.numOfBookClass,
		library.totalMemory,
		library.currentMemory);
	printf("1.查询书的信息\n");
	printf("2.借阅\n");
	printf("3.还书\n");
	printf("4.申请借书证号\n");
	printf("5.修改读者信息\n");
	printf("6.采编入库\n");
	printf("7.清除库存\n");
	printf("8.关于\n");
	printf("9.退出\n");
	printf("请输入数字进入相应功能：");
	while (scanf("%d", &index) == 1) {
		if (index == 1) {	//综合查询界面
			system("cls");
			BookClass *bookClasses[MAX_SIZE];
			char info[20];
			printf("请输入图书的名字或者作者：");
			scanf("%s", info);
			int length = queryLibrary(library, bookClasses, info);
			//printf("%d\n",length);
			if (length == 0) {
				printf("无此书\n");
			}
			else {
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
			printf("请输入你的借书证号：\n");
			scanf("%d", &readerId);
			if (searchReader(readerSet, readerId) == NULL) {
				printf("借书证号错误\n");
			}
			else {
				printf("请输入书号以及数量：");
				scanf("%d %d", &bookId, &amount);
				amount = Borrow(&library, &readerSet, &recordSet, readerId, bookId, amount);
				printf("你借走了 %d 本书\n", amount);
			}
		}
		if (index == 3) {
			int bookId, readerId, amount;
			printf("请输入你的借书证号、书号和数量\n");
			scanf("%d %d %d", &readerId, &bookId, &amount);
			if (searchReader(readerSet, readerId) == NULL) {
				printf("借书证号错误\n");
			}
			else {
				amount = Return(&library, &readerSet, &recordSet, readerId, bookId, amount);
				printf("你还了 %d 本书\n", amount);
			}
		}
		if (index == 4) {
			printf("输入你的名字：");
			char name[20];
			scanf("%s", name);
			printf("你的借书证号为：%d \n", addReader(&readerSet, name));
		}
		if (index == 5) {
			int readerId;
			char name[20];
			printf("请输入你的借书证号\n");
			scanf("%d",&readerId);
			if (searchReader(readerSet, readerId) == NULL) {
				printf("借书证号错误\n");
			}
			else {
				printf("输入你的新姓名\n");
				scanf("%s",name);
				printf("修改成功\n");
			}
		}
		if (index == 6) {	//采编入库
			system("cls");
			char name[20];
			char author[20];
			int amount;
			printf("请输入图书名字、作者以及册数\n");
			scanf("%s %s %d",name,author,&amount);
			addBooks(&library,name,author,amount);
			//system("cls");
			printf("入库成功\n");
		} 
		if (index == 7) {	//清除库存	
			int id;
			printf("输入旧书的书号：");
			scanf("%d",&id);
			clearBooks(&library,id);
		}
		if (index == 8) {
		}
		if (index == 9) {
			save(path[0], path[1], path[2],library,readerSet,recordSet);
			exit(0);
		}
		//system("cls");
		printf("1.查询书的信息\n");
		printf("请输入数字进入相应功能：");
	}
	getchar();
	return 0;
}


