/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                              S T R U C T U R E S . C                                         */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
#include "tete.h"
int ALLOCCOUNT = 0;

/************************************************************************************************/
/* META                                                                                         */
/************************************************************************************************/
META *initMETA(char *tag,char *author,int depth,char *id,char *parentid){
	/* Self */
	META* p = (META *) malloc (sizeof(META));
	signalAllocate(!p,            ANYTHINGtoMETAdepth(depth),     			METATAG,         "",            (uintptr_t) p);
   /* Tag */
	p->Tag = (char *) malloc (strlen(tag) + 1); 
	signalAllocate(!p->Tag,       ANYTHINGtoINSIDEMETAdepth(depth),   			TAGTAG,          tag,           (uintptr_t) p->Tag);
	strcpy(p->Tag,tag);
	/* Author */
	p->Author = (char *) malloc (strlen(author) + 1); 
	signalAllocate(!p->Author,    ANYTHINGtoINSIDEMETAdepth(depth),   			AUTHORTAG,       author,        (uintptr_t) p->Author);
	strcpy(p->Author,author);
	/* Depth */
	p->Depth=depth;
   /* ID */
	//char *id=newid();
	p->ID = (char *) malloc (strlen(id) + 1); 
	signalAllocate(!p->ID,        ANYTHINGtoINSIDEMETAdepth(depth),   			IDTAG,           id,            (uintptr_t) p->ID);
	strcpy(p->ID,id);
   /* ParentID */
	p->ParentID = (char *) malloc (strlen(parentid) + 1); 
	signalAllocate(!p->ParentID,  ANYTHINGtoINSIDEMETAdepth(depth),   			PARENTIDTAG,     parentid,      (uintptr_t) p->ParentID);
	strcpy(p->ParentID,parentid);
	/**/
	return p;
}

int freeMETA(META *p){
	/* ParentID */
	signalFree(                   ANYTHINGtoINSIDEMETAdepth(p->Depth),			PARENTIDTAG,     p->ParentID,   (uintptr_t) p->ParentID);	
	free(p->ParentID);
   /* ID */
	signalFree(                   ANYTHINGtoINSIDEMETAdepth(p->Depth),			IDTAG,           p->ID,         (uintptr_t) p->ID);	
	free(p->ID);
	/* Depth */
	/* Author */
	signalFree(                   ANYTHINGtoINSIDEMETAdepth(p->Depth),			AUTHORTAG,       p->Author,     (uintptr_t) p->Author);		
	free(p->Author);
   /* Tag */
	signalFree(                   ANYTHINGtoINSIDEMETAdepth(p->Depth),			TAGTAG,          p->Tag,        (uintptr_t) p->Tag);	
	free(p->Tag);
	/* Self */
	signalFree(                   ANYTHINGtoMETAdepth(p->Depth),		METATAG,         "",            (uintptr_t) p);
	free(p);
	/**/
	return ALLOCCOUNT;
}

/************************************************************************************************/
/* ATOM                                                                                         */
/************************************************************************************************/
ATOM *initATOM(META *meta,char* text,CONTEXTS *contexts, ATOMS *answers){
	if(meta==NULL){
		fprintf(stderr,"Error NULL META structure given as argument to function initATOM!\n");
		return NULL;
	}
	/* Self */
	ATOM* p = (ATOM *) malloc (sizeof(ATOM));
	//signalAllocateFromMeta(!p, meta,(uintptr_t) p);
	signalAllocate(!p,            meta->Depth,     	ATOMTAG,         "",            (uintptr_t) p);
	/* Meta */
	p->Meta = meta;
	/* Text */
	p->Text = (char *) malloc (strlen(text) + 1); 
	signalAllocate(!p->Text,      meta->Depth+1,   	TEXTTAG,         text,          (uintptr_t) p->Text);
	strcpy(p->Text,text);
   /* Context */
	p->Contexts=contexts;
	/* Answers */
	p->Answers=answers;
	/**/
	return p;
}

int freeATOM(ATOM *p){
	//signalFreeFromMeta(p->Meta,(uintptr_t) p);
	int depth = p->Meta->Depth;
	/* Answers */
	freeATOMS(p->Answers);
   /* Contexts */
	freeCONTEXTS(p->Contexts);
	/* Text */
	signalFree(                   depth+1,				TEXTTAG,         p->Text,       (uintptr_t) p->Text);	
	free(p->Text);
	/* Meta */
	freeMETA(p->Meta);
	/* Self */
	signalFree(                   depth,				ATOMTAG,         "",            (uintptr_t) p);
	free(p);
	/**/
	return ALLOCCOUNT;
}

