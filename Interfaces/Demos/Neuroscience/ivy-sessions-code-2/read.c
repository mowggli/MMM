/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                     R E A D . C                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
#include "tete.h"
char CURRENTTAG[5000];
bool CURRENTTAGISOPEN=false;
char CURRENTTEXT[5000];
/************************************************************************************************/
/*                                                                                      */
/************************************************************************************************/
char *readDATA(FILE *fp,char *tag){
	/* Open tag */
	readUntilNextTag(fp);
	if(strcmp(CURRENTTAG,tag)!=0 || !CURRENTTAGISOPEN){
		fprintf(stderr,"Problem: read %stag %s, when I was expecting %stag %s!\n",(!CURRENTTAGISOPEN)? "closing ":"",CURRENTTAG,(!CURRENTTAGISOPEN)? "opening ":"",tag);
	}
	/* Close tag */
	readUntilNextTag(fp);
	if(strcmp(CURRENTTAG,tag)!=0 || CURRENTTAGISOPEN){
		fprintf(stderr,"Problem: read %stag %s, when I was expecting %stag %s!\n",(CURRENTTAGISOPEN)? "opening ":"",CURRENTTAG,(CURRENTTAGISOPEN)? "closing ":"",tag);
	}
	return CURRENTTEXT;
}
/************************************************************************************************/
/* META                                                                                         */
/************************************************************************************************/
META *readMETA(FILE *fp,int depth){
	/* Open tag */
	readUntilNextTag(fp);
	if(!CURRENTTAGISOPEN || strcmp(CURRENTTAG,METATAG)!=0){
		fprintf(stderr,"Problem: read %stag %s, when I was expecting %stag %s!\n",(!CURRENTTAGISOPEN)? "closing ":"",CURRENTTAG,(!CURRENTTAGISOPEN)? "opening ":"",METATAG);
		return NULL;
	} 
	char tag[5000];		strcpy(tag,			readDATA(fp,TAGTAG));
	char author[5000];	strcpy(author,		readDATA(fp,AUTHORTAG));
	char id[5000];			strcpy(id,			readDATA(fp,IDTAG)); 
	char parentid[5000];	strcpy(parentid,	readDATA(fp,PARENTIDTAG));
	META *p= initMETA(tag,author,depth,id,parentid);
	/* Close tag */
	readUntilNextTag(fp);
	if(strcmp(CURRENTTAG,METATAG)!=0 || CURRENTTAGISOPEN){
		fprintf(stderr,"Problem: read %stag %s, when I was expecting %stag %s!\n",(CURRENTTAGISOPEN)? "opening ":"",CURRENTTAG,(CURRENTTAGISOPEN)? "closing ":"",METATAG);
	}
	return p;
}
/************************************************************************************************/
/* QUESTION                                                                                     */
/************************************************************************************************/
/************************************************************************************************/
/* ATOM                                                                                         */
/************************************************************************************************/
ATOM *readATOM(FILE *fp,int depth){
	META *m=readMETA(fp,depth);
	char text[5000];		strcpy(text,			readDATA(fp,TEXTTAG));
	ATOM *p = initATOM(m,text,NULL,NULL);
	/* Read next bit */
	readUntilNextTag(fp);
	/* Contexts */
	if(CURRENTTAGISOPEN && strcmp(CURRENTTAG,CONTEXTSTAG)==0){
		p->Contexts = readCONTEXTS(fp,ATOMtoCONTEXTSdepth(depth));
		readUntilNextTag(fp);
	}
	/* Answers */
	if(CURRENTTAGISOPEN && strcmp(CURRENTTAG,ATOMSTAG)==0){
		p->Answers = readATOMS(fp,QUESTIONtoANSWERSdepth(depth));
		readUntilNextTag(fp);		
	} 
	/* Closing tag */
	if(CURRENTTAGISOPEN || strcmp(CURRENTTAG,ATOMTAG)!=0 ){
		fprintf(stderr,"Problem: read %stag %s, when I was expecting %stag %s!\n",(CURRENTTAGISOPEN)? "opening ":"",CURRENTTAG,(CURRENTTAGISOPEN)? "closing ":"",ATOMTAG);
	} 
	return p;	
}

