#include "tete.h"

void fprintLatexQUESTION(FILE *fp,ATOM* q){
	//fprintf(fp,"\\begin{%s}{%s}",q->type,q->text);
	fprintf(fp,"\\begin{questionUnit}{%s}",resizetext(q->text));
	fprintLatexCONTEXT(fp,q->context,"\t");
	fprintf(fp,"\n\t\\%s{}{",q->answers->type);
	fprintLatexATOMS(fp,q->answers,"\t\t");
	fprintf(fp,"\n\t}\n");	
	//fprintf(fp,"\\end{%s}\n",q->type);
	fprintf(fp,"\\end{questionUnit}\n");
}

void fprintLatexCONTEXT(FILE* fp, CONTEXT *c, char *indent){
	if(c){
		char indentt[2000];
		strcpy(indentt,indent);
		strcat(indentt,"\t");
		fprintf(fp,"\n%s\\%s{}{",indent,c->type);
		fprintLatexLEXIC(fp,c->lexic,indentt);
		fprintLatexATOMS(fp,c->statements,indentt);
		fprintLatexATOMS(fp,c->examples,indentt);
		fprintf(fp,"\n%s}",indent);	
	}
}

void fprintLatexLEXIC(FILE* fp, LEXIC *l, char *indent){
	if(l){
		int i;
		for(i=0;i<l->count;i++){
			fprintf(fp,"\n%s\\l%s{%s}{%s}",indent,l->list[i]->type,l->list[i]->term,resizetext(l->list[i]->definition));
		}
	}
}

void fprintLatexATOMS(FILE* fp, ATOMS *a, char *indent){
	if(a){
		char indentt[2000];
		strcpy(indentt,indent);
		strcat(indentt,"\t");
		int i;
		for(i=0;i<a->count;i++){
			fprintf(fp,"\n%s\\%s{%s}{",indent,a->list[i]->type, resizetext(a->list[i]->text));
			if(a->list[i]->context){
				fprintLatexCONTEXT(fp,a->list[i]->context,indentt);
				fprintf(fp,"\n%s",indent);
			}		
			fprintf(fp,"}");
		}
	}
}