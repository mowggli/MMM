/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                      T E X T  . C                                            */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
#include "tete.h"
/************************************************************************************************/
/*  Question <~~~~> Statement                                                                   */
/************************************************************************************************/
char *statementToQuestion(char *a){
	static char q[5000];
	snprintf(q,5000,"Is it true that ' %s '?",a);
	return q;
}

char *questionToStatement(char *q){
	static char s[5000];
	char debut[]="Is it true that ' ";
	int i=0,l=strlen(debut);
	bool test=true;
	while(test && i<l){
		test = (debut[i]==q[i]);i++;
	}
	if(test){
		strncpy(s, q+l, strlen(q)-l-3);
		if(s[strlen(q)-l-4]!='.'){//printf("adding point\n");
			s[strlen(q)-l-3]='.';
			s[strlen(q)-l-2]='\0';
		} else{
			s[strlen(q)-l-3]='\0';
		}
	} else {
		snprintf(s,5000,"Yes to ' %s '.",q);
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
/* removeMarks                                                                                  */
/************************************************************************************************/
char *removeMarks(char* source){ /* source is supposed to be a tag in between < and > */
	if(source[0]=='<' && source[strlen(source)-1]=='>'){
		int i;
		static char output[2000];
		for(i=0;i<strlen(source)-2;i++){
			output[i]=source[i+1];
		}
		output[strlen(source)-2] = '\0';
		return output;
	} else {
		fprintf(stderr,"Expected keyword surrounded by '<' and '>' characters but got just '%s'!\n",source);
	}
	return NULL;
}
/************************************************************************************************/
/*                                                                                      */
/************************************************************************************************/
char *resizetext(char *text,char *ending){
	if(SHORTLENGTH<0 || strlen(text)<=SHORTLENGTH){
		return text;
	} else{
		static char newtext[5000];
		strncpy(newtext,text,SHORTLENGTH);
		strcat(newtext,ending);
		newtext[SHORTLENGTH+strlen(ending)] = '\0'; 
		return newtext;
	}
}
/************************************************************************************************/
/* Replace or remove spaces                                                                     */
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
		if(source[k]==' ' || source[k]=='\n'||source[k]=='\t'){
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
/* REPEAT                                                                                       */
/************************************************************************************************/
void fprintfRepeat(FILE *fp, char *c, int i){
	int j;
	for(j=0;j<i;j++){
		fprintf(fp, "%s",c);
	}
}
void repeatfprintf(FILE *fp,char *s, int n){
	int i;
	for (i=0;i<n;i++){
		fprintf(fp,"%s",s);
	}
}
/************************************************************************************************/
/*                                                                                      */
/************************************************************************************************/
bool startsWith(const char *pre, const char *str){
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? false : strncmp(pre, str, lenpre) == 0;
}
/************************************************************************************************/
/*                                                                                      */
/************************************************************************************************/
char *lowerthecase(char *input){
	if(strlen(input)>0){
		static char output[5000];
		strcpy(output,input);
		output[0]=toupper(output[0]);
		int i=1;
		while(output[i] !=0){
			output[i]=tolower(output[i]);
			i++;
		}
		return output;
	}	else return "";
}
/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/
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
/*   read addendum                                                                              */
/************************************************************************************************/
char readnextcharacter(FILE *fp){
	char ch = fgetc(fp);//printf("+%c+",ch);
	return ch;
}
void readUntilNextTag(FILE* fp){
	int i=0;
	char ch = readnextcharacter(fp);
	/* remove emptiness before */
	while(ch == '\n' || ch == '\t' || ch == ' '){
		ch = readnextcharacter(fp);
	}
	/* Get text before tag */
	while(ch != '<'){
		if(ch==EOF){
			fprintf(stderr, "I was expecting a tag but I failed to read a '<' character!\n");	
		}
		CURRENTTEXT[i]=ch; 
		ch = readnextcharacter(fp);
		i++; 
	}
	CURRENTTEXT[i] = '\0'; 
	/* Determine if it's a closing or opening tag */
	ch = readnextcharacter(fp);
	if(ch=='/'){
		CURRENTTAGISOPEN=false;
		ch = readnextcharacter(fp);
	} else {
		CURRENTTAGISOPEN=true;
	}
	/* Get tag */
	i=0;
	while(ch != '>'){
		CURRENTTAG[i]=ch; 
		ch = readnextcharacter(fp);
		i++; 
	}
	CURRENTTAG[i] = '\0'; 		//printf("\t%s CURRENTTAG=%s CURRENTTEXT=%s\n",(CURRENTTAGISOPEN)? "--->\tOPENING":"<---\tCLOSING",CURRENTTAG,CURRENTTEXT);
}

/************************************************************************************************/
/*                                                                                      */
/************************************************************************************************/
char *stampString(char *string){
	time_t current_time;
   char* c_time_string;
    /* Obtain current time. */
    current_time = time(NULL);
    if (current_time == ((time_t)-1)){
        (void) fprintf(stderr, "Failure to obtain the current time.\n");
        exit(EXIT_FAILURE);
    }
    /* Convert to local time format. */
    c_time_string = ctime(&current_time);
    if (c_time_string == NULL) {
        (void) fprintf(stderr, "Failure to convert the current time.\n");
        exit(EXIT_FAILURE);
    }
    /* Print to stdout. ctime() has already added a terminating newline character. */
    (void) printf("Current time is %s", c_time_string);
   //exit(EXIT_SUCCESS);
	static char newstring[5000];
	strcpy(newstring,RemoveSpaces(string));
	strcat(newstring,"-");
	strcat(newstring,RemoveSpaces(c_time_string));
	return newstring;
}
/************************************************************************************************/
/*                                                                                      */
/************************************************************************************************/
void printLineOnScreen(){
	printf("\n");repeatfprintf(stdout,"_",120);printf("\n");
}