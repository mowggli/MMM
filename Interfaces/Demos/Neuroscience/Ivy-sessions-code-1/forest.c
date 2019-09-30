#include "tete.h"
/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                                	      F O R E S T                                           */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
#define DEFAULT_SPACED false
#define SINGLE_INDENT "  "
#define SINGLE_INDENT_LENGTH 1
#define HLINE "___"

/************************************************************************************************/
/*                                                                                              */
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
/*                                                                                              */
/************************************************************************************************/
void fprintForestQUESTION(FILE *fp,ATOM* q){
	printInside("fprintForestQUESTION(FILE *fp,ATOM* q)", "ATOM",(uintptr_t) q );
	fprintf(fp,"\n");
	char indent[2000];
	strcpy(indent," ");
	char childindent[2000];
	strcpy(childindent,indentchild(false,indent));
	fprintf(fp,"%sQuestion: %s",indent,resizetext(q->text));
	// Print context who's children are nieces
	fprintForestCONTEXT(fp,q->context,true,true,childindent);
	// Print answers who are un-uncled
	fprintForestANSWERS(fp,q->answers,false,childindent);
	fprintf(fp,"\n");
}

void fprintForestQUESTIONperDepth(FILE *fp,ATOM* q,int d){
	fprintf(fp,"\n");
	char indent[2000];
	strcpy(indent," ");
	char childindent[2000];
	strcpy(childindent,indentchild(false,indent));
	fprintf(fp,"%sQuestion: %s",indent,resizetext(q->text));
	// Print context who's children are nieces
	fprintForestCONTEXTperDepth(fp,q->context,true,true,childindent,d-1);
	// Print answers who are un-uncled
	fprintForestANSWERSperDepth(fp,q->answers,false,childindent,d-1);
	fprintf(fp,"\n");
}

void fprintForestANSWERS(FILE* fp, ATOMS *a, bool neice, char *indent){
	printInside("fprintForestANSWERS(FILE* fp, ATOMS *a, bool neice, char *indent)", "ATOMS",(uintptr_t) a );
	char childindent[2000];
	strcpy(childindent,indentchild(false,indent));
	if(a){
		fprintf(fp,"%s%s",before(indent,true),a->type);
		fprintForestATOMS(fp,a,true,false,true,childindent);
	}
}

void fprintForestANSWERSperDepth(FILE* fp, ATOMS *a, bool neice, char *indent,int d){
	if(d>=0){
		char childindent[2000];
		strcpy(childindent,indentchild(false,indent));
		if(a){
			fprintf(fp,"%s%s",before(indent,true),a->type);
			fprintForestATOMSperDepth(fp,a,true,false,true,childindent,d);
		}
	}
}
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
	} else if(strcmp(tag,"Question")==0){
		snprintf(title,5000,"Question");
	} else if(strcmp(tag,"Answer")==0){
		snprintf(title,5000,"Answer");
	} else {
		snprintf(title,5000,"Statement");
	} 
	return title;
}

void fprintStatementLine(FILE *fp,ATOM *a, bool spaced, char *indent){
	//fprintf(fp,"%s%s: (%s Tag) %s",before(indent,spaced),a->type, a->tag, a->text); 
	//fprintf(fp,"%s%s: %s",before(indent,spaced),a->type, a->text);
	fprintf(fp,"%s%s: %s",before(indent,spaced),tagtotitle(a->tag), resizetext(a->text));	
}

void fprintForestATOMS(FILE* fp, ATOMS *a, bool neice, bool bigbrother, bool spaced, char *indent){
	printInside("fprintForestATOMS(FILE* fp, ATOMS *a, bool neice, bool bigbrother, bool spaced, char *indent)", "ATOMS",(uintptr_t) a );
	if(a&&a->count>0){
		int i;
		char childindent[2000];
		strcpy(childindent,indentchild(neice,indent));
		for(i=0;i<a->count-1;i++){
			fprintStatementLine(fp,a->list[i],spaced,indent);
			//fprintf(fp,"%s%s: (%s Tag) %s",before(indent,spaced),a->list[i]->type, a->list[i]->tag, a->list[i]->text); 
			//fprintf(fp,"%s%s: %s",before(indent,spaced),a->list[i]->type, a->list[i]->text);
			if(a->list[i]->context){
				fprintForestCONTEXT(fp,a->list[i]->context,true,false,childindent);
			}		
		}
		i=a->count-1;
		strcpy(childindent,indentchild(bigbrother,indent));
		fprintStatementLine(fp,a->list[i],spaced,indent);
			if(a->list[i]->context){
				fprintForestCONTEXT(fp,a->list[i]->context,true,false,childindent);
			}		
	}
}

