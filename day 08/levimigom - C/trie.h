#ifndef TRIE_H
#define TRIE_H

#define TRIE_ALPHABET_SIZE 256

struct trienode {
	struct trienode *neighbours[TRIE_ALPHABET_SIZE];
	void *value;
};

struct trienode *create_trienode(){
	struct trienode *node = malloc(sizeof(struct trienode));
	int i;

	for(i = 0; i < TRIE_ALPHABET_SIZE; i++){
		node->neighbours[i] = 0;
	}
	node->value = 0;

	return node;
}

void add_to_trie(struct trienode *node, char *key, void *value){
	if(*key == '\0'){
		node->value = value;
		return;
	}

	if(node->neighbours[*key] == 0){
		node->neighbours[*key] = create_trienode();
	}

	add_to_trie(node->neighbours[*key], key + 1, value);
}

void *get_from_trie(struct trienode *node, char *key){
	if(key[0] == '\0'){
		return node->value;
	} else {
		return get_from_trie(node->neighbours[*key], key + 1);
	}
}

void destroy_trienode(struct trienode *node){
	int i;

	for(i = 0; i < TRIE_ALPHABET_SIZE; i++){
		if(node->neighbours[i] != 0){
			destroy_trienode(node->neighbours[i]);
			free(node->neighbours[i]);
		}
	}

	if(node->value != 0){
		free(node->value);
	}
}

#endif
