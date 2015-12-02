#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

void start_program(record_list *rlist);
void instructions(void);
int selection_handler(record_list *rlist);

int validate_id(char line[], int *pid);
int get_name (char line[]);
int validate_score(char line[], int *pscore);

int enter_record(record_list *rlist);
int create_record(record_list *rlist, int id, char lastname[], char firstname[], int score);
void store_record(record_list *rlist, record cr);

void sortbyid(record_list *rlist);
int cmp(const void*p, const void *q);

void sortbyname(record_list *rlist);
int cmp(const void*p, const void *q);


void shutdown(record_list *rlist);



/* main function where execution starts */

int main () {
	
	record_list rlist;
	
	rlist.data = 0;
	
	rlist.nused=0;
	
	rlist.nalloc=0;
	
	start_program(&rlist);
	
	return 0;

}

void start_program (record_list *rlist){
	
	int selection;
		
	instructions();
	
	while ((selection=selection_handler(rlist))!=QUIT) {
		
	start_program(rlist);
	
	return;
	
	}
	
	
	return;
}


void instructions(void){
	
	fprintf(stderr,
		"Menu:\n"
		"1. Enter records\n"
		"2. Display records sorted by ID\n"
		"3. Display records sorted by name\n"
		"4. Quit\n");
	return;
}


int selection_handler(record_list *rlist){
	
	char selection[BUFSIZE];
	
	int option;
	
	char *line;
		
	while((line = fgets(selection,BUFSIZE,stdin))!=NULL){
				
		sscanf(selection,"%d",&option);
		
		if((option<1 || option>4) && line != NULL){
			start_program(rlist);
		}
		
		switch (option){
			
			case 1:
				enter_record(rlist);
				return 1;
						
			case 2:
				sortbyid(rlist);
				return 2;

			case 3:
				sortbyname(rlist);
				return 3;
		
			case QUIT:
				shutdown(rlist);
				return 4;
		}
	}
	
	shutdown(rlist);
	
	return 0;
}


int validate_id (char line[], int *pid){
	
	size_t i;
	
	/*loop through user input*/
	for(i=0;i!='\0';i++){
		
		/*check if all characters are digits, if not ID is not valid, return false*/
		if(!isdigit((unsigned)line[i])){
			return 0;
		}
		
		else break;
	}
	
	sscanf(line,"%d",pid);
		
	/*loop through user input is between 1000000 & 9999999*/
	if((*pid<1000000 || *pid> 9999999) && *pid!=0)
		return 0;
	
	return 1;
}


int get_name (char line[]){

	char n[BUFSIZE];
	size_t i;

	for(i=0;line[i]!='\0';i++){
	
	if(!isspace((unsigned)line[i]))
		break;
	
	if(i==strlen(line)-1)
		return 0;
	
	}
	
	sscanf(line,"%s",n);
	
	if(strlen(n)>=NAMESIZE)
		return 0;
	
	
	strcpy(line,n);
	
	for (i=0;line[i]!='\0';i++){
		line[i] = tolower((unsigned)line[i]);
	}
	
	line[0] = toupper((unsigned)line[0]);
	
		return 1;
}

int validate_score (char line[], int *pscore){
	
	size_t i;
	
	for(i=0;i!='\0';i++){
		if(!isdigit((unsigned)line[i])){
			return 0;
		}
		else break;
	}
	
	sscanf(line,"%d",pscore);
		
	/*loop through user input is between 1000000 & 9999999*/
	if(*pscore<0 || *pscore>100)
		return 0;
	
	return 1;
}


int enter_record (record_list *rlist){
	
	int id =-1, score=-1, *pid=&id, *pscore=&score;
	char line[BUFSIZE], lastname[NAMESIZE], firstname[NAMESIZE];
	char *l;
		
	/*continue to prompt for id if id is not between 1000000 and 9999999 and is not 0*/
	
	do{
		do{
			fprintf(stderr,"Please enter id:\n");
			l = fgets(line,BUFSIZE,stdin);
			clearerr(stdin);
		} while(!validate_id(line, pid) && id !=0 && l!=NULL);
	
		if (id==0 || l == NULL)
	
			return 0;
				
		do{
			fprintf(stderr,"Please enter last name:\n");
			l = fgets(line,BUFSIZE,stdin);
			clearerr(stdin);
		}while(!get_name(line) && l!=NULL);
		
		if (id==0 || l == NULL)
	
			return 0;
	
		strcpy(lastname,line);
	
			
		do{
			fprintf(stderr,"Please enter first name:\n");
			l = fgets(line,BUFSIZE,stdin);
			clearerr(stdin);
		}while(!get_name(line) && l!=NULL);
	
		if (id==0 || l == NULL)
	
			return 0;
		
		strcpy(firstname,line);
	
	
		do{
			fprintf(stderr,"Please enter score:\n");
			l = fgets(line,BUFSIZE,stdin);
			clearerr(stdin);
		} while(!validate_score(line, pscore)&& l!=NULL);
		
		if (id==0 || l == NULL)
	
			return 0;
			
		create_record(rlist, id, lastname, firstname, score);
	} while(id!=0 && (l!=NULL));
	
	return 1;
	
}

int create_record (record_list *rlist, int id, char lastname[], char firstname[], int score){
	
	record cr;
	
	cr.id = id;
	
	strcpy(cr.name.first,firstname);
	
	strcpy(cr.name.last,lastname);
	
	cr.score = score;
	
	store_record(rlist, cr);
	
	return 1;
}

void store_record(record_list *rlist, record cr){
		
	/*realloc records while create_record is true and maintain life of function via passing argument of selection*/
		
	if (rlist->nused == rlist->nalloc){
		
		record *tmp = realloc(rlist->data, (rlist->nalloc + BLOCK)*sizeof(record));
		#ifdef DEBUG
		fprintf(stderr, "#\n");
		#endif
		
		if (tmp == 0) {
			fprintf(stderr, "unable to resize dynamic dynamic array of records\n");
			return;
		}
		
		rlist->data = tmp;
		
		rlist->nalloc += BLOCK;
		
	}
				
	rlist->data[rlist->nused++] = cr;	
	
	return;
	
}


void sortbyid(record_list *rlist){
		
	size_t i;
	
	qsort(rlist->data, rlist->nalloc, sizeof(record), cmp);
	
	for (i=0;i<(rlist->nused);i++){
	
		fprintf(stdout,"%d %s, %s: %d\n", rlist->data[i].id, rlist->data[i].name.last, 
		rlist->data[i].name.first, rlist->data[i].score);		
	}
	
	return;
		
}

int cmp(const void*p, const void *q){
	
	const record *pp =p;
	const record *qq =q;
		
	return pp->id - qq->id;
	
	}

void shutdown (record_list *rlist){
	
	size_t i;
	
	
	for (i=0; i<(rlist->nused);i++){
	free(&(rlist->data[i]));
	#ifdef DEBUG
	fprintf(stderr, "@\n");
	#endif
	
	}
	
	return;
}