void fprintForestATOMSperDepth(FILE* fp, ATOMS *a, bool neice, bool bigbrother, bool spaced, char *indent,int d){
	if(a&&a->count>0 && d>=0){
		int i;
		char childindent[2000];
		strcpy(childindent,indentchild(neice,indent));
		for(i=0;i<a->count-1;i++){
			fprintStatementLine(fp,a->list[i],spaced,indent);
			//fprintf(fp,"%s%s: (%s Tag) %s",before(indent,spaced),a->list[i]->type, a->list[i]->tag, a->list[i]->text); 
			//fprintf(fp,"%s%s: %s",before(indent,spaced),a->list[i]->type, a->list[i]->text);
			if(a->list[i]->context){
				fprintForestCONTEXTperDepth(fp,a->list[i]->context,true,false,childindent,d-1);
			}		
		}
		i=a->count-1;
		strcpy(childindent,indentchild(bigbrother,indent));
		fprintStatementLine(fp,a->list[i],spaced,indent);
			if(a->list[i]->context){
				fprintForestCONTEXTperDepth(fp,a->list[i]->context,true,false,childindent,d-1);
			}		
	}
}

void fprintForestCONTEXT(FILE* fp, CONTEXT *c, bool neice, bool bigbrother, char *indent){
	printInside("fprintForestCONTEXT(FILE* fp, CONTEXT *c, bool neice, bool bigbrother, char *indent)", "CONTEXT",(uintptr_t) c );
	if(c && c->statements->count+c->lexic->count+c->examples->count>0){
		//printf("\nbigbrother=%d  neice=%d\n",bigbrother,neice);
		fprintf(fp,"%s%s",before(indent,DEFAULT_SPACED),c->type);
		char childindent[2000];
		strcpy(childindent,indentchild(bigbrother,indent));
		fprintForestLEXIC(fp,c->lexic,childindent);		//printf("\nin print context\n");
		fprintForestATOMS(fp,c->statements,neice,(c->examples->count>0),DEFAULT_SPACED,childindent);
		fprintForestATOMS(fp,c->examples,neice,false,DEFAULT_SPACED,childindent);
	}
}

void fprintForestCONTEXTperDepth(FILE* fp, CONTEXT *c, bool neice, bool bigbrother, char *indent,int d){
	if(c && d>=0 && c->statements->count+c->lexic->count+c->examples->count>0){
		//printf("\nbigbrother=%d  neice=%d\n",bigbrother,neice);
		fprintf(fp,"%s%s",before(indent,DEFAULT_SPACED),c->type);
		char childindent[2000];
		strcpy(childindent,indentchild(bigbrother,indent));
		fprintForestLEXICperDepth(fp,c->lexic,childindent,d);		//printf("\nin print context\n");
		fprintForestATOMSperDepth(fp,c->statements,neice,(c->examples->count>0),DEFAULT_SPACED,childindent,d);
		fprintForestATOMSperDepth(fp,c->examples,neice,false,DEFAULT_SPACED,childindent,d);
	}
}

void fprintForestLEXIC(FILE* fp, LEXIC *l,char *indent){
	printInside("fprintForestLEXIC(FILE* fp, LEXIC *l,char *indent)", "LEXIC",(uintptr_t) l );
	if(l){
		int i;
		for(i=0;i<l->count;i++){
			fprintf(fp,"%s%s: %s:=%s",before(indent,DEFAULT_SPACED),l->list[i]->type,l->list[i]->term,resizetext(l->list[i]->definition));
		}
	}
}

void fprintForestLEXICperDepth(FILE* fp, LEXIC *l,char *indent,int d){
	if(d>=0&& l){
		int i;
		for(i=0;i<l->count;i++){
			fprintf(fp,"%s%s: %s:=%s",before(indent,DEFAULT_SPACED),l->list[i]->type,l->list[i]->term,resizetext(l->list[i]->definition));
		}
	}
}

