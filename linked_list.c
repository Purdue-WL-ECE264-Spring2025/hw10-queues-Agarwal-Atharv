#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>

struct list_node *new_node(size_t value) {
    struct list_node *node = (struct list_node *)malloc(sizeof(struct list_node));
    if (node == NULL) {
        return NULL;
    }
    node->value = value;
    node->next = NULL;
    return node;
}

void insert_at_head(struct linked_list *list, size_t value) {
    struct list_node *node = new_node(value);
    if (node == NULL) {
        return;
    }
    node->next = list->head;
    list->head = node;
}

void insert_at_tail(struct linked_list *list, size_t value) {
    struct list_node *node = new_node(value);
    if (node == NULL) {
        return;
    }

    if (list->head == NULL) {
        list->head = node;
    } else {
        struct list_node *cur = list->head;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = node;
    }
}

size_t remove_from_head(struct linked_list *list) {
    if (list->head == NULL) {
        return 0;
    }

    struct list_node *node_to_remove = list->head;
    size_t value = node_to_remove->value;
    list->head = list->head->next;
    free(node_to_remove);

    return value;
}

// Remove a node from the tail of the list
size_t remove_from_tail(struct linked_list *list) {
    if (list->head == NULL) {
        return 0;
    }

    if (list->head->next == NULL) {
        size_t value = list->head->value;
        free(list->head);
        list->head = NULL;
        return value;
    }

    struct list_node *cur = list->head;
    while (cur->next->next != NULL) {
        cur = cur->next;
    }

    size_t value = cur->next->value;
    free(cur->next);
    cur->next = NULL;

    return value;
}

void free_list(struct linked_list list) {
    while (list.head != NULL) {
        remove_from_head(&list);
    }
}

void dump_list(FILE *fp, struct linked_list list) {
  fprintf(fp, "[ ");
  for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) {
    fprintf(fp, "%zu ", cur->value);
  }
  fprintf(fp, "]\n");
}
