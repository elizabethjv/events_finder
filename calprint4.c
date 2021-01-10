/*
 * calprint4.c
 *
 * Starter file provided to students for Assignment #4, SENG 265,
 * Summer 2019.
 */
#define _GNU_SOURCE
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "emalloc.h"
#include "ics.h"
#include "listy.h"


void print_event(node_t *n, void *arg) {
    assert(n != NULL);

    event_t *event = n->val;

    if (strcmp(event->rrule, "") == 0) {
        printf("EVENT: %s %s '%.10s' '%.10s'\n", event->dtstart,
            event->dtend, event->summary, event->location);
    } else {
        printf("EVENT: %s %s '%.10s' '%.10s' '%.10s'\n", event->dtstart,
            event->dtend, event->summary, event->location,
            event->rrule);
    }
}


#ifdef DEBUG

/*
 * Just showing the use of the linked-list routines.
 */

void _demo() {
    event_t *temp_event = NULL;
    node_t  *temp_node  = NULL;
    node_t  *head = NULL;

    /* Add one event, without an RRULE */

    temp_event = emalloc(sizeof(event_t));
    strncpy(temp_event->dtstart, "20190706T120000", 17);
    strncpy(temp_event->dtend, "20190706T160000", 17);
    strncpy(temp_event->summary, "summary 1", SUMMARY_LEN);
    strncpy(temp_event->location, "location 1", LOCATION_LEN);
    temp_event->rrule[0] = '\0';
    temp_node = new_node(temp_event);
    head = add_front(head, temp_node);

    /* Add a second event, with an RRULE */

    temp_event = emalloc(sizeof(event_t));
    strncpy(temp_event->dtstart, "20190707T093000", 17);
    strncpy(temp_event->dtend, "20190707T102000", 17);
    strncpy(temp_event->summary, "uvwxyz 1234567", SUMMARY_LEN);
    strncpy(temp_event->location, "abcde 1234567", LOCATION_LEN);
    strncpy(temp_event->rrule, "yada RULE yada UNTIL yada", RRULE_LEN);
    temp_node = new_node(temp_event);
    head = add_front(head, temp_node);

    /* Print the list of events. */

    apply(head, print_event, NULL);

    /* Free up the memory. This is done rather deliberately
     * and manually. A full-featured function might better
     * serve for this. Asserts are liberally used here as they
     * express state that *must* be true if all of the code is
     * correctly working.
     */

    temp_node = head;
    assert(temp_node != NULL);
    head = remove_front(head);
    temp_event = temp_node->val;
    assert(temp_event != NULL);
    free(temp_event);
    free(temp_node);

    temp_node = head;
    assert(temp_node != NULL);
    head = remove_front(head);
    temp_event = temp_node->val;
    assert(temp_event != NULL);
    free(temp_event);
    free(temp_node);
   
    assert(head == NULL); 
    
}

#endif


int main(int argc, char *argv[])
{
    int from_y = 0, from_m = 0, from_d = 0;
    int to_y = 0, to_m = 0, to_d = 0;
    char *filename = NULL;
    int i; 

    for (i = 0; i < argc; i++) {
        if (strncmp(argv[i], "--start=", 7) == 0) {
            sscanf(argv[i], "--start=%d/%d/%d", &from_d, &from_m, &from_y);
        } else if (strncmp(argv[i], "--end=", 5) == 0) {
            sscanf(argv[i], "--end=%d/%d/%d", &to_d, &to_m, &to_y);
        } else if (strncmp(argv[i], "--file=", 7) == 0) {
            filename = argv[i]+7;
        }
    }

    if (from_y == 0 || to_y == 0 || filename == NULL) {
        fprintf(stderr, "usage: %s --start=dd/mm/yyyy --end=dd/mm/yyyy --file=icsfile\n",argv[0]);
        exit(1);
    }


/* 
 * Showing some simple usage of the linked-list routines.
 */

//#ifdef DEBUG
    //_demo();
//#endif

    char *from_date = (char *)malloc(8*sizeof(char *));
    char *to_date = (char *)malloc(8*sizeof(char *));
	sprintf(from_date,"%d%d%dT", from_y, from_m, from_d);
    sprintf(to_date,"%d%d%dT", to_y, to_m, to_d);
    
    FILE *fp = fopen(filename,"r");
    if (fp == NULL){
      fprintf(stderr,"unable to open %s\n",filename);
      exit(1);
    }
    
    char *line = NULL;
    size_t len = 0;   /* size_t is really just an unsigned int*/
    ssize_t read;     /* ssize_t is where a function may return a size or a
    						  * negative number. */
    int flag = 0;	
    char *dtstart = NULL;
    char *dtend = NULL;
    char *summary = NULL;
    char *location = NULL;
    char *rrule = NULL;
    event_t *temp_event = NULL;
    node_t  *temp_node  = NULL;
    node_t  *head = NULL;	
    				  
    while ((read = getline(&line , & len , fp ) ) != - 1) { 
    	if (strncmp(line, "BEGIN:VEVENT", 12) == 0){
    			flag = 1;
		}
		if(flag == 1){
			if (strncmp(line, "DTSTART", 7) == 0){	
				line[strlen(line)-1] = '\0';
				*dtstart = *(line+8);
				printf("%s",dtstart);
			}    	
			if (strncmp(line, "DTEND", 5) == 0){	
				*dtend = *(line+6);
			}
			if (strncmp(line, "SUMMARY", 7) == 0){	
				*summary = *(line+8);
			}   
			if (strncmp(line, "LOCATION", 8) == 0){	
				*location = *(line+9);
			}
			if (strncmp(line, "RRULE", 6) == 0){	
				*rrule = *(line+7);
			} 
			if (strncmp(line, "END:VEVENT", 10) == 0){	
				if (rrule == NULL){
					/* Add one event, without an RRULE */

					temp_event = emalloc(sizeof(event_t));
					strncpy(temp_event->dtstart, *dtstart, 15);
					strncpy(temp_event->dtend, *dtend, 15);
					strncpy(temp_event->summary, *summary, strlen(*summary);
					strncpy(temp_event->location, *location, strlen(*location);
					temp_event->rrule[0] = '\0';
					temp_node = new_node(temp_event);
					head = add_front(head, temp_node);//MODIFY
				}
				else{
					/* Add a second event, with an RRULE */

					temp_event = emalloc(sizeof(event_t));
					strncpy(temp_event->dtstart, "20190707T093000", 17);
					strncpy(temp_event->dtend, "20190707T102000", 17);
					strncpy(temp_event->summary, "uvwxyz 1234567", SUMMARY_LEN);
					strncpy(temp_event->location, "abcde 1234567", LOCATION_LEN);
					strncpy(temp_event->rrule, "yada RULE yada UNTIL yada", RRULE_LEN);
					temp_node = new_node(temp_event);
					head = add_front(head, temp_node);//MODIFY
			}       
			
    	} 
    }
    
    if(line){
    	free(line);
    }
    fclose(fp);
    exit(0);    
}
