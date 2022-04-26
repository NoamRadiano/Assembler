#include "Main.h"

int SecondPass(int RamMemory[], int SymbolMemory[],int *IC, int *DC, lebel *lebels,char* path){
	char c;
	char *firstpart,*secondpart,*thirdpart;
	int i = 0,count = 0,node = 0,problem = 0,linenumber = 1;
	char line[82] = {0};
	FILE *fp;
	fp = fopen(path, "r");
	if(fp == NULL){
		exit(1);
	}
	while(c!= EOF){
		while((c = fgetc(fp))!= '\n'){
			if(c == EOF)
				break;
			if(c==';')
				node = 10;
			if(node == 10)
			    continue;
			else if((c==' ' || c=='\t') && (node == 0 || count == 2))
				continue;
			else if((c==' ' || c=='\t') && node == 1 && count < 2){
				line[i] = c;
				i++;
				node = 0;
				count++;
			}
			else{
				if(i == 83){
					node = 10;
					problem = 1;
				}
				line[i] = c;
				i++;
				node = 1;		
			}
			
		}
		if(c == EOF)
			break;
		node = 0;
		count = 0;
		i = 0;
		if(line[0]!='\0'){
		firstpart = strtok(line, " ");
		secondpart = strtok(NULL, " ");
		thirdpart = strtok(NULL, " ");
		investigation2(firstpart,secondpart,thirdpart,&IC,&DC,RamMemory,SymbolMemory,lebels,&problem,linenumber);
		}
		linenumber++;
		memset(line,0,82);
	}
	fclose(fp);

	if(problem == 1)
		return 0;
	return 1;
		
			
	

}



void investigation2(char firstpart[],char secondpart[],char thirdpart[],int **IC, int **DC, int RamMemory[], int SymbolMemory[], lebel *lebels, int *problem, int linenumber){
	int a,i;
	char commands[][20] = {"mov","cmp","add","sub","lea","clr","not","inc","dec","jmp","bne","jsr","red","prn","rts","stop"};
	a = strlen(firstpart);
	if(firstpart[a-1] == ':'){/*everything that start with a label*/
			/*we look for the commands only*/
		for(i = 0; i< 16; i++){
			if(strcmp(secondpart, commands[i])==0){
				check(firstpart,secondpart,thirdpart,2,&IC, RamMemory, lebels , 2, &problem, linenumber);/*we send to analasis*/
				return;
			}
		}
	}
	/*we look for the commands only*/
	for(i = 0; i< 16; i++){
		if(strcmp(firstpart, commands[i])==0){
			if(thirdpart != NULL && secondpart != NULL)
				strcat(secondpart,thirdpart);
				check(firstpart,secondpart,thirdpart,1,&IC, RamMemory, lebels, 2, &problem, linenumber);/*we send to analasis*/
				return;	
		}
	}
	
	/*we look for .entry to add to our lebel structs*/
	if(strcmp(firstpart, ".entry")==0){
		int i = 0,node = 0;
		while(((lebels+i) -> lit)!=0){
			if(strcmp((lebels+i) -> name, secondpart)==0){
				if((lebels+i) -> attributes[2] == 0)
					(lebels+i) -> attributes[1] = 1;
				else{
					printf("line %d : external and entry lebel\n" , linenumber);
					*problem = 1;
				}
				node = 1;
			}
		i++;
		}
		if(node == 0){
			printf("line %d : error lebel not found\n" , linenumber);
			*problem = 1;
		}
		
		return;
	}

}
/*here we insert to the ram memmory in the second pass, we get all the information we need and dicide which bits to light
  we dont return anything, we only change the array itself*/
