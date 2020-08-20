#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "second-move.h"

#define PAD 5



void create_files(char* name, int ICF, int DCF, label* label_head, code* image_code, data* data_head, external* external_head);
void make_externals(char* name, external* external_head);
void make_entries(char* name, label* label_head);
void create_object(FILE* ob_file, code* image_code, data* data_head, int ICF, int DCF);
void create_entries(FILE* ent_file, label* label_head);
void create_externals(FILE* ext_file, external* external_head);
int ext_num(label* label_head);
int ent_num(label* label_head);
unsigned long word_to_int(code curr);

