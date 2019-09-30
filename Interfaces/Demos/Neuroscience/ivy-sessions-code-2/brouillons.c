/************************************************************************************************/
/* REFERENCE                                                                                    */
/************************************************************************************************/
ATOM *getREFERENCE(ATOM *p){
	ATOM *reference=NULL;
	char text[5000],author[5000];
	unmberedinstruction("Please give reference for this statement if you know it.");
	strcpy(text,promptandscan(REFPROMPT,false));	
	if(strlen(text)>0){
		// Missing note on referencing others works
		// Missing note on different kinds: DOI or ref in MMM
		unmberedinstruction("Please list authors of this reference.");
		strcpy(author,promptandscan(AUTHORPROMPT,false));	
		strcat(author,"(referenced by ");
		strcat(author,p->Meta->Author);
		strcat(author,")");
		reference = initATOM(initMETA(REFTAG,author,ATOMtoATOMdepth(p->Meta->Depth),newid(),p->Meta->ID),text,NULL,NULL); // ! parentID is directly referencee !
	}
	return reference;
}/************************************************************************************************/
/*                                                                                      */
/************************************************************************************************/
int buildFromFile(int argc, char *argv[]){
	/* Get the file to read from */
	char filename[5000];
	strcpy(filename,getAfilename(argc,argv,2,DEFAULTFILETOREAD,"\t(0) What is the name of the markup file where the study is described?"));
	/* */
	printf("\nUsing 'build from file', reading file %s ...\n\n",filename);
	/* open file */
	FILE *fp=openTheFile(filename);
	/* make object */
	ATOM *p = readATOM(fp,0);
	showContent("This is what the  question I read looks like:",p);
			r = checkDeallocation(freeATOM(p));

	/* Output */
	//strcpy(filename,questionToPointlessStatement(q->Text));
	printf("Do you want to create folder '%s' with output Latex and tree-like visualisation files? ",DEFAULTNAME);
	if(scanYesNo("", "a")){
		createFiles(DEFAULTNAME,p);
	}
	/* Free object */
	return checkDeallocation(freeATOM(p));
}
/************************************************************************************************/
/*                                                                                      */
/************************************************************************************************/
void getSecondArgument(int argc, char *argv[], char *arguments[]){
	if(argc>3){
		arguments[1] = (char *) malloc (strlen(argv[2]) + 1);				
		strcpy(arguments[1],argv[2]);
	} else {
		printf("\t(0) What is the name of the markup file where the study is described? (if you skip, defaults to %s)\nFILENAME ",DUMMYMARKUP);
		char filename[5000];
		strcpy(filename,promptandscan("FILENAME"));
		if(strlen(filename)>0){
			arguments[1] = (char *) malloc (strlen(filename + 1));				
			strcpy(arguments[1],filename);
		} else {
			arguments[1] = (char *) malloc (strlen(DUMMYMARKUP) + 1);				
			strcpy(arguments[1],DUMMYMARKUP);
		}		
	}
}
/************************************************************************************************/
/*                                                                                      */
/************************************************************************************************/
void getThirdArgument(int argc, char *argv[], char *arguments[]){
	if(argc>4){
		arguments[2] = (char *) malloc (strlen(argv[3]) + 1);				
		strcpy(arguments[2],argv[3]);
	} else {
		printf("\t(0) What is the name of the markup file where the study is described? (if you skip, defaults to %s)\nFILENAME ",OTHERDUMMYMARKUP);
		char filename[5000];
		strcpy(filename,promptandscan("FILENAME"));
		if(strlen(filename)>0){
			arguments[2] = (char *) malloc (strlen(filename) + 1);				
			strcpy(arguments[2],filename);
		} else {
			arguments[2] = (char *) malloc (strlen(DUMMYMARKUP) + 1);				
			strcpy(arguments[2],DUMMYMARKUP);
		}		
	}
}/************************************************************************************************/
/*                                                                                      */
/************************************************************************************************/
void dealWithArguments(int argc, char *argv[], char *arguments[]){
	int i;
	if(argc == 1){
		/* No arguments given */
		arguments[0] = (char *) malloc (2);										strcpy(arguments[0],"Z");
		arguments[1] = (char *) malloc (strlen(DUMMYMARKUP) + 1);		strcpy(arguments[1],DUMMYMARKUP);
		arguments[2] = (char *) malloc (strlen(OTHERDUMMYMARKUP) + 1);	strcpy(arguments[2],OTHERDUMMYMARKUP);
	} else if (argc ==2) {
		/* A choice given */
		arguments[0] = (char *) malloc (strlen(argv[1]) + 1);				strcpy(arguments[0],argv[1]);
		arguments[1] = (char *) malloc (strlen(DUMMYMARKUP) + 1);		strcpy(arguments[1],DUMMYMARKUP);
		arguments[2] = (char *) malloc (strlen(OTHERDUMMYMARKUP) + 1);	strcpy(arguments[2],OTHERDUMMYMARKUP);
	} else if (argc ==3) {
		/* A choice and a file given */
		arguments[0] = (char *) malloc (strlen(argv[1]) + 1);				strcpy(arguments[0],argv[1]);
		arguments[1] = (char *) malloc (strlen(argv[2]) + 1);				strcpy(arguments[1],argv[2]);
		arguments[2] = (char *) malloc (strlen(OTHERDUMMYMARKUP) + 1);	strcpy(arguments[2],OTHERDUMMYMARKUP);
	} else if (argc >=4) {
		/* A choice and 2 files given */
		arguments[0] = (char *) malloc (strlen(argv[1]) + 1);				strcpy(arguments[0],argv[1]);
		arguments[1] = (char *) malloc (strlen(argv[2]) + 1);				strcpy(arguments[1],argv[2]);
		arguments[2] = (char *) malloc (strlen(argv[3]) + 1);				strcpy(arguments[2],argv[3]);
	}
	for(i=0;i<3;i++){
		printf("arguments[%d] = %s\n",i,arguments[i]);
	}
}/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                   W R I T E . C                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
#include "tete.h"
/************************************************************************************************/
/* META                                                                                         */
/************************************************************************************************/

