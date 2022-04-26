#include "Main.h"
/*preAssembler function is getting path(name of a file represnting by char array) and
argc(the number of arguments in the command line).
the function gets files written with assembly language and
is going throw each file(can get a few).
it spreads macros from the file and saving the name of the macro and the content.
it makes a new file with out the defines of the macros inside the file and puts the macro content instead of the macro name where it shown up in the file.
the file after preAssembler is a file with out macro defines and instead of macro name it will be the macro content that taken and saved from the define at the beggining*/
void preAssembler(char* path, int argc)
{
	char ch;
	FILE* filePointer;
	FILE* newFilePointer;
	int flag = 0, countForNames = 0, rol = 0, i = 0, macroNum = 0, p = 0, o = 0, count = 0, count1 = 0;
	char commands[20][8] = { "mov","cmp","add","sub","lea","clr","not","inc","dec","jmp","bne","jsr","red","prn","rts","stop",".entry",".extern",".data",".string" };
	char row[maxChar];
	char row1[maxChar];
	Macro* macros = (Macro*)malloc(sizeof(Macro));
	char as[] = ".as";
	char am[] = ".am";
	char firstPath[maxChar];
	char secondPath[maxChar];
	char nameForMacro[maxChar];

	memset(row1, '\0', maxChar);
	memset(row, '\0', maxChar);
	memset(firstPath, '\0', maxChar);
	memset(secondPath, '\0', maxChar);
	memset(nameForMacro, '\0', maxChar);

	strcpy(firstPath, path);
	strcpy(secondPath, path);
	strcat(firstPath, as);
	strcat(secondPath, am);


	if (access(firstPath, F_OK) != 0)/*check if a file is exist*/
	{
		printf("%s is not exist.\n", firstPath);
		exit(0);
	}

	filePointer = fopen(firstPath, "r");
	newFilePointer = fopen(secondPath, "w+");

	if (!macros)
	{
		printf("Could not make a pointer.\n");
		exit(0);
	}

	if (!filePointer || !newFilePointer)
	{
		perror("No such file.\n");
		exit(0);
	}

	if (argc < 2)
	{
		printf("Missing file.\n");
		exit(0);
	}

	while (fgets(row, maxChar, filePointer) != NULL)/*getting row by row from the file to  check*/
	{
		p = 0;

		if ((strstr(row, "macro")) != NULL)/*check if it's a start of defining macro*/
		{

			for (i = 0; i < strlen(row); i++)
			{
				ch = row[i];

				if (ch == (int)"\n")
				{
					break;
				}

				switch (ch)
				{
				case 'm':
					if (flag == 0)
					{
						flag = 1;
					}
					break;
				case 'a':
					if (flag == 1)
					{
						flag = 2;
					}
					break;
				case 'c':
					if (flag == 2)
					{
						flag = 3;
					}
					break;
				case 'r':
					if (flag == 3)
					{
						flag = 4;
					}
					break;
				case 'o':
					if (flag == 4)
					{
						flag = 5;
					}
					break;
				case ' ':
					if (flag == 5)
					{
						flag = 6;
					}
					break;
				}

				if (flag == 6)
				{

					flag = 7;
					if (countForNames != 0)/*dynamic allocation of the macros*/
					{
						macros = (Macro*)realloc(macros, (countForNames + 1) * sizeof(Macro));

						if (!macros)
						{
							printf("Could not make a pointer.\n");
							exit(0);
						}

					}
					continue;
				}

				if (flag == 7)
				{
					if (ch == '\n')
						continue;
					(*(macros + countForNames)).macroName[p++] = ch;
					count = p;
					/*save the name of the macro into macroNames*/
				}

			}

			if (flag == 7)
			{

				(*(macros + countForNames)).macroContent = (char*)malloc(sizeof(row));
				if (!(*(macros + countForNames)).macroContent)
				{
					printf("Could not make a pointer.\n");
					exit(0);
				}

				rol = 1;
				continue;
			}
		}

		(*(macros + countForNames)).macroName[count] = '\0';

		if ((strstr(row, "endm")) != NULL)/*check if it's an end of a define of macro*/
		{
			(*(macros + countForNames)).macroContent[count1] = '\0';
			countForNames++;
			flag = 0;
			o = 0;
			rol = 0;

			for (i = 0; i < strlen(row); i++)
			{
				ch = row[i];
				if (ch == (int)"\n")
				{
					break;
				}
				switch (ch) {
				case 'e':
					if (flag == 0)
					{
						flag = 1;
					}
					break;
				case 'n':
					if (flag == 1)
					{
						flag = 2;
					}
					break;
				case 'd':
					if (flag == 2)
					{
						flag = 3;
					}
					break;
				case 'm':
					if (flag == 3)
					{
						flag = 4;
					}
					break;
				case ' ':
					if (flag == 4)
					{
						flag = 5;
					}
					break;
				default:
					continue;
				}

				if (flag == 5)
				{
					flag = 0;
				}

			}
		}

		if (flag == 7)
		{
			if (rol != 1)/*dynamic allocation of memory for the macro content*/
			{
				(*(macros + countForNames)).macroContent = (char*)realloc((*(macros + countForNames)).macroContent, sizeof(row) + sizeof((*(macros + countForNames)).macroContent));
			}
			rol++;

			for (i = 0; i < strlen(row); i++)
			{
				(*(macros + countForNames)).macroContent[o++] = row[i];
				count1 = o;
				/*save the content of the macro into macroContent*/
			}

			if (!(*(macros + countForNames)).macroContent)
			{
				printf("Could not make a pointer.\n");
				exit(0);
			}
			

		}

	}

	for (i = 0; i < countForNames; i++)
	{
		removeWhiteSpaces(macros[i].macroName);
	}
	for (i = 0; i < countForNames; i++)
	{

		for (p = 0; p < 20; p++)/*check if the macro name is a valid name*/
		{
			if (strstr(commands[p], macros[i].macroName) != NULL)
			{
				printf("%s is not a valid name for a macro.\n", macros[i].macroName);
				exit(0);
			}
		}
	}

	fseek(filePointer, 0, SEEK_SET);

	flag = 0;
	i = 0;

	while (fgets(row, maxChar, filePointer) != NULL)/*getting a row by row second time from a file for making a new one after saving macros names and contents*/
	{
		strcpy(row1, row);
		row1[strlen(row1)] = '\0';

		if (strstr(row1, "macro") != NULL)/*check if a row is a start of defining a macro*/
		{
			flag = 1;
		}

		if (flag != 1)
		{
			for (i = 0; i < countForNames; i++)/*check if a row is a macro name for replacing it with macro content*/
			{
				memset(nameForMacro, '\0', maxChar);
				strcpy(nameForMacro, ((*(macros + i)).macroName));

				if (strstr(removeWhiteSpaces(row), nameForMacro) != NULL)
				{
					macroNum = i;
					flag = 2;
				}

			}

		}

		if (strstr(row1, "endm") != NULL)/*check if a row is an end of defining a macro*/
		{
			flag = 0;
			continue;
		}

		if (flag != 1 && flag != 2)
		{
			fputs(row1, newFilePointer);
		}

		if (flag == 2 && flag != 1)
		{
			fputs(((macros + macroNum)->macroContent), newFilePointer);
			flag = 0;
		}

	}

	for (i = 0; i < countForNames; i++)
	{
		memset(macros[i].macroName, '\0', maxChar);
		memset(macros[i].macroContent, '\0', maxChar);
		free(((macros + i)->macroContent));
		((macros + i)->macroContent) = NULL;
	}

	fclose(filePointer);
	free(macros);
	fclose(newFilePointer);

}
/*removeWhiteSpaces function is getting char array.
the function is removing white spaces from the array for comparing strings.
it returns the char array with out white spaces*/
char* removeWhiteSpaces(char* str)
{
	int i = 0, j = 0;
	while (i <= strlen(str) - 1)
	{
		if (str[i] != ' ' && str[i] != '\n')
		{
			str[j++] = str[i];
		}
		i++;
	}
	for (i = j; i <= strlen(str) - 1; i++)
	{
		str[i] = '\0';
	}
	return str;
}


