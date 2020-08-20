#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FALSE 0
#define TRUE 1
#define MAXLABEL 31
#define MAXLENGTH 82

typedef struct name {
	struct name* next;
	int val;
	int mem_num;
} data;

typedef struct node {
	struct node* next;
	char symbol_name[MAXLABEL + 1];
	char symbol_type[MAXLENGTH]; /* code or data */
	int mem_num;
	char guidnace[MAXLENGTH];
} label;

typedef struct arg {
	struct arg* next;
	char arg_name[MAXLENGTH];
	int mem_num;
} external;

void add_data(data**, int, int);
void free_data(data**); 
void update_data(data* head, int ICF);

void add_label(label**, char*, char*, int, char*);
void free_label(label**);
int has_label_in_table(label*, char*);
int equals(char[],char[]);
void update_label(label* head, int ICF);
void add_ent_guid(label*, char*);
int get_mem_num(label* head,char* arg);
int is_external_label(label* head, char* arg);

void add_external(external** head, int mem, char* name);
void free_external(external** head);






