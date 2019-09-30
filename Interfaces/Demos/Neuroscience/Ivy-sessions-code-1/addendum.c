#include "tete.h"


/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                                I N S T R U C T I O N S                                       */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/



/************************************************************************************************/
/*                                  P R O M P T  &  S C A N                                     */
/************************************************************************************************/

int promptAndScan(char *prompt, char *text){
	printf("%s",prompt);
	int r=scanf("%[^\n]s\n",text);
	getchar();
	if(!r){*text='\0';}
	return r;
}


int promptAndScanInstruction(char *instruction, char *text, char *sub){
	char prompt[5000];
	int n;
	if(strcmp(sub,"")==0){
		n = promptno;
		promptno++;
	} else{
		n = promptno -1;
	}
	snprintf(prompt,5000,"\t(%d%s) %s",n,sub,instruction);
	return promptAndScan(prompt,text);
}

int promptAndScanManditoryInstruction(char *instruction, char *text, char *sub){
	int r = promptAndScanInstruction(instruction, text, sub);
	while(!r){
		//printf("\tPlease answer\n");
		r=scanf("%[^\n]s\n",text);
		getchar();
	}
	return r;
}


bool scanYesNo(char *instruction, char *sub){
	char prompt[5000], answer[]="Z";
	int n;
	if(strcmp(sub,"")==0){
		n = promptno;
		promptno++;
	} else{
		n = promptno -1;
	}
	snprintf(prompt,5000,"\t(%d%s) %s Please type 'Y' for yes, and 'N' for no.\nCHOICE ",n,sub,instruction);
	promptAndScan(prompt, answer);
	//promptAndScanInstruction(prompt,answer,"");
	while(strcmp(answer,"Y")!=0 && strcmp(answer,"N")!=0){
		promptAndScan("",answer);
	}
	return strcmp(answer,"N");
}

/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/

char * ReplaceSpaces(char source[]){
	static char name[5000];
	int len=0;
	int k;
	for(k=0;k<strlen(source);k++){ 
		if(source[k]==' '){
			name[len]='-';
			len ++;
		} else if (ispunct(source[k])>0){ }
		else{
			name[len]=source[k];
			len ++;
		}
	}
	name[len] = '\0';
	return name;
}

char * RemoveSpaces(char source[]){
	static char name[5000];
	int len=0;
	int k;
	for(k=0;k<strlen(source);k++){ 
		if(source[k]==' '){
		} else if (ispunct(source[k])>0){ }
		else{
			name[len]=source[k];
			len ++;
		}
	}
	name[len] = '\0';
	return name;
}

/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/


char *statementToQuestion(char *a){
	static char q[5000];
	snprintf(q,5000,"Is it true that ' %s '?",a);
	return q;
}
char *questionToStatement(char *q){
//printf("arrived with q = %s\n",q);
	static char s[5000];
	char debut[]="Is it true that ' ";
	int i=0,l=strlen(debut);
	bool test=true;
	while(test && i<l){
		//printf("i=%d\n",i);
		//printf("debut[%d]=%c q[%d] = %c\n",i,debut[i],i,q[i]);
		test = (debut[i]==q[i]);i++;
	}
	//printf("test=%d\n",(int) test);
	if(test){
		//printf("l=%d, strlen(q)=%d strlen(q)-l=%d\n",l,(int) strlen(q),(int)strlen(q)-l);
		strncpy(s, q+l, strlen(q)-l-3);
		if(s[strlen(q)-l-4]!='.'){//printf("adding point\n");
			s[strlen(q)-l-3]='.';
			s[strlen(q)-l-2]='\0';
		} else{//printf("not adding point\n");
			s[strlen(q)-l-3]='\0';
		}
	//printf("And now this is s now: >%s<\n",s);
	} else {
		snprintf(s,5000,"Yes to ' %s '.",q);
		//printf("coming out with s = %s\n",s);
	}
	return s;
}

char *questionToPointlessStatement(char *q){
	static char s[5000];
	strcpy(s,questionToStatement(q));
	s[strlen(s)-1]='\0';
	return RemoveSpaces(s);
}