/************************************************************************************************/
/* QUESTION                                                                                     */
/************************************************************************************************/
int writeQUESTION(FILE *fp,ATOM *a){
}
/************************************************************************************************/
/* ATOM                                                                                         */
/************************************************************************************************/
int writeATOM(FILE *fp,ATOM *a){
}
/************************************************************************************************/
/* ATOMS                                                                                        */
/************************************************************************************************/
int writeATOMS(FILE *fp,ATOMS *a,bool entitle){
}
/************************************************************************************************/
/* CONTEXT                                                                                      */
/************************************************************************************************/
int writeCONTEXT(FILE *fp,CONTEXT *c){
}
/************************************************************************************************/
/* ENTRY                                                                                        */
/************************************************************************************************/
int writeENTRY(FILE *fp,ENTRY *e){
}
/************************************************************************************************/
/* LEXIC                                                                                        */
/************************************************************************************************/
int writeLEXIC(FILE *fp,LEXIC *l){
}



char *scanTaggedLine(FILE* fp){
	int i=0;
	static char tag[5000],text[5000];
	char ch = fgetc(fp); 
	/* remove emptiness before */
	while(ch == '\n' || ch == '\t' ch == ' '){
		ch = fgetc(fp);
	}
	/* Fnd beginning of opening tag */
	if(ch!='<'){
		fprintf(stderr, "I was expecting an opening tag but I failed to read a '<' character!\n");		
		return NULL;
	}
	/* Get opening tag */
	while(ch != '>'){
		tag[i]=ch; 
		ch = fgetc(fp);
		i++; 
	}
	tag[i] = '\0'; 
	/* Get text before closing tag */
	i=0;
	while(ch != '<'){
		text[i]=ch; 
		ch = fgetc(fp);
		i++; 
	}
	text[i] = '\0'; 
	/* Check closing tag */
	ch = fgetc(fp); 
	if(ch!='/'){
		fprintf(stderr, "Failed to read '/' character in closing %s tag after reading text %s!\n",tag,text);		
		return NULL;
	}
	/* Get closing tag */
	ch = fgetc(fp);
	i=0;
	while(ch != '>' ){
		if(ch!=tag[i]){
			fprintf(stderr, "Failed to read closing <%s> tag!\n",tag);
			return NULL;
		}
		ch = fgetc(fp);
		i++;
	}
	return text;
}

