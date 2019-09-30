#include "tete.h"
/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
int allocCount=0;

// todo constant field in structure for ENTRIES and for CONTEXTS
/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/
char *depth2indent(int depth){
	static char indent[5000];
	strcpy(indent,"");
	int d=depth;
	while(d>0){
		strcat(indent,"   ");
		d--;
	}
	return indent;
}

char *depth2branch(int depth){
	static char indent[5000];
	strcpy(indent,"    |\n    |");
	int d=depth;
	while(d>0){
		strcat(indent,"___");
		d--;
	}
	return indent;
}
/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/
void printAllocate(bool fail, int depth, char *general,char *special, uintptr_t p){
	if(fail){	
		fprintf(stderr, "Malloc for %s%s failed!\n",general,special);		
	}
	else{			
		allocCount++;	
		//printf("%d\t>> %s%zu = %s %s\n",allocCount,depth2indent(depth),p,general,special);		
	}
}

void printFree(int depth, char *general,char *special,  uintptr_t p){
	//printf("%d\t<< %s%zu = %s %s\n",allocCount,depth2indent(depth),p,general,special);
	allocCount--;
}

void printInside(char *fcn, char *type, uintptr_t p){
	//printf("\nIn function %s\n",fcn);
	//printf("Received %s pointer = %zu\n",type,p);	
}
/************************************************************************************************/
/*                                C O P Y                                                       */
/************************************************************************************************/
ATOM *copyATOM(ATOM *a,int d){
	ATOM *newa=NULL;
	if(d>=0){
		newa = initATOM(a->text,	a->type,a->tag, a->depth);
		copyCONTEXT(newa,a,d-1);
		copyANSWERS(newa,a,d-1);
	}
	return newa;
}

void copyLEXIC(LEXIC *newl,LEXIC *l,int d){
//printf("I'm in copy lexic with d=%d\n",d);
	if(d>=0){
		ENTRY *e;
		int i=0;
		while(i<l->count){	
			e = l->list[i];
	//printf("copying entry %d term =%s, def=%s\n",i,e->term,e->definition);
			recordNewENTRY(newl,e->term,e->definition);
			i++;		
		}
	}
}

void copyCONTEXT(ATOM *newa,ATOM *a,int d){
	if(d>=0){
		initCONTEXT(a);
		copyLEXIC(newa->context->lexic,a->context->lexic,d-1);
		copyATOMS(newa->context->statements,a->context->statements,d-1);
		copyATOMS(newa->context->examples,a->context->examples,d-1);
	}
}

void copyATOMS(ATOMS *newa,ATOMS *a, int d){
	//if(d>=0){
		int i=0;
		while(i<a->count){
			newa->list[i] = copyATOM(a->list[i],d);
			i++;	
		}
	//}
}

void copyANSWERS(ATOM *newa,ATOM *a,int d){
	copyATOMS(newa->answers,a->answers,d);
}

/************************************************************************************************/
/* LEXIC : { 	int depth;	int count;	ENTRY* list[LISTMAX];	}    					               */
/************************************************************************************************/
void initLEXIC(CONTEXT *c){
	//printInside("initLEXIC(CONTEXT *c)", "context",(uintptr_t) c );
	c->lexic = (LEXIC*) malloc (sizeof(LEXIC)); 
	printAllocate(!(c->lexic),c->depth + 1, "", "lexic", (uintptr_t) c->lexic);
	c->lexic->depth = c->depth + 1 ;
	c->lexic->count = 0;
}

