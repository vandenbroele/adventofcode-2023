#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define bool unsigned char
#define true 1
#define false 0

int *read_dataset(int *entry_count){
	int c, *dataset, dataset_size, dataset_items;
	bool negative = false;

	dataset_items = 1;
	dataset_size = 4;
	dataset = calloc(sizeof(int), dataset_size);
	while((c = getchar()) != '\n' && c != EOF){
		if(c == ' '){
			if(negative){
				dataset[dataset_items - 1] = -dataset[dataset_items - 1];
			}

			dataset_items++;
			
			if(dataset_items == dataset_size){
				dataset_size *= 2;
				dataset = realloc(dataset, sizeof(int) * dataset_size);
			}
			
			dataset[dataset_items - 1] = 0;
			negative = false;
			continue;
		}

		if(c == '-'){
			negative = true;
			continue;
		}
		
		dataset[dataset_items - 1] = (dataset[dataset_items - 1] * 10) + c - '0';
	}
	
	if(negative){
		dataset[dataset_items - 1] = -dataset[dataset_items - 1];
	}

	*entry_count = dataset_items;
	return dataset;
}

bool all_entries_are_zero(int *dataset, int dataset_entry_count){
	int i;

	for(i = 0; i < dataset_entry_count; i++){
		if(dataset[i] != 0){
			return false;
		}
	}

	return true;
}

int predict_previous_value(int *dataset, int dataset_entry_count){
	if(dataset_entry_count == 0 || all_entries_are_zero(dataset, dataset_entry_count)){
		return 0;
	}

	int *sequence = calloc(sizeof(int), dataset_entry_count - 1);
	int i;

	for(i = 0; i < dataset_entry_count - 1; i++){
		sequence[i] = dataset[i + 1] - dataset[i];
	}

	int previous_sequence_value = predict_previous_value(sequence, dataset_entry_count - 1);
	free(sequence);

	return dataset[0] - previous_sequence_value;
}

int main(){
	int i, previous_value_sum, *dataset, dataset_entry_count;

	previous_value_sum = 0;
	dataset = read_dataset(&dataset_entry_count);
	while(1 < dataset_entry_count){
		previous_value_sum += predict_previous_value(dataset, dataset_entry_count);
		
		free(dataset);
		dataset = read_dataset(&dataset_entry_count);
	}
	free(dataset);

	printf("%d\n", previous_value_sum);

	return 0;
}