char *scanTextLineEnd(FILE* fp,char *tag){
	int i=0,j=0;
	static char text[2000];
	char endTag[2000], ch = fgetc(fp); 
	/* Get text before closing tag */
	while(ch != '<'){
		text[i]=ch; 
		ch = fgetc(fp);
		i++; 
	}
	/* Stop text here */
	text[i] = '\0'; 
	/* Check closing tag */
	ch = fgetc(fp); 
	if(ch!='/'){
		fprintf(stderr, "Failed to read '/' character in closing %s tag after reading text %s!\n",tag,text);		
		return NULL;
	}
	/* Get tag */
	ch = fgetc(fp);
	while(ch != '>' ){
		if(ch!=tag[j]){
			fprintf(stderr, "Failed to read closing <%s> tag!\n",tag);
			return NULL;
		}
		ch = fgetc(fp);
		j++;
	}
	return text;
}




















///////////

bool *findTag(FILE* fp, char *wantedtag){
	char ch = fgetc(fp); 
	int i=0;
	/* remove emptiness before */
	while(ch == '\n' || ch == '\t' ch == ' '){
		ch = fgetc(fp);
	}
	/* Fnd beginning of opening tag */
	if(ch!='<'){
		fprintf(stderr, "I was expecting an opening tag but I failed to read a '<' character!\n");		
		return NULL;
	}
	/* Get opening tag */
	while(ch != '>'){
		if(ch!=wantedtag[i]){
			fprintf(stderr, "I was expecting tag %s!\n",wantedtag);		
			return false;
		}
		ch = fgetc(fp);
		i++; 
	}
	return true;
}
bool *findClosingTag(FILE* fp, char *wantedtag){
	char ch = fgetc(fp); 
	int i=0;
	/* remove emptiness before */
	while(ch == '\n' || ch == '\t' ch == ' '){
		ch = fgetc(fp);
	}
	/* Fnd beginning of opening tag */
	if(ch!='<'){
		fprintf(stderr, "I was expecting an opening tag but I failed to read a '<' character!\n");		
		return NULL;
	}
	/* Get opening tag */
	while(ch != '/'){
		if(ch!=wantedtag[i]){
			fprintf(stderr, "I was expecting closing tag %s!\n",wantedtag);		
			return false;
		}
		ch = fgetc(fp);
		i++; 
	}
	ch = fgetc(fp);
	if(ch!='>'){
		fprintf(stderr, "I was expecting an closing tag but I failed to read a '>' character at the end!\n");		
		return NULL;
	}	
	return true;
}

bool findTagsGetContentInBetween(FILE* fp, char *wantedtag, char *text){
	int i=0;
	static char tag[5000];
	char ch = fgetc(fp); 
	/* remove emptiness before */
	while(ch == '\n' || ch == '\t' ch == ' '){
		ch = fgetc(fp);
	}
	/* Fnd beginning of opening tag */
	if(ch!='<'){
		fprintf(stderr, "I was expecting an opening tag but I failed to read a '<' character!\n");		
		return false;
	}
	/* Get opening tag */
	while(ch != '>'){
		tag[i]=ch; 
		ch = fgetc(fp);
		i++; 
	}
	tag[i] = '\0'; 
	/* Check it's te tag we wanted */
	if(strcmp(tag,wantedtag)!=0){
		fprintf(stderr, "I was expecting tag %s but found %s!\n",wantedtag,tag);		
		return false;		
	}
	/* Get text before closing tag */
	i=0;
	while(ch != '<'){
		text[i]=ch; 
		ch = fgetc(fp);
		i++; 
	}
	text[i] = '\0'; 
	/* Check closing tag */
	ch = fgetc(fp); 
	if(ch!='/'){
		fprintf(stderr, "Failed to read '/' character in closing %s tag after reading text %s!\n",tag,text);		
		return false;
	}
	/* Get closing tag */
	ch = fgetc(fp);
	i=0;
	while(ch != '>' ){
		if(ch!=tag[i]){
			fprintf(stderr, "Failed to read closing <%s> tag!\n",tag);
			return false;
		}
		ch = fgetc(fp);
		i++;
	}
	return true;
}
/////////////////