void freeLEXIC(LEXIC* l){//printf("in free lexic\n\n");
	if(l){
		while(l->count>0){	
			l->count--;		
			freeENTRY(l->list[l->count], l->count);
		}
		printFree(l->depth,"lexic","",(uintptr_t) l);
		free(l);
	}
}
/************************************************************************************************/
/* ENTRY : {	int depth;	char *term;		char *definition;	}    					                  */
/************************************************************************************************/
void recordNewENTRY(LEXIC* l,char *term,char *definition){
	//printInside("recordNewENTRY(LEXIC* l,char *term,char *definition)", "LEXIC",(uintptr_t) l );
	char temp[5000];
	// initialise entry
	snprintf(temp,5000,"entry[%d]",l->count);
	l->list[l->count] = (ENTRY *) malloc (sizeof(ENTRY));
	printAllocate(!(l->list[l->count]),l->depth + 1,"",temp,(uintptr_t) l->list[l->count]);
	// Initialise entry depth
	l->list[l->count]->depth=l->depth ;
	// Initialise entry type to "Term"
	snprintf(temp,5000,"entry[%d] type: %s",l->count,"Entry");
	l->list[l->count]->type = (char *) malloc (strlen("Entry") + 1);
	printAllocate(!(l->list[l->count]->type),l->depth + 2,"",temp,(uintptr_t) l->list[l->count]->type);
	strcpy(l->list[l->count]->type,"Entry");
	// Initialise entry term 
	snprintf(temp,5000,"entry[%d] term: %s",l->count,term);
	l->list[l->count]->term = (char *) malloc (strlen(term) + 1);
	printAllocate(!(l->list[l->count]->term),l->depth + 2,"",temp,(uintptr_t) l->list[l->count]->term);
	strcpy(l->list[l->count]->term,term);
	// Initialise entry definition
	snprintf(temp,5000,"entry[%d] def : %s",l->count,definition);
	l->list[l->count]->definition = (char *) malloc (strlen(definition) + 1);
	printAllocate(!(l->list[l->count]->definition),l->depth + 2,"",temp, (uintptr_t) l->list[l->count]->definition);
	strcpy(l->list[l->count]->definition,definition);
	// 
	l->count++ ;
}

void freeENTRY(ENTRY *e, int n){
	char temp[5000];
	// Free entry definition
	snprintf(temp,5000,"entry[%d] def : %s",n,e->definition);
	printFree(e->depth+2,"",temp, (uintptr_t) e->definition);
	free(e->definition);
	// Free entry term
	snprintf(temp,5000,"entry[%d] term: %s",n,e->term);
	printFree(e->depth+2,"",temp, (uintptr_t) e->term);
	free(e->term);
	// Free entry type
	snprintf(temp,5000,"entry[%d]->type",n);
	printFree(e->depth+2,"",temp, (uintptr_t) e->type);
	free(e->type);
	// Free Entry
	snprintf(temp,5000,"entry[%d]",n);
	printFree(e->depth+1,"",temp, (uintptr_t) e);
	free(e);
}
/************************************************************************************************/
/* ATOMS :  {	int depth;	int count;	char* type;		ATOM* list[LISTMAX];	}                    */
/************************************************************************************************/
ATOMS *initATOMS(int depth,char *type){
	//printInside("ATOMS *initATOMS(int depth,char *type)", "type",(uintptr_t) type );
	ATOMS *a  = (ATOMS *) malloc (sizeof(ATOMS)); 
	printAllocate(!a,depth , type, "", (uintptr_t) a);
	a->depth = depth ;
	a->count = 0;
	a->type = (char *) malloc (strlen(type) + 1); 
	printAllocate(!(a->type),depth +1,type,"->type", (uintptr_t) a->type);
	strcpy(a->type,type);
	return a;
}

/*void recordNewATOMalt(ATOMS *a, char *text, char *type,char *tag){
	a->list[a->count] = initATOM(text,type,tag,a->depth+1);
	a->count++;
}*/

ATOM * recordNewATOM(ATOMS *a, char *text,char *tag){//printf("in recordNewATOM\n");
	//printInside("void recordNewATOM(ATOMS *a, char *text)", "ATOMS",(uintptr_t) a);
	//char type[5000];
	//strcpy(type, a->type);
	//type[strlen(type) - 1]  = '\0'; 
	a->list[a->count] = initATOM(text,"Statement",tag,a->depth);
	a->count++;
	return a->list[a->count-1];
}

