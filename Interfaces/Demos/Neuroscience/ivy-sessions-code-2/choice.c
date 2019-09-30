/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                       C H O I C E  . C                                       */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
#include "tete.h"
/************************************************************************************************/
/*                                                                                      */
/************************************************************************************************/
char *getFirstArgument(int argc, char *argv[]){
	static char choice[5000];
	if(argc>=2 && (strcmp(argv[1],"S")==0 || strcmp(argv[1],"P")==0 || strcmp(argv[1],"B")==0 || strcmp(argv[1],"M")==0 ) ){
		strcpy(choice,argv[1]);
	} else {
		printf("\n\t(0) Please choose and type:\n");
		printf("\t    'B' to use basic source interface\n");
		printf("\t    'S' if you want to create some files based on a predefined question unit.\n");
		printf("\t    'P' if you want to play the part of a neuroscientist registering a new question unit.\n");
		printf("\t    'M' if you want to two pre-registered studies.\n");
		strcpy(choice,promptandscan("CHOICE",false));
		while( (strcmp(choice,"S")!=0) && (strcmp(choice,"P")!=0)  && (strcmp(choice,"M")!=0) && strcmp(choice,"B")!=0  ){
			strcpy(choice,promptandscan("",false));   
		}
	}
	return choice;
}
/************************************************************************************************/
/*                                                                                      */
/************************************************************************************************/
char *getAfilename(int argc, char *argv[],int i,char *defaultfilename,char *prompt){
	static char filename[5000];
	if(argc>i+1){
		strcpy(filename,argv[i]);
	} else {
		printf("%s (if you skip, defaulting to %s)\n",prompt,defaultfilename);
		strcpy(filename,promptandscan("FILENAME",false));
		if(strlen(filename)<=0){
			strcpy(filename,defaultfilename);
		}		
	}
	return filename;
}
/************************************************************************************************/
/*    checkDeallocation                                                                         */
/************************************************************************************************/
int checkDeallocation(int f){
	if( f!= 0){
		fprintf(stderr, "Failed to free all memory allocations! (alloCount = %d)\n",f);	
	} else{
		printf("All memory allocations freed (alloCount = %d).\n",f);	
	}
	return f;
}
/************************************************************************************************/
/*                                                                                      */
/************************************************************************************************/
void showContent(char *prompt, ATOM *p){
	printLineOnScreen();
	printf("\n\n%s\n\n",prompt);
	writeForestQUESTION(stdout,30,p," ");
	//writeMarkupQUESTION(stdout,p,true);printf("\n\nLATEX FILE NOW\n\n\n");
	//writeLatexQUESTION(stdout,p,"  ");printf("\n\n");;printf("\n\nTIKZ FILE NOW\n\n\n");
	//writeTikzQUESTION(stdout,p,"  ");printf("\n\n");
	printLineOnScreen();
}

/************************************************************************************************/
/* write content                                                                                */
/************************************************************************************************/
void writeContent(int argc, char *argv[],int i,char *defaultfilename,ATOM *p){
	printf("Do you want to record output Latex and tree-like visualisation files? ");
	if(scanYesNo("", "a")){
		static char filename[5000];
		if(argc>i+1){
			strcpy(filename,argv[i]);
		} else {
			printf("Please type the name of the folder you want to create for the outputs (if you skip, defaulting to %s)\n",defaultfilename);
			strcpy(filename,promptandscan("FILENAME",false));
			if(strlen(filename)<=0){
				strcpy(filename,defaultfilename);
			}		
		}
		//strcpy(filename,questionToPointlessStatement(q->Text));
		createFiles(filename,p);
	}
}
/************************************************************************************************/
/*  Basic source interface                                                                      */
/************************************************************************************************/
int basicsourceinterface(){
	printf("\nUsing basic source interface...\n\n");
	/* Make object */
	META *m=initMETA(QUESTIONTAG,promptandscan("Author(s)",false),QUESTIONdepth(0),newid(),"NONE");
	ATOM *p = getATOM(9,m); 
	/* Show result */
	showContent("This is what the study looks like in MMM format:",p);
	/* record in files */
	createFiles(DEFAULTNAME,p);
	/* Free object */
	return checkDeallocation(freeATOM(p));
}
/************************************************************************************************/
/* buildFromFile                                                                                */
/************************************************************************************************/
int buildFromFile(int argc, char *argv[]){
	int DEPTH=12;
	/* Get the file to read from */
	char filename[5000];
	strcpy(filename,getAfilename(argc,argv,2,DEFAULTFILETOREAD,"\t(0) What is the name of the markup file where the study is described?"));
	/* */
	printf("\nUsing basic reading source interface, reading file %s ...\n\n",filename);
	/* OPen file */
	FILE *fp=openTheFile(filename); 
	int r=1;
	/* Read next bit */
	readUntilNextTag(fp);
	/**/
	if(CURRENTTAGISOPEN){
		if(strcmp(CURRENTTAG,ATOMTAG)==0){
			/* Atom */
			/* Make object */
			ATOM *p = readATOM(fp,0);	
			/* Show result */
			showContent("This is what the  question I read looks like in MMM format:",p);
			/* Record result */
			writeContent(argc, argv,3,stampString(DEFAULTOUTPUT),p);
			/* Free object */
			r = checkDeallocation(freeATOM(p));
		} else if(strcmp(CURRENTTAG,ATOMSTAG)==0){
			/* Atoms */
			ATOMS *p = readATOMS(fp,0);
			writeForestATOMS(stdout,DEPTH,p," ",false,false,false);printf("\n\n");
			r = checkDeallocation(freeATOMS(p));
		} else if(strcmp(CURRENTTAG,CONTEXTSTAG)==0){
			/* Contexts */
			CONTEXTS *p = readCONTEXTS(fp,0);
			writeForestCONTEXTS(stdout,DEPTH,p," ",false,false);printf("\n\n");
			r = checkDeallocation(freeCONTEXTS(p));
		} else if(strcmp(CURRENTTAG,CONTEXTTAG)==0){
			/* Context */
			CONTEXT *p = readCONTEXT(fp,0);
			writeForestCONTEXT(stdout,DEPTH,p," ",false,false);printf("\n\n");
			r = checkDeallocation(freeCONTEXT(p));	
		} else if(strcmp(CURRENTTAG,LEXICTAG)==0){
			/* Lexic */
			LEXIC *p = readLEXIC(fp,0);
			writeForestLEXIC(stdout,DEPTH,p," ");printf("\n\n");
			r = checkDeallocation(freeLEXIC(p));
		} else if(strcmp(CURRENTTAG,ENTRYTAG)==0){
			/* Entry */
			ENTRY *p = readENTRY(fp,0);
			writeForestENTRY(stdout,DEPTH,p," ");printf("\n\n");
			r = checkDeallocation(freeENTRY(p));
		} else if(strcmp(CURRENTTAG,METATAG)==0){
			/* Meta */
			META *p = readMETA(fp,DEPTH);
			writeForestMETA(stdout,DEPTH,p," ");printf("\n\n");
			r = checkDeallocation(freeMETA(p));
		}
	}
	fclose(fp);
	return r;
}

