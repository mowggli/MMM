/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                            I N T E R F A C E  0 . C                                          */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
#include "tete.h"
bool PROMPTMETA=false;
bool PROMPTQUESTION=true;
bool PROMPTCONTEXTS=false;
bool PROMPTLEXIC=false;
bool PROMPTANSWERS=false;
bool PROMPTATOMS=false;
bool PROMPTATOM=true;
bool PROMPTCONTEXT=false;
bool PROMPTENTRY=false;
bool MULTIPLECONTEXTS=false;
/************************************************************************************************/
/* ATOM                                                                                         */
/************************************************************************************************/
ATOM *getATOM(int maxdepth,META *meta){ // recursive not used
	if(maxdepth>=meta->Depth){
		char text[5000];		
		CONTEXTS *c=NULL;
		ATOMS *a=NULL; 
		bool isquestion = strcmp(meta->Tag,QUESTIONTAG)==0;
		printf("%s",isquestion? QUESTIONPROMPT : ATOMPROMPT);
		strcpy(text, promptandscan("",false));
		if(strlen(text)>0){
			c	=	getCONTEXTS(maxdepth,initMETA(CONTEXTSTAG,meta->Author,QUESTIONtoCONTEXTSdepth(meta->Depth),newid(),meta->ID));
			if(isquestion){
				changeATOMPROMPT("ANSWER");
				a =getATOMS(   maxdepth,initMETA(ANSWERSTAG,meta->Author, QUESTIONtoANSWERSdepth(meta->Depth), newid(),meta->ID));
				restoreATOMPROMPT();
			} 
			return initATOM(meta,text,c,a); 
		}
	}
	freeMETA(meta);
	return NULL;
}

ATOM *getNecessaryATOM(META *meta,char *alttext){ // recursive not used
	char text[5000],reference[5000],author[5000];	
	ATOM *p=NULL;		
	printf("%s",ATOMPROMPT);
	strcpy(text, promptandscan("",false));
	if(strlen(text)==0){
		strcpy(text,alttext);
	}
	/* Self */
	p = initATOM(meta,text,NULL,NULL); 
	/* Contexts */
	p->Contexts = initCONTEXTS(initMETA(CONTEXTSTAG,meta->Author,ATOMtoCONTEXTSdepth(meta->Depth),newid(),meta->ID));
	if(strlen(text)==0){
		/* Context */	
		CONTEXT *c = recordNewCONTEXT(newid(),p->Contexts,NULL,NULL,NULL,NULL,NULL);
		/* Lexic */
		c->Lexic = getLEXIC(CONTEXTtoENTRYdepth(c->Meta->Depth),initMETA(LEXICTAG, c->Meta->Author,CONTEXTtoLEXICdepth(c->Meta->Depth),newid(),c->Meta->ID));
	}
	return p; 
}

ATOM *getREASON(META *meta){ // recursive not used
	char text[5000],reference[5000],author[5000];	
	ATOM *p=NULL;
	printf("%s",ATOMPROMPT);
	strcpy(text, promptandscan("",false));
	if(strlen(text)>0){
		/* Self */
		p = initATOM(meta,text,NULL,NULL); 
		/* Contexts */
		p->Contexts = initCONTEXTS(initMETA(CONTEXTSTAG,p->Meta->Author,ATOMtoCONTEXTSdepth(p->Meta->Depth),newid(),p->Meta->ID));
		/* Context */	
		CONTEXT *c = recordNewCONTEXT(newid(),p->Contexts,NULL,NULL,NULL,NULL,NULL);
		/* Lexic */
		c->Lexic = getLEXIC(CONTEXTtoENTRYdepth(c->Meta->Depth),initMETA(LEXICTAG, c->Meta->Author,CONTEXTtoLEXICdepth(c->Meta->Depth),newid(),c->Meta->ID));
		/* Reference */		
		unmberedinstruction("Please give reference for this statement if you know it.");
		strcpy(reference,promptandscan(REFPROMPT,false));	
		if(strlen(reference)>0){
			unmberedinstruction("Please list authors of this reference.");
			strcpy(author,promptandscan(AUTHORPROMPT,false));	strcat(author,"(referenced by "); strcat(author,p->Meta->Author);	strcat(author,")");
			incrementSUPPORT(p,reference,author,REFTAG);
		} 
	} else {
		freeMETA(meta);	
	}
	return p;
}

