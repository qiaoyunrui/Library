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
	char time[20];	//借书时间
	char limitedTime[20];	//归还期限
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
//-----------------------------综合检索表---------------------

/*Control*/
//-----------------------------读者信息-----------------------
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
	reader->id = readerSet->amount;
	strcpy(reader->name,name);
	reader->borBookSet.amount = 0;
	reader->next = readerSet->headReader;
	readerSet->headReader = reader;
	readerSet->amount++;
	return readerSet->amount - 1;
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
	t += 2592000;
	timeinfo = localtime(&t);
	strcpy(record->time,time);
	strcpy(record->limitedTime, asctime(timeinfo));
	record->next = recordSet->headRecord;
	recordSet->headRecord = record;
	recordSet->amount++;
}
//-----------------------------个人书库-----------------------
/*查找个人书库的书籍*/
BorBook *searchBorBook(BorBookSet borBookSet,int id) {
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
		borBookSet->borBooks[borBookSet->amount] = borBook;
		borBookSet->amount++;
		return 1;
	}
}
//-----------------------------ID索引表-----------------------
SearchIdList CreateSearchIdList(Library library) {
	SearchIdList searchIdList;
	int i = 0;
	searchIdList.amount = library.numOfBookClass;
	while (library.headBookClass) {
		searchIdList.searchId[i].id = library.headBookClass->id;
		searchIdList.searchId[i].bookClass = library.headBookClass;
		i++;
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
/*综合查询*/

/*ID查询*/
BookClass *queryLibraryById(Library library, int id) {
	BookClass *bookClass;
	SearchIdList searchIdList = CreateSearchIdList(library);
	bookClass = searchById(searchIdList, id);
	return bookClass;
}
/*采编入库*/
void addBooks(Library *library, char *name, char *author, int amount) {
	BookClass *bookClass = NULL;
	while (library->headBookClass) {
		if (strcmp(name, library->headBookClass->name) && strcmp(author, library->headBookClass->author)) {
			bookClass = library->headBookClass;
			break;
		}
		library->headBookClass = library->headBookClass->next;
	}
	if (bookClass) {
		bookClass->totalMemory += amount;
		bookClass->currentMemory += amount;
	}
	else {
		bookClass = (BookClass*)malloc(sizeof(BookClass));
		strcpy(bookClass->name, name);
		strcpy(bookClass->author, author);
		bookClass->currentMemory = amount;
		bookClass->totalMemory = amount;
		if (library->headBookClass) {
			bookClass->id = library->headBookClass->id + 1;
		}
		else {
			bookClass->id = 0;
		}
		bookClass->next = library->headBookClass;
		library->headBookClass = bookClass;
		library->numOfBookClass++;
	}
	library->totalMemory += amount;
	library->currentMemory += amount;
}
/*旧书清零*/
void clearBooks(Library *library, int id) {
	BookClass *bookClass;
	BookClass *temp;
	bookClass = queryLibraryById(*library, id);
	if (bookClass) {
		library->numOfBookClass--;
		library->totalMemory -= bookClass->totalMemory;
		library->currentMemory -= bookClass->currentMemory;
		temp = bookClass;
		bookClass = bookClass->next;
		free(temp);
	}
}
/*借书*/
int borrowBooks(Library *library, int id, int amount) {
	BookClass *bookClass;
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
	amount = borrowBooks(library, bookId, amount);	//从图书馆借书
	if (amount) {
		int temp = amount;
		amount = addBorBooks(readerSet, readerId, bookId, amount);		//个人仓库增加书籍
		if (amount) {
			addRecord(recordSet, readerId, bookId, amount, 1);
		}
		else {	//个人仓库无法添加书籍，把书返回仓库
			returnBooks(library, bookId, temp);
		}
	}
	return 0;
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
void load(char *file, Library *library, ReaderSet *readerSet, RecordSet *recordSet) {

}
/*保存*/
void save(char *file, Library *library, ReaderSet *readerSet, RecordSet *recordSet) {

}