/************************************************************************************************/
/* ATOMS                                                                                        */
/************************************************************************************************/
ATOMS *initATOMS(META *meta){
	/* Self */
	ATOMS *p  = (ATOMS *) malloc (sizeof(ATOMS)); 
	//signalAllocateFromMeta(!p, meta,(uintptr_t) p);
	signalAllocate(!p,            meta->Depth,   	meta->Tag,        "",            (uintptr_t) p);
	/* Meta */
	p->Meta = meta;
	/* Count */
	p->Count = 0;
	/* List */
	/**/
	return p;
}

int freeATOMS(ATOMS* p){
	if(p){
		//signalFreeFromMeta(p->Meta,(uintptr_t) p);
		int depth = p->Meta->Depth;
		/* List */
		while(p->Count>0){	
			p->Count--;		
			freeATOM(p->List[p->Count]);
		}
		/* Count */
		/* Meta */
		freeMETA(p->Meta);
		/* Self */
		signalFree(                depth,  				ATOMSTAG,        "",            (uintptr_t) p);
		free(p);
	} 
	/**/
	return ALLOCCOUNT;
}
/************************************************************************************************/
/* CONTEXT                                                                                      */
/************************************************************************************************/
CONTEXT *initCONTEXT(META *meta,LEXIC* lexic,ATOMS* support,ATOMS* disport,ATOMS* obsoletion,ATOMS* instantiation){
	/* Self */
	CONTEXT *p =  (CONTEXT *) malloc (sizeof(CONTEXT)); 
	//signalAllocateFromMeta(!p, meta,(uintptr_t) p);
	signalAllocate(!p,            meta->Depth,     CONTEXTTAG,      "",            (uintptr_t) p);
   /* Meta */
	p->Meta = meta;
	/* Lexic */
	p->Lexic=lexic;
	/* Support */
	p->Support=support;
	/* Disport */
	p->Disport=disport;
	/* Obsoletion */
	p->Obsoletion=obsoletion;
	/* Instantiation */
	p->Instantiation=instantiation;
	/*  */
	return p;
}
int freeCONTEXT(CONTEXT* p){
	if(p){
		//signalFreeFromMeta(p->Meta,(uintptr_t) p);
		int depth = p->Meta->Depth;
		/* Instantiation */
		freeATOMS(p->Instantiation);
		/* Obsoletion   */
		freeATOMS(p->Obsoletion);
		/* Disport     */
		freeATOMS(p->Disport);
		/* Support    */
		freeATOMS(p->Support);
		/* Lexic     */
		freeLEXIC(p->Lexic);
		/* Meta */
		freeMETA(p->Meta);
		/* Self  */
		signalFree(                depth,  				CONTEXTTAG,      "",            (uintptr_t) p);
		free(p);
	}
	/**/
	return ALLOCCOUNT;
}
/************************************************************************************************/
/* CONTEXTS                                                                                     */
/************************************************************************************************/
CONTEXTS *initCONTEXTS(META *meta){
	/* Self */
	CONTEXTS *p  = (CONTEXTS *) malloc (sizeof(CONTEXTS)); 
	//signalAllocateFromMeta(!p, meta,(uintptr_t) p);
	signalAllocate(!p,            meta->Depth,     CONTEXTSTAG,     "",            (uintptr_t) p);
	/* Meta */
	p->Meta = meta;
	/* Count */
	p->Count = 0;
	/* List */
	/**/
	return p;
}

int freeCONTEXTS(CONTEXTS* p){
	if(p){
		//signalFreeFromMeta(p->Meta,(uintptr_t) p);
		int depth = p->Meta->Depth;
		/* List */
		while(p->Count>0){	
			p->Count--;		
			freeCONTEXT(p->List[p->Count]);
		}
		/* Count */
		/* Meta */
		freeMETA(p->Meta);
		/* Self */
		signalFree(                depth,  				CONTEXTSTAG,        "",            (uintptr_t) p);
		free(p);
	} 
	/**/
	return ALLOCCOUNT;
}

/************************************************************************************************/
/* ENTRY                                                                                        */
/************************************************************************************************/
ENTRY *initENTRY(META *meta,char *term,char *definition){
	/* Self */
	ENTRY *p = (ENTRY *) malloc (sizeof(ENTRY));
	//signalAllocateFromMeta(!p, meta,(uintptr_t) p);
	signalAllocate(!p,            meta->Depth,     meta->Tag,        "",            (uintptr_t) p);
	/* Meta */
	p->Meta = meta;
	/* Term */
	p->Term = (char *) malloc (strlen(term) + 1); 
	signalAllocate(!p->Term,      meta->Depth+1,   meta->Tag,         term,          (uintptr_t) p->Term);
	strcpy(p->Term,term);
	/* Definition */
	p->Definition = (char *) malloc (strlen(definition) + 1); 
	signalAllocate(!p->Definition,meta->Depth+1,   meta->Tag,   definition,    (uintptr_t) p->Definition);
	strcpy(p->Definition,definition);
   /* */
	return p;
}

