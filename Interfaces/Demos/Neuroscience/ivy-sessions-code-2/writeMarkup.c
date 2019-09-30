/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                             W R I T E   M A R K U P . C                                      */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
#include "tete.h"
bool MARKMETA=true;
bool MARKCONTEXTS=true;
bool MARKLEXIC=true;
bool MARKANSWERS=true;
bool MARKATOMS=true;
bool MARKATOM=true;
bool MARKCONTEXT=true;
bool MARKENTRY=true;
/************************************************************************************************/
/* QUESTION                                                                                     */
/************************************************************************************************/
/************************************************************************************************/
/* META                                                                                         */
/************************************************************************************************/
int writeMarkupMETA(FILE *fp,META *p,bool entitle){
	if(entitle){
	fprintf(fp,"\n%s<%s>",		           depth2indent(p->Depth+1),METATAG);
	fprintf(fp,"\n%s<%s>%s</%s>",          depth2indent(p->Depth+2),TAGTAG,p->Tag,TAGTAG);
	fprintf(fp,"\n%s<%s>%s</%s>",    depth2indent(p->Depth+2),AUTHORTAG,p->Author,AUTHORTAG);
	fprintf(fp,"\n%s<%s>%s</%s>",            depth2indent(p->Depth+2),IDTAG,p->ID,IDTAG);
	fprintf(fp,"\n%s<%s>%s</%s>",depth2indent(p->Depth+2),PARENTIDTAG,p->ParentID,PARENTIDTAG);
	fprintf(fp,"\n%s</%s>",	depth2indent(p->Depth+1),METATAG);
	}
	return 0;
}
/************************************************************************************************/
/* QUESTION                                                                                     */
/************************************************************************************************/
int writeMarkupQUESTION(FILE *fp,ATOM *p,bool entitle){
	if(p){
		if(entitle){fprintf(fp,"\n%s<%s>",                 depth2indent(p->Meta->Depth),ATOMTAG);}
		writeMarkupMETA(fp,p->Meta,MARKMETA);
		fprintf(fp,"\n%s<%s>%s</%s>",        depth2indent(p->Meta->Depth+1),TEXTTAG,resizetext(p->Text,"...?"),TEXTTAG);
		writeMarkupCONTEXTS(fp,p->Contexts,MARKCONTEXTS);
		writeMarkupATOMS(   fp,p->Answers, MARKANSWERS);
		if(entitle){fprintf(fp,"\n%s</%s>",            depth2indent(p->Meta->Depth),ATOMTAG);}
	}
	return 0;
}
/************************************************************************************************/
/* ATOM                                                                                         */
/************************************************************************************************/
int writeMarkupATOM(FILE *fp,ATOM *p,bool entitle){
	if(p){
		if(entitle){fprintf(fp,"\n%s<%s>",                 depth2indent(p->Meta->Depth),ATOMTAG);}
		writeMarkupMETA(fp,p->Meta,MARKMETA);
		fprintf(fp,"\n%s<%s>%s</%s>",        depth2indent(p->Meta->Depth+1),TEXTTAG,resizetext(p->Text,"..."),TEXTTAG);
		writeMarkupCONTEXTS(fp,p->Contexts,MARKCONTEXTS);
		writeMarkupATOMS(   fp,p->Answers, MARKATOMS);
		if(entitle){fprintf(fp,"\n%s</%s>",            depth2indent(p->Meta->Depth),ATOMTAG);}
	}
	return 0;
}
/************************************************************************************************/
/* ATOMS                                                                                        */
/************************************************************************************************/
int writeMarkupATOMS(FILE *fp,ATOMS *p,bool entitle){
	if(p){
		int i;
		if(entitle){fprintf(fp,"\n%s<%s>", depth2indent(p->Meta->Depth),ATOMSTAG);}
		writeMarkupMETA(fp,p->Meta,MARKMETA);
		fprintf(fp,"\n%s<%s>%d</%s>",depth2indent(p->Meta->Depth+1),COUNTTAG,p->Count,COUNTTAG);
		for(i=0;i<p->Count;i++){
			writeMarkupATOM(fp,p->List[i],MARKATOM);
		}
		if(entitle){fprintf(fp,"\n%s</%s>",depth2indent(p->Meta->Depth),ATOMSTAG);}
	}
	return 0;		
}
/************************************************************************************************/
/* CONTEXT                                                                                      */
/************************************************************************************************/
int writeMarkupCONTEXT(FILE *fp,CONTEXT *p,bool entitle){
	if(p){
		if(entitle){fprintf(fp,"\n%s<%s>", depth2indent(p->Meta->Depth),CONTEXTTAG);}
		writeMarkupMETA(fp,p->Meta,MARKMETA);
		writeMarkupLEXIC(fp,p->Lexic,			 MARKLEXIC);	
		writeMarkupATOMS(fp,p->Support,      MARKATOMS);
		writeMarkupATOMS(fp,p->Disport,      MARKATOMS);
		writeMarkupATOMS(fp,p->Obsoletion,   MARKATOMS);
		writeMarkupATOMS(fp,p->Instantiation,MARKATOMS);
		if(entitle){fprintf(fp,"\n%s</%s>",depth2indent(p->Meta->Depth),CONTEXTTAG);}
	}
	return 0;	
}
/************************************************************************************************/
/* CONTEXTS                                                                                     */
/************************************************************************************************/
int writeMarkupCONTEXTS(FILE *fp,CONTEXTS *p,bool entitle){
	if(p){
		int i;
		if(entitle){fprintf(fp,"\n%s<%s>", depth2indent(p->Meta->Depth),CONTEXTSTAG);}
		writeMarkupMETA(fp,p->Meta,MARKMETA);
		fprintf(fp,"\n%s<%s>%d</%s>",depth2indent(p->Meta->Depth+1),COUNTTAG,p->Count,COUNTTAG);
		for(i=0;i<p->Count;i++){
			writeMarkupCONTEXT(fp,p->List[i],MARKCONTEXT);
		}
		if(entitle){fprintf(fp,"\n%s</%s>",depth2indent(p->Meta->Depth),CONTEXTSTAG);}
	}
	return 0;		
}

