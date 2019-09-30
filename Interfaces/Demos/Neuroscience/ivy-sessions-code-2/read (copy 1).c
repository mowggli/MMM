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
/************************************************************************************************/
/*                                                                                      */
/************************************************************************************************/
/************************************************************************************************/
/* META                                                                                         */
/************************************************************************************************/
META *readMETA(FILE *fp,int depth){
	char dump[5000],tag[5000],author[5000],id[5000],parentid[5000];
	/* Find opening tag, check it's an opening tag, and that it's the right kind of tag */
	if(readUntilNextTag(fp,1,METATAG,dump)==-1){return NULL;}
	/* Tag */
	if(readUntilNextTag(fp,1,TAGTAG,dump)==-1){return NULL;}
	if(readUntilNextTag(fp,0,TAGTAG,tag)==-1){return NULL;}
	/* Auther */
	if(readUntilNextTag(fp,1,AUTHORTAG,dump)==-1){return NULL;}
	if(readUntilNextTag(fp,0,AUTHORTAG,author)==-1){return NULL;}
	/* id */
	if(readUntilNextTag(fp,1,IDTAG,dump)==-1){return NULL;}
	if(readUntilNextTag(fp,0,IDTAG,id)==-1){return NULL;}
	/* Parentid */
	if(readUntilNextTag(fp,1,PARENTIDTAG,dump)==-1){return NULL;}
	if(readUntilNextTag(fp,0,PARENTIDTAG,parentid)==-1){return NULL;}
	/* Find closing tag, check it's a closing tag, and that it's the right kind of tag */
	if(readUntilNextTag(fp,0,METATAG,dump)==-1){return NULL;}
	/**/
	return initMETA(tag,author,depth,parentid);

}
/************************************************************************************************/
/* QUESTION                                                                                     */
/************************************************************************************************/
ATOM *readQUESTION(FILE *fp,int depth){
	char dump[5000],text[5000];
	/* Find opening tag, check it's an opening tag, and that it's the right kind of tag */
	if(readUntilNextTag(fp,1,ATOMTAG,dump)==-1){return NULL;}
	/* Read Meta */
	META *m=readMETA(fp,depth);
	/* Read Text */
	if(readUntilNextTag(fp,1,TEXTTAG,dump)==-1){return NULL;}
	if(readUntilNextTag(fp,0,TEXTTAG,text)==-1){return NULL;}
printf("read text of question = %s\n",text);
	/* Read contexts */
	CONTEXTS *c=readCONTEXTS(fp,QUESTIONtoCONTEXTSdepth(depth)); printf("read context of question \n"); //writeForestCONTEXTS(stdout,12,c,"",false,false);printf("ok\n");
	/* Read answers */
	ATOMS *a=readATOMS(fp,QUESTIONtoANSWERSdepth(depth));printf("read answers of question \n"); writeForestATOMS(stdout,12,a,"",false,false,false); printf("CURRENTTAG=%s\n",CURRENTTAG);
	/* Find closing tag, check it's a closing tag, and that it's the right kind of tag */
	if(readUntilNextTag(fp,0,ATOMTAG,dump)==-1){return NULL;}
printf("read last tag = %s\n",CURRENTTAG);
	/**/
	return initATOM(m,text,c,a);
}
/************************************************************************************************/
/* ATOM                                                                                         */
/************************************************************************************************/
ATOM *readATOM(FILE *fp,int depth){
	char dump[5000],text[5000];
	/* Find opening tag, check it's an opening tag, and that it's the right kind of tag */
	if(readUntilNextTag(fp,1,ATOMTAG,dump)==-1){return NULL;}
	/* Read Meta */
	META *m=readMETA(fp,depth);
	/* Read Text */
	if(readUntilNextTag(fp,1,TEXTTAG,dump)==-1){return NULL;}
	if(readUntilNextTag(fp,0,TEXTTAG,text)==-1){return NULL;}
	/* Read contexts */
	CONTEXTS *c=readCONTEXTS(fp,ATOMtoCONTEXTSdepth(depth)); printf("READ CONTEXTS in reasATOM\n");
	/* Read answers */
	//ATOMS *a=readATOMS(fp,QUESTIONtoANSWERSdepth(depth));
	/* Find closing tag, check it's a closing tag, and that it's the right kind of tag */
	if(readUntilNextTag(fp,0,ATOMTAG,dump)==-1){return NULL;}printf("here\n");
	/**/
	return initATOM(m,text,c,NULL);
}
/************************************************************************************************/
/* ATOMS                                                                                        */
/************************************************************************************************/
ATOMS *readATOMS(FILE *fp,int depth){printf("reading ATOMS\n");
	char dump[5000],counttext[5000];
	int count,i;
	ATOMS *p=NULL;
	/* Find opening tag, check it's an opening tag, and that it's the right kind of tag */
	if(readUntilNextTag(fp,1,ATOMSTAG,dump)==-1){return NULL;}
	/* Read Meta */
	META *m=readMETA(fp,depth);
	/* Read count */
	if(readUntilNextTag(fp,1,COUNTTAG,dump)==-1){return NULL;}
	if(readUntilNextTag(fp,0,COUNTTAG,counttext)==-1){return NULL;}
	count=atoi(counttext);printf("ATOMS->Count=%d\n",count);
	if(count==0){
		freeMETA(m);
	}	else {
		/* Self */
		p = initATOMS(m);printf("IINITIALISED ATOMS\n");
		/* Read items */
		for(i=0;i<count;i++){printf("Atom no i=%d\n",i);
			ATOM *sp = readATOM(fp,ATOMStoATOMdepth(depth));
			if(sp==NULL){fprintf(stderr,"I shouldn't be reading NULL ATOMs. Must be a problem with count!\n");}
			p->List[p->Count] = sp;
			p->Count++;
		}
	}
	/* Find closing tag, check it's a closing tag, and that it's the right kind of tag */
	if(readUntilNextTag(fp,0,ATOMSTAG,dump)==-1){return NULL;}
	/**/
printf("RETURNING ATOMS\n");
	return p;
}
/************************************************************************************************/
/* CONTEXT                                                                                      */
/************************************************************************************************/
CONTEXT *readCONTEXT(FILE *fp,int depth){
	char dump[5000];
	int count,i;
	/* Find opening tag, check it's an opening tag, and that it's the right kind of tag */
	if(readUntilNextTag(fp,1,CONTEXTTAG,dump)==-1){return NULL;}
	/* Read Meta */
	META *m=readMETA(fp,depth);
	LEXIC *lexic            = readLEXIC(fp,CONTEXTtoLEXICdepth(depth));printf("Read LEXIC\n");
	ATOMS *support 			= readATOMS(fp,CONTEXTtoATOMSdepth(depth));printf("Read SUPPORT\n");
	ATOMS *disport 			= readATOMS(fp,CONTEXTtoATOMSdepth(depth));printf("Read DISPORT\n");
	ATOMS *obsoletion 		= readATOMS(fp,CONTEXTtoATOMSdepth(depth));printf("Read OBS\n");
	ATOMS *instantiation 	= readATOMS(fp,CONTEXTtoATOMSdepth(depth));printf("Read INSTANT\n");
	/* Self */
	if(lexic==NULL && support==NULL && disport==NULL && obsoletion==NULL && instantiation==NULL){freeMETA(m);return NULL;}
	/**/
	if(readUntilNextTag(fp,0,CONTEXTTAG,dump)==-1){return NULL;}
	return initCONTEXT(m,lexic,support,disport,obsoletion,instantiation);
}
/************************************************************************************************/
/* CONTEXTS                                                                                     */
/************************************************************************************************/
CONTEXTS *readCONTEXTS(FILE *fp,int depth){
	char dump[5000],counttext[5000];
	int count,i;
	CONTEXTS *p =NULL;
	/* Find opening tag, check it's an opening tag, and that it's the right kind of tag */
	if(readUntilNextTag(fp,1,CONTEXTSTAG,dump)==-1){return NULL;}
	/* Read Meta */
	META *m=readMETA(fp,depth);
	/* Read count */
	if(readUntilNextTag(fp,1,COUNTTAG,dump)==-1){return NULL;}
	if(readUntilNextTag(fp,0,COUNTTAG,counttext)==-1){return NULL;}
	count=atoi(counttext);printf("CONTEXTS->Count=%d\n",count);
	if(count==0){
		freeMETA(m);
	}	else {
		/* Self */
		p = initCONTEXTS(m);
		/* Read items */
		for(i=0;i<count;i++){
			CONTEXT *sp = readCONTEXT(fp,CONTEXTStoCONTEXTdepth(depth));
			if(sp==NULL){fprintf(stderr,"I shouldn't be reading NULL CONTEXTs. Must be a problem with count!\n");}
			p->List[p->Count] = sp;
			p->Count++;
		}
	}
	/* Find closing tag, check it's a closing tag, and that it's the right kind of tag */
	if(readUntilNextTag(fp,0,CONTEXTSTAG,dump)==-1){return NULL;}
	/**/
	return p;
}

