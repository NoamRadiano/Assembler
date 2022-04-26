#include "Main.h"

/*function checkoperand checks if a command is valid and if the command is valid it checks if the first and second operands are valid for the specific command.
if there is an error (something is not valid) it will write the specific problem,
and where is the problem(line).
the functino is getting command(char array), the first operand(char array),
the second operand(char array), IC(int pointer),ramMemory(int array), lebel(pointer saving labels),what pass(int),line number(int) and problem(int)*/
void checkoperand(char command[], char firstoperand[], char secondoperand[], int**** IC, int RamMemory[], lebel *lebels, int whatpass,int linenumber,int ***problem)
 {

	int sourceAddress = 0, destinationAddress = 0, opcode = 0, funct = 0, ogerSource = 0, ogerTarget = 0, numForTav = 0, secondnumForTav = 0, ifNum = 0;
	char tavNameFirstOperand[80];/*for saving label name in first operand*/
	char tavNameSecondOperand[80];/*for saving label name in second operand*/
	memset(tavNameFirstOperand, 0, 80);
	memset(tavNameSecondOperand, 0, 80);

	if ((firstoperand != NULL) && (secondoperand != NULL))/*check validation of commands with two operands*/
	{
		if (strcmp(command, "mov") == 0 || strcmp(command, "add") == 0 || strcmp(command, "sub") == 0)
		{/*check which command is it*/
			if (strcmp(command, "add") == 0)
			{
				opcode = 2;
				funct = 10;
			}
			if (strcmp(command, "mov") == 0)
			{
				opcode = 0;
			}
			if (strcmp(command, "sub") == 0)
			{
				opcode = 2;
				funct = 11;
			}
			if (firstoperand[0] == '#')/*check if first operand is using immediate method*/
			{
				sourceAddress = 0;
				numForTav = checkNumZero(firstoperand,linenumber,&problem);
			}
			else if (firstoperand[0] == 'r' && (noR(firstoperand) == 0))/*check if first operand is using register direct method*/
			{
				sourceAddress = 3;
				ogerSource = checkNumThird(firstoperand);
			}
			else if (checkNumTwo(firstoperand) == 0)/*check if first operand is using direct method*/
			{
				sourceAddress = 1;
				tavNameForOne(firstoperand, tavNameFirstOperand,linenumber,&problem);
			}
			else/*first operand is using index method*/
			{
				sourceAddress = 2;
				ogerSource = checkRegNum(firstoperand,linenumber,&problem);
				tavNameFunc(firstoperand, tavNameFirstOperand);
			}
			if (secondoperand[0] == '#')/*immediate method is not a valid method for the specific command*/
			{
				printf("Line %d problem is: immediate method is invalid for %s in second operand\n", linenumber,command);
				***problem = 1;
				return;
			}
			else if (secondoperand[0] == 'r' && (noR(secondoperand) == 0))/*check if second operand is using register direct method*/
			{
				destinationAddress = 3;
				ogerTarget = checkNumThird(secondoperand);
			}
			else if (checkNumTwo(secondoperand) == 0)/*check if second operand is using direct method*/
			{
				destinationAddress = 1;
				tavNameForOne(secondoperand, tavNameSecondOperand,linenumber,&problem);
			}
			else/*second operand is using index method*/
			{
				destinationAddress = 2;
				ogerTarget = checkRegNum(secondoperand,linenumber,&problem);
				tavNameFunc(secondoperand, tavNameSecondOperand);
			}

		}

		if (strcmp(command, "cmp") == 0)
		{
			opcode = 1;

			if (firstoperand[0] == '#')
			{
				sourceAddress = 0;
				numForTav = checkNumZero(firstoperand,linenumber,&problem);
			}
			else if (firstoperand[0] == 'r' && (noR(firstoperand) == 0))
			{
				sourceAddress = 3;
				ogerSource = checkNumThird(firstoperand);
			}
			else if (checkNumTwo(firstoperand) == 0)
			{
				sourceAddress = 1;
				tavNameForOne(firstoperand, tavNameFirstOperand,linenumber,&problem);
			}
			else
			{
				sourceAddress = 2;
				ogerSource = checkRegNum(firstoperand,linenumber,&problem);
				tavNameFunc(firstoperand, tavNameFirstOperand);
			}
			if (secondoperand[0] == 'r' && (noR(secondoperand) == 0))
			{
				destinationAddress = 3;
				ogerTarget = checkNumThird(secondoperand);
			}
			else if (secondoperand[0] == '#')
			{
				destinationAddress = 0;
				secondnumForTav = checkNumZero(secondoperand,linenumber,&problem);
			}
			else if (checkNumTwo(secondoperand) == 0)
			{
				destinationAddress = 1;
				tavNameForOne(secondoperand, tavNameSecondOperand,linenumber,&problem);
			}
			else
			{
				destinationAddress = 2;
				ogerTarget = checkRegNum(secondoperand,linenumber,&problem);
				tavNameFunc(secondoperand, tavNameSecondOperand);
			}
		}

		if (strcmp(command, "lea") == 0)
		{
			opcode = 4;

			if (firstoperand[0] == '#')/*immediate method is not a valid method for the specific command*/
			{
				printf("Line %d problem is: immediate method is invalid for %s in first operand\n", linenumber,command);
				***problem = 1;
				return;
			}
			else if (firstoperand[0] == 'r' && (noR(firstoperand) == 0))/*register direct method is not a valid method for the specific command*/
			{
				printf("Line %d problem is: index method is invalid for %s in first operand\n", linenumber,command);
				***problem = 1;
				return;
			}
			else if (checkNumTwo(firstoperand) == 0)
			{
				sourceAddress = 1;
				tavNameForOne(firstoperand, tavNameFirstOperand,linenumber,&problem);
			}
			else
			{
				sourceAddress = 2;
				ogerSource = checkRegNum(firstoperand,linenumber,&problem);
				tavNameFunc(firstoperand, tavNameFirstOperand);
			}
			if (secondoperand[0] == '#')/*immediate method is not a valid method for the specific command*/
			{
				printf("Line %d problem is: immediate method is invalid for %s in second operand\n", linenumber,command);
				***problem = 1;
				return;
			}
			else if (secondoperand[0] == 'r')
			{
				destinationAddress = 3;
				ogerTarget = checkNumThird(secondoperand);
			}
			else if (checkNumTwo(secondoperand) == 0)
			{
				destinationAddress = 1;
				tavNameForOne(secondoperand, tavNameSecondOperand,linenumber,&problem);
			}
			else
			{
				destinationAddress = 2;
				ogerTarget = checkRegNum(secondoperand,linenumber,&problem);
				tavNameFunc(secondoperand, tavNameSecondOperand);
			}
		}
	}

	if ((secondoperand != NULL) && (firstoperand == NULL))/*check validation of commands with one operand operands*/
	{
		sourceAddress = -1;

		if (strcmp(command, "clr") == 0 || strcmp(command, "not") == 0 || strcmp(command, "inc") == 0 || strcmp(command, "dec") == 0)
		{

			if (strcmp(command, "clr") == 0)
			{
				opcode = 5;
				funct = 10;
			}
			if (strcmp(command, "not") == 0)
			{
				opcode = 5;
				funct = 11;
			}
			if (strcmp(command, "inc") == 0)
			{
				opcode = 5;
				funct = 12;
			}
			if (strcmp(command, "dec") == 0)
			{
				opcode = 5;
				funct = 13;
			}
			if (secondoperand[0] == '#')/*immediate method is not a valid method for the specific command*/
			{
				printf("Line %d problem is: immediate method is invalid for %s in second operand\n", linenumber,command);
				***problem = 1;
				return;
			}
			else if (secondoperand[0] == 'r' && (noR(secondoperand) == 0))
			{
				destinationAddress = 3;
				ogerTarget = checkNumThird(secondoperand);
			}
			else if (checkNumTwo(secondoperand) == 0)
			{
				destinationAddress = 1;
				tavNameForOne(secondoperand, tavNameSecondOperand,linenumber,&problem);
			}
			else
			{
				destinationAddress = 2;
				ogerTarget = checkRegNum(secondoperand,linenumber,&problem);
				tavNameFunc(secondoperand, tavNameSecondOperand);
			}
		}

		if (strcmp(command, "jmp") == 0 || strcmp(command, "bne") == 0 || strcmp(command, "jsr") == 0)
		{
			if (strcmp(command, "jmp") == 0)
			{
				opcode = 9;
				funct = 10;
			}
			if (strcmp(command, "bne") == 0)
			{
				opcode = 9;
				funct = 11;
			}
			if (strcmp(command, "jsr") == 0)
			{
				opcode = 9;
				funct = 12;
			}
			if (secondoperand[0] == '#')/*immediate method is not a valid method for the specific command*/
			{
				printf("Line %d problem is: immediate method is invalid for %s in second operand\n", linenumber,command);
				***problem = 1;
				return;
			}
			else if (secondoperand[0] == 'r' && (noR(secondoperand) == 0))/*register direct method is not a valid method for the specific command*/
			{
				printf("Line %d problem is: index method is invalid for %s in second operand\n", linenumber,command);
				***problem = 1;
				return;
			}
			else if (checkNumTwo(secondoperand) == 0)
			{
				destinationAddress = 1;
				tavNameForOne(secondoperand, tavNameSecondOperand,linenumber,&problem);
			}
			else
			{
				destinationAddress = 2;
				ogerTarget = checkRegNum(secondoperand,linenumber,&problem);
				tavNameFunc(secondoperand, tavNameSecondOperand);
			}
		}

		if (strcmp(command, "red") == 0)
		{
			opcode = 12;
			if (secondoperand[0] == '#')/*immediate method is not a valid method for the specific command*/
			{
				printf("Line %d problem is: immediate method is invalid for %s in second operand\n", linenumber,command);
				***problem = 1;
				return;
			}
			else if (secondoperand[0] == 'r' && (noR(secondoperand) == 0))
			{
				destinationAddress = 3;
				ogerTarget = checkNumThird(secondoperand);
			}
			else if (checkNumTwo(secondoperand) == 0)
			{
				destinationAddress = 1;
				tavNameForOne(secondoperand, tavNameSecondOperand,linenumber,&problem);
			}
			else
			{
				destinationAddress = 2;
				ogerTarget = checkRegNum(secondoperand,linenumber,&problem);
				tavNameFunc(secondoperand, tavNameSecondOperand);
			}
		}

		if (strcmp(command, "prn") == 0)
		{
			opcode = 13;
			if (secondoperand[0] == 'r' && (noR(secondoperand) == 0))
			{
				destinationAddress = 3;
				ogerTarget = checkNumThird(secondoperand);
			}
			else if (secondoperand[0] == '#')
			{
				destinationAddress = 0;
				secondnumForTav = checkNumZero(secondoperand,linenumber,&problem);
			}
			else if (checkNumTwo(secondoperand) == 0)
			{
				destinationAddress = 1;
				tavNameForOne(secondoperand, tavNameSecondOperand,linenumber,&problem);
			}
			else
			{
				destinationAddress = 2;
				ogerTarget = checkRegNum(secondoperand,linenumber,&problem);
				tavNameFunc(secondoperand, tavNameSecondOperand);
			}
		}
	}

	if ((firstoperand == NULL) && (secondoperand == NULL))/*check validation of commands with no operands*/
	{
		sourceAddress = -1;
		destinationAddress = -1;


		if (strcmp(command, "rts") == 0)
		{
			opcode = 14;
		}
		else if (strcmp(command, "stop") == 0)
		{
			opcode = 15;
		}

	}

	if ((opcode == 0) && (funct == 0))
	{

		if (strcmp(command, "mov") != 0)/*check if there is too many operands for the command*/
		{
				printf("Line %d problem is: too many operands for %s\n", linenumber, command);
				***problem = 1;
				return;
		}
	}
	/*check if there is two,one,or zero immediate methods*/
	if (numForTav == 0 && secondnumForTav == 0)
		ifNum = 0;
	else if (numForTav != 0 && secondnumForTav == 0)
		ifNum = 1;
	else if (numForTav == 0 && secondnumForTav != 0)
		ifNum = 2;
	else
		ifNum = 3;
	if (whatpass == 1) {
		****IC = ToMemoryData(sourceAddress, destinationAddress, opcode, funct, ogerSource, ogerTarget, numForTav, secondnumForTav, tavNameFirstOperand, tavNameSecondOperand, ifNum, ****IC, RamMemory);
	}
	else {
		****IC = ToMemoryData2(sourceAddress, destinationAddress, opcode, funct, ogerSource, ogerTarget, numForTav, secondnumForTav, tavNameFirstOperand, tavNameSecondOperand, ifNum, ****IC, RamMemory, lebels);
	}
	
}
/*noR function is checking if index method is written as should and have no errors,
it gets operand(char array).
if there is an error it will send 1, else 0*/
int noR(char operand[])
{
	int i = 0, flag = 0;
	char str[80];
	memset(str,0,80);

	for (i = 1; i < strlen(operand); i++)
	{
		str[i-1] = operand[i];
	}
	flag = atoi(str);

	if(flag > 15)
			return 1;
	for (i = 0; i < strlen(operand); i++)
	{
		if (operand[i] == '[')
		{
			return 1;
			break;
		}
	}

	return 0;

}
/*tavNameForOne function is checking if direct method is written as should and have no errors,
if there is an error it will write the error type and what number of line, else it will 
get the number into a string to know the number to add or to subtract.*/
void tavNameForOne(char operand[], char tavName[],int linenumber,int ****problem)
{
	int i = 0;

	for (i = 0; i < strlen(operand); i++)
	{
			if((operand[i] >= 48 && operand[i] <= 57) || (operand[i] >= 65 && operand[i] <= 90) || (operand[i] >= 97 && operand[i] <= 122)){
		tavName[i] = operand[i];
			}
		else
			{
				printf("Line %d problem is: invalid label in direct method in %s\n", linenumber, operand);
				****problem = 1;
				memset(tavName,0,80);
				strcpy(tavName,operand);
			}
	}

}
/*tavNameFunc function is getting operand(char array) with index method and tavName(char array),
it changes tavName to the label name in the index method.*/
void tavNameFunc(char operand[], char tavName[])
{
	int i = 0;

	for (i = 0; i < strlen(operand); i++)
	{
		if (operand[i] == '[')
			break;
		tavName[i] = operand[i];
	}

}
/*checkNumZero function is checking if a number in immediate method is valid.
it gets operand(char array),line number(int) and problem(int pointer).
if there is an error it returns 0. else, it returns the number in the method.*/
int checkNumZero(char operand[],int linenumber,int ****problem)
{
	int i = 0, numAfterTav = 0, counting = 0;
	char str[80];
	memset(str, 0, 80);

	if (operand[1] == '+')
	{
		for (i = 2; i <= (strlen(operand) - 1); i++)
		{
			if (operand[i] >= 48 && operand[i] <= 57)
			{
				str[counting] = operand[i];
				counting++;
			}
			else
			{
				printf("Line %d problem is: invalid number in immediate method in %s\n", linenumber, operand);
				****problem = 1;
				return 0;
			}
		}
	}

	else
	{
		for (i = 1; i <= strlen(operand) - 1; i++)
		{
			if (i == 1)
			{
				if (operand[i] == '-')
				{
					str[counting] = operand[i];
					counting++;
					continue;
				}
			}
			if (operand[i] >= 48 && operand[i] <= 57)
			{
				str[counting] = operand[i];
				counting++;
			}
			else
			{
				printf("Line %d problem is: invalid number in immediate method in %s\n", linenumber, operand);
				****problem = 1;
				return 0;
			}
		}
	}
	numAfterTav = atoi(str);
	return numAfterTav;
}
/*checkNumThird function is checking the number of a register in register direct.
it gets operand(char array), and returns the register number.*/
int checkNumThird(char operand[])
{
	int i = 0, numAfterR = 0, counting = 0;
	char str[80];
	memset(str, 0, 80);

	for (i = 1; i <= strlen(operand) - 1; i++)
	{
		if (operand[i] >= 48 && operand[i] <= 57)
		{
			str[counting] = operand[i];
			counting++;
		}
	}

	numAfterR = atoi(str);

	return numAfterR;
}
/*checkNumTwo function is checking if an operand is using direct method or index method.
the function is getting operand(char array).
if it returns 0 it's direct method, if it returns 1 it's index method.*/
int checkNumTwo(char operand[])
{
	int i = 0, firstCount = 0, secondCount = 0;

	for (i = 0; i <= strlen(operand) - 1; i++)
	{
		if (operand[i] == '[')
		{
			firstCount = 1;
		}

		if (operand[i] == ']')
		{
			secondCount = 1;
		}
	}

	if (secondCount == 1 && firstCount == 1)
	{
		return 1; /*address number 2*/
	}

	else
	{
		return 0;/*address number 1*/
	}
}
/*checkRegNum function is checking if an operand that using index method is valid.
if it's valid, it checks if the label and the register are valids.
if there is no errors, it will return the number of the register.
if there is an error, it will write the specific error and line number.
the function is getting operand(char array),line number(int) and problen(int pointer)*/
int checkRegNum(char operand[],int linenumber,int ****problem)
{
	int i = 0, firstCount = 0, secondCount = 0, numAfterTav = 0, r = 0, counting = 0,counter = 0;
	char str[80];
	memset(str, 0, 80);

	for (i = 0; i <= strlen(operand) - 1; i++)
	{

		if((operand[i] >= 48 && operand[i] <= 57) || (operand[i] >= 65 && operand[i] <= 90) || (operand[i] >= 97 && operand[i] <= 122))
			{
			if(firstCount == 0)
				continue;

				if (firstCount == 1 && secondCount == 0)
		{
			if (operand[i] == 'r')
			{
				r = 1;
				counter++;
				continue;
			}
			if(r == 1 && counter > 0)
				{
			if ((operand[i] >= 48) && (operand[i] <= 57))
			{
				if(counter > 1)
				{
				printf("Line %d problem is: invalid number in index method in %s\n", linenumber, operand);
				****problem = 1;
				return 0;
				}
				str[counting] = operand[i];
				counting++;
			}
			else
			{
				printf("Line %d problem is: invalid number in index method in %s\n", linenumber, operand);
				****problem = 1;
				return 0;
			}
			}
			}
	}
			else if(operand[i] == '[')
			{
				if(firstCount != 0)
				{
				printf("Line %d problem is: invalid number in index method in %s.\n", linenumber, operand);
				****problem = 1;
				return 0;
				}
			firstCount = 1;
			continue;
			}	
			else if(operand[i] == ']')
			{
				if(secondCount != 0)
				{
				printf("Line %d problem is: invalid label in index method in %s\n", linenumber, operand);
				****problem = 1;
				return 0;
				}
			secondCount = 1;
			continue;
			}
			else
			{
				printf("Line %d problem is: invalid label in index method in %s\n", linenumber, operand);
				****problem = 1;
				return 0;
			}
				
		}

	
	if ((firstCount == 1 && secondCount == 0) || (firstCount == 0 && secondCount == 1))
	{
		printf("Line %d problem is: invalid number in index method in %s\n", linenumber, operand);
		****problem = 1;
		return 0;
	}

	numAfterTav = atoi(str);

	if (numAfterTav < 10 || numAfterTav > 15)
	{
		
		printf("Line %d problem is: invalid number in index method in %s\n", linenumber, operand);
		****problem = 1;
		return 0;
	}

	return numAfterTav;
}






