#include "queue.h"
#include "tile_game.h"

void enqueue(struct queue *q, struct game_state state) {
    uint64_t serialized_state = serialize(state);
    insert_at_tail(&(q->data), serialized_state);
}

struct game_state dequeue(struct queue *q) {
    uint64_t serialized_state = remove_from_head(&(q->data));
    return deserialize(serialized_state);
}

int number_of_moves(struct game_state start) {
    struct queue q;
    q.data.head = NULL;

    enqueue(&q, start);

    while (q.data.head != NULL) {
        struct game_state current_state = dequeue(&q);

        if (is_goal_state(current_state)) {
            return current_state.num_steps;
        }

        for (int i = 0; i < 4; i++) {
            struct game_state next_state = generate_next_state(current_state, i);
            if (!is_state_visited(next_state)) {
                enqueue(&q, next_state);
            }
        }
    }

    return -1;
}
