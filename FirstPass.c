#include "Main.h"
/*this is the first pass here we go though the text line by line trying to find mistakes and send the text to investigation
  we divide the line to 3 parts and then the next function devides the probabilities of what we can expect
  the function return 1 if everything is ok and 0 if there is a problem*/
int StartPass(int RamMemory[], int SymbolMemory[],int *IC, int *DC, lebel *lebels,char* path){
	char c;
	char *firstpart,*secondpart,*thirdpart;
	int i = 0,count = 0,node = 0, k = 0,linenumber = 1,problem = 0, string = 0, data = 0, loop = 0,skip = 0;
	char line[82] = {0};
	char am[] = ".am";
	FILE *fp;
	strcat(path, am);
	fp = fopen(path, "r");
	if(fp == NULL)
		exit(1);
	while(c!= EOF){
			
		while((c = fgetc(fp))!= '\n'){
			if(c == EOF)
				break;
			if(c==';')
				node = 10;
			if(node == 10)
			    continue;
			if(i == 82){
					printf("line %d : too much characters in a line\n", linenumber);
					problem = 1;
					node = 10;
					skip = 1;
				}
			if(data == 1){
				if((loop == 0 || loop == 1) && (c==' ' || c=='\t')){
					loop = 1;
				}
				else if((loop == 0 || loop == 1) && (c=='-' || c=='+')){
					loop = 2;
				}
				else if((loop == 0 || loop == 1 || loop == 2 || loop == 3) && (c>47 && c<58)){
					loop = 3;
				}
				else if((loop == 3 || loop == 4) && (c==' ' || c=='\t')){
					loop = 4;
				}
				else if((loop == 3 || loop == 4) && c==',' ){
					loop = 0;
				}
				else{
					printf("line %d : problem with numbers of data\n", linenumber);
					problem = 1;	
					skip = 1;
				}
			}
			if(string == 1 && c=='"' && count == 3){
				count = 2;
				string = 0;
			}
			if(string == 1 && c=='"' && count < 3)
				count = 3;
			if((c==' ' || c=='\t') && (node == 0 || count == 2))
				continue;
			else if((c==' ' || c=='\t') && node == 1 && count < 2){
				line[i] = c;
				if(i > 6)
					if(line[i-1]=='g' && line[i-2]=='n' && line[i-3]=='i' && line[i-4]=='r' && line[i-5]=='t' && line[i-6]=='s' && line[i-7]=='.')
						string = 1;
				if(i > 4)
					if(line[i-1]=='a' && line[i-2]=='t' && line[i-3]=='a' && line[i-4]=='d' && line[i-5]=='.')
						data = 1;
				i++;
				node = 0;
				count++;
			}
			else{
				if(i == 82){
					printf("line %d : too much characters in a line\n", linenumber);
					problem = 1;
					node = 10;
					skip = 1;
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
		string = 0;
		
		if(data == 1 && (loop != 3 && loop != 4)){
			printf("line %d : spacing problem in the numbers\n", linenumber);
			problem = 1;
			skip = 1;
		}
		data = 0;
		loop = 0;
		if(*IC+*DC >8185){
			printf("line %d : too much characters in a line\n", linenumber);
			problem = 1;
			break;	
		}
		if(skip == 1){
			memset(line,0,82);
			linenumber++;
			skip = 0;
			continue;
		}
		if(line[0]!='\0'){
		firstpart = strtok(line, " ");
		secondpart = strtok(NULL, " ");
		thirdpart = strtok(NULL, " ");
		/*printf("first word: %s, second word: %s, third word: %s\n",firstpart ,secondpart ,thirdpart );*/
		if(strcmp(firstpart, ".entry")==0){
			memset(line,0,82);
			linenumber++;
			continue;
		}
		if(investigation(firstpart,secondpart,thirdpart,&IC,&DC,RamMemory,SymbolMemory,lebels,&problem,linenumber))
			problem = 1;
		}
		memset(line,0,82);
		linenumber++;	
	}
	fclose(fp);
	while(((lebels + k) -> lit)!=0){
		if((lebels + k) -> attributes[3] == 1){
			(lebels + k) -> value = (lebels + k) -> value + *IC;	
			(lebels + k) -> base = (((lebels + k) -> value/16)*16);
			(lebels + k) -> offset = ((lebels + k) -> value%16);
		}
		k++;
	}
	if(problem == 1){
		printf("end of problems file : %s\n\n" , path);
		return 0;
	}
	return 1;
	
}


/*this is where we determine what is written in the command
return 1 if there is a problem and return 0 if its ok*/
int investigation(char firstpart[],char secondpart[],char thirdpart[],int **IC, int **DC, int RamMemory[], int SymbolMemory[], lebel *lebels,int *problem, int linenumber){
	int a,i,j,k;
	char commands[][20] = {"mov","cmp","add","sub","lea","clr","not","inc","dec","jmp","bne","jsr","red","prn","rts","stop"};
	char names[][20] = {"r0","r1","r2","r3","r4","r5","r6","r7","r8","r9","r10","r11","r12","r13","r14","r15",".string",".data", ".entry",".extern"};
	a = strlen(firstpart);
	
	if(firstpart[a-1] == ':'){/*everything that start with a label*/
		if(a>32){
			printf("line %d : the lebel is too long\n", linenumber);
			return 1;
		}
		firstpart[a-1] = '\0';
		for(j = 0; j< 16; j++){
			if(strcmp(firstpart, commands[j])==0){
				printf("line %d : lebel name is illigal\n", linenumber);
				return 1;
			}
		}
		for(k = 0; k< 20; k++){
			if(strcmp(firstpart, names[k])==0){
				printf("line %d : lebel name is illigal\n", linenumber);
				return 1;
			}
		}
		/*if the commend after the lebel is .string*/
		if(strcmp(secondpart, ".string")==0){
			lebel new;
			strcpy( new.name, firstpart);
			new.value = **DC;	
			new.base = 0;
			new.offset = 0;
			new.lit = 1;	
			if(addlebel(new, lebels , 3)){
				printf("line %d : the lebel is defined twice\n", linenumber);
				return 1;
			}
			if((**IC+**DC+(strlen(secondpart))) < 8193)
				**DC = tosymbolstring(thirdpart, SymbolMemory , **DC, &problem, linenumber);
			else{
			printf("line %d : out of bounds\n", linenumber);
			return 1;
			}
			return 0;
		}
		/*if the commend after the lebel is .data*/
		if(strcmp(secondpart, ".data")==0){
			lebel new;
			strcpy( new.name, firstpart);
			new.value = **DC;	
			new.base = 0;
			new.offset = 0;
			new.lit = 1;	
			if(addlebel(new, lebels , 3)){
				printf("line %d : the lebel is defined twice\n", linenumber);
				return 1;
			}
			if((**IC+**DC+((strlen(secondpart))/2)) < 8193)
				**DC = tosymboldata(thirdpart, SymbolMemory , **DC, &problem, linenumber);
			else{
			printf("line %d : out of bounds\n", linenumber);
			return 1;
			}
			return 0;
		}
			/*if the commend after the lebel is a commend*/
		for(i = 0; i< 16; i++){
			if(strcmp(secondpart, commands[i])==0){
				lebel new;
				strcpy(new.name, firstpart);
				new.value = **IC;	
				new.base = ((**IC/16)*16);
				new.offset = (**IC%16);	
				new.lit = 1;
				if(addlebel(new, lebels , 0)){
					printf("line %d : the lebel is defined twice\n", linenumber);
					return 1;
				}
				check(firstpart,secondpart,thirdpart,2,&IC, RamMemory, lebels, 1 , &problem, linenumber);
				return 0;
			}
		}
	}
	/*if the commend is .data*/
	if(strcmp(firstpart, ".data")==0){
		if(thirdpart != NULL)
			strcat(secondpart,thirdpart);
		if((**IC+**DC+((strlen(secondpart))/2)) < 8193)
			**DC = tosymboldata(secondpart, SymbolMemory , **DC,&problem, linenumber);
		else{
			printf("line %d : out of bounds\n", linenumber);
			return 1;
		}
		return 0;
	}
		/*if the commend is .string*/
	if(strcmp(firstpart, ".string")==0){
		if(thirdpart != NULL)
			strcat(secondpart,thirdpart);
		if((**IC+**DC+(strlen(secondpart))) < 8193)
			**DC = tosymbolstring(secondpart, SymbolMemory , **DC,&problem, linenumber);
		else{
			printf("line %d : out of bounds\n", linenumber);
			return 1;
		}	
		return 0;
	}
	/*if the commend is .extern*/
	if(strcmp(firstpart, ".extern")==0){
		lebel new;
		strcpy(new.name, secondpart);
		new.value = 0;	
		new.base = 0;
		new.offset = 0;	
		new.lit = 1;
		if(addlebel(new, lebels , 2)){
			printf("line %d : the lebel is defined twice\n", linenumber);
			return 1;
		}
		return 0;
	}
	/*if the commend is a formal commend*/
	for(i = 0; i< 16; i++){
		if(strcmp(firstpart, commands[i])==0){
			if(thirdpart != NULL && secondpart != NULL)
				strcat(secondpart,thirdpart);
			check(firstpart,secondpart,thirdpart,1,&IC, RamMemory, lebels, 1, &problem, linenumber);
			return 0;	
		} 
	}
	/*if none of the options above worked we return error*/
	printf("line %d : unknown command\n", linenumber);
	return 1;
	
}

/*this is where we get when we have a command with operands
 in the end we send the commend and 2 operands to checkoperand*/
void check(char firstpart[],char secondpart[],char thirdpart[], int what,int ***IC, int RamMemory[],lebel *lebels,int whatpass, int **problem , int linenumber){
	char* firstoperand = (char*)malloc(sizeof(char));
	char* secondoperand = (char*)malloc(sizeof(char));
	char a;
	int i = 0,node = 0,j = 0,k = 0;
		firstoperand[0] = '\0';
		secondoperand[0] = '\0';
		if(what == 1){ /*action with operands*/ 
			if(secondpart != NULL){
			while((a = secondpart[k])!= '\0')
			{
				if(a == ',' && (node == 0 || node == 2 || node == 3)){
					printf("line %d : comma in the wrong place\n", linenumber);
					**problem = 1;
					break;
				}
				if(a == ',' && node == 1){
					node = 2;
					k++;
				}
				
				if(a != ',' && (node == 0 || node == 1)){
					firstoperand[i] = a;
					i++;
					k++;
					firstoperand = (char *) realloc(firstoperand, (sizeof(char)*i));
					node = 1;
				}
				if(a != ',' && (node == 2 || node == 3)){
					secondoperand[j] = a;
					j++;
					k++;
					node = 3;
					secondoperand = (char *) realloc(secondoperand, (sizeof(char)*j));
				}						
			}
			}
			if(node == 2){
				printf("line %d : no second operand\n", linenumber);
				**problem = 1;
			}
			
			if(secondoperand[0] != '\0'){
				firstoperand[i] = '\0';
				secondoperand[j] = '\0';
				checkoperand(firstpart,firstoperand,secondoperand,&IC, RamMemory,lebels, whatpass, linenumber, &problem);
			}
			else if (firstoperand[0] != '\0'){
				firstoperand[i] = '\0';
				secondoperand = NULL;
				checkoperand(firstpart,secondoperand,firstoperand,&IC, RamMemory,lebels, whatpass, linenumber, &problem);
			}
			else{
				firstoperand = NULL;
				secondoperand = NULL;
				checkoperand(firstpart,secondoperand,firstoperand,&IC, RamMemory,lebels, whatpass, linenumber, &problem);
			}
		}	
		else if(what == 2){ /*lebel with action with operands*/
			if(thirdpart != NULL){
			while((a = thirdpart[k])!= '\0')
			{
				if(a == ',' && (node == 0 || node == 2 || node == 3)){
					printf("line %d : comma in the wrong place\n", linenumber);
					**problem = 1;
					break;
				}
				if(a == ',' && node == 1){
					node = 2;
					k++;
				}
				
				if(a != ',' && (node == 0 || node == 1)){
					firstoperand[i] = a;
					i++;
					k++;
					firstoperand = (char *) realloc(firstoperand, (sizeof(char)*i));
					node = 1;
				}
				if(a != ',' && (node == 2 || node == 3)){
					secondoperand[j] = a;
					j++;
					k++;
					node = 3;
					secondoperand = (char *) realloc(secondoperand, (sizeof(char)*j));
				}							
			}
			}
			if(node == 2){
				printf("line %d : no second operand\n", linenumber);
				**problem = 1;
			}

			if(secondoperand[0] != '\0'){
				firstoperand[i] = '\0';
				secondoperand[j] = '\0';
				checkoperand(secondpart,firstoperand,secondoperand,&IC, RamMemory,lebels, whatpass, linenumber, &problem);
			}
			else if (firstoperand[0] != '\0'){
				firstoperand[i] = '\0';
				secondoperand = NULL;
				checkoperand(secondpart,secondoperand,firstoperand,&IC, RamMemory,lebels, whatpass, linenumber, &problem);
			}
			else{
				firstoperand = NULL;
				secondoperand = NULL;
				checkoperand(secondpart,secondoperand,firstoperand,&IC, RamMemory,lebels, whatpass, linenumber, &problem);
			}
		
		}
		
		/*printf("first - %s , second - %s\n", firstoperand , secondoperand);*/
		free(firstoperand);
		free(secondoperand);
	}




/*here we fill the SymbolMemory with the values of the string we got*/
int tosymbolstring(char thirdpart[], int SymbolMemory[] ,int DC, int **problem, int linenumber){
	int i = 1,a;
	a = strlen(thirdpart);
	if(thirdpart[0] == '"' && thirdpart[a-1] == '"'){
		thirdpart[a-1] = '\0';
		while(thirdpart[i]!= '\0'){
			SymbolMemory[DC] = (int)thirdpart[i];
			DC++;
			i++;
		}
		SymbolMemory[DC] = 0;
		DC++;
	}
	else{
		printf("line %d : there are no opostrophes\n", linenumber);
		**problem = 1;
	}
	return DC;
}

/*here we fill the SymbolMemory with the values of the numbers we got*/
int tosymboldata(char thirdpart[], int SymbolMemory[] , int DC , int **problem, int linenumber){
	int i = 0,j = 0, p = 0,f;
	char num[6];
	int numbers[38] = {0};
	while(thirdpart[i]!= '\0'){
		while(thirdpart[i]!= ',' && thirdpart[i]!= '\0'){
			if(isnum(thirdpart[i])){
				num[j] = thirdpart[i];
				j++;
			}
			i++;
		}
		numbers[p] = atoi(num);
		if(numbers[p]>65535){
			printf("line %d : number too large\n", linenumber);
			**problem = 1;
		}
		p++;
		i++;
		j=0;
		memset(num,0,6);
	}
	for(f=0;f<(p); f++){
		SymbolMemory[DC] = numbers[f];
		DC++;	
	}
	return DC;
}

int isnum(char a){
	if((a>47 && a<58) || a == '-' || a == '+')
		return 1;
	return 0;
}


/*here we get a new lebel that we need to enter to the lebels array which is dynamic*/
int addlebel(lebel new,lebel *lebels, int attribute){
	int i = 0;
	while(((lebels + i) -> lit)!=0){
		if(strcmp((lebels + i) -> name, new.name)==0){
			return 1;
		}
		i++;
	}
	strcpy(((lebels + i) -> name), new.name);
	(lebels + i) -> value = new.value;	
	(lebels + i) -> base = new.base;
	(lebels + i) -> offset = new.offset;	
	(lebels + i) -> lit = 1;
	(lebels + i) -> attributes[attribute] = 1;
	lebels = (lebel *) malloc((i+3)*sizeof(lebel));/*here we make the array bigger*/
	if(lebels == NULL)
		exit(1);
	return 0;
}

/*here we insert to the ram memmory, we get all the information we need and dicide which bits to light
  we dont return anything, we only change the array itself*/
int ToMemoryData(int sourceAddress, int destinationAddress, int opcode, int funct, int ogerSource,int ogerTarget, int numForTav, int secondnumForTav, char tavNameFirstOperand[], char tavNameSecondOperand[],int ifNum, int IC, int RamMemory[]){
	SetBit(RamMemory,(IC*20+opcode));
	SetBit(RamMemory,(IC*20+18));
	IC++;
	if(sourceAddress == (-1) && destinationAddress != (-1)){/*1 operand*/
	RamMemory[IC] = (destinationAddress|RamMemory[IC]);
	RamMemory[IC] = ((ogerTarget<<2)|RamMemory[IC]);
	RamMemory[IC] = ((funct<<12)|RamMemory[IC]);
	SetBit(RamMemory,(IC*20+18));
	IC++;	
	if(destinationAddress == 1 || destinationAddress == 2){
		IC = IC + 2;
	}
	else if(destinationAddress == 0){
		RamMemory[IC] = secondnumForTav;
		SetBit(RamMemory,(IC*20+18));
		ClearBit(RamMemory,(IC*20+16));
		ClearBit(RamMemory,(IC*20+17));
		ClearBit(RamMemory,(IC*20+19));
		IC++;
	}	
	}
	else if(sourceAddress == (-1) && destinationAddress == (-1)){/*no operands*/	
	}
	else{/*2 operands*/
	RamMemory[IC] = (destinationAddress|RamMemory[IC]);
	RamMemory[IC] = ((ogerTarget<<2)|RamMemory[IC]);
	RamMemory[IC] = ((sourceAddress<<6)|RamMemory[IC]);
	RamMemory[IC] = ((ogerSource<<8)|RamMemory[IC]);
	RamMemory[IC] = ((funct<<12)|RamMemory[IC]);
	SetBit(RamMemory,(IC*20+18));
	IC++;	
	if(sourceAddress == 1 || sourceAddress == 2){
		IC = IC + 2;
	}
	else if(sourceAddress == 0){
		RamMemory[IC] = numForTav;
		SetBit(RamMemory,(IC*20+18));
		ClearBit(RamMemory,(IC*20+16));
		ClearBit(RamMemory,(IC*20+17));
		ClearBit(RamMemory,(IC*20+19));
		IC++;
	}
	if(destinationAddress == 1 || destinationAddress == 2){
		IC = IC + 2;
	}
	else if(destinationAddress == 0){
		RamMemory[IC] = secondnumForTav;
		SetBit(RamMemory,(IC*20+18));
		ClearBit(RamMemory,(IC*20+16));
		ClearBit(RamMemory,(IC*20+17));
		ClearBit(RamMemory,(IC*20+19));
		IC++;
	}
	}
	return IC;
}
















