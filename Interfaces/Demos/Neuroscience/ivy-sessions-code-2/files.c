/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                           F I L E S . C                                      */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
#include "tete.h"
int shortlength=SHORTLENGTH;
/************************************************************************************************/
/* visualisefile                                                                                */
/************************************************************************************************/
void visualisefile(FILE *fp,char *filename){
   char ch;
	printf("The contents of %s file are:\n", filename);
	while((ch = fgetc(fp)) != EOF)
	printf("%c", ch);
}
/************************************************************************************************/
/*  OPEN                                                                                        */
/************************************************************************************************/
FILE *openTheFile(char *string){
	/* Prepare filename */
	char filename[5000];
	strcpy(filename,string);
	int i=strlen(filename)-1;
	while(i>0 && filename[i]==' '){
		i--;	
	}
	filename[i+1]='\0';
	/* Open file */
	FILE *fp = fopen(filename,"r"); 
   if( fp == NULL){
		fprintf(stderr, "Failed to open file '%s'!\n",filename);		
		return NULL;
	}
	printf("Opened filename = %s\n",filename);
	/* */
	return fp;
}
/************************************************************************************************/
/* OUTPUT FILES                                                                                 */
/************************************************************************************************/
void createFiles(char givenname[], ATOM *p){
	printf("\n");repeatfprintf(stdout,"_",120);printf("\n");
	/* Open general output Folder */
	int r = mkdir(OUTPUTFOLDER,0777);
	if(errno == EEXIST ){
	} else if(r && errno != EEXIST ){
   	printf("Error while trying to create folder %s! (r=%d) errno = %d\n",OUTPUTFOLDER,r,errno);
	} else{
		printf("Created folder %s\n",OUTPUTFOLDER);
	} 		
	char name[5000], filename[5000],folder[5000];
	/* sub output folder */
	strcpy(name,ReplaceSpaces(givenname));
	errno=0;
	snprintf(folder,5000,"%s/%s",OUTPUTFOLDER,name);
	r= mkdir(folder,0777);
	if(errno == EEXIST ){
		printf("Folder %s already exists: overwriting its contents!\n",folder);
	} else if(r && errno != EEXIST ){
   	printf("Error while trying to create folder %s! (r=%d) errno = %d\n",folder,r,errno);
	} else{
		printf("Created folder %s\n",folder);
	} 		
	/* Forest */
	snprintf(filename,5000,"%s/%s-Forest.txt",folder,name);
	FILE *fp = fopen(filename,"w+"); 
	if( fp ){
		shortlength = -1;
		writeForestQUESTION(fp,30,p,"   ");
		printf( "Wrote 'tree-like visualisation' file:\t %s \n", filename);
	}
	/* Short Forest */
	snprintf(filename,5000,"%s/%s-Forest-short.txt",folder,name);
	fp = fopen(filename,"w+"); 
	if( fp ){
		shortlength = SHORTLENGTH;
		writeForestQUESTION(fp,30,p,"   ");
		printf( "Wrote 'tree-like visualisation' file:\t %s \n", filename);
	}
	fclose(fp);
	/* Markup */
	snprintf(filename,5000,"%s/%s-MU.txt",folder,name);
	fp = fopen(filename,"w+"); 
	if( fp ){
		shortlength = -1;
		writeMarkupQUESTION(fp,p,true);
		printf( "Wrote markup file:\t\t\t %s \n", filename);
	}
	fclose(fp);
	/* Short Markup */
	snprintf(filename,5000,"%s/%s-MU-short.txt",folder,name);
	fp = fopen(filename,"w+"); 
	if( fp ){
		shortlength = SHORTLENGTH;
		writeMarkupQUESTION(fp,p,true);
		printf( "Wrote markup file:\t\t\t %s \n", filename);
	}
	fclose(fp);
	/* Latex Long and short */
	snprintf(filename,5000,"%s/%s-Latex.tex",folder,name);
	fp = fopen(filename,"w+"); 
	if( fp ){
		shortlength = -1;
		fprintf(fp,"\\subsection{%s (long)}\n\n",givenname);
		writeLatexQUESTION(fp,p,"");
		fprintf(fp,"\n\n");
		writeTikzQUESTION(fp,p,"");
		fprintf(fp,"\n\n\\subsection{%s (short)}\n\n",givenname);
		shortlength = SHORTLENGTH;
		writeLatexQUESTION(fp,p,"");
		fprintf(fp,"\n\n");
		writeTikzQUESTION(fp,p,"");
		printf( "Wrote Latex file:\t\t\t %s \n", filename);
	} else 	if(fp == NULL) {
      printf("Error opening file %s! errno = %d\n",filename,errno);   
      exit(1);             
	}
	fclose(fp);
	printf("\n");repeatfprintf(stdout,"_",120);printf("\n");
}