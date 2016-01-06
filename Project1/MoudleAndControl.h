#define _CRT_SECURE_NO_WARNINGS
#define MAX_SIZE 1000
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<Windows.h>
/*MOUDLE*/
//-----------------------------图书馆-------------------------
typedef struct PriBookClass {
	int id;	//书号
	char name[20];
	char author[20];
	char publisher[20];
	int totalMemory;
	int currentMemory;
	struct PriBookClass *next;
}BookClass;
typedef struct PriLibrary {
	int totalMemory;
	int currentMemory;
	int numOfBookClass;	
	BookClass *headBookClass;	
}Library;
//-----------------------------个人书库-----------------------
typedef struct PriBorBook {
	int id;
	int amount;
}BorBook;
typedef struct PriBorBookSet {
	BorBook *borBooks[MAX_SIZE];
	int amount;
}BorBookSet;
//-----------------------------读者信息-----------------------
typedef struct PriReader {
	int id;
	char name[20];
	BorBookSet borBookSet;
	struct PriReader *next;
}Reader;
typedef struct PriReaderSet {
	int amount;
	Reader *headReader;
}ReaderSet;
//-----------------------------借还记录-----------------------
typedef struct PriRecord {
	int readerId;
	int bookId;
	int amount;	//图书数量
	char time[30];	//借书时间
	char limitedTime[30];	//归还期限
	int action;
	struct PriRecord *next;
}Record;
typedef struct PriRecordSet {
	int amount;	//记录数量
	Record *headRecord;	//头节点
}RecordSet;
//-----------------------------Id检索表-----------------------
typedef struct PriSearchListById {
	int id;
	BookClass *bookClass;
}SearchIdNode;
typedef struct PriSearchListByIdSet {
	SearchIdNode searchId[MAX_SIZE];
	int amount;
}SearchIdList;
//-----------------------------综合检索表-------------------
typedef struct PriSearchNode {
	char name[20];
	char author[20];
	char publisher[20];
	BookClass *bookClass;
}SearchNode;
typedef struct PriSearchList {
	SearchNode searchNode[MAX_SIZE];
	int amount;
}SearchList;
/*Control*/
//-----------------------------个人书库-----------------------
/*查找个人书库的书籍*/
 BorBook *searchBorBook(BorBookSet borBookSet, int id) {
	BorBook *borBook = NULL;
	for (int i = 0; i < borBookSet.amount; i++) {
		if (id == borBookSet.borBooks[i]->id) {
			borBook = borBookSet.borBooks[i];
		}
	}
	return borBook;
}
/*新增个人书库的书籍*/
int addNewBorBook(BorBookSet *borBookSet, int id, int amount) {
	if (borBookSet->amount + 1 == MAX_SIZE) {
		return 0;
	}
	else {
		BorBook *borBook = (BorBook *)malloc(sizeof(BorBook));
		borBook->amount = amount;
		borBook->id = id;
		borBookSet->borBooks[borBookSet->amount] = borBook;	//ERROR
		borBookSet->amount++;
		return 1;
	}
}
//-----------------------------读者信息-----------------------
/*获取读者最大ID*/
int getReaderMaxId(ReaderSet readerSet) {
	int maxId;
	if (readerSet.headReader) {
		maxId = readerSet.headReader->id;
		while (readerSet.headReader) {
			maxId = maxId > readerSet.headReader->id ?  maxId : readerSet.headReader->id;
			readerSet.headReader = readerSet.headReader->next;
		}
	}
	else {
		maxId = 0;
	}
	return maxId;
}
/*创建读者列表*/
ReaderSet createReaderSet() {
	ReaderSet readerSet;
	readerSet.amount = 0;
	readerSet.headReader = NULL;
	return readerSet;
}
/*注册,返回读者id*/
int addReader(ReaderSet *readerSet,char *name) {
	Reader *reader;
	reader = (Reader *)malloc(sizeof(Reader));
	reader->id = getReaderMaxId(*readerSet) + 1;	//使其id称为最大的一个
	strcpy(reader->name,name);
	reader->borBookSet.amount = 0;
	reader->next = readerSet->headReader;
	readerSet->headReader = reader;
	readerSet->amount++;
	return reader->id;
}
/*显示读者列表*/
void showReaders(ReaderSet readerSet) {
	while (readerSet.headReader) {
		printf("->%d\t%s\t%d\t\n",
			readerSet.headReader->id,
			readerSet.headReader->name,
			readerSet.headReader->borBookSet.amount);
		for (int i = 0; i < readerSet.headReader->borBookSet.amount; i++) {
			printf("%d\t%d\t", readerSet.headReader->borBookSet.borBooks[i]->id,
				readerSet.headReader->borBookSet.borBooks[i]->amount);
		}
		printf("\n");
		readerSet.headReader = readerSet.headReader->next;
	}
}
/*根据id查找读者*/
Reader *searchReader(ReaderSet readerSet, int id) {
	Reader *reader = NULL;
	while (readerSet.headReader) {
		if (readerSet.headReader->id == id) {
			reader = readerSet.headReader;
		}
		readerSet.headReader = readerSet.headReader->next;
	}
	return reader;
}
/*修改读者名字*/
void alterReader(ReaderSet *readerSet, int id, char *name) {
	Reader *reader;
	reader = searchReader(*readerSet,id);
	if (reader) {
		strcpy(reader->name,name);
	}
	else {
		printf("未找到该id\n");
	}
}
/*为个人书库增加书籍，即借书*/
int addBorBooks(ReaderSet *readerSet,int readerId,int bookId,int amount) {
	Reader *reader;
	BorBook *borBook;
	reader = searchReader(*readerSet,readerId);
	if (reader) {
		borBook = searchBorBook(reader->borBookSet,bookId);
		if (borBook) {	//已经有这本书
			borBook->amount += amount;
		}
		else {	//没有这本书
			if (!addNewBorBook(&(reader->borBookSet),bookId,amount)) {
				amount = 0;
			}
		}
	}
	return amount;
}
/*从个人书库取书，即还书*/
int deteleBorBook(ReaderSet *readerSet, int readerId, int bookId, int amount) {
	Reader *reader;
	BorBook *borBook;
	reader = searchReader(*readerSet, readerId);
	if (reader) {
		borBook = searchBorBook(reader->borBookSet, bookId);
		if (borBook) {
			amount = amount > borBook->amount ? borBook->amount : amount;	//所还的书超出容量
			borBook->amount -= amount;
		}
	}
	return amount;
}
//-----------------------------借还记录-----------------------
/*创建记录表*/
RecordSet createRecordSet() {
	RecordSet recordSet;
	recordSet.amount = 0;
	recordSet.headRecord = NULL;
	return recordSet;
}
/*添加纪录*/
void addRecord(RecordSet *recordSet,int readerId,int bookId,int amount,int action) {
	Record *record = (Record *)malloc(sizeof(Record));
	record->readerId = readerId;
	record->bookId = bookId;
	record->amount = amount;
	record->action = action;
	time_t t;
	struct tm *timeinfo;
	t = time(NULL);
	char *time;
	time = ctime(&t);
	printf("%s\n",time);
	t += 2592000;
	timeinfo = localtime(&t);
	strcpy(record->time,time);
	if (action) {	//借书
		strcpy(record->limitedTime, asctime(timeinfo));
	}
	else {	//还书
		strcpy(record->limitedTime, time);
	}
	record->next = recordSet->headRecord;
	recordSet->headRecord = record;
	recordSet->amount++;
}
/*显示记录*/
void showRecord(RecordSet recordSet) {
	while (recordSet.headRecord) {
		printf("->借书证号：%d\t书号：%d\t数量：%d\n日期：%s",
			recordSet.headRecord->readerId,
			recordSet.headRecord->bookId,
			recordSet.headRecord->amount,
			recordSet.headRecord->time);
		if (recordSet.headRecord->action == 1) {
			printf("截至日期：%s", recordSet.headRecord->limitedTime);
			printf("借书\n");
		}
		else {
			printf("还书\n");
		}
		recordSet.headRecord = recordSet.headRecord->next;
	}
}
//-----------------------------ID索引表-----------------------
/*创建Id索引表*/
SearchIdList CreateSearchIdList(Library library) {
	SearchIdList searchIdList;
	int i = 0;
	searchIdList.amount = library.numOfBookClass;
	while (library.headBookClass) {
		searchIdList.searchId[i].id = library.headBookClass->id;
		searchIdList.searchId[i].bookClass = library.headBookClass;
		i++;
		library.headBookClass = library.headBookClass->next;
	}
	return searchIdList;
}
/*通过Id查询*/
BookClass *searchById(SearchIdList searchIdList, int id) {
	BookClass *bookClass = NULL;
	for (int i = 0; i < searchIdList.amount; i++) {
		if (id == searchIdList.searchId[i].id) {
			bookClass = searchIdList.searchId[i].bookClass;
			break;
		}
	}
	return bookClass;
}
//-----------------------------综合检索表-------------------
/*创建综合索引表*/
SearchList createSearchList(Library library) {
	SearchList searchList;
	searchList.amount = library.numOfBookClass;	//获取书的总类数
	int i = 0;
	while (library.headBookClass) {
		strcpy(searchList.searchNode[i].name, library.headBookClass->name);
		strcpy(searchList.searchNode[i].author, library.headBookClass->author);
		strcpy(searchList.searchNode[i].publisher, library.headBookClass->publisher);
		searchList.searchNode[i].bookClass = library.headBookClass;
		i++;
		library.headBookClass = library.headBookClass->next;	//指向下一个节点
	}
	return searchList;
}
/*综合查找*/
int search(SearchList searchList, int index[1000], char *key) {
	char id[10];
	int sign = 0;
	for (int i = 0; i < searchList.amount; i++) {
		if (strcmp(searchList.searchNode[i].name, key)  == 0||
			strcmp(searchList.searchNode[i].author, key) == 0 || 
			strcmp(searchList.searchNode[i].publisher,key) == 0) {	//匹配
			//printf("Match\n");
			index[sign] = i;
			sign++;
		}
	}
	return sign;
}
//-----------------------------图书馆-------------------------
/*创建*/
Library createLibrary() {
	Library library;
	library.currentMemory = 0;
	library.totalMemory = 0;
	library.numOfBookClass = 0;
	library.headBookClass = NULL;
	return library;
}
/*获取最大ID*/
int getBookClassMaxId(Library library) {
	int maxId = 0;
	while (library.headBookClass) {
		maxId = maxId > library.headBookClass->id ? maxId : library.headBookClass->id;
		library.headBookClass = library.headBookClass->next;
	}
	return maxId;
}
/*综合查询*/
int queryLibrary(Library library, BookClass *bookClasses[MAX_SIZE], char* key) {
	SearchList searchList;
	searchList = createSearchList(library);	//获得检索线性表
	int index[1000];
	int length = search(searchList, index, key);
	for (int i = 0; i < length; i++) {
		bookClasses[i] = searchList.searchNode[index[i]].bookClass;
	}
	return length;
}
/*查询前驱节点*/
BookClass *findPreBookClass(Library library,int id) {
	BookClass *preBookClass = NULL,*bookClass;
	if (library.headBookClass) {
		bookClass = library.headBookClass;
		while (bookClass && bookClass->id != id) {
			preBookClass = bookClass;
			bookClass = bookClass->next;
		}
	}
	return preBookClass;
}
/*ID查询*/
BookClass *queryLibraryById(Library library, int id) {
	BookClass *bookClass;
	SearchIdList searchIdList = CreateSearchIdList(library);
	bookClass = searchById(searchIdList, id);
	return bookClass;
}
/*采编入库*/
void addBooks(Library *library, char *name, char *author,char *publisher, int amount) {
	BookClass *bookClass = NULL;
	BookClass *temp = library->headBookClass;
	while (temp) {
		if (strcmp(name, temp->name) == 0 && 
			strcmp(author,temp->author) == 0 && 
			strcmp(publisher,temp->publisher) == 0) {
			bookClass = library->headBookClass;
			break;
		}
		temp = temp->next;
	}
	if (!bookClass) {	//此书在图书馆不存在
		bookClass = (BookClass*)malloc(sizeof(BookClass));
		strcpy(bookClass->name, name);
		strcpy(bookClass->author, author);
		strcpy(bookClass->publisher,publisher);
		bookClass->currentMemory = amount;
		bookClass->totalMemory = amount;
		bookClass->id = getBookClassMaxId(*library) + 1;		//为其分配最大id
		//printf("%d\n", library->headBookClass == NULL);
		bookClass->next = library->headBookClass;
		library->headBookClass = bookClass;
		//printf("%d\n", library->headBookClass == NULL);
		library->numOfBookClass++;
	}
	else {
		bookClass->currentMemory += amount;
		bookClass->totalMemory += amount;
	}
	library->totalMemory += amount;
	library->currentMemory += amount;
}
/*旧书清零*/
void clearBooks(Library *library, int id) {
	BookClass *preBookClass = NULL;
	BookClass *temp;
	if (library->headBookClass->id == id) {	//头节点
		temp = library->headBookClass;
		library->numOfBookClass--;
		library->currentMemory -= temp->currentMemory;
		library->totalMemory -= temp->totalMemory;
		library->headBookClass = library->headBookClass->next;
		free(temp);
		printf("->清除完毕。\n");
	}
	else {	//可能存在于后续节点
		preBookClass = findPreBookClass(*library, id);	//获取前驱节点
		if (preBookClass) {		//存在这本书
			temp = preBookClass->next;
			library->numOfBookClass--;
			library->currentMemory -= temp->currentMemory;
			library->totalMemory -= temp->totalMemory;
			preBookClass->next = temp->next;
			free(temp);
			printf("->清除完毕。\n");
		}
		else {
			printf("->不存在此书。\n");
		}
	}
}
/*借书*/
int borrowBooks(Library *library, int id, int amount) {
	BookClass *bookClass;
	//printf("LLALAL\n");
	bookClass = queryLibraryById(*library, id);
	if (bookClass) {
		amount = amount > bookClass->currentMemory ? bookClass->currentMemory : amount;
		bookClass->currentMemory -= amount;
		library->currentMemory -= amount;
	}
	else {
		amount = 0;
	}
	return amount;
}
/*还书*/
int returnBooks(Library *library, int id, int amount) {
	BookClass *bookClass = queryLibraryById(*library, id);
	if (bookClass) {
		bookClass->currentMemory += amount;
		library->currentMemory += amount;
	}
	else {
		amount = 0;
	}
	return amount;
}
//-----------------------------借书--------------------------
/*借书，返回借的数量*/
int Borrow(Library *library, ReaderSet *readerSet, RecordSet *recordSet, int readerId, int bookId, int amount) {
	//printf("HEllo1\n");
	amount = borrowBooks(library, bookId, amount);	//从图书馆借书
	//printf("%d\n",amount);
	//printf("HEllo2\n");
	if (amount) {
		int temp = amount;
		amount = addBorBooks(readerSet, readerId, bookId, amount);		//个人仓库增加书籍
		//printf("%d\n", amount);
		if (amount) {
			addRecord(recordSet, readerId, bookId, amount, 1);
		}
		else {	//个人仓库无法添加书籍，把书返回仓库
			returnBooks(library, bookId, temp);
		}
	}
	printf("%d\n", amount);
	return amount;
}
//-----------------------------还书--------------------------
/*还书，返回还回去的数量*/
int Return(Library *library, ReaderSet *readerSet, RecordSet *recordSet, int readerId, int bookId, int amount) {
	amount = deteleBorBook(readerSet, readerId, bookId, amount);	//从个人书库里移除书籍
	if (amount) {
		int temp = amount;
		amount = returnBooks(library, bookId, amount);		//把书返还到图书馆
		if (amount) {
			addRecord(recordSet,readerId,bookId,amount,0);
		}
		else {
			amount = addBorBooks(readerSet,readerId,bookId,temp);
		}
	}
	return amount;
}
/*加载文件*/
void load(char *path1, char *path2, char *path3, Library *library, ReaderSet *readerSet, RecordSet *recordSet) {
	BookClass *bookClass;
	Record *record;
	Reader *reader;
	FILE *file;
	file = fopen(path1,"r");
	if (file) {
		fscanf(file,"%d%d%d",&library->currentMemory
			,&library->totalMemory,
			&library->numOfBookClass);
		for (int i = 0; i < library->numOfBookClass; i++) {
			bookClass = (BookClass *)malloc(sizeof(BookClass));
			fscanf(file,"%d%s%s%s%d%d",
				&bookClass->id,
				bookClass->name,
				bookClass->author,
				bookClass->publisher,
				&bookClass->totalMemory,
				&bookClass->currentMemory);	//输入书籍信息
			bookClass->next = library->headBookClass;
			library->headBookClass = bookClass;
		}
		fclose(file);
	}
	file = fopen(path2, "r");
	if (file) {
		fscanf(file,"%d",&recordSet->amount);
		for (int i = 0; i < recordSet->amount; i++) {
			record = (Record *)malloc(sizeof(Record));
			fscanf(file,"%d%d%d",
				&record->readerId,
				&record->bookId,
				&record->amount);
			fgets(record->time,30,file);
			fgets(record->limitedTime, 30, file);
			fscanf(file,"%d",&record->action);
			record->next = recordSet->headRecord;
			recordSet->headRecord = record;
		}
		fclose(file);
	}
	file = fopen(path3,"r");	//开始输入个人信息
	if (file) {
		fscanf(file,"%d",&readerSet->amount);
		for (int i = 0; i < readerSet->amount; i++) {
			reader = (Reader *)malloc(sizeof(Reader));
			fscanf(file,"%d%s%d",&reader->id,reader->name,&reader->borBookSet.amount);
			int a;
			for (int j = 0; j < reader->borBookSet.amount; j++) {
				BorBook *borBook = (BorBook *)malloc(sizeof(BorBook));
				reader->borBookSet.borBooks[i] = borBook;
				fscanf(file, "%d%d", &borBook->id,
					&borBook->amount);
			}
			reader->next = readerSet->headReader;
			readerSet->headReader = reader;
		}
		fclose(file);
	}

}
/*保存*/
void save(char *path1, char *path2, char *path3, Library library, ReaderSet readerSet, RecordSet recordSet) {
	FILE *file;
	file = fopen(path1,"w");
	if (file) {
		fprintf(file,"%d\t%d\t%d\n",library.currentMemory,
			library.totalMemory,library.numOfBookClass);
		while (library.headBookClass) {
			fprintf(file,"%d\t%s\t%s\t%s\t%d\t%d\n", 
				library.headBookClass->id,
				library.headBookClass->name,
				library.headBookClass->author,
				library.headBookClass->publisher,
				library.headBookClass->totalMemory,
				library.headBookClass->currentMemory);
			library.headBookClass = library.headBookClass->next;
		}
		fclose(file);
	}
	file = fopen(path2,"w");
	if (file) {
		fprintf(file,"%d\n",recordSet.amount);
		while (recordSet.headRecord) {
			fprintf(file,"%d\t%d\t%d%s%s%d\n",
				recordSet.headRecord->readerId,
				recordSet.headRecord->bookId,
				recordSet.headRecord->amount,
				recordSet.headRecord->time,
				recordSet.headRecord->limitedTime,
				recordSet.headRecord->action);
			recordSet.headRecord = recordSet.headRecord->next;
		}
		fclose(file);
	}
	file = fopen(path3,"w");
	if (file) {
		fprintf(file,"%d\n",readerSet.amount);
		while (readerSet.headReader) {
			fprintf(file,"%d\t%s\t%d\t", 
				readerSet.headReader->id, 
				readerSet.headReader->name,
				readerSet.headReader->borBookSet.amount);
			for (int i = 0; i < readerSet.headReader->borBookSet.amount; i++) {
				fprintf(file,"%d\t%d\t", readerSet.headReader->borBookSet.borBooks[i]->id,
					readerSet.headReader->borBookSet.borBooks[i]->amount);
			}
			fprintf(file,"\n");
			readerSet.headReader = readerSet.headReader->next;
		}
		fclose(file);
	}
}