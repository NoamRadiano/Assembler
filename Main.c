
#include "Main.h"

/*function main connects all the project and calls for the diffrent functions to do their part in the project (will be explained)
  it recieves the amount of arguments and the arguments themselfs in a string array*/
int main(int argc, char* argv[]) {
	char path[80];
	int RamMemory [8192] = {0};
	int SymbolMemory [8192] = {0};
	int IC = 100, DC = 0,IC2 = 100, j, a;
	lebel *lebels;
	lebels = (lebel *) malloc(100*sizeof(lebel));/*this is the array of lebels that we save through the program*/
	if(lebels == NULL)
		exit(1);
	for(j = 1;j < argc; j++)/*here we go through all the commend line arguments*/
	{
		strcpy(path, argv[j]);
		a = strlen(path);
		path[a] = '\0';
		preAssembler(path, argc);/*here we send to the pre assembler that removes all the macros*/
		if(StartPass(RamMemory,SymbolMemory,&IC,&DC,lebels,path)){/*here we send to the first pass*/
			if(SecondPass(RamMemory,SymbolMemory,&IC2,&DC,lebels,path)){/*here we send to the second pass*/
				AddArrays( RamMemory , SymbolMemory , IC , DC);/*here we send to connect the arrays*/
				FinalToFile(RamMemory,(IC-100),DC, path,lebels);/*here we send to make the output files*/
			}
		}/*from now we reset everything for the next line*/
		IC = 100;
		DC = 0;
		IC2 = 100;
		memset(lebels, 0, sizeof(*lebels));
		memset(RamMemory, 0, 8192);
		memset(SymbolMemory, 0, 8192);
		memset(path, 0, 80);
	}
    free(lebels);
    return 0;
}

char toDex(long n)/*a normal long converted to a dex base until 16*/
{
	if(n<10 && n>=0)
		return (char)n + '0';
	else
		return (((char)(n-8) + 47) + '0');
}

/*takes all the elements from SymbolMemory and insert into RamMemory for the final product*/
void AddArrays( int RamMemory[] , int SymbolMemory[] ,int IC ,int DC){
	int i;
	for(i = 0; i<DC; i++)
	{
		RamMemory[IC] = SymbolMemory[i];
		SetBit(RamMemory,(IC*20+18));
		ClearBit(RamMemory,(IC*20+16));
		ClearBit(RamMemory,(IC*20+17));
		ClearBit(RamMemory,(IC*20+19));
		IC++;
	}
}


/*this function creates the file .ob,.ent,.ext from the final RamMemory*/
void FinalToFile(int RamMemory [8192], int IC, int DC, char* path,lebel *lebels)
{
	long ret;
	char *ptr;
	int i,j,k,a,q = 0,p = 0,size;
	FILE *fp;
	a = strlen(path);
	path[a-1] = 'b';
	path[a-2] = 'o';
	fp = fopen (path, "w");
	if(fp == NULL)
		exit(1);
	fprintf(fp,"            %d    %d\n",IC,DC);
	for(i = 100; i<(100+IC+DC); i++)
	{
		char* symbols = (char*)malloc(sizeof(char) * 5);
		if(symbols == NULL)
			exit(1);
		for(j = 5; j>0; j--)
		{
			char* number = (char*)malloc(sizeof(char) * 4);
			if(number == NULL)
				exit(1);
			for(k = 0; k<4; k++)
			{
				if(WhatBit(RamMemory,(i*20)+(j*4)-k-1))
					number[k] = '1';
				else
					number[k] = '0';
			}
			number[k] = '\0';
			ret = strtol(number, &ptr, 2);
			
			symbols[j-1] = toDex(ret);
			free(number);
		}
	if(i>999)	
		fprintf(fp,"%d  A%c-B%c-C%c-D%c-E%c\n",i,symbols[4],symbols[3],symbols[2],symbols[1],symbols[0]);
	else
		fprintf(fp,"0%d  A%c-B%c-C%c-D%c-E%c\n",i,symbols[4],symbols[3],symbols[2],symbols[1],symbols[0]);
	free(symbols);
	}
	fclose(fp);
	path[a-1] = 'n';
	path[a-2] = 'e';
	strcat(path,"t");
	fp = fopen (path, "w");
	if(fp == NULL)
		exit(1);
	while(((lebels+q) -> lit)!=0){
		if((lebels+q) -> attributes[1] == 1){
			fprintf(fp,"%s,%d,%d\n",lebels[q].name,lebels[q].base,lebels[q].offset);	
		}
		q++;
	}
	if (NULL != fp) {
    		fseek (fp, 0, SEEK_END);
    		size = ftell(fp);

    	if (0 == size) {
        	remove(path);
    	}
	}
	q = 0;
	fclose(fp);
	path[a] = 't';
	path[a-1] = 'x';
	path[a-2] = 'e';
	fp = fopen (path, "w");
	if(fp == NULL)
		exit(1);
	while(((lebels+q) -> lit)!=0){
		while((lebels+q) -> ExternPlace[p]!=0){
			fprintf(fp,"%s BASE %d\n",lebels[q].name,lebels[q].ExternPlace[p]);
			fprintf(fp,"%s OFFSET %d\n\n",lebels[q].name,(lebels[q].ExternPlace[p])+1);
			p++;
		}
		p = 0;
		q++;
	}
	if (NULL != fp) {
    		fseek (fp, 0, SEEK_END);
    		size = ftell(fp);

    	if (0 == size) {
        	remove(path);
    	}
	}
	fclose(fp);
	
}














