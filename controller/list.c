#include "list.h"
#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Insert a new entry between two known consecutive entries. 
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static  void __list_add(struct list_head * NEW,
    struct list_head * prev,
    struct list_head * next)
{
    next->prev = NEW;
    NEW->next = next;
    NEW->prev = prev;
    prev->next = NEW;
}

/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
void list_add(struct list_head *NEW, struct list_head *head)
{
    __list_add(NEW, head, head->next);
}

/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
void list_add_tail(struct list_head *NEW, struct list_head *head)
{
    __list_add(NEW, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static  void __list_del(struct list_head * prev,
                  struct list_head * next)
{
    next->prev = prev;
    prev->next = next;
}

/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty on entry does not return true after this, the entry is in an undefined state.
 */
void list_del(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
}

/**
 * list_del_init - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
void list_del_init(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    INIT_LIST_HEAD(entry); 
}

/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
int list_empty(struct list_head *head)
{
    return head->next == head;
}

/**
 * list_replace - replace old entry by new one
 * @old : the element to be replaced
 * @new : the new element to insert
 *
 * If @old was empty, it will be overwritten.
 */
void list_replace(struct list_head *old,
				struct list_head *NEW)
{
	NEW->next = old->next;
	NEW->next->prev = NEW;
	NEW->prev = old->prev;
	NEW->prev->next = NEW;
}

void list_replace_init(struct list_head *old,
					struct list_head *NEW)
{
	list_replace(old, NEW);
	INIT_LIST_HEAD(old);
}
#ifdef __cplusplus
}
#endif

