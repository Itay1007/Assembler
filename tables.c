#include "tables.h"

void add_data(data** head, int mem, int value)
{
	data *new, *p;
	new = (data*) malloc(sizeof(data));

	if(new != NULL)
	{
		new ->  mem_num = mem;
		new ->  val = value; 
		new -> next = NULL;

		if(*head == NULL)
		{
			*head = new;
		}
		else
		{
			p = *head;

			while(p-> next != NULL)
				{
					p = p -> next;
				}

			p -> next = new;
		}
	}
	else
	{
		fprintf(stderr, "could not make alloc for a new label in symbol table");
		exit(0);
		
	}
}

void update_data(data* head,int ICF)
{
data* curr = head;
	while(curr != NULL)
	{
		curr->mem_num = (curr->mem_num + ICF);
		curr = curr -> next;
	}
}

/*void print_data(data* head)
{
	data* curr = head;
	while(curr != NULL)
	{
		printf("%d \n", curr -> mem_num);
		curr = curr -> next;
	}
}*/

void free_data(data** head)
{
	data* p, * q;
	p = *head;

	if (p != NULL)
	{
		q = p;
		p = p->next;

		free(q);
		*head = NULL;

		while (p != NULL)
		{
			q = p;
			p = p->next;
			free(q);
		}
	}
}

void add_label(label** head, char* name, char* type, int mem, char* guid)
{
	label *new, *p;
	new = (label*) malloc(sizeof(label));

	if(new != NULL)
	{
		strcpy(new-> symbol_name, name);
		strcpy(new-> symbol_type, type);
		new ->  mem_num = mem;
		strcpy(new->guidnace, guid);
		new -> next = NULL;

		if(*head == NULL)
		{
			*head = new;
		}
		else
		{
			p = *head;

			while(p-> next != NULL)
				{
					p = p -> next;
				}

			p -> next = new;
		}
	}
	else
	{
		fprintf(stderr, "could not make alloc for a new label in symbol table");
		exit(0);	
	}
}

/*void print_label(label* head)
{
	label* curr = head;
	while(curr != NULL)
	{
		printf("%s \n", curr -> symbol_name);
		curr = curr -> next;
	}
}*/

void free_label(label **head) 
{
	label *p, *q;
	p = *head;

	if(p != NULL)
	{
		q = p;
		p = p->next;

		free(q);
		*head = NULL;

		while(p != NULL)
		{
			q = p;
			p = p->next;
			free(q);
		}
	}
}

/*checks if 2 strings are equal*/
int equals(char func_name[], char command[])
{
	return ((strlen(func_name) == strlen(command)) && (strcmp(func_name, command) == 0)) ? TRUE : FALSE;
}

void update_label(label *head, int ICF)
{
	label* p = head;
	while(p != NULL)
	{
		if(equals((p->symbol_type), "data"))
		{
			p-> mem_num += ICF;
		}
		p = p -> next;
	}
}

int has_label_in_table(label *head, char* name)
{
	label* p = head;
	int found = FALSE;

	while (p != NULL)
	{
		if (equals((p -> symbol_name), name))	
			found = TRUE;	
		p = p-> next;
	}
	return found;
}

int get_mem_num(label* head, char* name)
{
	label* p = head;
	int number = -1;

	while (p != NULL)
	{
		if (equals((p -> symbol_name), name))	
			number = p -> mem_num;	
		p = p-> next;
	}
	return number;
}

int is_external_label(label *head, char * name)
{
	label* p = head;
	int data = FALSE;

	while (p != NULL)
	{
		if (equals((p -> symbol_name), name) && equals((p -> guidnace), "external"))	
			data = TRUE;	
		p = p-> next;
	}
	return data;
}

void add_ent_guid(label* head, char* name)
{
	label* p = head;
	while (p != NULL)
	{
		if (equals((p->symbol_name), name))
			strcpy(p->guidnace, name);
		p = p->next;
	}
}

void add_external(external** head, int mem, char* name)
{
	external* new, * p;
	new = (external*)malloc(sizeof(external));

	if (new != NULL)
	{
		new->mem_num = mem;
		strcpy(new-> arg_name, name);
		new->next = NULL; 

		if (*head == NULL)
		{
			*head = new;
		}
		else
		{
			p = *head;

			while (p->next != NULL)
			{
				p = p->next;
			}

			p->next = new;
		}
	}
	else
	{
		fprintf(stderr, "could not make alloc for a new label in symbol table");
		exit(0);
	}
}

void free_external(external** head)
{
	external* p, * q;
	p = *head;

	if (p != NULL)
	{
		q = p;
		p = p->next;

		free(q);
		*head = NULL;

		while (p != NULL)
		{
			q = p;
			p = p->next;
			free(q);
		}
	}
}