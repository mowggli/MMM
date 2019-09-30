/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                          W R I T E    A D D E N D U M . C                                    */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
#include "tete.h"
#define HLINE "___"
#define SINGLE_INDENT "  "
#define SINGLE_INDENT_LENGTH 1

/************************************************************************************************/
/*                                                                                      */
/************************************************************************************************/
char *depth2indent(int depth){
	static char indent[5000];
	strcpy(indent,"");
	int d=depth;
	while(d>0){
		strcat(indent,"   ");
		d--;
	};
	return indent;
}

/************************************************************************************************/
/* INDENT                                                                                     */
/************************************************************************************************/
/*
	Say ***\t| is your parents indent

	If you're a niece of someone, then your own indent is your parents + \t| that is: it is ***\t|\t|

	If you're ununcled (no uncles older than your father) then your own indent is ***\t\t|
*/

char * uncledIndent(char *parentIndent){
	static char indent[2000];
	strcpy(indent, parentIndent);
	strcat(indent,SINGLE_INDENT);
	strcat(indent,"|");
	return indent;
}

char * ununcledIndent(char *parentIndent){
	static char indent[2000];
	strcpy(indent, parentIndent);
	indent[strlen(indent)-SINGLE_INDENT_LENGTH]='\0';
	strcat(indent,SINGLE_INDENT);
	strcat(indent,"|");
	return indent;
}

char *indentchild(bool neice, char *parentIndent){
	static char indent[2000];
	if(neice){
		strcpy(indent, uncledIndent(parentIndent));
	}
	else{
		strcpy(indent, ununcledIndent(parentIndent));
	}
	return indent;
}

char *before(char *indent, bool spaced){
	static char before[2000];
	strcpy(before,"\n");
	strcat(before,indent);
	if(spaced){
	strcat(before,"\n");
	strcat(before,indent);
	}
	strcat(before,HLINE);
	return before;
}
/************************************************************************************************/
/*                                                                                      */
/************************************************************************************************/
char *tagtotitle(char *tag){
	static char title[5000];
	if(strcmp(tag,"MAIN")==0){
		snprintf(title,5000,"Main Argument");
	} else if(strcmp(tag,"US MAIN")==0){
		snprintf(title,5000,"Main Argument");
	} else if(strcmp(tag,"T1-CNFDR")==0){
		snprintf(title,5000,"Addressed Confounder");
	} else if(strcmp(tag,"T2-CNFDR")==0){
		snprintf(title,5000,"Confounder");
	} else if(strcmp(tag,"T3-CNFDR")==0){
		snprintf(title,5000,"Known Limitation");
	} else if(strcmp(tag,"BIB")==0){
		snprintf(title,5000,"Publication Reference");
	} else if(strcmp(tag,"NO BIB")==0){
		snprintf(title,5000,"Publication Reference");
	} else if(strcmp(tag,"UPSTREAM")==0){
		snprintf(title,5000,"Upstream Statement");
	} else if(strcmp(tag,"EXPMT")==0){
		snprintf(title,5000,"Experiment Description");
	} else if(strcmp(tag,"NO EXPMT")==0){
		snprintf(title,5000,"Experiment Description");
	} else if(strcmp(tag,"DATA")==0){
		snprintf(title,5000,"Data");
	} else if(strcmp(tag,"Question")==0 || strcmp(tag,"QUESTION")==0){
		return lowerthecase(QUESTIONTAG);
	} else if(startsWith("ANSWER",tag) || startsWith("Answer",tag) || startsWith("answer",tag)){
		return lowerthecase(ANSWERTAG);
	} else {
		//return ATOMTAG;
		int i=0;
		while(tag[i]!='\0' && tag[i]!='-'){
			title[i]=tag[i];
			i++;
		}
		title[i]='\0';
		return lowerthecase(title);
	} 
	return title;
}

