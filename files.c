#include "files.h"

void make_externals(char* name, external* external_head) 
{																	
	FILE* p;

	/* check if need an externals file */
	if ((p = fopen(name, "w")) == NULL)
		fprintf(stderr, "error in the open of an object file");
	else
	{
		create_externals(p, external_head);
	}
}

void make_entries(char* name, label* label_head)
{
	FILE* p;

	/* ccheck if need an entries file */
	if ((p = fopen(name, "w")) == NULL)
		fprintf(stderr, "error in the open of an object file");
	else
	{
		create_entries(p, label_head);
	}
}


void create_files(char* name, int ICF, int DCF, label* label_head, code* image_code, data* data_head, external* external_head)
{
	FILE* p;
	char* ob_file = (char*)malloc((strlen(name) + PAD) * sizeof(char));
	char* ext_file = (char*)malloc((strlen(name) + PAD) * sizeof(char));
	char* ent_file = (char*)malloc((strlen(name) + PAD) * sizeof(char));
	int i = 0;

	if (ob_file == NULL || ext_file == NULL || ent_file == NULL)
	{
		fprintf(stderr, "could not make enough memory");
		exit(0);
	}
	else
	{
		/* copies the name until exlucise final after the point char */
		while (name[i] != '.' && *name != '\0')
		{
			ob_file[i] = ext_file[i] = ent_file[i] = name[i];
			i++;
		}
		ob_file[i] = ext_file[i] = ent_file[i] = '.';
		i++;
		ob_file[i] = ext_file[i] = ent_file[i] = '\0';

		strcat(ob_file, "ob");
		strcat(ext_file, "ext");
		strcat(ent_file, "ent");

		/* copied all the file names */

		if (ext_num(label_head) > 0)
		{
			make_externals(ext_file, external_head);
		}
		free(ext_file);


		if (ent_num(label_head) > 0)
		{
			make_entries(ent_file, label_head);
		}
		free(ent_file);

		if ((p = fopen(ob_file, "w")) == NULL)
			fprintf(stderr, "error in the open of an object file");
		else
		{
			create_object(p, image_code, data_head, ICF, DCF);
				free(ob_file);
			/*create_object(p);*/
		}
	}
}

void create_object(FILE *ob_file, code* image_code, data* data_head, int ICF, int DCF)
{
	int i;
	data* p = data_head;

	fprintf(ob_file, "%d %d\n", ICF, DCF);
	for (i = 0; i < ICF - START_ALLOC; i++)
	{
		fprintf(ob_file, "%07d %06x\n", i + START_ALLOC, word_to_int(image_code[i]));
	}
	while (p != NULL)
	{
		fprintf(ob_file, "%07d %06x\n", p->mem_num, p->val);
	}
}

void create_entries(FILE* ent_file, label* label_head)
{
	label* p;
	p = label_head;

	while (p != NULL)
	{
		if (equals(p->guidnace, "enternal"))
			fprintf(ent_file, "%s %07d\n", p->symbol_name, p->mem_num);
		p = p->next;
	}

}

void create_externals(FILE* ext_file, external* external_head)
{
	external* p;
	p = external_head;

	while (p != NULL)
	{
		fprintf(ext_file, "%s %07d\n", p->arg_name, p->mem_num);
		p = p->next;
	} 
}

int ext_num(label* label_head)
{
	label* p = label_head;
	int cnt = 0;

	while (p != NULL)
	{
		if (equals((p->symbol_name), "data"))
			cnt++;
		p = p->next;
	}
	return cnt;
}

int ent_num(label* label_head)
{
	label* p = label_head;
	int cnt = 0;

	while (p != NULL)
	{
		if (equals((p->symbol_name), "enternal"))
			cnt++;
		p = p->next;
	}
	return cnt;
}

unsigned long word_to_int(code curr)
{
	unsigned long num = 0;
	unsigned long E = curr.word.E;
	unsigned long R = curr.word.R;
	unsigned long A = curr.word.A;
	unsigned long funct = curr.word.funct;
	unsigned long target_reg = curr.word.target_reg;
	unsigned long target_address = curr.word.target_address;
	unsigned long source_reg = curr.word.source_reg;
	unsigned long source_address = curr.word.source_address;
	unsigned long opcode = curr.word.opcode;
	unsigned long val = 0;

	/* check if this a first word code*/
	if (curr.word.E == 1 || curr.word.R == 1 || curr.word.A == 1)
	{
		R = R << 1;
		A = A << 2;
		funct = funct << 3;
		target_reg = target_reg << 8;
		target_address = target_address << 11;
		source_reg = source_reg << 13;
		source_address = source_address << 16;
		opcode = opcode << 18;

		num = num | E;
		num = num | R;
		num = num | A;
		num = num | funct;
		num = num | target_reg;
		num = num | target_address;
		num = num | source_reg;
		num = num | source_address;
		num = num | opcode;

	}
	else
	{
		E = curr.extra_word.E;
		R = curr.extra_word.R;
		A = curr.extra_word.A;
		val = curr.extra_word.val;
		R = R << 1;
		A = A << 2;
		val = val << 3;
		num = num | E;
		num = num | R;
		num = num | A;
		num = num | val;
	}
	return num;
}