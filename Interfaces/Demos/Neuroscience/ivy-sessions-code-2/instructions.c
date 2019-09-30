/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                    I N S T R U C T I O N S  . C                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
#include "tete.h"
char REASONPROMPT[]="REASON";
char REASONSPROMPT[]="";
char REFPROMPT[]="REFERENCE";
char AUTHORPROMPT[]="AUTHOR(S)";
char SUPPORTPROMPT[]="SupporT";
char DISPORTPROMPT[]="DisporT";
char OBSOLETIONPROMPT[]="ObsoletioN";
char INSTANTIATIONPROMPT[]="InstantiatioN";
char CONTEXTPROMPT[]="";
char CONTEXTSPROMPT[]="";
char ANSWERSPROMPT[]="ANSWERS";
char ANSWERPROMPT[]="ANSWER";
char QUESTIONPROMPT[]="QUESTION";
char ATOMPROMPT[]="ATOM";
char ATOMSPROMPT[]="";
char LEXICPROMPT[]="";
char ENTRYPROMPT[]="";
char TERMPROMPT[]="TERM";
char DEFINITIONPROMPT[]="\t:=";
char STOREDATOMPROMPT[]="";
/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/
void changeATOMPROMPT(char *newprompt){
	strcpy(STOREDATOMPROMPT,ATOMPROMPT);
	strcpy(ATOMPROMPT,newprompt);
}

void restoreATOMPROMPT(){
	strcpy(ATOMPROMPT,STOREDATOMPROMPT);
}

/************************************************************************************************/
/* P R O M P T  &  S C A N                                                                      */
/************************************************************************************************/
char *promptandscan(char *prompttext,bool manditory){
	static char text[5000]; 
	printf("%s\t",prompttext);
	int r = scanf("%[^\n]s\n",text);
	getchar();
	if(!r && manditory){
		while(!r){
			r=scanf("%[^\n]s\n",text);
			getchar();
		}
	} else if(r==0){
		*text='\0';
	}
	//printf("r=%d text=%s\n",r,text);
	return text;
}

/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/
void numberedinstruction(char *string){
	printf("\t(%d) %s\n",PROMPTNO,string);
	PROMPTNO++;
}
void unmberedinstruction(char *string){
	printf("\t     %s\n",string);
}
/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/


int promptAndScan(char *prompt, char *text){
	printf("%s",prompt);
	int r=scanf("%[^\n]s\n",text);
	getchar();
	if(!r){*text='\0';}
	return r;
}

int promptAndScanInstruction(char *instruction, char *text, char *sub){
	char prompt[5000];
	int n;
	if(strcmp(sub,"")==0){
		n = PROMPTNO;
		PROMPTNO++;
	} else{
		n = PROMPTNO -1;
	}
	snprintf(prompt,5000,"\t(%d%s) %s",n,sub,instruction);
	return promptAndScan(prompt,text);
}

int promptAndScanManditoryInstruction(char *instruction, char *text, char *sub){
	int r = promptAndScanInstruction(instruction, text, sub);
	while(!r){
		//printf("\tPlease answer\n");
		r=scanf("%[^\n]s\n",text);
		getchar();
	}
	return r;
}


bool scanYesNo(char *instruction, char *sub){
	char prompt[5000], answer[]="Z";
	int n;
	if(strcmp(sub,"")==0){
		n = PROMPTNO;
		PROMPTNO++;
	} else{
		n = PROMPTNO -1;
	}
	snprintf(prompt,5000,"\t(%d%s) %s Please type 'Y' for yes, and 'N' for no.\nCHOICE ",n,sub,instruction);
	promptAndScan(prompt, answer);
	//promptAndScanInstruction(prompt,answer,"");
	while(strcmp(answer,"Y")!=0 && strcmp(answer,"N")!=0){
		promptAndScan("",answer);
	}
	return strcmp(answer,"N");
}

