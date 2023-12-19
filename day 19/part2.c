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

struct range {
	int min['x' + 1], max['x' + 1];
};

int max(int a, int b){ return(a < b ? b : a); }

long calculate_combinations(struct range *range){
	printf("Found an accepted range: ");

	long combinations = 1;
	int i;

	for(i = 0; i < 4; i++){
		printf("%c between %4d and %4d\t", "xmas"[i], range->min["xmas"[i]], range->max["xmas"[i]]);
		combinations *= max(0, range->max["xmas"[i]] - range->min["xmas"[i]] + 1);
	}
	printf("\n%ld combinations\n", combinations);

	return combinations;
}

long calculate_total_combinations(struct trie *trie, char *workflow_name, int rule_number, struct range *range){
	if(workflow_name[0] == 'R'){
		return 0;
	}	

	if(workflow_name[0] == 'A'){
		return calculate_combinations(range);
	}

	struct workflow *workflow = get_from_trie(trie, workflow_name);
	struct rule *rule = workflow->rules[rule_number];

	if(rule->is_default){
		// default, no need to adjust range
		return calculate_total_combinations(trie, rule->output, 0, range);
	}

	long combinations = 0;
	int placeholder;

	if(rule->operator == '<'){
		if(range->max[rule->input] < rule->value){
			// the range is already small enough
			combinations += calculate_total_combinations(trie, rule->output, 0, range);
		} else {
			placeholder = range->max[rule->input];

			// adjust range to new maximum and go to the next workflow
			range->max[rule->input] = rule->value - 1;
			combinations += calculate_total_combinations(trie, rule->output, 0, range);

			// adjust range to new minimum and go to the next rule in the current workflow
			range->max[rule->input] = placeholder;
			range->min[rule->input] = rule->value;
			combinations += calculate_total_combinations(trie, rule->output, 1, range);
		}
	}

	if(rule->operator == '>'){
		if(rule->value < range->min[rule->input]){
			// the range is already small enough
			combinations += calculate_total_combinations(trie, rule->output, 0, range);
		} else {
			placeholder = range->min[rule->input];

			// adjust range to new minimum and go to the next workflow
			range->min[rule->input] = rule->value + 1;
			combinations += calculate_total_combinations(trie, rule->output, 0, range);

			// adjust range to new maximum and go to the next rule in the current workflow
			range->min[rule->input] = placeholder;
			range->max[rule->input] = rule->value;
			combinations += calculate_total_combinations(trie, rule->output, 1, range);
		}
	}

	return combinations;
}

void solve(){
	int i, workflow_count;
	struct workflow *workflow;
	struct trie *trie = create_trie_node();

	scanf("%d ", &workflow_count);
	for(i = 0; i < workflow_count; i++){
		workflow = read_workflow();
		add_to_trie(trie, workflow, workflow->name);
	}

	struct range *range = malloc(sizeof(struct range));
	range->min['x'] = range->min['m'] = range->min['a'] = range->min['s'] = 1;
	range->max['x'] = range->max['m'] = range->max['a'] = range->max['s'] = 4000;
	printf("%ld\n", calculate_total_combinations(trie, "in", 0, range));
	// free memory
	free_trie(trie);
	free(range);
}

int main(){
	solve();
	return 0;
}