/************************************************************************************************/
/*                                                                                      */
/************************************************************************************************/
int playNeuroscientist(int argc, char *argv[]){
	ATOM *p,*pp;
	FILE *fp;
	char filename[5000];
	/* Status */
	printf("Have you finished working on the study you want to register?\n");
	if(!scanYesNo("", "--")){
	/* Not Finished */
		p = registerStudy();
		showContent("This is what the study looks like in MMM format:",p);
		writeContent(argc, argv,2,stampString(DEFAULTOUTPUT),p);
		//strcpy(filename,questionToPointlessStatement(q->text));
		return checkDeallocation(freeATOM(p));
	} else {
	/* Finished */
		printf("Did you previously register an earlier version of this study?\n");
		if(!scanYesNo("", "--")){
			/* From scratch (idem above: presently the program does not deal neatly with this case) */
			presentlyNote();
			p = registerStudy();
			showContent("This is what the study looks like in MMM format:",p);
			writeContent(argc, argv,2,stampString(DEFAULTOUTPUT),p);
			return checkDeallocation(freeATOM(p));
		} else {
			/* From earlier version */
			/* 1. Get the file to read from */
			char filename[5000];
			strcpy(filename,getAfilename(argc,argv,2,DUMMYMARKUP,"What is the name of the markup file where the MMM is described?"));
			/* 1. Open file */
			fp=openTheFile(filename); 
			/* 1. Make object */
			p = readATOM(fp,0);	
			/* Close file */
			fclose(fp);
			/* 1. Show result */
			showContent("This is what the study recorded earlier looked like:",p);
			/* 2. Make object */
			pp = closeStudy(p);
			/* 2. Show result */
			showContent("This is what the closed study now looks like:",pp);
			/* 2. Record result */
			writeContent(argc, argv,3,stampString(DEFAULTOUTPUT),pp);
			/* Free objects */
			return checkDeallocation(freeATOM(p)) + checkDeallocation(freeATOM(pp));
		}
	}
}
/************************************************************************************************/
/*                                                                                      */
/************************************************************************************************/
int tryMerge(int argc, char *argv[]){
	char filename1[5000],filename2[5000],filename[5000];
	/* 1. Get filename */
	strcpy(filename1,getAfilename(argc,argv,2,DUMMYMARKUP,"\t(0) What is the name of the markup file where the 1st study is described?"));
	/* 1. Open file */
	FILE *fp1=openTheFile(filename1);
	/* 1. Make object */
	ATOM *p1 = readATOM(fp1,0);
	/* 1. Show on screen */
	showContent("This is what the 1st study looks like in MMM format:",p1);
	/* 2. Get filename */
	strcpy(filename2,getAfilename(argc,argv,3,DUMMYMARKUP,"\t(0) What is the name of the markup file where the 2nd study is described?"));
	/* 2. Open file */
	FILE *fp2=openTheFile(filename2);
	/* 2. Make object */
	ATOM *p2 = readATOM(fp2,0);
	/* 2. Show on screen */
	showContent("This is what the 1st study looks like in MMM format:",p2);
	/* 3. Get filename */
	strcpy(filename,filename1);
	strcat(filename,"X");
	strcat(filename,filename2);
	/* 3. Make object / Merge */
	ATOM *p = mergeQuestions(p1,p2);
	/* 3. Write content */
	writeContent(argc, argv,4,stampString(filename),p);
	/* Free objects */
	return checkDeallocation(freeATOM(p1))+checkDeallocation(freeATOM(p2))+checkDeallocation(freeATOM(p));
}

/************************************************************************************************/
/*                                                                                      */
/************************************************************************************************/
