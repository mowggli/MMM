/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                     S T R U C T U R E S    A D D E N D U M . C                               */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
#include "tete.h"
int NUMBER = 0;

char REFTAG[]="REFERENCE";
char LEXICTAG[]="LEXIC";
char SUPPORTTAG[]="SUPPORT";
char DISPORTTAG[]="DISPORT";
char OBSOLETIONTAG[]="OBSOLETION";
char INSTANTIATIONTAG[]="INSTANTIATION";
char CONTEXTTAG[]="CONTEXT";
char CONTEXTSTAG[]="CONTEXTS";
char ANSWERSTAG[]="ANSWERS";
char ANSWERTAG[]="ANSWER";
char QUESTIONTAG[]="QUESTION";
char ATOMTAG[]="ATOM";
char ATOMSTAG[]="ATOMS";
char ENTRYTAG[]="ENTRY";
char TERMTAG[]="TERM";
char DEFINITIONTAG[]="DEFINITION";
char TEXTTAG[]="TEXT";
char METATAG[]="META";
char TAGTAG[]="TAG";
char AUTHORTAG[]="AUTHOR";
char IDTAG[]="ID";
char PARENTIDTAG[]="PARENTID";
char COUNTTAG[]="COUNT";
/************************************************************************************************/
/* SIGNAL (DE-)ALLOCATIONS                                                                      */
/************************************************************************************************/
int signalAllocate(bool fail,int depth,char *tag,char *text,uintptr_t p){
	if(fail){	
		fprintf(stderr,"%d\t>>%s Malloc for %zu ~ <%s>%s</%s> failed!\n",ALLOCCOUNT,depth2indent(depth),p,tag,text,tag);	
		return -1;	
	}
	else{			
		ALLOCCOUNT++;	
		//fprintf(stdout,"%d\t>>%s %zu (depth %d) ~ <%s>%s</%s>\n",ALLOCCOUNT,depth2indent(depth),p,depth,tag,text,tag);	
		if(strcmp(tag,"ID")==0){
			fprintf(stdout,"%d\t>>%s ...MADE %s %s \n",ALLOCCOUNT,depth2indent(depth),tag, text);	
		} else if(strcmp(tag,"TAG")==0){
			fprintf(stdout," %s...\n",text);	
		}
		return 0;
	}
}
int signalFree(int depth, char *tag,char *text,uintptr_t p){
	//fprintf(stdout,"%d\t<<%s %zu ~ <%s>%s</%s>\n",ALLOCCOUNT,depth2indent(depth),p,tag,text,tag);	
	if(strcmp(tag,"ID")==0){
		fprintf(stdout,"%d\t<<%s FREED %s %s  ",ALLOCCOUNT,depth2indent(depth),tag, text);	
	} else if(strcmp(tag,"TAG")==0){
			fprintf(stdout," %s\n",text);	
		}
	ALLOCCOUNT--;
	return 0;
}

int signalAllocateFromMeta(bool fail, META*m,uintptr_t p){
	if(fail){	
		fprintf(stderr,"\t>>%s Malloc for %zu ~ OBJECT %s (%s) <--%s\n",depth2indent(m->Depth),p,m->ID,m->Tag,m->ParentID);	
		return -1;	
	}
	else{			
		//ALLOCCOUNT++;	
		fprintf(stdout,"\t>>%s %zu ~ OBJECT %s (%s) <--%s\n",depth2indent(m->Depth),p,m->ID,m->Tag,m->ParentID);		
		return 0;
	}
}
int signalFreeFromMeta(META*m,uintptr_t p){
	fprintf(stdout,"\t<<%s %zu ~ OBJECT %s (%s) <--%s\n",depth2indent(m->Depth),p,m->ID,m->Tag,m->ParentID);		
	//ALLOCCOUNT--;
	return 0;
}
/************************************************************************************************/
/* newID()                                                                                      */
/************************************************************************************************/
char *newid(){
	static char id[5000];
	snprintf(id,5000,"%d",NUMBER);
	NUMBER++;
	return id;
}
/************************************************************************************************/
/* subtag                                                                                       */
/************************************************************************************************/
char *subtag(char *tag,int n){
	static char subtag[5000];
	if(tag[strlen(tag)-1]=='S' || tag[strlen(tag)-1]=='s'){
		char copy[5000];
		strcpy(copy,tag);
		copy[strlen(tag)-1]='\0';
		snprintf(subtag,5000,"%s-%d",copy,n);
	} else {
		snprintf(subtag,5000,"%s-%d",tag,n);
	}
	return subtag;
}
/************************************************************************************************/
/* isemptycontext                                                                               */
/************************************************************************************************/
bool isemptyCONTEXT(CONTEXT *p){
	if(p){
		int i=0;
		if(p->Lexic){
			i=p->Lexic->Count;
		}
		if(p->Support){
			i=i+p->Support->Count;
		}
		if(p->Disport){
			i=i+p->Disport->Count;
		}
		if(p->Obsoletion){
			i=i+p->Obsoletion->Count;
		}
		if(p->Instantiation){
			i=i+p->Instantiation->Count;
		}
		return i==0;
	} else return true;
}