#ifndef Main_h
#define Main_h
#define  SetBit(RamMemory,k)     ( RamMemory[(k/20)] |= (1 << (k%20)) ) /*this define sets a specific bit to 1*/
#define ClearBit(RamMemory,k)   ( RamMemory[(k/20)] &= ~(1 << (k%20)) ) /*this define sets a specific bit to 0*/
#define WhatBit(RamMemory,k)    (RamMemory[((k)/20)] & (1 << ((k)%20))) /*this define lets us know what the specific bit is*/
#define maxChar 81
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {/*this will define each lebel with the right attributes*/
	char name[31];
	int value;
	int base;
	int offset;
	int lit;
	int ExternPlace[31];
	int attributes[4];
}lebel;
typedef struct Macro {/*this saves the macro's names and contant for the pre assembler*/
	char macroName[maxChar];
	char* macroContent;
}Macro;

void preAssembler(char* path, int argc);
char toDex(long n);
void AddArrays( int RamMemory[] , int SymbolMemory[] ,int IC ,int DC);
void FinalToFile(int RamMemory [8192], int n, int f, char* path,lebel lebels[]);
int StartPass(int RamMemory[], int SymbolMemory[],int *IC, int *DC, lebel *lebels,char* path);
int SecondPass(int RamMemory[], int SymbolMemory[],int *IC, int *DC, lebel *lebels,char* path);
int investigation(char firstpart[],char secondpart[],char thirdpart[],int **IC, int **DC, int RamMemory[], int SymbolMemory[], lebel *lebels, int *problem, int linenumber);
void check(char firstpart[],char secondpart[],char thirdpart[], int what,int ***IC, int RamMemory[],lebel *lebels,int whatpass, int **problem , int linenumber);
int tosymbolstring(char thirdpart[], int SymbolMemory[] , int DC, int **problem, int linenumber);
int tosymboldata(char thirdpart[], int SymbolMemory[] , int DC , int **problem, int linenumber);
int isnum(char a);
int addlebel(lebel new,lebel lebels[], int attribute);
void checkoperand(char command[], char firstoperand[], char secondoperand[], int**** IC, int RamMemory[], lebel *lebels, int whatpass,int linenumber,int ***problem);
int checkNumZero(char operand[],int linenumber,int ****problem);
int checkNumTwo(char operand[]);
int checkNumThird(char operand[]);
int checkRegNum(char operand[],int linenumber,int ****problem);
int ToMemoryData(int sourceAddress, int destinationAddress, int opcode, int funct, int ogerSource,int ogerTarget, int numForTav, int secondnumForTav, char tavNameFirstOperand[], char tavNameSecondOperand[],int ifNum, int IC, int RamMemory[]);
void investigation2(char firstpart[],char secondpart[],char thirdpart[],int **IC, int **DC, int RamMemory[], int SymbolMemory[], lebel *lebels, int *problem, int linenumber);
int ToMemoryData2(int sourceAddress, int destinationAddress, int opcode, int funct, int ogerSource,int ogerTarget, int numForTav, int secondnumForTav, char tavNameFirstOperand[], char tavNameSecondOperand[],int ifNum, int IC, int RamMemory[], lebel *lebels);
void tavNameFunc(char operand[], char tavName[]);
void tavNameForOne(char operand[], char tavName[],int linenumber,int ****problem);
int noR(char operand[]);
char* removeWhiteSpaces(char* str);
#endif