int freeENTRY(ENTRY *p){
	if(p){
		//signalFreeFromMeta(p->Meta,(uintptr_t) p);
		int depth = p->Meta->Depth;
		/* Definition */
		signalFree(                   depth+1,			DEFINITIONTAG,   p->Definition, (uintptr_t) p->Definition);	
		free(p->Definition);
		/* Term */
		signalFree(                   depth+1,			TERMTAG,         p->Term,       (uintptr_t) p->Term);	
		free(p->Term);
		/* Meta */
		freeMETA(p->Meta);
		/* Self */
		signalFree(                   depth,     		ENTRYTAG,         "",           (uintptr_t) p);
		free(p);
	} 
	/**/
	return ALLOCCOUNT;
}

/************************************************************************************************/
/* LEXIC                                                                                        */
/************************************************************************************************/
LEXIC *initLEXIC(META *meta){
	/* Self */
	LEXIC *p  = (LEXIC *) malloc (sizeof(LEXIC)); 
	//signalAllocateFromMeta(!p, meta,(uintptr_t) p);
	signalAllocate(!p,            meta->Depth,     meta->Tag,        "",            (uintptr_t) p);
	/* Meta */
	p->Meta = meta;
	/* Count */
	p->Count = 0;
	/* List */
	/**/
	return p;
}
int freeLEXIC(LEXIC* p){
	if(p){
		//signalFreeFromMeta(p->Meta,(uintptr_t) p);
		int depth = p->Meta->Depth;
		/* List */
		while(p->Count>0){	
			p->Count--;		
			freeENTRY(p->List[p->Count]);
		}
		/* Count */
		/* Meta */
		freeMETA(p->Meta);
		/* Self */
		signalFree(                depth,  				LEXICTAG,        "",            (uintptr_t) p);
		free(p);
	} 
	/**/
	return ALLOCCOUNT;
}
/************************************************************************************************/
/* RECORD                                                                                       */
/************************************************************************************************/
ATOM *recordNewATOM(char *id,ATOMS *p,char *text){
	//int depth = (entitle)? (p->Meta->Depth+1) : (p->Meta->Depth);
	META *m							= initMETA(subtag(p->Meta->Tag,p->Count),p->Meta->Author,ATOMStoATOMdepth(p->Meta->Depth),id,p->Meta->ID);
	p->List[p->Count] = initATOM(m,text,NULL,NULL);
	p->Count++;
	return p->List[p->Count-1];
}
ATOM *recordNewANSWER(char *id,ATOMS *p,char *text){
	META *m							= initMETA(subtag(p->Meta->Tag,p->Count),p->Meta->Author,ANSWERStoANSWERdepth(p->Meta->Depth),id,p->Meta->ID);
	p->List[p->Count] = initATOM(m,text,NULL,NULL);
	p->Count++;
	return p->List[p->Count-1];
}
CONTEXT *recordNewCONTEXT(char *id, CONTEXTS *p,LEXIC* lexic,ATOMS* support,ATOMS* disport,ATOMS* obsoletion,ATOMS* instantiation){
	META *m							= initMETA(subtag(p->Meta->Tag,p->Count),p->Meta->Author,CONTEXTStoCONTEXTdepth(p->Meta->Depth),id,p->Meta->ID);
	p->List[p->Count] = initCONTEXT(m,lexic,support,disport,obsoletion,instantiation);
	p->Count++;
	return p->List[p->Count-1];
}
ENTRY *recordNewENTRY(char *id,LEXIC *p,char *term,char *definition){
	META *m							= initMETA(subtag(ENTRYTAG,p->Count),p->Meta->Author,LEXICtoENTRYdepth(p->Meta->Depth),id,p->Meta->ID);
	p->List[p->Count] = initENTRY(m,term,definition);
	p->Count++;
	return p->List[p->Count-1];
}
/************************************************************************************************/
/* COPY                                                                                         */
/************************************************************************************************/
ATOM *copyATOM(META *meta,ATOM *p){
	if(meta->Depth>=0){
		META     *m					= initMETA(p->Contexts->Meta->Tag,p->Contexts->Meta->Author,ATOMtoCONTEXTSdepth(meta->Depth),newid(),meta->ID);		
		CONTEXTS *contexts		= copyCONTEXTS(m,p->Contexts);
					 m					= initMETA(p->Answers->Meta->Tag, p->Answers->Meta->Author, QUESTIONtoANSWERSdepth(meta->Depth),newid(),meta->ID);		
		ATOMS    *answers			= copyATOMS(m,p->Answers);
		return initATOM(meta,p->Text,contexts,answers);
	} 
	freeMETA(meta);
	return NULL;
}

