#include "tete.h"
/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                                	      M A R K U P                                           */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
#define DEFAULT_SPACED false
#define SINGLE_INDENT "\t"
#define SINGLE_INDENT_LENGTH 1
#define HLINE "__"

/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/
void fprintMarkupQUESTION(FILE *fp,ATOM* q){
	//fprintf(fp,"<%s>",q->tag);
	fprintf(fp,"%s<Statement>",depth2indent(q->depth));
	fprintf(fp,"\n%s<text>%s</text>",depth2indent(q->depth+1),resizetext(q->text));
	fprintf(fp,"\n%s<tag>%s</tag>",depth2indent(q->depth+1),q->tag);
	fprintMarkupCONTEXT(fp,q->context);
	fprintMarkupANSWERS(fp,q->answers);
	//fprintf(fp,"\n%s</%s>",depth2indent(q->depth),q->tag);
	fprintf(fp,"\n%s</Statement>",depth2indent(q->depth));
}

void fprintMarkupANSWERS(FILE* fp, ATOMS *a){
	if(a){
		fprintf(fp,"\n%s<%s>",depth2indent(a->depth),a->type);
		//fprintMarkupATOMS(fp,a);
		int i;
		for(i=0;i<a->count;i++){
			fprintMarkupATOM(fp, a->list[i]);
		}
		fprintf(fp,"\n%s</%s>",depth2indent(a->depth),a->type);
	}
}

void fprintMarkupATOMS(FILE* fp, ATOMS *a){
	if(a){
		int i;
		for(i=0;i<a->count;i++){
			fprintMarkupATOM(fp, a->list[i]);
		}
	}
}

void fprintMarkupATOM(FILE* fp, ATOM *a){
	if(a){
		//char indent[200];
		//strcpy(indent,depth2indent(a->depth));
		fprintf(fp,"\n%s<%s>",depth2indent(a->depth),a->type);
		fprintf(fp,"\n%s<text>%s</text>",depth2indent(a->depth+1),resizetext(a->text));
		fprintf(fp,"\n%s<tag>%s</tag>",depth2indent(a->depth+1),a->tag);
		if(a->context){
			fprintMarkupCONTEXT(fp,a->context);
		}		
		fprintf(fp,"\n%s</%s>",depth2indent(a->depth),a->type);
	}
}
/*
void fprintMarkupANSWER(FILE* fp, ATOM *a){
	if(a){
		//char indent[200];
		//strcpy(indent,depth2indent(a->depth));
		fprintf(fp,"\n%s<%s>",depth2indent(a->depth),a->type);
		fprintf(fp,"\n%s<text>%s</text>",depth2indent(a->depth+1),a->text);
		fprintf(fp,"\n%s<tag>%s</tag>",depth2indent(a->depth+1),a->tag);
		if(a->context){
			fprintMarkupCONTEXT(fp,a->context);
		}		
		fprintf(fp,"\n%s</%s>",depth2indent(a->depth),a->type);
	}
}*/

void fprintMarkupENTRY(FILE* fp, ENTRY *e){
		//char indent[200];
		//strcpy(indent,depth2indent(e->depth));
		fprintf(fp,"\n%s<%s>",depth2indent(e->depth),e->type);
		fprintf(fp,"\n%s<term>%s</term>",depth2indent(e->depth+1),e->term);
		fprintf(fp,"\n%s<definition>%s</definition>",depth2indent(e->depth+1),resizetext(e->definition));
		fprintf(fp,"\n%s</%s>",depth2indent(e->depth),e->type);
}

void fprintMarkupLEXIC(FILE* fp, LEXIC *l){
	if(l){
		int i;
		for(i=0;i<l->count;i++){
			fprintMarkupENTRY(fp, l->list[i]);
		}
	}
}

void fprintMarkupCONTEXT(FILE* fp, CONTEXT *c){
	if(c){
		//char indent[200];
		//strcpy(indent,depth2indent(c->depth));
		fprintf(fp,"\n%s<%s>",depth2indent(c->depth),c->type);
		fprintMarkupLEXIC(fp,c->lexic);	
		fprintMarkupATOMS(fp,c->statements);
		fprintMarkupATOMS(fp,c->examples);
		fprintf(fp,"\n%s</%s>",depth2indent(c->depth),c->type);
	}
}


