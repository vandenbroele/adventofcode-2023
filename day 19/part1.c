#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define NAME_LEN 4
#define TRIE_SIZE 'z' + 1
#define ACCEPTED "A"
#define REJECTED "R"

struct rule {
	char input, operator, output[NAME_LEN];
	int value;
	bool is_default;
};

struct workflow {
	char name[NAME_LEN];
	int rule_count;
	struct rule **rules;
};

struct part {
	int ratings['x' + 1];
};

struct trie {
	struct workflow *workflow;
	struct trie *children[TRIE_SIZE];
};

struct trie* create_trie_node(){
	struct trie *trie_node = malloc(sizeof(struct trie));
	int i;

	for(i = 0; i < TRIE_SIZE; i++){
		trie_node->children[i] = 0;
	}
	trie_node->workflow = 0;

	return trie_node;
}

void add_to_trie(struct trie *trie_node, struct workflow *workflow, char *name){
	if(*name == '\0'){
		trie_node->workflow = workflow;
		return;
	}

	if(trie_node->children[*name] == 0){
		trie_node->children[*name] = create_trie_node();
	}
	add_to_trie(trie_node->children[*name], workflow, name + 1);
}

struct workflow *get_from_trie(struct trie *trie_node, char *name){
	if(*name == '\0'){
		return trie_node->workflow;
	} else {
		return get_from_trie(trie_node->children[*name], name + 1);
	}
}

bool contains(char *haystack, char needle){
	while(*haystack){
		if(*haystack == needle){
			return true;
		}
		haystack++;
	}
	return false;
}

struct rule *read_rule(){
	int c, size, capacity;
	char *s;

	size = 0;
	s = calloc(sizeof(char), (capacity = 2));
	while((c = getchar()) != ',' && c != '}'){
		if(size == capacity){
			s = realloc(s, sizeof(char) * (capacity *= 2));
		}
		s[size++] = c;
	}

	if(size == capacity){
		s = realloc(s, sizeof(char) * (++capacity));
	}
	s[size++] = '\0';

	struct rule *rule = malloc(sizeof(struct rule));
	int i, j;

	i = 0;
	if(contains(s, ':')){
		rule->is_default = false;
		rule->input = s[0];
		rule->operator = s[1];
		rule->value = 0;
		
		for(i = 2; i < size && s[i] != ':'; i++){
			rule->value = (10 * rule->value) + s[i] - '0';
		}
		i++;
	} else {
		rule->is_default = true;
	}

	for(j = 0; i + j < size; j++){
		rule->output[j] = s[i + j];
	}
	rule->output[j] = '\0';

	if(rule->is_default) getchar();

	free(s);
	return rule;
}

struct workflow *read_workflow(){
	int c, size, capacity;
	struct workflow *workflow = malloc(sizeof(struct workflow));

	size = 0;
	while((c = getchar()) != '{'){
		workflow->name[size++] = c;
	}
	workflow->name[size++] = '\0';

	size = -1;
	capacity = 2;
	workflow->rules = calloc(sizeof(struct rule*), capacity);

	do {
		if(++size == capacity){
			capacity *= 2;
			workflow->rules = realloc(workflow->rules, sizeof(struct rule*) * capacity);
		}
		workflow->rules[size] = read_rule();
	}
	while(!workflow->rules[size]->is_default);

	workflow->rule_count = size + 1;
	
	return workflow;
}

void free_rule(struct rule *rule){
	free(rule);
}

void free_workflow(struct workflow *workflow){
	int i;

	for(i = 0; i < workflow->rule_count; i++){
		free(workflow->rules[i]);
	}
	free(workflow->rules);
	free(workflow);
}

void free_trie(struct trie *trie_node){
	int i;

	for(i = 0; i < TRIE_SIZE; i++){
		if(trie_node->children[i] != 0){
			free_trie(trie_node->children[i]);
		}
	}

	if(trie_node->workflow != 0){
		free_workflow(trie_node->workflow);
	}

	free(trie_node);
}

struct part* read_part(){
	struct part *part = malloc(sizeof(struct part));
	int c, index;

	index = 0;
	while((c = getchar()) != '\n'){
		if(contains("xmas", c)){
			index = c;
			part->ratings[index] = 0;
		} else if('0' <= c && c <= '9'){
			part->ratings[index] = (10 * part->ratings[index]) + c - '0';
		}
	}

	return part;
}

bool rule_matches(struct rule *rule, struct part *part){
	if(rule->is_default){
		return true;
	}

	if(rule->operator == '<'){
		return part->ratings[rule->input] < rule->value;
	} else {
		return part->ratings[rule->input] > rule->value;
	}
}

bool is_accepted(struct part *part, struct trie *trie){
	int i;
	char *label = "in";
	struct rule *rule;
	struct workflow *workflow;

	while(label[0] != 'A' && label[0] != 'R'){
		workflow = get_from_trie(trie, label);
		for(i = 0; i < workflow->rule_count && !rule_matches(workflow->rules[i], part); i++);
		label = workflow->rules[i]->output;
	}

	return label[0] == 'A';
}

void solve(){
	int i, workflow_count, part_count, accepted_part_rating_sum;
	struct workflow *workflow;
	struct part *part;
	struct trie *trie = create_trie_node();

	scanf("%d ", &workflow_count);
	for(i = 0; i < workflow_count; i++){
		workflow = read_workflow();
		add_to_trie(trie, workflow, workflow->name);
	}

	accepted_part_rating_sum = 0;

	scanf("%d ", &part_count);
	for(i = 0; i < part_count; i++){
		part = read_part();
		if(is_accepted(part, trie)){
			accepted_part_rating_sum += part->ratings['x'] + part->ratings['m'] + part->ratings['a'] + part->ratings['s'];	
		}
		free(part);
	}

	free_trie(trie);
	printf("%d\n", accepted_part_rating_sum);
}

int main(){
	solve();
	return 0;
}
