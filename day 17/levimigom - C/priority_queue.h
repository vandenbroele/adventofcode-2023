#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

struct priority_queue_node {
	void *data;
	int priority;
};

struct priority_queue_node *create_priority_queue_node(void *data, int priority) {
	struct priority_queue_node *node = malloc(sizeof(struct priority_queue_node));
	node->data = data;
	node->priority = priority;
}

// minimum priority heap priority queue
struct priority_queue {
	struct priority_queue_node **nodes;
	int size, capacity;
};

struct priority_queue *create_priority_queue(){
	struct priority_queue *queue = malloc(sizeof(struct priority_queue));
	queue->size = 0;
	queue->capacity = 2;
	queue->nodes = calloc(sizeof(struct priority_queue_node*), queue->capacity);
}

void swap(struct priority_queue *queue, int index_a, int index_b){
	struct priority_queue_node *placeholder = queue->nodes[index_a];
	queue->nodes[index_a] = queue->nodes[index_b];
	queue->nodes[index_b] = placeholder;
}

void swim(struct priority_queue *queue, int index){
	while(0 < index && queue->nodes[index]->priority < queue->nodes[index / 2]->priority){
		swap(queue, index, index / 2);
		index /= 2;
	}
}

void sink(struct priority_queue *queue, int index){
	while(index * 2 < queue->size){
		int new_index = index * 2;

		if(new_index + 1 < queue->size && queue->nodes[new_index + 1]->priority < queue->nodes[new_index]->priority){
			new_index++;
		}

		if(queue->nodes[index]->priority <= queue->nodes[new_index]->priority){
			break;
		}
		
		swap(queue, index, new_index);
		index = new_index;
	}
}

void enqueue_priority_queue(struct priority_queue *queue, void *data, int priority){
	struct priority_queue_node *new_node = create_priority_queue_node(data, priority);

	if(queue->size == queue->capacity){
		queue->capacity *= 2;
		queue->nodes = realloc(queue->nodes, sizeof(struct priority_queue_node*) * queue->capacity);
	}

	queue->nodes[queue->size] = new_node;
	swim(queue, queue->size++);
}

void *dequeue_priority_queue(struct priority_queue *queue){
	if(queue->size == 0){
		return 0;
	}

	struct priority_queue_node *first = queue->nodes[0];
	swap(queue, 0, --(queue->size));
	sink(queue, 0);

	void *data = first->data;
	free(first);
	return data;	
}

void free_priority_queue(struct priority_queue *queue){
	free(queue->nodes);
	free(queue);
}

#endif
