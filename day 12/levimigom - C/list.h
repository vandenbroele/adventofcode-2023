#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

struct list{
	int length, size;
	void **values;
};

struct list* create_list(){
	struct list* list = malloc(sizeof(struct list));
	list->size = 1;
	list->length = 0;
	list->values = calloc(sizeof(void*), list->size);
}

void add_to_list(struct list *list, void *value){
	if(list->length == list->size){
		list->size *= 2;
		list->values = realloc(list->values, sizeof(void*) * list->size);
	}
	list->values[list->length++] = value;
}

void *get_from_list(struct list *list, int index){
	return list->values[index];
}

void destroy_list(struct list *list){
	int i;

	for(i = 0; i < list->length; i++){
		free(list->values[i]);
	}
	free(list->values);
	free(list);
}

#endif
