#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "trie.h"
#define MAX_DIRECTIONS 512
#define LABEL_LENGTH 3

struct network_node {
	char label[LABEL_LENGTH + 1];
        char left_label[LABEL_LENGTH + 1];
        char right_label[LABEL_LENGTH + 1];
};

struct trienode *create_network(){
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
	}

	return trie_root;
}

int main(){
	char directions[MAX_DIRECTIONS];
	scanf("%s", directions);
	
	struct trienode *network = create_network();
	int i;

	struct network_node *current_node = get_from_trie(network, "AAA");	
	
	for(i = 0; strcmp(current_node->label, "ZZZ") != 0; i++){
		if(directions[i % strlen(directions)] == 'R'){
			current_node = get_from_trie(network, current_node->right_label);
		} else {
			current_node = get_from_trie(network, current_node->left_label);
		}
	}

	printf("Reached destination in %d steps\n", i);
	destroy_trienode(network);
	free(network);

	return 0;
}
