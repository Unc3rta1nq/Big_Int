#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include "header.h"


// функция конвертации строки в биг число
BigNum GetBigNumByStr(const char* str, size_t* bigNumSize, char* sign)
{
	size_t count_zeroes = 0;
	if (str[0] == '+' || str[0] == '-') // Check for the sign
	{
		*sign = str[0];
		str++;
	}
	else
	{
		*sign = '+';
	}

	// Calculate the size of BigNum in bytes with some extra space
	size_t strSize = strlen(str);
	size_t integer_part = strSize / 8;
	size_t remainder = strSize % 8; // remainder

	*bigNumSize = (remainder > 0) ? integer_part + 1 : integer_part;

	// Allocate memory
	BigNum bigNum = (BigNum)malloc((*bigNumSize) * sizeof(unsigned int));
	size_t j = 0;
	char buffer[9];

	// Handle the integer part
	for (int i = 0; i < integer_part; i++)
	{
		memcpy(buffer, str + strSize - ((i + 1) * 8), 8);
		buffer[8] = '\0'; // Null-terminate the buffer
		bigNum[j++] = strtoul(buffer, NULL, 16); // Convert to unsigned int
	}

	// Handle the remainder
	if (remainder > 0)
	{
		memcpy(buffer, str, remainder);
		buffer[remainder] = '\0'; // Null-terminate the buffer
		bigNum[j] = strtoul(buffer, NULL, 16);
	}

	return bigNum;
}
//функция сложения больших чисел
BIG add(BIG* first, BIG* second, int flag)
{
	BIG result;
	if (first->sign == second->sign)
		result.sign = first->sign;
	else
	{
		if (flag == 0)
		{
			int cmp = compare(first, second); // сравниваем биг инты
			char SignResult = ' ';
			if ((cmp == 1 && first->sign == '-') || cmp == 2 && second->sign == '-')
				SignResult = '-';
			else
				SignResult = '+'; //-2 + 5
			if (cmp == 1)
				result = sub(first, second, 1);
			else
				result = sub(second, first, 1);
			result.sign = SignResult;
			return result;
		}
	}
	if (first->size >= second->size)
		result.size = first->size + 1;
	else
		result.size = second->size + 1; //максимум увеличится на 1 знак

	result.bigNum = (BigNum)malloc(result.size * sizeof(unsigned int));
	result.bigNum[result.size - 1] = 0;
	size_t size_a = first->size;
	size_t size_b = second->size;
	size_t rest = 0;
	for (size_t i = 0; i < result.size - 1; i++)
	{
		if (size_a && size_b)
		{
			result.bigNum[i] = first->bigNum[i] + second->bigNum[i] + rest;
			if (result.bigNum[i] < first->bigNum[i] || 
				result.bigNum[i] < second->bigNum[i] ||
				((result.bigNum[i] == first->bigNum[i] && second->bigNum[i] != 0) ||
				(result.bigNum[i] == second->bigNum[i] && first->bigNum[i] != 0))) 			//проверяем остаток
				rest = 1;
			else
				rest = 0;
			size_a--;
			size_b--;
		}
		else if (size_a == 0 && size_b != 0)
		{
			result.bigNum[i] = second->bigNum[i] + rest;
			if (result.bigNum[i] < second->bigNum[i] || result.bigNum[i] < rest)
				rest = 1;
			else
				rest = 0;
			size_b--;
		}
		else if (size_b == 0 && size_a != 0)
		{
			result.bigNum[i] = first->bigNum[i] + rest;
			if (result.bigNum[i] < first->bigNum[i] || result.bigNum[i] < rest)
				rest = 1;
			else
				rest = 0;
			size_a--;
		}
	}
	if (rest == 0)
	{
		result.size -= 1;
		result.bigNum = realloc(result.bigNum, (result.size) * sizeof(unsigned int));
	}
	else
	{
		result.bigNum[result.size - 1] = rest;//заполняем последний элемент
	}
	return result;
}

BIG sub(BIG* first, BIG* second, int flag)
{
	BIG result;
	int cmp = compare(first, second);
	if (first->sign != second->sign)
	{
		if (flag == 0)
		{
			char SignResult = ' ';
			if ((cmp == 1 && first->sign == '-') || (cmp == 2 && first->sign == '-'))
				SignResult = '-';
			else
				SignResult = '+';
			result = add(first, second, 1);
			result.sign = SignResult;
			return result;
		}
	}
	else
	{
		if ((cmp == 1 && first->sign == '+') || (cmp == 2 && first->sign == '-') || (cmp == 0))
		{
			if (cmp == 0)
			{
				result.size = 1;
				result.sign = '+';
				result.bigNum = (BigNum)calloc(result.size, sizeof(unsigned int));
				return result;
			}
			result.sign = '+';
		}
		else
			result.sign = '-';
		if (cmp == 2) // условно если а > b то b - a = a - b | * (-1)
		{
			BIG* tmp = first;
			first = second;
			second = tmp;
		}
	}
	result.size = first->size;
	result.bigNum = (BigNum)malloc(result.size * sizeof(unsigned int));
	size_t size_b = second->size;
	for (size_t i = 0; i < result.size; i++)
	{
		if (size_b)
		{
			int number_ptr = i;
			result.bigNum[i] = first->bigNum[i] - second->bigNum[i];
			size_b--;
		}
		else
		{
			result.bigNum[i] = first->bigNum[i];
		}
	}
	return result;
}

int str_hex_check(char* str)
{
	if (((str[0] == '-' || str[0] == '+') && strlen(str) == 1) || strlen(str) == 0)
		return 0;
	for (size_t i = 0; str[i] != '\0'; i++)
	{
		if ((i == 0 && (str[i] == '-' || str[i] == '+')) || ((str[i] >= 'a' && str[i] <= 'f') || (str[i] >= 'A' && str[i] <= 'F') || (str[i] >= '0' && str[i] <= '9')))
			continue;
		else
			return 0;
	}
	return 1;
}

void PrintBigNum(BIG* num) {
	if (isZero(num)) {
		printf("0\n");
		return;
	}

	if (num->sign == '-')
		printf("%c", num->sign);

	printf("%x", num->bigNum[num->size - 1]);

	for (int i = (int)(num->size) - 2; i >= 0; i--) {
		printf("%08x", num->bigNum[i]);
	}

	printf("\n");
}

int compare(BIG* first, BIG* second)
{
	if ((first->size > second->size) || second->sign == '-')
		return 1;
	if ((first->size < second->size) || first->sign == '-')
		return 2;
	if (first->size == second->size) {
		for (int i = (int)(first->size) - 1; i >= 0; i--)
		{
			if (first->bigNum[i] > second->bigNum[i])
				return 1;
			if (first->bigNum[i] < second->bigNum[i])
				return 2;
			if (first->bigNum[i] == second->bigNum[i])
				continue;
		}
	}
	return 0;
}

int isZero(BIG* num) {
	for (size_t i = 0; i < num->size; i++) {
		if (num->bigNum[i] != 0) {
			return 0; // Если хоть один элемент не равен нулю, то число не ноль
		}
	}
	return 1; // Все элементы равны нулю, число ноль
}


