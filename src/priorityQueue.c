//
// Created by baka475 on 28.04.19.
//
#include "priorityQueue.h"
priorityQueue * newQueue () {
    priorityQueue * new = malloc(sizeof(priorityQueue));
    if (new) {
        new -> city = NULL;
        new -> next = NULL;
    }
    return new;
}
int isEmpty(priorityQueue * queue) {
    return queue -> next == NULL;
}
ListOfCities * pop(priorityQueue * queue) {
    if (isEmpty(queue))
        return NULL;
    priorityQueue * first = queue -> next;
    queue -> next = (queue -> next) -> next;
    ListOfCities * toReturn = first -> city;
    free(first);
    return toReturn;
}
void deleteSameElement(priorityQueue * queue, ListOfCities * element, priorityQueue * previous) {
    int test = true;
    while (queue && test) {
        if (queue->city == element) {
            previous->next = queue->next;
            free(queue);
            test = false;
        }
        if (test) {
            previous = queue;
            queue = queue->next;
        }
    }
}
bool push(priorityQueue * queue, ListOfCities * element, int weight) {
    while (queue -> next && (queue -> next) -> weight < weight)
        queue = queue -> next;
    priorityQueue * nextElement = queue -> next;
    priorityQueue * newElementInQueue = malloc(sizeof(priorityQueue));
    if (!newElementInQueue)
        return false;
    newElementInQueue -> city = element;
    newElementInQueue -> weight = weight;
    newElementInQueue -> next = nextElement;
    queue -> next = newElementInQueue;

    queue = nextElement;
    priorityQueue * previous = newElementInQueue;
    deleteSameElement(queue, element, previous);
    return true;
}
void removeQueue(priorityQueue * queue) {
    if (queue) {
        priorityQueue * next = queue -> next;
        removeQueue(next);
        free(queue);
    }
}
