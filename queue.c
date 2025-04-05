#include "queue.h"
#include "tile_game.h"

struct queue {
    struct linked_list list;
};

struct queue *new_queue() {
    struct queue *q = (struct queue *)malloc(sizeof(struct queue));
    if (q) {
        q->list.head = q->list.tail = NULL;
    }
    return q;
}

void enqueue(struct queue *q, struct game_state state) {
    size_t serialized_state = serialize(&state);
    insert_at_tail(&(q->list), serialized_state);
}

struct game_state dequeue(struct queue *q) {
    size_t serialized_state = remove_from_head(&(q->list));
    return deserialize(serialized_state);
}

void free_queue(struct queue *q) {
    free_list(&(q->list));
    free(q);
}

int number_of_moves(struct game_state start) {
    struct game_state goal = get_goal_state();
    struct queue *q = new_queue();
    enqueue(q, start);

    while (q->list.head != NULL) {
        struct game_state current = dequeue(q);

        if (is_goal_state(&current, &goal)) {
            int moves = current.num_moves;
            free_queue(q);
            return moves;
        }

        struct game_state next_states[4];
        int num_next_states = get_next_states(&current, next_states);

        for (int i = 0; i < num_next_states; i++) {
            enqueue(q, next_states[i]);
        }
    }

    free_queue(q);
    return -1;
}