/************************************************************************************************/
/* ENTRY                                                                                        */
/************************************************************************************************/
ENTRY *readENTRY(FILE *fp,int depth){
	char dump[5000],term[5000],definition[5000];
	/* Find opening tag, check it's an opening tag, and that it's the right kind of tag */
	if(readUntilNextTag(fp,1,ENTRYTAG,dump)==-1){return NULL;}
	/* Read Meta */
	META *m=readMETA(fp,depth);
	/* Read Term */
	if(readUntilNextTag(fp,1,TERMTAG,dump)==-1){return NULL;}
	if(readUntilNextTag(fp,0,TERMTAG,term)==-1){return NULL;}
	/* Read Definition */
	if(readUntilNextTag(fp,1,DEFINITIONTAG,dump)==-1){return NULL;}
	if(readUntilNextTag(fp,0,DEFINITIONTAG,definition)==-1){return NULL;}
	/* Find closing tag, check it's a closing tag, and that it's the right kind of tag */
	if(readUntilNextTag(fp,0,ENTRYTAG,dump)==-1){return NULL;}
	/**/
	return initENTRY(m,term,definition);
}
/************************************************************************************************/
/* LEXIC                                                                                        */
/************************************************************************************************/
LEXIC *readLEXIC(FILE *fp,int depth){
	char dump[5000],counttext[5000];
	int count,i;
	LEXIC *p = NULL;
	/* Find opening tag, check it's an opening tag, and that it's the right kind of tag */
	if(readUntilNextTag(fp,1,LEXICTAG,dump)==-1){return NULL;}
	/* Read Meta */
	META *m=readMETA(fp,depth);
	/* Read count */
	if(readUntilNextTag(fp,1,COUNTTAG,dump)==-1){return NULL;}
	if(readUntilNextTag(fp,0,COUNTTAG,counttext)==-1){return NULL;}
	count=atoi(counttext);printf("LEXIC->Count=%d\n",count);
	if(count==0){
		freeMETA(m);
	}	else {
		/* Self */
		p = initLEXIC(m);
		/* Read items */
		for(i=0;i<count;i++){
			ENTRY *sp = readENTRY(fp,LEXICtoENTRYdepth(depth));
			if(sp==NULL){fprintf(stderr,"I shouldn't be reading NULL ENTRYs. Must be a problem with count!\n");}
			p->List[p->Count] = sp;
			p->Count++;
		}
	}
	/* Find closing tag, check it's a closing tag, and that it's the right kind of tag */
	if(readUntilNextTag(fp,1,LEXICTAG,dump)==-1){return NULL;}
	/**/
	return p;
}