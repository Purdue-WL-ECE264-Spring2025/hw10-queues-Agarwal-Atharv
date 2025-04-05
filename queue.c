#include "queue.h"
#include "tile_game.h"
#include <stdlib.h>
#include <stdbool.h>

static bool is_goal_state(struct game_state state) {
  int expected = 1;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (i == 3 && j == 3) {
        if (state.tiles[i][j] != 0)
          return false;
      } else {
        if (state.tiles[i][j] != expected)
          return false;
        expected++;
      }
    }
  }
  return true;
}

static int bfs(struct game_state start) {
  if (is_goal_state(start))
    return start.num_steps;

  struct queue q;
  q.data.head = NULL;
  struct linked_list visited;
  visited.head = NULL;

  uint64_t startSer = serialize(start);
  insert_at_tail(&(q.data), (size_t)startSer);
  insert_at_tail(&visited, (size_t)startSer);

  while (q.data.head) {
    uint64_t ser = (uint64_t)remove_from_head(&(q.data));
    struct game_state cur = deserialize(ser);
    if (is_goal_state(cur)) {
      free_list(q.data);
      free_list(visited);
      return cur.num_steps;
    }

    struct game_state next;

    next = cur;
    move_up(&next);
    if (next.num_steps != cur.num_steps) { 
      uint64_t key = serialize(next);
      bool seen = false;
      for (struct list_node *n = visited.head; n; n = n->next) {
        if (n->value == key) {
          seen = true;
          break;
        }
      }
      if (!seen) {
        insert_at_tail(&visited, (size_t)key);
        insert_at_tail(&(q.data), (size_t)key);
      }
    }

    next = cur;
    move_down(&next);
    if (next.num_steps != cur.num_steps) {
      uint64_t key = serialize(next);
      bool seen = false;
      for (struct list_node *n = visited.head; n; n = n->next) {
        if (n->value == key) {
          seen = true;
          break;
        }
      }
      if (!seen) {
        insert_at_tail(&visited, (size_t)key);
        insert_at_tail(&(q.data), (size_t)key);
      }
    }

    next = cur;
    move_left(&next);
    if (next.num_steps != cur.num_steps) {
      uint64_t key = serialize(next);
      bool seen = false;
      for (struct list_node *n = visited.head; n; n = n->next) {
        if (n->value == key) {
          seen = true;
          break;
        }
      }
      if (!seen) {
        insert_at_tail(&visited, (size_t)key);
        insert_at_tail(&(q.data), (size_t)key);
      }
    }

    next = cur;
    move_right(&next);
    if (next.num_steps != cur.num_steps) {
      uint64_t key = serialize(next);
      bool seen = false;
      for (struct list_node *n = visited.head; n; n = n->next) {
        if (n->value == key) {
          seen = true;
          break;
        }
      }
      if (!seen) {
        insert_at_tail(&visited, (size_t)key);
        insert_at_tail(&(q.data), (size_t)key);
      }
    }
  }

  free_list(q.data);
  free_list(visited);
  return -1;
}

int number_of_moves(struct game_state start) {
  return bfs(start);
}

void enqueue(struct queue *q, struct game_state state) {
  uint64_t ser = serialize(state);
  insert_at_tail(&(q->data), (size_t)ser);
}

struct game_state dequeue(struct queue *q) {
  size_t ser = remove_from_head(&(q->data));
  return deserialize((uint64_t)ser);
}
