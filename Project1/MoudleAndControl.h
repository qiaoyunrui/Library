/*
图书管理系统
*/
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
//---Moudle
typedef struct PriBook{
char name[20];
char author[20];
}Book;	//无卵用
typedef struct PriBookClass{
	int id;	//书号
	char name[20];
	char author[20];
	int totalMemory;
	int currentMemory;
	struct PriBookClass *next;	//只想下一个节点
}BookClass;
typedef struct PriLibrary{
	int totalMemory;
	int currentMemory;
	int numOfBookClass;	//书的种类
	BookClass *headBookClass;	//头节点
}Library;
typedef struct PriReader{
	int id;
	char name[20];
	//散列表->所借的书
	struct PriReader *next;
}Reader;
typedef struct PriReaderSet{
	int amount;
	Reader *headReader;		//头节点
}ReaderSet;
typedef struct PriRecord {
	char time[20];	//借书时间
	char limitedTime[20];	//归还期限
	Reader reader;
	BookClass bookClass;
	int amount;	//图书数量
	int action;
	struct PriRecord *next;
}Record;
typedef struct PriRecordSet{
	int amount;	//记录数量
	Record *headRecord;	//头节点
}RecordSet;
typedef struct PriSearchNode {
	int id;
	char name[20];
	char author[20];
	BookClass *bookClass;
}SearchNode;
typedef struct PriSearchList {
	SearchNode searchNode[1000];
	int amount;
}SearchList;
//Control
//--------------------------------------------------------------
Book createBook(char *name, char *author) {
	Book book;
	strcpy(book.name,name);
	strcpy(book.author,author);
	return book;
}
//--------------------------------------------------------------
/*初始化一类书*/
BookClass *createBookClass(char *name,char *author) {
	BookClass* bookClass;
	bookClass = (BookClass *)malloc(sizeof(BookClass));
	bookClass->id = -1;	//还没有分配书号
	strcpy(bookClass->name,name);
	strcpy(bookClass->author,author);
	bookClass->totalMemory = 0;
	bookClass->currentMemory = 0;
	bookClass->next = NULL;
	return bookClass;
}
/*添加书籍*/
void priAddBooks(BookClass *bookClass,int amount) {
	bookClass->totalMemory += amount;
	bookClass->currentMemory += amount;
}
/*删除书籍*/
void priDeleteBooks(BookClass *bookClass, int amount) {
	if (amount > bookClass->totalMemory) {
		bookClass->totalMemory = 0;
	}
	else {
		bookClass->totalMemory -= amount;
	}
	if (amount > bookClass->currentMemory) {
		bookClass->currentMemory = 0;
	}
	else {
		bookClass->currentMemory -= amount;
	}
}
/*清除书籍*/
void priClear(BookClass *bookClass) {
	BookClass *tempBook;
	tempBook = bookClass;
	bookClass = bookClass->next;
	free(tempBook);	//释放内存
}
/*借书*/
void priBor(BookClass *bookClass,int *amount) {
	if (bookClass->currentMemory < *amount) {
		*amount = bookClass->currentMemory;
		bookClass->currentMemory = 0;
	}
	else {
		bookClass->currentMemory -= *amount;
	}
}
/*还书*/
void priRet(BookClass *bookClass, int amount) {
	bookClass->currentMemory += amount;
}
/*修改书籍信息*/
void priModifyBookClass(BookClass *bookClass,char *name,char *author) {
	strcpy(bookClass->name, name);
	strcpy(bookClass->author,author);
}
/*查询书籍存量信息*/
void priQueryBookClass(BookClass bookClass, int *totalMemory, int *currentMemory) {
	*totalMemory = bookClass.totalMemory;
	*currentMemory = bookClass.currentMemory;
}
//--------------------------------------------------------------
/*ReaderSet*/
/*创建读者列表*/
ReaderSet createReaderSet() {
	ReaderSet readerSet;
	readerSet.amount = 0;
	readerSet.headReader = NULL;
}
/*添加读者*/
void addReader(ReaderSet *readerSet,char *name) {
	Reader *reader;
	reader = (Reader*)malloc(sizeof(Reader));
	reader->id = readerSet->amount;
	strcpy(reader->name,name);
	reader->next = readerSet->headReader;
	readerSet->headReader = reader;
}
//--------------------------------------------------------------
/*RecordSet*/
/*创建*/
RecordSet createRecordSet() {
	RecordSet recordSet;
	recordSet.amount = 0;
	recordSet.headRecord = NULL;
	return recordSet;
}
/*添加纪录*/
void addRecord(BookClass bookClass, Reader reader, int amount, int action) {
	Record *record;
	record = (Record*)malloc(sizeof(Record));
	record->amount = amount;
	record->bookClass = bookClass;
	record->reader = reader;
	record->time = ;
	record->limitedTime = ;
	record->action = action;
}
//--------------------------------------------------------------
/*SearchList*/
/*为图书创建一个线性检索表*/
SearchList createSearchList(Library library) {
	SearchList searchList;
	searchList.amount = library.numOfBookClass;	//获取书的总类数
	int i = 0;
	while (library.headBookClass) {
		searchList.searchNode[i].id = library.headBookClass->id;
		strcpy(searchList.searchNode[i].name, library.headBookClass->name);
		strcpy(searchList.searchNode[i].author, library.headBookClass->author);
		searchList.searchNode[i].bookClass = library.headBookClass;
		i++;
		library.headBookClass = library.headBookClass->next;	//指向下一个节点
	}
	return searchList;
}
/*二分查找*/
int search(SearchList searchList,int index[1000],char *key) {
	char id[10];
	int sign = 0;
	for (int i = 0; i < searchList.amount; i++) {
		itoa(searchList.searchNode[i].id, id, 10);
		if (strcmp(id, key) ||
			strcpy(searchList.searchNode[i].name, key) ||
			strcpy(searchList.searchNode[i].author, key)) {	//匹配
			index[sign] = i;
			sign++;
		}
	}
	return sign;	
}
//--------------------------------------------------------------
/*Library*/
/*创建*/
Library createLibrary() {
	Library library;
	library.currentMemory = 0;
	library.numOfBookClass = 0;
	library.totalMemory = 0;
	library.headBookClass = NULL;
}
/*综合查询*/
BookClass **queryLibrary(Library library,char* key) {
	SearchList searchList;
	searchList = createSearchList(library);	//获得检索线性表
	BookClass *bookClasses[1000];	//放置找到的bookClass
	int index[1000];
	int length = search(searchList,index, key);
	for (int i = 0; i < length; i++) {
		bookClasses[i] = searchList.searchNode[index[i]].bookClass;
	}
	return bookClasses;
}
/*ID查询*/
BookClass *queryLibraryById(Library library, int id) {
	BookClass *bookClass = NULL; 
	SearchList searchList;
	searchList = createSearchList(library);
	for (int i = 0; i < searchList.amount; i++) {
		if (id = searchList.searchNode[i].id) {
			bookClass = searchList.searchNode[i].bookClass;
			break;
		}
	}
	return bookClass;
}
/*增加*/
int addBooks(Library *library, int id,char *name,char *author,int amount) {
	BookClass *bookClass;
	BookClass *temp;
	bookClass = queryLibraryById(*library,id);
	if (bookClass == NULL) {	//未查找到
		bookClass = createBookClass(name,author);	//已经开辟空间
		bookClass->id = library->numOfBookClass;
		library->numOfBookClass++;
		library->headBookClass = temp;
		library->headBookClass = bookClass;
		bookClass->next = temp;
	}
	else {	//图书馆中有对应的书
		priAddBooks(bookClass, amount);
	}
	return bookClass->id;
}
/*删除*/
void deleteBooks(Library *library, int id, int amount) {
	BookClass *bookClass;
	bookClass = queryLibraryById(*library,id);
	if (bookClass) {
		priDeleteBooks(bookClass, amount);
	}
}
/*清除*/
void clearBooks(Library *library, int id) {
	BookClass *bookClass;
	bookClass = queryLibraryById(*library,id);
	priClear(bookClass);
}
/*修改*/
void modifyBooks(Library *library,int id,char *name,char *author) {
	BookClass *bookClass;
	bookClass = queryLibraryById(*library,id);
	priModifyBookClass(bookClass,name,author);
}
/*借书*/
int borBooks(Library *library, int id, int amount) {
	BookClass *bookClass;
	bookClass = queryLibraryById(*library,id);
	if (bookClass) {
		priBor(bookClass, &amount);
	}
	else {
		amount = 0;
	}
	return amount;	//返回最终所借的书数
}
/*还书*/
void retBooks(Library *library, int id, int amount) {
	BookClass *bookClass;
	bookClass = queryLibraryById(*library,id);
	if (bookClass) {
		priRet(bookClass,amount);
	}
	else {	//未找到图书
		//清除散列表数据
	}
}