int findCurrentTag(FILE *fp,char *text){
	int i=0,kind=1;
	static char tag[5000];
	char ch = fgetc(fp); 
	/* remove emptiness before */
	while(ch == '\n' || ch == '\t' ch == ' '){
		ch = fgetc(fp);
	}
	/* Fnd beginning of tag */
	if(ch!='<'){
		fprintf(stderr, "I was expecting a tag but I failed to read a '<' character!\n");		
		return -1;
	}
	/* Determine if it's a closing or opening tag */
	ch = fgetc(fp);
	if(ch=='/'){
		kind=0;
		ch = fgetc(fp);
	}
	/* Get tag */
	while(ch != '>'){
		CURRENTTAG[i]=ch; 
		ch = fgetc(fp);
		i++; 
	}
	CURRENTTAG[i] = '\0'; 
	return kind;
}

bool readContentInBetween(FILE* fp, char *wantedtag, char *content){
	/* Opening tag */
	if(findCurrentTag(fp)==0 || strcmp(CURRENTTAG,wantedtag)!=0){
		fprintf(stderr,"I was expecting an opening %s tag. Found a (closing) tag %s!\n",wantedtag,CURRENTTAG);
		return false;
	}
	/* In between */
	
	/* Closing Tag */
	if(findCurrentTag(fp)==1 || strcmp(CURRENTTAG,ATOMTAG)!=0){
		fprintf(stderr,"I was expecting a closing %s tag. Found a (opening) tag %s!\n",ATOMTAG,CURRENTTAG);
		return false;
	}
	/* */
	return true;
}


	int i=0;
	static char tag[5000];
	char ch = fgetc(fp); 
	/* remove emptiness before */
	while(ch == '\n' || ch == '\t' ch == ' '){
		ch = fgetc(fp);
	}
	
	/* Get text before closing tag */
	i=0;
	while(ch != '<'){
		text[i]=ch; 
		ch = fgetc(fp);
		i++; 
	}
	text[i] = '\0'; 
	
	
}


///////
int findCurrentTag(FILE *fp,char *text){
	int i=0,kind=1;
	static char tag[5000];
	char ch = fgetc(fp); 
	/* remove emptiness before */
	while(ch == '\n' || ch == '\t' ch == ' '){
		ch = fgetc(fp);
	}
	/* Fnd beginning of tag */
	if(ch!='<'){
		fprintf(stderr, "I was expecting a tag but I failed to read a '<' character!\n");		
		return -1;
	}
	/* Determine if it's a closing or opening tag */
	ch = fgetc(fp);
	if(ch=='/'){
		kind=0;
		ch = fgetc(fp);
	}
	/* Get tag */
	while(ch != '>'){
		CURRENTTAG[i]=ch; 
		ch = fgetc(fp);
		i++; 
	}
	CURRENTTAG[i] = '\0'; 
	return kind;
}

/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/
int readUntilNextTagold(FILE* fp,int wantedkind,char *wantedtag,char *text){
	int i=0,kind=1;
	char tag[5000];
	char ch = readnextcharacter(fp);
	/* remove emptiness before */
	while(ch == '\n' || ch == '\t' || ch == ' '){
		ch = readnextcharacter(fp);
	}
	/* Get text before tag */
	while(ch != '<'){
		if(ch==EOF){
			fprintf(stderr, "I was expecting a tag but I failed to read a '<' character!\n");		
			return -1;
		}
		text[i]=ch; 
		ch = readnextcharacter(fp);
		i++; 
	}
	text[i] = '\0'; 
	/* Determine if it's a closing or opening tag */
	ch = readnextcharacter(fp);
	if(ch=='/'){
		kind=0; //printf("\ncame here\n ");
		ch = readnextcharacter(fp);
	}
	if(wantedkind!=kind){
		fprintf(stderr,"I was expecting a%s %s tag but got a%s one! Aborting all! kind =%d\n",(wantedkind==1)? "n opening":" closing",wantedtag,(wantedkind==1)? " closing": "n opening",kind);
	}
	/* Get tag */
	i=0;
	while(ch != '>'){
		CURRENTTAG[i]=ch; 
		ch = readnextcharacter(fp);
		i++; 
	}
	CURRENTTAG[i] = '\0'; 
	if(strcmp(CURRENTTAG,wantedtag)!=0){
		fprintf(stderr, "I was expecting the tag %s but I found %s instead! Aborting all!\n",wantedtag,CURRENTTAG);		
		return -1;
	}
	CURRENTTAG[i] = '\0'; printf("\n%s CURRENTTAG=%s text=%s\n",(kind==1)? "--->\tOPENING":"<---\tCLOSING",CURRENTTAG,text);
	return kind;
}