ATOMS *copyATOMS(META *meta,ATOMS *p){
	if(meta->Depth>=0){
		ATOMS    *np 				= initATOMS(meta);
		META		*m;
		ATOM     *sp;
		while(np->Count < p->Count){
			sp = p->List[np->Count];
			m 							= initMETA(sp->Meta->Tag,sp->Meta->Author,ATOMStoATOMdepth(meta->Depth),newid(),meta->ID);
			np->List[np->Count] 	= copyATOM(m,sp);
			np->Count++;
		}
		return np;
	}
	freeMETA(meta);
	return NULL;
}

ATOMS *copyANSWERS(META *meta,ATOMS *p){
	if(meta->Depth>=0){
		ATOMS    *np 				= initATOMS(meta);
		META		*m;
		ATOM     *sp;
		while(np->Count < p->Count){
			sp = p->List[np->Count];
			m 							= initMETA(sp->Meta->Tag,sp->Meta->Author,ANSWERStoANSWERdepth(meta->Depth),newid(),meta->ID);
			np->List[np->Count] 	= copyATOM(m,sp);
			np->Count++;
		}
		return np;
	}
	freeMETA(meta);
	return NULL;
}

CONTEXT *copyCONTEXT(META *meta,CONTEXT *p){
	if(meta->Depth>=0){
		META  *m    				= initMETA(p->Lexic->Meta->Tag,p->Lexic->Meta->Author,						CONTEXTtoLEXICdepth(meta->Depth),newid(),meta->ID);
		LEXIC *lexic   			= copyLEXIC(m,p->Lexic);
				 m    				= initMETA(p->Support->Meta->Tag,p->Support->Meta->Author,					CONTEXTtoATOMSdepth(meta->Depth),newid(),meta->ID);
		ATOMS *support 			= copyATOMS(m,p->Support);
				 m    				= initMETA(p->Disport->Meta->Tag,p->Disport->Meta->Author,					CONTEXTtoATOMSdepth(meta->Depth),newid(),meta->ID);
		ATOMS *disport 			= copyATOMS(m,p->Disport);
				 m    				= initMETA(p->Obsoletion->Meta->Tag,p->Obsoletion->Meta->Author,			CONTEXTtoATOMSdepth(meta->Depth),newid(),meta->ID);
		ATOMS *obsoletion 		= copyATOMS(m,p->Obsoletion);
				 m    				= initMETA(p->Instantiation->Meta->Tag,p->Instantiation->Meta->Author,	CONTEXTtoATOMSdepth(meta->Depth),newid(),meta->ID);
		ATOMS *instantiation 	= copyATOMS(m,p->Instantiation);
		return initCONTEXT(meta,lexic,support,disport,obsoletion,instantiation);
	}
	freeMETA(meta);
	return NULL;
}

CONTEXTS *copyCONTEXTS(META *meta,CONTEXTS *p){
	if(meta->Depth>=0){
		CONTEXTS *np 				= initCONTEXTS(meta);
		META		*m;
		CONTEXT  *sp;
		while(np->Count < p->Count){
			sp = p->List[np->Count];
			m 							= initMETA(sp->Meta->Tag,sp->Meta->Author,CONTEXTStoCONTEXTdepth(meta->Depth),newid(),meta->ID);
			np->List[np->Count] 	= copyCONTEXT(m,sp);
			np->Count++;
		}
		return np;
	}
	freeMETA(meta);
	return NULL;
}

ENTRY *copyENTRY(META *meta,ENTRY *p){
	if(meta->Depth>=0){
		return initENTRY(meta,p->Term,p->Definition);
	}
	freeMETA(meta);
	return NULL;
}

LEXIC *copyLEXIC(META *meta,LEXIC *p){
	if(meta->Depth>=0){
		LEXIC 	*np 				= initLEXIC(meta);
		META		*m;
		ENTRY	   *sp;
		while(np->Count < p->Count){
			sp = p->List[np->Count];
			m 							= initMETA(sp->Meta->Tag,sp->Meta->Author,LEXICtoENTRYdepth(meta->Depth),newid(),meta->ID);
			np->List[np->Count] 	= copyENTRY(m,sp);
			np->Count++;
		}
		return np;
	}
	freeMETA(meta);
	return NULL;
}

