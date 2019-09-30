/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                           W R I T E    F O R E S T . C                                       */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
#include "tete.h"
#define DEFAULT_SPACED false

/************************************************************************************************/
/* META                                                                                         */
/************************************************************************************************/
int writeForestMETA(FILE *fp,int DEPTH,META *p,char *indent){
	return 0;
}
/************************************************************************************************/
/* QUESTION                                                                                     */
/************************************************************************************************/
//writeForestQUESTION(FILE *fp,int DEPTH,ATOM *p," ")
// DEPTH = max depth
int writeForestQUESTION(FILE *fp,int DEPTH,ATOM *p,char *indent){
	if(p&& DEPTH>=p->Meta->Depth){
		char childindent[2000];
		strcpy(childindent,indentchild(false,indent)); 
		/* Self */
		fprintf(fp,"%s%s: %s",indent,lowerthecase(p->Meta->Tag),resizetext(p->Text,"..."));
		/* Context who's children are nieces */
		writeForestCONTEXTS(fp,DEPTH,p->Contexts,childindent,true,true);
		/* Answers who are un-uncled */
		writeForestANSWERS(fp,DEPTH,p->Answers,childindent,false);
		/* */
		fprintf(fp,"\n");
	}
	return 0;
}
/************************************************************************************************/
/* ANSWERS                                                                                      */
/************************************************************************************************/
int writeForestANSWERS(FILE* fp,int DEPTH,ATOMS *p,char *indent,bool neice){
	if(p && DEPTH>=p->Meta->Depth){
		char childindent[2000];
		strcpy(childindent,indentchild(false,indent));
		fprintf(fp,"%s%s",before(indent,true),lowerthecase(p->Meta->Tag));
		writeForestATOMS(fp,DEPTH,p,childindent,true,false,true);
	}	
	return 0;
}
/************************************************************************************************/
/* ATOM     ?                                                                                   */
/************************************************************************************************/
int writeForestATOM(FILE *fp,int DEPTH,ATOM *p,char *indent){
	if(p && DEPTH>=p->Meta->Depth){
		char childindent[2000];
		strcpy(childindent,indentchild(false,indent));
		/* Self */
		fprintf(fp,"\n%s%s: %s",indent,lowerthecase(p->Meta->Tag),resizetext(p->Text,"..."));
		/* Context who's children are not nieces */
		writeForestCONTEXTS(fp,DEPTH,p->Contexts,childindent,false,true);
	}
	return 0;
}

void writeForestSTATEMENTLINE(FILE *fp,ATOM *p,char *indent,bool spaced){
	//fprintf(fp,"%s%s: (%s Tag) %s",before(indent,spaced),a->type, a->tag, a->text); 
	//fprintf(fp,"%s%s: %s",before(indent,spaced),a->type, a->text);
	fprintf(fp,"%s%s: %s",before(indent,spaced),tagtotitle(lowerthecase(p->Meta->Tag)),resizetext(p->Text,"..."));	
}
/************************************************************************************************/
/* ATOMS                                                                                        */
/************************************************************************************************/
int writeForestATOMS(FILE *fp,int DEPTH,ATOMS *p,char *indent,bool neice,bool bigbrother,bool spaced){
	if(p && DEPTH>=p->Meta->Depth && p->Count>0){
		int i;
		char childindent[2000];
		strcpy(childindent,indentchild(neice,indent));
		/* */
		for(i=0;i<p->Count-1;i++){	//printf("\np->Count=%d i=%d\n",p->Count,i);printf("ptr=%zu, ID=%s\n",(uintptr_t) p->List[i],p->List[i]->Meta->ID); printf("item=%s\n",p->List[i]->Text);

			writeForestSTATEMENTLINE(fp,p->List[i],indent,spaced);
			//if(p->List[i]->Contexts){
				writeForestCONTEXTS(fp,DEPTH,p->List[i]->Contexts,childindent,true,false);
			//}		
		}
		i=p->Count-1;
	//printf("\nLASTlty, p->Count=%d i=%d\n",p->Count,i);printf("ptr=%zu\n",(uintptr_t) p->List[i]); printf("item=%s\n",p->List[i]->Text);
		strcpy(childindent,indentchild(bigbrother,indent));
		writeForestSTATEMENTLINE(fp,p->List[i],indent,spaced);
		writeForestCONTEXTS(fp,DEPTH,p->List[i]->Contexts,childindent,true,false);
	}
	return 0;		
}
/************************************************************************************************/
/* CONTEXT                                                                                      */
/************************************************************************************************/
int writeForestCONTEXT(FILE *fp,int DEPTH,CONTEXT *p,char *indent,bool neice,bool bigbrother){
	if(p && DEPTH>=p->Meta->Depth && !isemptyCONTEXT(p)){
		char childindent[2000];
		strcpy(childindent,indentchild(bigbrother,indent)); 
		/* Self */
		fprintf(fp,"%s%s",before(indent,DEFAULT_SPACED),lowerthecase(CONTEXTTAG));
		/* */
		writeForestLEXIC(fp,DEPTH,p->Lexic,childindent);
		writeForestATOMS(fp,DEPTH,p->Support,childindent,neice,(p->Disport && p->Obsoletion && p->Instantiation),DEFAULT_SPACED);
		writeForestATOMS(fp,DEPTH,p->Disport,childindent,neice,(p->Obsoletion && p->Instantiation),DEFAULT_SPACED);
		writeForestATOMS(fp,DEPTH,p->Obsoletion,childindent,neice,(p->Instantiation),DEFAULT_SPACED);
		writeForestATOMS(fp,DEPTH,p->Instantiation,childindent,neice,false,DEFAULT_SPACED);
	}
	return 0;	
}
/************************************************************************************************/
/* CONTEXTS                                                                                     */
/************************************************************************************************/
int writeForestCONTEXTS(FILE *fp,int DEPTH,CONTEXTS *p,char *indent,bool neice,bool bigbrother){
	if(p && DEPTH>=p->Meta->Depth){
		int i;
		for(i=0;i<p->Count;i++){
			writeForestCONTEXT(fp,DEPTH,p->List[i],indent,neice,bigbrother);
		}
	}
	return 0;		
}
/************************************************************************************************/
/* ENTRY                                                                                        */
/************************************************************************************************/
int writeForestENTRY(FILE *fp,int DEPTH,ENTRY *p,char *indent){
	if(p && DEPTH>=p->Meta->Depth){
		fprintf(fp,"%s%s: %s:=%s",indent,lowerthecase(ENTRYTAG),p->Term,resizetext(p->Definition,"..."));
	}
	return 0;
}
/************************************************************************************************/
/* LEXIC                                                                                        */
/************************************************************************************************/
int writeForestLEXIC(FILE *fp,int DEPTH,LEXIC *p,char *indent){
	if(p && DEPTH>=p->Meta->Depth){
		int i;
		for(i=0;i<p->Count;i++){
			writeForestENTRY(fp,DEPTH,p->List[i],before(indent,DEFAULT_SPACED));
		}
	}
	return 0;		
}
