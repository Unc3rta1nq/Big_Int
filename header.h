#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int* BigNum;

typedef struct bigint
{
	BigNum bigNum;
	size_t size;
	char sign;
}BIG;


BigNum GetBigNumByStr(const char* str, size_t* bigNumSize, char* sign);

BIG add(BIG* first, BIG* second, int flag);

BIG sub(BIG* first, BIG* second, int flag);



int str_hex_check(char* str);

void PrintBigNum(BIG* value);

int compare(BIG* first, BIG* second);

int isZero(BIG* num);