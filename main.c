#include "header.h"


int main(int argc, char** argv)
{
	BIG first, second, result;

	char* first_big_num = argv[1],
		* second_big_num = argv[2];

	int operation = 0;

	if (str_hex_check(first_big_num) && str_hex_check(second_big_num)) // проверка на корректность шестнадцатеричных символов
	{
		first.bigNum = GetBigNumByStr(first_big_num, &first.size, &first.sign);
		second.bigNum = GetBigNumByStr(second_big_num, &second.size, &second.sign);

		printf_s("first:\n");
		PrintBigNum(&first);

		printf_s("second:\n");
		PrintBigNum(&second);

		result = add(&first, &second, 0);
		printf_s("Add and Sub result:\n");
		PrintBigNum(&result);
		result = sub(&first, &second, 0);
		PrintBigNum(&result);


	}

	return 0;
}