ATOM *getCONFOUNDER(META *meta){ // recursive not used
	char text[5000],completetext[5000],experiment[5000];	
	ATOM *p=NULL;
	printf("%s",ATOMPROMPT);
	strcpy(text, promptandscan("",false));
	if(strlen(text)>0){
		numberedinstruction("If you have any reasons to believe this is not a real issue, please give them here - eg, if you have made an experiement to address the problem, please describe it here."); 
		unmberedinstruction("Type enter otherwise.");
		unmberedinstruction("..."); 
		strcpy(experiment, promptandscan("EXPERIMENT",false));
		if(strlen(experiment)>0){
			snprintf(completetext,5000,"%s But this issue has been addressed.",text);
		} else {
			snprintf(completetext,5000,"%s But this is assumed not to happen.",text);
		}
		/* Self */
		p = initATOM(meta,completetext,NULL,NULL); 
		/* Contexts */
		p->Contexts = initCONTEXTS(initMETA(CONTEXTSTAG,p->Meta->Author,ATOMtoCONTEXTSdepth(p->Meta->Depth),newid(),p->Meta->ID));
		/* Context */	
		CONTEXT *c = recordNewCONTEXT(newid(),p->Contexts,NULL,NULL,NULL,NULL,NULL);
		/* Lexic */
		c->Lexic = getLEXIC(CONTEXTtoENTRYdepth(c->Meta->Depth),initMETA(LEXICTAG, c->Meta->Author,CONTEXTtoLEXICdepth(c->Meta->Depth),newid(),c->Meta->ID));
		/* experiment */
		c->Support = initATOMS(initMETA(CONTEXTTAG,meta->Author,CONTEXTtoATOMSdepth(c->Meta->Depth), newid(),c->Meta->ID));
		c->Support->List[0] = initATOM(initMETA("EXPERIMENT",meta->Author,ATOMStoATOMdepth(c->Support->Meta->Depth),newid(),c->Support->Meta->ID),experiment,NULL,NULL); // recursivity needed here
		c->Support->Count++;
	} else {
		freeMETA(meta);	
	}
	return p;
}


/************************************************************************************************/
/* ATOMS                                                                                        */
/************************************************************************************************/
ATOMS *getATOMS(int maxdepth,META *meta){
	if(maxdepth>=ATOMStoATOMdepth(meta->Depth)){
		printf("%s",ATOMSPROMPT);
		ATOM *sp						= getATOM(maxdepth,initMETA(subtag(meta->Tag,0),meta->Author,ATOMStoATOMdepth(meta->Depth),newid(),meta->ID));
		if(sp!=NULL){
			ATOMS *p = initATOMS(meta);
			p->List[p->Count] = sp;
			while(p->List[p->Count]!=NULL){
				p->Count++;
				p->List[p->Count]	= getATOM(maxdepth,initMETA(subtag(meta->Tag,p->Count),meta->Author,ATOMStoATOMdepth(meta->Depth),newid(),meta->ID));
			}
			return p;
		}
	}
	freeMETA(meta);
	return NULL;
}

ATOMS *getREASONS(META *meta){
	printf("%s",ATOMSPROMPT);
	ATOM *sp						= getREASON(initMETA(subtag(meta->Tag,0),meta->Author,ATOMStoATOMdepth(meta->Depth),newid(),meta->ID));
	if(sp!=NULL){
		ATOMS *p = initATOMS(meta);
		p->List[p->Count] = sp;
		while(p->List[p->Count]!=NULL){
			p->Count++;
			p->List[p->Count]	= getREASON(initMETA(subtag(meta->Tag,p->Count),meta->Author,ATOMStoATOMdepth(meta->Depth),newid(),meta->ID));
		}
		return p;
	} else {
		freeMETA(meta);
		return NULL;
	}
}

ATOMS *getCONFOUNDERS(META *meta){
	printf("%s",ATOMSPROMPT);
	ATOM *sp						= getCONFOUNDER(initMETA(subtag(meta->Tag,0),meta->Author,ATOMStoATOMdepth(meta->Depth),newid(),meta->ID));
	if(sp!=NULL){
		ATOMS *p = initATOMS(meta);
		p->List[p->Count] = sp;
		while(p->List[p->Count]!=NULL){
			p->Count++;
			p->List[p->Count]	= getCONFOUNDER(initMETA(subtag(meta->Tag,p->Count),meta->Author,ATOMStoATOMdepth(meta->Depth),newid(),meta->ID));
		}
		return p;
	} else {
		freeMETA(meta);
		return NULL;
	}
}

/************************************************************************************************/
/* Support                                                                                    */
/************************************************************************************************/
ATOM *incrementSUPPORT(ATOM *p,char *text,char *author,char *tag){
	if(!p->Contexts){	 p->Contexts = initCONTEXTS(initMETA(CONTEXTSTAG,p->Meta->Author,ATOMtoCONTEXTSdepth(p->Meta->Depth),newid(),p->Meta->ID));}
	if(p->Contexts->Count>1){fprintf(stderr,"Don't know which context to register this support in ! Aborting\n"); exit(EXIT_FAILURE);}
	if(p->Contexts->Count==0){recordNewCONTEXT(newid(),p->Contexts,NULL,NULL,NULL,NULL,NULL);}
	CONTEXT *c =  p->Contexts->List[0];
	if(!c->Support){ c->Support  			= initATOMS(initMETA(SUPPORTTAG,	c->Meta->Author,CONTEXTtoATOMSdepth(c->Meta->Depth),		 newid(),c->Meta->ID));}
	c->Support->List[c->Support->Count] = initATOM( initMETA(tag,			author,			 ATOMStoATOMdepth(c->Support->Meta->Depth),newid(c->Support->Meta->Depth),c->Support->Meta->ID),text,NULL,NULL);
	c->Support->Count++;
	return c->Support->List[c->Support->Count-1];
}

