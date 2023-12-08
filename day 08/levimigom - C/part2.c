#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "trie.h"
#include "list.h"
#define MAX_DIRECTIONS 512
#define LABEL_LENGTH 3

struct network_node {
	char label[LABEL_LENGTH + 1];
        char left_label[LABEL_LENGTH + 1];
        char right_label[LABEL_LENGTH + 1];
};

struct trienode *create_network(struct list *nodes){
	struct trienode *trie_root = create_trienode();
	char label[LABEL_LENGTH + 3];

	while(scanf("%s", label) != EOF){
		struct network_node *node = malloc(sizeof(struct network_node));
		
		// set label
		strcpy(node->label, label);

		// set left label
		scanf("%*s %s", label);
		label[4] = '\0';	// remove trailing ,
		strcpy(node->left_label, label + 1); 		
		
		// set right label
		scanf("%s", label);
		label[3] = '\0';	// remove trailing )
		strcpy(node->right_label, label);
		
		add_to_trie(trie_root, node->label, (void *) node);

		if(node->label[2] == 'A'){
			add_to_list(nodes, node->label);
		}
	}

	return trie_root;
}

long gcd(long a, long b){
	if(a == 0){
		return b;
	} else {
		return gcd(b % a, a);
	}
}

long lcm(long a, long b){
	return (a * b) / gcd(a, b);
}

int main(){
	char directions[MAX_DIRECTIONS];
	scanf("%s", directions);

	struct list *nodes = create_list();
	struct trienode *network = create_network(nodes);
	int i, j;

	long steps = 0;
	for(i = 0; i < nodes->item_count; i++){
		struct network_node *node = get_from_trie(network, (char*) nodes->items[i]);
		
		for(j = 0; node->label[2] != 'Z'; j++){
			if(directions[j % strlen(directions)] == 'R'){
				node = get_from_trie(network, node->right_label);
			} else {
				node = get_from_trie(network, node->left_label);
			}
		}	

		if(i == 0){
			steps = j;
		} else {
			steps = lcm(steps, j);
		}
	}

	printf("Reached destination in %ld steps\n", steps);
	destroy_trienode(network);
	free(network);
	destroy_list(nodes);

	return 0;
}
