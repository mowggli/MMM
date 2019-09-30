#include "tete.h"
void visualisefile(FILE *fp,char *filename){
   char ch;
	printf("The contents of %s file are:\n", filename);
	while((ch = fgetc(fp)) != EOF)
	printf("%c", ch);
}

/*
char * RemoveMarks(char* source){
	int i;
	static char output[2000];
	for(i=0;i<strlen(source);i++){
		if(source[i]!= '<' && source[i] != '>'){output[i]=source[i];}
	}
	while(*j != 0){
		*i = *j++;
		if(*i != '>' && *i != '<'){i++;}
		else{*i = '-';i++;}
	}
	*i = 0;
}*/

char * removeMarks(char* source){
	int i;
	static char output[2000];
	for(i=0;i<strlen(source)-2;i++){
		output[i]=source[i+1];
	}
	output[strlen(source)-2] = '\0';
	return output;
}
/*
char *scanKeyword(FILE* fp){
	int i=0;
	static char keyword[200];
	char ch= fgetc(fp);
	while(ch != '<'){ch = fgetc(fp); }
	ch = fgetc(fp);
	while(ch != '>'){keyword[i]=ch;ch = fgetc(fp);  i++;}
	keyword[i] = '\0'; printf("Keyword is %s\n", keyword);
	return keyword;
}*/

char *scanKeyword(FILE* fp){
	int i=0;
	static char keyword[2000];
	char ch= fgetc(fp);
	while(ch != '<'){ch = fgetc(fp); }
	ch = fgetc(fp);
	if(ch=='/'){
		keyword[i]=ch;
		i++;
		//printf("Closing ");
		ch = fgetc(fp);
	}
	while(ch != '>'){keyword[i]=ch;ch = fgetc(fp);  i++;}
	keyword[i] = '\0'; //printf("Keyword is %s\n", keyword);
	return keyword;
}

char *scanMustKeyword(FILE* fp, char *expectedKeyword){
	int i=0;
	static char keyword[2000];
	char ch= fgetc(fp); 
	while(ch != '<'){//printf("lost hereeee ch =%c\n",ch);
		ch = fgetc(fp); 
	}
	ch = fgetc(fp);
	while(ch != '>'){ keyword[i]=ch;ch = fgetc(fp);  i++;}
	keyword[i] = '\0'; //printf("Keyword is %s\n", keyword);
	if(strcmp(keyword,expectedKeyword)!=0){
		fprintf(stderr, "Problem with '%s' keyword!\n",expectedKeyword);		
		return NULL;
	}
	return keyword;
}

char *scanTextLineEnd(FILE* fp, char *keyword){
	//printf("The keyword given is %s\n",keyword);
	int i=0,j=0;
	static char text[2000];
	char endKeyword[2000];
	char ch= fgetc(fp); //printf("ch=%c\n",ch);
	while(ch != '<'){text[i]=ch; ch = fgetc(fp);i++; }
	text[i]= '\0'; //printf("text is: %s\n",text);
	ch = fgetc(fp); //printf("ch=%c\n",ch);
	if(ch!='/'){
		fprintf(stderr, "Failed to read '/' character in closing %s mark after reading text %s!\n",keyword,text);		
		return NULL;
	}
	while(ch != '>'){ch = fgetc(fp);endKeyword[j]=ch;  j++;}
	endKeyword[j-1] = '\0';
	//printf("End Keyword is %s\n", keyword);
	//for(i=0;i<6;i++){printf("endKeyword[%d]=%c\tkeyword[%d]=%c\n",i,endKeyword[i],i,keyword[i]);}
	if(strcmp(endKeyword,keyword)!=0){
		fprintf(stderr, "Failed to read closing <%s> mark!\n",keyword);		
		return NULL;
	}
	return text;
}
/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/

