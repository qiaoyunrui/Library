#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<string.h>
#include<time.h>
int main() {
	FILE *file;
	file = fopen("data1.txt","r");
	char name[2][20];
	int id1, id2;
	if (file) {
		fscanf(file,"%s%s%d%d",name[0],name[1],&id1,&id2);
		puts(name[0]);
		puts(name[1]);
		printf("%d %d\n", id1,id2);
		fclose(file);
	}
	getchar();
	return 0;
}


