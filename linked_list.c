#include "linked_list.h"

#include <stdlib.h>

struct list_node *new_node(size_t value) {
    struct list_node *node = (struct list_node *)malloc(sizeof(struct list_node));
    if (node) {
        node->value = value;
        node->next = NULL;
    }
    return node;
}

void insert_at_head(struct linked_list *list, size_t value) {
    struct list_node *node = new_node(value);
    if (node) {
        node->next = list->head;
        list->head = node;
        if (list->tail == NULL) {
            list->tail = node;
        }
    }
}

void insert_at_tail(struct linked_list *list, size_t value) {
    struct list_node *node = new_node(value);
    if (node) {
        if (list->tail) {
            list->tail->next = node;
        } else {
            list->head = node;
        }
        list->tail = node;
    }
}

size_t remove_from_head(struct linked_list *list) {
    if (list->head == NULL) {
        return 0; 
    }
    struct list_node *temp = list->head;
    size_t value = temp->value;
    list->head = list->head->next;
    if (list->head == NULL) {
        list->tail = NULL;
    }
    free(temp);
    return value;
}

size_t remove_from_tail(struct linked_list *list) {
    if (list->tail == NULL) {
        return 0; 
    }
    struct list_node *current = list->head;
    struct list_node *prev = NULL;
    while (current->next != NULL) {
        prev = current;
        current = current->next;
    }
    size_t value = current->value;
    if (prev) {
        prev->next = NULL;
        list->tail = prev;
    } else {
        list->head = list->tail = NULL;
    }
    free(current);
    return value;
}

void free_list(struct linked_list list) {
    struct list_node *current = list.head;
    while (current != NULL) {
        struct list_node *next = current->next;
        free(current);
        current = next;
    }
    list.head = list.tail = NULL;
}

// Utility function to help you debugging, do not modify
void dump_list(FILE *fp, struct linked_list list) {
  fprintf(fp, "[ ");
  for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) {
    fprintf(fp, "%zu ", cur->value);
  }
  fprintf(fp, "]\n");
}
