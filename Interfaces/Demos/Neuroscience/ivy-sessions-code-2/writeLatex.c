/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                               W R I T E   L A T E X . C                                      */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
#include "tete.h"

char LEXICLatexKEYWORD[]="LEXIC";
char SUPPORTLatexKEYWORD[]="SUPPORT";
char DISPORTLatexKEYWORD[]="DISPORT";
char OBSOLETIONLatexKEYWORD[]="OBSOLETION";
char INSTANTIATIONLatexKEYWORD[]="INSTANTIATION";
char CONTEXTLatexKEYWORD[]="Context";
char CONTEXTSLatexKEYWORD[]="CONTEXTS";
char ANSWERSLatexKEYWORD[]="Answers";
char ANSWERLatexKEYWORD[]="ANSWER";
char QUESTIONLatexKEYWORD[]="questionUnit";
char ATOMLatexKEYWORD[]="Statement";
char ATOMSLatexKEYWORD[]="ATOMS";
char ENTRYLatexKEYWORD[]="Entry";
char TERMLatexKEYWORD[]="TERM";
char DEFINITIONLatexKEYWORD[]="DEFINITION";
char TEXTLatexKEYWORD[]="TEXT";
char METALatexKEYWORD[]="META";
char TAGLatexKEYWORD[]="TAG";
char AUTHORLatexKEYWORD[]="AUTHOR";
char IDLatexKEYWORD[]="ID";
char PARENTIDLatexKEYWORD[]="PARENTID";
char COUNTLatexKEYWORD[]="COUNT";
/************************************************************************************************/
/* QUESTION                                                                                     */
/************************************************************************************************/
/************************************************************************************************/
/* META                                                                                         */
/************************************************************************************************/
int writeLatexMETA(FILE *fp,META *p,char *indent){
	return 0;
}
/************************************************************************************************/
/* QUESTION                                                                                     */
/************************************************************************************************/
int writeLatexQUESTION(FILE *fp,ATOM *p,char *indent){
	if(p){
		char indentt[2000];
		strcpy(indentt,indent);
		strcat(indentt,"\t");
		fprintf(fp,"\\begin{%s}{%s}",QUESTIONLatexKEYWORD,resizetext(p->Text,"...?"));
		writeLatexCONTEXTS(fp,p->Contexts,indentt);
		fprintf(fp,"\n\t\\%s{}{",ANSWERSLatexKEYWORD);
		strcat(indentt,"\t");
		writeLatexATOMS(fp,p->Answers,indentt);
		fprintf(fp,"\n\t}\n");
		fprintf(fp,"\\end{%s}\n",QUESTIONLatexKEYWORD);
	}
	return 0;
}
/************************************************************************************************/
/* ATOM                                                                                         */
/************************************************************************************************/
int writeLatexATOM(FILE *fp,ATOM *p,char *indent){
	if(p){
		char indentt[2000];
		strcpy(indentt,indent);
		strcat(indentt,"\t");
		fprintf(fp,"\n%s\\%s{%s}{",indent,ATOMLatexKEYWORD, resizetext(p->Text,"..."));
		writeLatexCONTEXTS(fp,p->Contexts,indentt);
		writeLatexATOMS(fp,p->Answers,indentt);
		fprintf(fp,"}");		
	}
	return 0;
}
/************************************************************************************************/
/* ATOMS                                                                                        */
/************************************************************************************************/
int writeLatexATOMS(FILE *fp,ATOMS *p,char *indent){
	if(p){
		int i;
		for(i=0;i<p->Count;i++){
			writeLatexATOM(fp,p->List[i],indent);
		}
	}
	return 0;		
}
/************************************************************************************************/
/* CONTEXT                                                                                      */
/************************************************************************************************/
int writeLatexCONTEXT(FILE *fp,CONTEXT *p,char *indent){
	if(p && !isemptyCONTEXT(p)){
		char indentt[2000];
		strcpy(indentt,indent);
		strcat(indentt,"\t");
		fprintf(fp,"\n%s\\%s{}{",indent,CONTEXTLatexKEYWORD);
		writeLatexLEXIC(fp,p->Lexic,indentt);
		writeLatexATOMS(fp,p->Support,      indentt);
		writeLatexATOMS(fp,p->Disport,      indentt);
		writeLatexATOMS(fp,p->Obsoletion,   indentt);
		writeLatexATOMS(fp,p->Instantiation,indentt);
		fprintf(fp,"\n%s}",indent);	
	}
	return 0;	
}
/************************************************************************************************/
/* CONTEXTS                                                                                     */
/************************************************************************************************/
int writeLatexCONTEXTS(FILE *fp,CONTEXTS *p,char *indent){
	if(p){
		int i;
		for(i=0;i<p->Count;i++){
			writeLatexCONTEXT(fp,p->List[i],indent);
		}
	}
	return 0;		
}
/************************************************************************************************/
/* ENTRY                                                                                        */
/************************************************************************************************/
int writeLatexENTRY(FILE *fp,ENTRY *p,char *indent){
	fprintf(fp,"\n%s\\l%s{%s}{%s}",indent,ENTRYLatexKEYWORD,p->Term,resizetext(p->Definition,"..."));
	return 0;
}
/************************************************************************************************/
/* LEXIC                                                                                        */
/************************************************************************************************/
int writeLatexLEXIC(FILE *fp,LEXIC *p,char *indent){
	if(p){
		int i;
		for(i=0;i<p->Count;i++){
			writeLatexENTRY(fp,p->List[i],indent);
		}
	}
	return 0;		
}