void readCONTEXT(FILE *fp, CONTEXT *c){
	char text[2000],tag[2000],term[2000],definition[2000],keyword[2000];
	strcpy(keyword,scanKeyword(fp));	
	while(strcmp(keyword,"Entry")==0){
		strcpy(keyword,scanMustKeyword(fp,"term"));	
		strcpy(term,scanTextLineEnd(fp, "term"));
		strcpy(keyword,scanMustKeyword(fp,"definition"));	
		strcpy(definition,scanTextLineEnd(fp, "definition"));
		recordNewENTRY(c->lexic,term,definition);
		scanTextLineEnd(fp, "Entry");
		strcpy(keyword,scanKeyword(fp));	
	}
	while(strcmp(keyword,"Statement")==0){
		strcpy(keyword,scanMustKeyword(fp,"text"));	
		strcpy(text,scanTextLineEnd(fp, "text"));
		strcpy(keyword,scanMustKeyword(fp,"tag"));	
		strcpy(tag,scanTextLineEnd(fp, "tag"));
		recordNewATOM(c->statements, text,tag);
		strcpy(keyword,scanKeyword(fp));	
		if(strcmp(keyword,"Context")==0){
			initCONTEXT(c->statements->list[c->statements->count-1]);
			readCONTEXT(fp, c->statements->list[c->statements->count-1]->context);
			strcpy(keyword,scanKeyword(fp));	
		}
		strcpy(keyword,scanKeyword(fp));	
		//printf("..................................THIS IS THE END OF THE ITERATION\n");
	}
	while(strcmp(keyword,"Example")==0){
		strcpy(keyword,scanMustKeyword(fp,"text"));	
		strcpy(text,scanTextLineEnd(fp, "text"));
		recordNewATOM(c->examples, text,"Unspecified example");
		strcpy(keyword,scanKeyword(fp));	
		if(strcmp(keyword,"Context")==0){
			initCONTEXT(c->examples->list[c->examples->count-1]);
			readCONTEXT(fp, c->examples->list[c->examples->count-1]->context);
			strcpy(keyword,scanKeyword(fp));	
		}
		strcpy(keyword,scanKeyword(fp));	
		//printf("..................................THIS IS THE END OF THE ITERATION\n");
	}
}
void readANSWERS(FILE *fp, ATOMS *a){
	char text[2000],tag[2000],keyword[2000];
	strcpy(keyword,scanKeyword(fp));	//printf("blipp\n");	
	while(strcmp(keyword,"Statement")==0){
		strcpy(keyword,scanMustKeyword(fp,"text"));	
		strcpy(text,scanTextLineEnd(fp, "text"));
		strcpy(keyword,scanMustKeyword(fp,"tag"));	
		strcpy(tag,scanTextLineEnd(fp, "tag"));
		recordNewATOM(a, text,"Answer");
		a->list[a->count-1]->depth++;
		strcpy(keyword,scanKeyword(fp));	
		if(strcmp(keyword,"Context")==0){
			initCONTEXT(a->list[a->count-1]);
			readCONTEXT(fp, a->list[a->count-1]->context);
			strcpy(keyword,scanKeyword(fp));	
		}
		strcpy(keyword,scanKeyword(fp));	
		//printf("..................................THIS IS THE END OF THE ITERATION\n");
	}
}
/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/

ATOM * readQUESTION(char *filename){
	int i=strlen(filename)-1;
	while(i>0 && filename[i]==' '){
		i--;	
	}
	filename[i+1]='\0';
	FILE *fp = fopen(filename,"r"); 
   if( fp == NULL){
		fprintf(stderr, "Failed to open file '%s'!\n",filename);		
		return NULL;
	}
	char text[2000], othertext[2000],type[2000], keyword[2000]; 
	printf("Opened filename = %s\n",filename);

	strcpy(type,scanMustKeyword(fp, "Statement"));	
	strcpy(keyword,scanMustKeyword(fp, "text"));	
	strcpy(text,scanTextLineEnd(fp, "text")); //printf("The text is %s\n",text);
	strcpy(keyword,scanMustKeyword(fp, "tag"));	
	strcpy(othertext,scanTextLineEnd(fp, "tag")); 
	if(strcmp(othertext,"Question")==0){
		//printf("all good\n");
	} else {
		fprintf(stderr,"Failed to read Question tag where expected! read %s instead\n",othertext);
		return NULL;
	}
	ATOM* q = initATOM(text,"Question","Question",0);

	strcpy(keyword,scanKeyword(fp));	
	if(strcmp(keyword,"Context")==0){
		initCONTEXT(q);
		readCONTEXT(fp,q->context);
		strcpy(keyword,scanKeyword(fp));	
	}
	if(strcmp(keyword,"Answers")==0){
		q->answers=initATOMS(q->depth+1,"Answers");
		readANSWERS(fp,q->answers);
	}
	scanTextLineEnd(fp,"Statement");
	//printf("\n----------------------------------------------------------------------------------------------\n");
	//fprintForestQUESTION(stdout,q);
   fclose(fp);
	return q;	
}


/*
void feedFromFile(char *filename){

	FILE *fp = fopen(filename,"r"); 
   if( fp ){
		//visualisefile(fp,filename);
    	char line[1000];
		fscanf(fp, "%s", line);	
		if(strcmp(line, "<Question>") == 0){
			ATOM* q = initATOM(line,removeMarks(line),0);
			printf("The line is %s\n",q->text);
			printf("Type is %s\n",q->type);
			//fscanf(fp, "%s", line);	
			//fgets(line, 1000, fp);
			fscanf(fp,"%[^\n]s\n",line);
			printf("The line is %s\n",line);
			fscanf(fp,"%[^\n]s\n",line);
			printf("The line is %s\n",line);
size_t len = 0;
char * linee = NULL;
ssize_t read;
while ((read = getline(&linee, &len, fp)) != -1) {
        printf("Retrieved linee of length %zu :\n", read);
        printf("%s", linee);
    }

getline(&linee, &len, fp);
getline(&linee, &len, fp);
printf("The linee is %s\n",linee);
		}

//while(fscanf(fp, "%s", line) != EOF){
//printf("The line is %s\n",line);
//}

/////

char ch;
//while((ch = fgetc(fp)) != '\n' && ch !='\t'){}
//printf("%s",ch);
		int r;

		//getchar();
		r=scanf("%[^\n\t]s\n",line);
		getchar();
     		//fscanf(fp,"<%s>",line);
		printf("A line: %s\n",line);

    	while(fgets(line, 1000, fp) != NULL){
			r=scanf("%[^\n]s\n",line);
printf("%d ",r);
     		//fscanf(fp,"<%s>",line);
		printf("A line: %s\n",line);
    	}
////
   }
   else{
      printf("Failed to open the file\n\n");
   }
   fclose(fp);
}
*/