/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/
void createFiles(char givenname[], ATOM *q){
	printf("\n----------------------------------------------------------------------------------------------\n");
	int r= mkdir(outputfolder,0777);
	if(errno == EEXIST ){
	} else if(r && errno != EEXIST ){
   	printf("Error while trying to create folder %s! (r=%d) errno = %d\n",outputfolder,r,errno);
	} else{
		printf("Created folder %s\n",outputfolder);
	} 		
	char name[5000], filename[5000],folder[5000];

	strcpy(name,ReplaceSpaces(givenname));
	//folder
	errno=0;
	snprintf(folder,5000,"%s/%s",outputfolder,name);
	r= mkdir(folder,0777);
	if(errno == EEXIST ){
		printf("Folder %s already exists: overwriting its contents!\n",folder);
	} else if(r && errno != EEXIST ){
   	printf("Error while trying to create folder %s! (r=%d) errno = %d\n",folder,r,errno);
	} else{
		printf("Created folder %s\n",folder);
	} 		
	// Forest
	snprintf(filename,5000,"%s/%s-Forest.txt",folder,name);
	FILE *fp = fopen(filename,"w+"); 
	if( fp ){
		shortlength = -1;
		fprintForestQUESTION(fp,q);
		printf( "Wrote 'tree-like visualisation' file:\t %s \n", filename);
	}
	// Short Forest
	snprintf(filename,5000,"%s/%s-Forest-short.txt",folder,name);
	fp = fopen(filename,"w+"); 
	if( fp ){
		shortlength = SHORTLENGTH;
		fprintForestQUESTION(fp,q);
		printf( "Wrote 'tree-like visualisation' file:\t %s \n", filename);
	}
	fclose(fp);
	// Markup
	snprintf(filename,5000,"%s/%s-MU.txt",folder,name);
	fp = fopen(filename,"w+"); 
	if( fp ){
		shortlength = -1;
		fprintMarkupQUESTION(fp,q);
		printf( "Wrote markup file:\t\t\t %s \n", filename);
	}
	fclose(fp);
	// Short Markup
	snprintf(filename,5000,"%s/%s-MU-short.txt",folder,name);
	fp = fopen(filename,"w+"); 
	if( fp ){
		shortlength = SHORTLENGTH;
		fprintMarkupQUESTION(fp,q);
		printf( "Wrote markup file:\t\t\t %s \n", filename);
	}
	fclose(fp);
	// Latex Long and short
	snprintf(filename,5000,"%s/%s-Latex.tex",folder,name);
	fp = fopen(filename,"w+"); 
	if( fp ){
		shortlength = -1;
		fprintf(fp,"\\subsection{%s (long)}\n\n",givenname);
		fprintLatexQUESTION(fp,q);
		fprintf(fp,"\n\n");
		fprintTikzQUESTION(fp,q);
		fprintf(fp,"\n\n\\subsection{%s (short)}\n\n",givenname);
		shortlength = SHORTLENGTH;
		fprintLatexQUESTION(fp,q);
		fprintf(fp,"\n\n");
		fprintTikzQUESTION(fp,q);
		printf( "Wrote Latex file:\t\t\t %s \n", filename);
	} else 	if(fp == NULL) {
      printf("Error opening file %s! errno = %d\n",filename,errno);   
      exit(1);             
	}
	fclose(fp);
	printf("----------------------------------------------------------------------------------------------\n");
}
/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/
void repeatfprintf(FILE *fp,char *s, int n){
	int i;
	for (i=0;i<n;i++){
		fprintf(fp,"%s",s);
	}
}


void framefprintf(FILE *fp,char *s, int n){
	printf("\t|");
	int i=(n-2-strlen(s))/2;
	repeatfprintf(stdout," ",i);
	printf("%s",s);
	i=n-i-2-strlen(s);
	repeatfprintf(stdout," ",i);
	printf("|\n");
}

/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/
char *resizetext(char *text){
	if(shortlength<0 || strlen(text)<=shortlength){
		return text;
	} else{
/*		char *newtext = (char *) malloc(shortlength +4);
		strncpy(newtext, text, shortlength+3);
		newtext[shortlength] = '.';
		newtext[shortlength+1] = '.';
		newtext[shortlength+2] = '.';
		newtext[shortlength+3] = '\0';
*/
		//char *newtext = (char *) malloc(shortlength +20);
		strncpy(newtext, text, shortlength);
		strcat(newtext, "... [CLICK TO SEE MORE]");//printf("\tshortlength = %d < strlen('%s') = %d\n",shortlength,text,(int) strlen(text));
		newtext[shortlength+23] = '\0'; 
		return newtext;
	}
}