/************************************************************************************************/
/* ENTRY                                                                                        */
/************************************************************************************************/
int writeMarkupENTRY(FILE *fp,ENTRY *p,bool entitle){
	if(entitle){fprintf(fp,"\n%s<%s>",                    depth2indent(p->Meta->Depth),ENTRYTAG);}
	writeMarkupMETA(fp,p->Meta,MARKMETA);
	fprintf(fp,"\n%s<%s>%s</%s>",            depth2indent(p->Meta->Depth+1),TERMTAG,p->Term,TERMTAG);
	if(strlen(p->Definition)>0){
		fprintf(fp,"\n%s<%s>%s</%s>",depth2indent(p->Meta->Depth+1),DEFINITIONTAG,resizetext(p->Definition,"..."),DEFINITIONTAG);
	} else{
		fprintf(fp,"\n%s<%s>?</%s>",depth2indent(p->Meta->Depth+1),DEFINITIONTAG,DEFINITIONTAG);
	}
	if(entitle){fprintf(fp,"\n%s</%s>",                   depth2indent(p->Meta->Depth),ENTRYTAG);}
	return 0;
}
/************************************************************************************************/
/* LEXIC                                                                                        */
/************************************************************************************************/
int writeMarkupLEXIC(FILE *fp,LEXIC *p,bool entitle){
	if(p){
		int i;
		if(entitle){fprintf(fp,"\n%s<%s>", depth2indent(p->Meta->Depth),LEXICTAG);}
		writeMarkupMETA(fp,p->Meta,MARKMETA);
		fprintf(fp,"\n%s<%s>%d</%s>",depth2indent(p->Meta->Depth+1),COUNTTAG,p->Count,COUNTTAG);
		for(i=0;i<p->Count;i++){
			writeMarkupENTRY(fp,p->List[i],MARKENTRY);
		}
		if(entitle){fprintf(fp,"\n%s</%s>",depth2indent(p->Meta->Depth),LEXICTAG);}
	}
	return 0;		
}