/************************************************************************************************/
/* ATOMS                                                                                        */
/************************************************************************************************/
ATOMS *readATOMS(FILE *fp,int depth){
	ATOMS *p=initATOMS(readMETA(fp,depth));
	int i=0,count=atoi(readDATA(fp,COUNTTAG));
	/* Read next bit */
	readUntilNextTag(fp);
	while(i<count && CURRENTTAGISOPEN && strcmp(CURRENTTAG,ATOMTAG)==0){
		p->List[i] = readATOM(fp,ATOMStoATOMdepth(depth));
		p->Count++;
		readUntilNextTag(fp);
		i++;
	}
	/* Closing tag */
	if(CURRENTTAGISOPEN || strcmp(CURRENTTAG,ATOMSTAG)!=0 ){
		fprintf(stderr,"Problem: read %stag %s, when I was expecting %stag %s!\n",(CURRENTTAGISOPEN)? "opening ":"",CURRENTTAG,(CURRENTTAGISOPEN)? "closing ":"",ATOMSTAG);
	} 
	return p;
}
/************************************************************************************************/
/* CONTEXT                                                                                      */
/************************************************************************************************/
CONTEXT *readCONTEXT(FILE *fp,int depth){
	CONTEXT *p = initCONTEXT(readMETA(fp,depth),NULL,NULL,NULL,NULL,NULL);
	/* Read next bit */
	readUntilNextTag(fp);
	/* Lexic */
	if(CURRENTTAGISOPEN && strcmp(CURRENTTAG,LEXICTAG)==0){
		p->Lexic = readLEXIC(fp,CONTEXTtoLEXICdepth(depth));
		readUntilNextTag(fp);
	}
	/* Atoms */
	int i=0,count = 4;
	ATOMS *sp;
	while(i<count && CURRENTTAGISOPEN && strcmp(CURRENTTAG,ATOMSTAG)==0){
		sp = readATOMS(fp,CONTEXTtoATOMSdepth(depth));
		if(strcmp(sp->Meta->Tag,SUPPORTTAG)==0){
			p->Support =sp;
			readUntilNextTag(fp);
		} else if(strcmp(sp->Meta->Tag,DISPORTTAG)==0){
			p->Disport =sp;
			readUntilNextTag(fp);
		}  else if(strcmp(sp->Meta->Tag,OBSOLETIONTAG)==0){
			p->Obsoletion =sp;
			readUntilNextTag(fp);
		}  else if(strcmp(sp->Meta->Tag,INSTANTIATIONTAG)==0){
			p->Instantiation =sp;
			readUntilNextTag(fp);
		}  
		i++;		
	}
	/* Closing tag */
	if(CURRENTTAGISOPEN || strcmp(CURRENTTAG,CONTEXTTAG)!=0 ){
		fprintf(stderr,"Problem: read %stag %s, when I was expecting %stag %s!\n",(CURRENTTAGISOPEN)? "opening ":"",CURRENTTAG,(CURRENTTAGISOPEN)? "closing ":"",CONTEXTTAG);
	} 
	return p;
}
/************************************************************************************************/
/* CONTEXTS                                                                                     */
/************************************************************************************************/
CONTEXTS *readCONTEXTS(FILE *fp,int depth){
	CONTEXTS *p =initCONTEXTS(readMETA(fp,depth));
	int i=0,count=atoi(readDATA(fp,COUNTTAG));
	/* Read next bit */
	readUntilNextTag(fp);
	while(i<count && CURRENTTAGISOPEN && strcmp(CURRENTTAG,CONTEXTTAG)==0){
		p->List[i] = readCONTEXT(fp,CONTEXTStoCONTEXTdepth(depth));
		p->Count++;
		readUntilNextTag(fp);
		i++;
	}
	/* Closing tag */
	if(CURRENTTAGISOPEN || strcmp(CURRENTTAG,CONTEXTSTAG)!=0 ){
		fprintf(stderr,"Problem: read %stag %s, when I was expecting %stag %s!\n",(CURRENTTAGISOPEN)? "opening ":"",CURRENTTAG,(CURRENTTAGISOPEN)? "closing ":"",CONTEXTSTAG);
	} 
	return p;
}
/************************************************************************************************/
/* ENTRY                                                                                        */
/************************************************************************************************/
ENTRY *readENTRY(FILE *fp,int depth){
	META *m=readMETA(fp,depth);
	char term[5000];			strcpy(term,			readDATA(fp,TERMTAG));
	char definition[5000];	strcpy(definition,	readDATA(fp,DEFINITIONTAG));
	ENTRY *p = initENTRY(m,term,definition);
	/* Closing tag */
	readUntilNextTag(fp);
	if(strcmp(CURRENTTAG,ENTRYTAG)!=0 || CURRENTTAGISOPEN){
		fprintf(stderr,"Problem: read %stag %s, when I was expecting %stag %s!\n",(CURRENTTAGISOPEN)? "opening ":"",CURRENTTAG,(CURRENTTAGISOPEN)? "closing ":"",ENTRYTAG);
	}	
	return p;	
}
/************************************************************************************************/
/* LEXIC                                                                                        */
/************************************************************************************************/
LEXIC *readLEXIC(FILE *fp,int depth){
	LEXIC *p =initLEXIC(readMETA(fp,depth));
	int i=0,count=atoi(readDATA(fp,COUNTTAG));
	/* Read next bit */
	readUntilNextTag(fp);
	while(i<count && CURRENTTAGISOPEN && strcmp(CURRENTTAG,ENTRYTAG)==0){
		p->List[i] = readENTRY(fp,LEXICtoENTRYdepth(depth));
		p->Count++;
		readUntilNextTag(fp);
		i++;
	}
	/* Closing tag */
	if(CURRENTTAGISOPEN || strcmp(CURRENTTAG,LEXICTAG)!=0 ){
		fprintf(stderr,"Problem: read %stag %s, when I was expecting %stag %s!\n",(CURRENTTAGISOPEN)? "opening ":"",CURRENTTAG,(CURRENTTAGISOPEN)? "closing ":"",LEXICTAG);
	} 
	return p;
}