#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<time.h>
int main() {
	time_t t;
	t = time(NULL);
	char *time;
	struct tm *timeinfo;
	time = ctime(&t);
	t += 2592000;
	printf("%s",time);
	timeinfo = localtime(&t);
	printf("%s",asctime(timeinfo));
	printf("HELLO WORLD\n");
	getchar();
	return 0;
}