void freeATOMS(ATOMS* a){
	if(a){
		char temp[5000];
		strcpy(temp,a->type);
		while(a->count>0){	
			a->count--;		
			freeATOM(a->list[a->count]);
		}
		printFree(a->depth+1,temp,"->type",(uintptr_t) a->type);
		free(a->type);
		printFree(a->depth,temp,"",(uintptr_t) a);
		free(a);
	}
}
/************************************************************************************************/
/*  CONTEXT : {	int depth;   LEXIC *lexic;  ATOMS* statements;		ATOMS* examples;		}     */
/************************************************************************************************/
void initCONTEXT(ATOM* a){
	//printInside("void initCONTEXT(ATOM* a)", "ATOM",(uintptr_t) a);
	// Initialise context
	a->context  =  (CONTEXT *) malloc (sizeof(CONTEXT)); 
	printAllocate((!a->context),a->depth + 1,a->type,"->context", (uintptr_t) a->context);
	// Initialise context depth
	(a->context)->depth = a->depth + 1;
	// Initialise context type
	(a->context)->type = (char *) malloc (strlen("Context") + 1);
	printAllocate(!((a->context)->type),(a->context)->depth,a->type,"->context->type",(uintptr_t) (a->context)->type);
	strcpy((a->context)->type,"Context");
	// lexic
	initLEXIC(a->context);
	// statements
	a->context->statements = initATOMS(a->depth + 2,"Statements");
	// examples
	a->context->examples = initATOMS(a->depth + 2,"Examples");	
}

void freeCONTEXT(CONTEXT* c,char* type){//printf("In freeCONTEXT\n");
	if(c){
		char temp[5000];
		// Free examples, statements and lexic
		freeATOMS(c->examples);//printf("Freed examples from context\n");
		freeATOMS(c->statements);//printf("Freed statements from context\n");
		freeLEXIC(c->lexic);//printf("Freed lexic from context\n");
		// Free type
		snprintf(temp,5000,"%s->context->type",type);
		printFree(c->depth,"",temp, (uintptr_t) c->type);
		free(c->type);
		// Free context
		printFree(c->depth,type, "->context", (uintptr_t) c);
		free(c); 
	}
}
/************************************************************************************************/
/*  ATOM : {	int depth;   char* text;	char * type;   CONTEXT* context;	ATOMS* answers;	}  */
/************************************************************************************************/
ATOM* initATOM(char* text,	char * type,char *tag, int depth){
	//printInside("ATOM* initATOM(char* text,	char * type, int depth)", "type",(uintptr_t) type);
	char temp[5000];
	// atom
	ATOM* a = (ATOM *) malloc (sizeof(ATOM));
	printAllocate((!a),depth,type, text, (uintptr_t) a);
	// text
	a->text = (char *) malloc (strlen(text) + 1); 
	printAllocate((!a->text),depth+1,type,"->text", (uintptr_t) a->text);
	strcpy(a->text,text);
	// type
	a->type = (char *) malloc (strlen(type) + 1); 
	printAllocate((!a->type),depth+1,type,"->type", (uintptr_t) a->type);
	strcpy(a->type,"Statement");
	// tag
	a->tag = (char *) malloc (strlen(tag) + 1); 
	printAllocate((!a->tag),depth+1,type,"->tag", (uintptr_t) a->tag);
	strcpy(a->tag,tag);
	
	a->depth=depth;
	a->context=NULL;
	a->answers=NULL;
	return a;
}
int freeATOM(ATOM *a){//printf("In freeATOM\n");
	printInside("freeATOM(ATOM *a)", "ATOM",(uintptr_t) a);
	
	char type[5000];
	char text[5000];
	strcpy(type,a->type);
	strcpy(text,a->text); 
	
	freeATOMS(a->answers);
	freeCONTEXT(a->context,a->type);
	// tag
	printFree(a->depth+1,type,"->tag", (uintptr_t) a->tag);	
	free(a->tag);
	// type
	printFree(a->depth+1,type,"->type", (uintptr_t) a->type);	
	free(a->type);
	// text
	printFree(a->depth+1,type,"->text",  (uintptr_t) a->text);		
	free(a->text);
	// atom
	printFree(a->depth,type, text, (uintptr_t) a);
	free(a);
	return allocCount;
}