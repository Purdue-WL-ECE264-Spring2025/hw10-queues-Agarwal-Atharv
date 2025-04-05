#include "queue.h"
#include "tile_game.h"
#include <stdlib.h>

struct queue *new_queue() {
    struct queue *q = (struct queue *)malloc(sizeof(struct queue));
    if (q) {
        q->data.head = NULL; 
    }
    return q;
}

void enqueue(struct queue *q, struct game_state state) {
    if (q == NULL) return;

    size_t serialized_state = serialize(state);
    insert_at_tail(&(q->data), serialized_state); 
}

struct game_state dequeue(struct queue *q) {
    if (q == NULL || q->data.head == NULL) {
        return (struct game_state){0};  
    }

    size_t serialized_state = remove_from_head(&(q->data));
    return deserialize(serialized_state);
}

void free_queue(struct queue *q) {
    if (q == NULL) return;

    free_list(q->data);
    free(q);
}

int number_of_moves(struct game_state start) {
    struct game_state goal = get_goal_state();

    // Validate the start state
    if (!is_valid_state(&start)) {
        return -1;
    }

    struct queue *q = new_queue();
    if (q == NULL) return -1;  

    enqueue(q, start);

    struct linked_list visited = {0}; 
    insert_at_head(&visited, serialize(start)); 

    while (q->data.head != NULL) {
        struct game_state current = dequeue(q);

        if (is_goal_state(current, goal)) {
            int moves = current.num_moves;
            free_queue(q);
            free_list(visited);
            return moves;
        }

        struct game_state next_states[4];
        int num_next_states = get_next_states(current, next_states);

        for (int i = 0; i < num_next_states; i++) {
            size_t serialized_next_state = serialize(next_states[i]);

            bool already_visited = false;
            for (struct list_node *n = visited.head; n; n = n->next) {
                if (n->value == serialized_next_state) {
                    already_visited = true;
                    break;
                }
            }

            if (!already_visited) {
                insert_at_head(&visited, serialized_next_state);
                enqueue(q, next_states[i]);
            }
        }
    }

    free_queue(q);
    free_list(visited);
    return -1;
}