/************************************************************************************************/
/* CONTEXT                                                                                      */
/************************************************************************************************/
CONTEXT *getCONTEXT(int maxdepth,META *meta){
	if(maxdepth>=meta->Depth){
		printf("%s",CONTEXTPROMPT);
		LEXIC *lexic   			= getLEXIC(maxdepth,initMETA(LEXICTAG,        meta->Author,CONTEXTtoLEXICdepth(meta->Depth),newid(),meta->ID));
		changeATOMPROMPT("REASON");
		ATOMS *support 		  = getATOMS(maxdepth,initMETA(SUPPORTTAG,      meta->Author,CONTEXTtoATOMSdepth(meta->Depth),newid(),meta->ID));
		ATOMS *disport 		  = getATOMS(maxdepth,initMETA(DISPORTTAG,      meta->Author,CONTEXTtoATOMSdepth(meta->Depth),newid(),meta->ID));
		ATOMS *obsoletion 	  = getATOMS(maxdepth,initMETA(OBSOLETIONTAG,   meta->Author,CONTEXTtoATOMSdepth(meta->Depth),newid(),meta->ID));
		ATOMS *instantiation = getATOMS(maxdepth,initMETA(INSTANTIATIONTAG,meta->Author,CONTEXTtoATOMSdepth(meta->Depth),newid(),meta->ID));
		restoreATOMPROMPT();
		if(lexic==NULL && support==NULL && disport==NULL && obsoletion==NULL && instantiation==NULL){
			freeMETA(meta);
			return NULL;
		}
		return initCONTEXT(meta,lexic,support,disport,obsoletion,instantiation);
	}
	freeMETA(meta);
	return NULL;
}
/************************************************************************************************/
/* CONTEXTS                                                                                     */
/************************************************************************************************/
CONTEXTS *getCONTEXTS(int maxdepth,META *meta){
	if(maxdepth>=meta->Depth){
		printf("%s",CONTEXTSPROMPT);
		CONTEXT *sp 					= getCONTEXT(maxdepth,initMETA(subtag(meta->Tag,0),meta->Author,CONTEXTStoCONTEXTdepth(meta->Depth),newid(),meta->ID));
		if(sp!=NULL){
			CONTEXTS *p = initCONTEXTS(meta);
			//CONTEXTS *p = initCONTEXTS(meta);
			p->List[p->Count] = sp;
			p->Count++;
			if(MULTIPLECONTEXTS){
				while(p->List[p->Count-1]!=NULL && p->Count<1){	
					p->List[p->Count]		= getCONTEXT(maxdepth,initMETA(subtag(meta->Tag,p->Count),meta->Author,CONTEXTStoCONTEXTdepth(meta->Depth),newid(),meta->ID));
					p->Count++;
				}
			}
			return p;
		}
	}
	freeMETA(meta);
	return NULL;
}
/************************************************************************************************/
/* LEXIC                                                                                        */
/************************************************************************************************/
LEXIC *getLEXIC(int maxdepth,META *meta){
	if(maxdepth>=meta->Depth){
		printf("%s",LEXICPROMPT);
		LEXIC *p = initLEXIC(meta);
		p->List[p->Count] 	= getENTRY(maxdepth,initMETA(subtag(ENTRYTAG,p->Count),meta->Author,LEXICtoENTRYdepth(meta->Depth),newid(),meta->ID));
		p->Count++;
		if(p->List[p->Count-1]==NULL){
			freeLEXIC(p);
			return NULL;
		}
		while(p->List[p->Count-1]!=NULL){
			p->List[p->Count] = getENTRY(maxdepth,initMETA(subtag(ENTRYTAG,p->Count),meta->Author,LEXICtoENTRYdepth(meta->Depth),newid(),meta->ID));
			p->Count++;
		}
		p->Count--;
		return p;
	}
	freeMETA(meta);
	return NULL;
}
/************************************************************************************************/
/* ENTRY                                                                                        */
/************************************************************************************************/
ENTRY *getENTRY(int maxdepth,META *meta){
	if(maxdepth>=meta->Depth){
		printf("%s",ENTRYPROMPT);
		char term[5000], definition[5000];
		strcpy(term,promptandscan(TERMPROMPT,false));
		if(strlen(term)>0){
			return initENTRY(meta,term,promptandscan(DEFINITIONPROMPT,false));
		}
	}
	freeMETA(meta);
	return NULL;
}
