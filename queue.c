#include "queue.h"
#include "tile_game.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void enqueue(struct queue *q, struct game_state state) {
  uint64_t serialized = serialize(state);
  insert_at_tail(&q->data, serialized);
}

struct game_state dequeue(struct queue *q) {
  uint64_t serialized = remove_from_head(&q->data);
  return deserialize(serialized);
}

static bool is_solved(struct game_state state) {
  uint8_t solved[4][4] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 0}
  };
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (state.tiles[i][j] != solved[i][j])
        return false;
    }
  }
  return true;
}

typedef struct {
  uint64_t *data;
  size_t size;
  size_t capacity;
} visited_set_t;

static void init_visited(visited_set_t *set) {
  set->capacity = 1024;
  set->size = 0;
  set->data = malloc(set->capacity * sizeof(uint64_t));
  if (!set->data) exit(1);
}

static void free_visited(visited_set_t *set) {
  free(set->data);
}

static bool visited_contains(visited_set_t *set, uint64_t state) {
  for (size_t i = 0; i < set->size; i++) {
    if (set->data[i] == state)
      return true;
  }
  return false;
}

static void visited_add(visited_set_t *set, uint64_t state) {
  if (set->size == set->capacity) {
    set->capacity *= 2;
    set->data = realloc(set->data, set->capacity * sizeof(uint64_t));
    if (!set->data) exit(1);
  }
  set->data[set->size++] = state;
}

static struct game_state copy_state(const struct game_state *state) {
  struct game_state copy;
  memcpy(&copy, state, sizeof(struct game_state));
  return copy;
}

typedef void (*move_func_t)(struct game_state *);

static bool try_move(const struct game_state *orig, move_func_t move, struct game_state *result) {
  *result = copy_state(orig);
  move(result);
  return serialize(*result) != serialize(*orig);
}

int number_of_moves(struct game_state start) {
  if (is_solved(start))
    return start.num_steps;

  struct queue q;
  q.data.head = NULL;
  enqueue(&q, start);

  visited_set_t visited;
  init_visited(&visited);
  visited_add(&visited, serialize(start));

  while (q.data.head != NULL) {
    struct game_state cur = dequeue(&q);
    if (is_solved(cur)) {
      free_visited(&visited);
      free_list(q.data);
      return cur.num_steps;
    }

    move_func_t moves[4] = { move_up, move_down, move_left, move_right };

    for (int i = 0; i < 4; i++) {
      struct game_state next;
      if (try_move(&cur, moves[i], &next)) {
        uint64_t serialized_next = serialize(next);
        if (!visited_contains(&visited, serialized_next)) {
          visited_add(&visited, serialized_next);
          enqueue(&q, next);
        }
      }
    }
  }

  free_visited(&visited);
  return -1;
}
