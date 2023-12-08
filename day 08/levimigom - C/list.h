#ifndef LIST_H
#define LIST_H

struct list {
	void **items;
	int size, item_count;
};

struct list *create_list(){
	struct list *list = malloc(sizeof(struct list));
	list->size = 1;
	list->item_count = 0;
	list->items = calloc(sizeof(void *), list->size);
}

void add_to_list(struct list *list, void *item){
	if(list->size == list->item_count){
		list->size *= 2;
		list->items = realloc(list->items, sizeof(void *) * list->size);
	}

	list->items[list->item_count++] = item;
}

void destroy_list(struct list *list){
	free(list->items);
	free(list);
}
#endif