int ToMemoryData2(int sourceAddress, int destinationAddress, int opcode, int funct, int ogerSource,int ogerTarget, int numForTav, int secondnumForTav, char tavNameFirstOperand[], char tavNameSecondOperand[],int ifNum, int IC, int RamMemory[], lebel *lebels){
	int i = 0,j = 0, k = 0,q = 0;
	/*printf("%s\n", tavNameFirstOperand);*/
	/*printf("%s\n", tavNameSecondOperand);*/
	IC++;
	if(sourceAddress == (-1) && destinationAddress != (-1)){/*1 operand*/
	IC++;	
	if(destinationAddress == 1 || destinationAddress == 2){
		/*we fill the gaps, 2 lines*/
		while((lebels[i].lit)!=0){
			if(strcmp(lebels[i].name, tavNameSecondOperand)==0){
				/*printf("%s\n", tavNameFirstOperand);*/
				/*printf("%s\n", tavNameSecondOperand);*/
				break;
			}
		i++;
		}
		RamMemory[IC] = lebels[i].base;
		if(lebels[i].attributes[2] == 1){
			while(lebels[i].ExternPlace[q]!=0)
				q++;
			lebels[i].ExternPlace[q] = IC;
			/*printf("%s BASE %d\n",lebels[i].name,IC);*/
			/*printf("%s OFFSET %d\n",lebels[i].name,IC+1);*/
			SetBit(RamMemory,(IC*20+16));
			ClearBit(RamMemory,(IC*20+17));
			ClearBit(RamMemory,(IC*20+18));
			ClearBit(RamMemory,(IC*20+19));
			IC++;	
		}
		else{
			SetBit(RamMemory,(IC*20+17));
			ClearBit(RamMemory,(IC*20+16));
			ClearBit(RamMemory,(IC*20+18));
			ClearBit(RamMemory,(IC*20+19));
			IC++;	
		}
		RamMemory[IC] = lebels[i].offset;
		if(lebels[i].attributes[2] == 1){
			SetBit(RamMemory,(IC*20+16));
			ClearBit(RamMemory,(IC*20+17));
			ClearBit(RamMemory,(IC*20+18));
			ClearBit(RamMemory,(IC*20+19));
			IC++;	
		}
		else{
			SetBit(RamMemory,(IC*20+17));
			ClearBit(RamMemory,(IC*20+16));
			ClearBit(RamMemory,(IC*20+18));
			ClearBit(RamMemory,(IC*20+19));
			IC++;	
		}
	}
	else if(destinationAddress == 0){
		IC++;
	}		
	}
	else if(sourceAddress == (-1) && destinationAddress == (-1)){/*no operands*/	
	}
	else{/*2 operands*/
	IC++;	
	if(sourceAddress == 1 || sourceAddress == 2){
		/*we fill the gaps, 2 lines*/
		while((lebels[j].lit)!=0){
			if(strcmp(lebels[j].name, tavNameFirstOperand)==0){
				break;
			}
		j++;
		}
		RamMemory[IC] = lebels[j].base;
		if(lebels[j].attributes[2] == 1){
			while(lebels[j].ExternPlace[q]!=0)
				q++;
			lebels[j].ExternPlace[q] = IC;
			q = 0;
			SetBit(RamMemory,(IC*20+16));
			ClearBit(RamMemory,(IC*20+17));
			ClearBit(RamMemory,(IC*20+18));
			ClearBit(RamMemory,(IC*20+19));
			IC++;	
		}
		else{
			SetBit(RamMemory,(IC*20+17));
			ClearBit(RamMemory,(IC*20+16));
			ClearBit(RamMemory,(IC*20+18));
			ClearBit(RamMemory,(IC*20+19));
			IC++;	
		}
		RamMemory[IC] = lebels[j].offset;
		if(lebels[j].attributes[2] == 1){
			SetBit(RamMemory,(IC*20+16));
			ClearBit(RamMemory,(IC*20+17));
			ClearBit(RamMemory,(IC*20+18));
			ClearBit(RamMemory,(IC*20+19));
			IC++;	
		}
		else{
			SetBit(RamMemory,(IC*20+17));
			ClearBit(RamMemory,(IC*20+16));
			ClearBit(RamMemory,(IC*20+18));
			ClearBit(RamMemory,(IC*20+19));
			IC++;	
		}
	}
	else if(sourceAddress == 0){
		IC++;
	}
	if(destinationAddress == 1 || destinationAddress == 2){
		/*we fill the gaps, 2 lines*/
		while((lebels[k].lit)!=0){
			if(strcmp(lebels[k].name, tavNameSecondOperand)==0){
				break;
			}
		k++;
		}
		RamMemory[IC] = lebels[k].base;
		if(lebels[k].attributes[2] == 1){
			while(lebels[k].ExternPlace[q]!=0)
				q++;
			lebels[k].ExternPlace[q] = IC;
			q = 0;
			/*printf("%s BASE %d\n",lebels[k].name,IC);*/
			/*printf("%s OFFSET %d\n",lebels[k].name,IC+1);*/
			SetBit(RamMemory,(IC*20+16));
			ClearBit(RamMemory,(IC*20+17));
			ClearBit(RamMemory,(IC*20+18));
			ClearBit(RamMemory,(IC*20+19));
			IC++;	
		}
		else{
			SetBit(RamMemory,(IC*20+17));
			ClearBit(RamMemory,(IC*20+16));
			ClearBit(RamMemory,(IC*20+18));
			ClearBit(RamMemory,(IC*20+19));
			IC++;	
		}
		RamMemory[IC] = lebels[k].offset;
		if(lebels[k].attributes[2] == 1){
			SetBit(RamMemory,(IC*20+16));
			ClearBit(RamMemory,(IC*20+17));
			ClearBit(RamMemory,(IC*20+18));
			ClearBit(RamMemory,(IC*20+19));
			IC++;	
		}
		else{
			SetBit(RamMemory,(IC*20+17));
			ClearBit(RamMemory,(IC*20+16));
			ClearBit(RamMemory,(IC*20+18));
			ClearBit(RamMemory,(IC*20+19));
			IC++;	
		}
	}
	else if(destinationAddress == 0){
		IC++;
	}
	}
	return IC;
}


	
	




























