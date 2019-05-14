/*
 * list.h
 * list operations
 *
 * copied from Linux_Kernel_Dir/include/linux/list.h
 */

#ifndef _MG_LIST_H
  #define _MG_LIST_H
#ifdef __cplusplus
  extern "C"
  {
#endif


struct list_head {
    struct list_head *next, *prev;
};

typedef struct list_head list_t;

#define LIST_HEAD_INIT(name) { &(name), &(name) }

/* define and init a list head */
#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)

/* init a list head */
#define INIT_LIST_HEAD(ptr) do { \
    (ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)

void list_add(struct list_head *NEW, struct list_head *head);
void list_add_tail(struct list_head *NEW, struct list_head *head);
void list_del(struct list_head *entry);
void list_del_init(struct list_head *entry);
int list_empty(struct list_head *head);


/**
 * list_entry - get the struct for this entry
 * @ptr:    the &struct list_head pointer.
 * @type:    the type of the struct this is embedded in.
 * @member:    the name of the list_struct within the struct.
 */
#define list_entry(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

/**
 * list_for_each    -    iterate over a list
 * @pos:    the &struct list_head to use as a loop counter.
 * @head:    the head for your list.
 */
#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)
            
/**
 * list_for_each_safe    -    iterate over a list safe against removal of list entry
 * @pos:    the &struct list_head to use as a loop counter.
 * @n:        another &struct list_head to use as temporary storage
 * @head:    the head for your list.
 */
#define list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); \
        pos = n, n = pos->next)

/**
 * list_for_each_ex    -    iterate over a list
 * @pos:    the &struct list_head to use as a loop counter.
 * @head:   the head for your list.
 * @begin:  the previous item of the begining item
 */
#define list_for_each_ex(pos, head, begin) \
    for (pos = (begin)->next; pos != (head); pos = (pos)->next)



    /**
     * list_first_entry - get the first element from a list
     * @ptr:    the list head to take the element from.
     * @type:   the type of the struct this is embedded in.
     * @member: the name of the list_struct within the struct.
     *
     * Note, that list is expected to be not empty.
     */
#define list_first_entry(ptr, type, member) \
        list_entry((ptr)->next, type, member)



    /**
     * list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
     * @pos:    the type * to use as a loop cursor.
     * @n:      another type * to use as temporary storage
     * @head:   the head for your list.
     * @member: the name of the list_struct within the struct.
     */
#define list_for_each_entry_safe(pos, n, head, member)			\
        for (pos = list_entry((head)->next, typeof(*pos), member),  \
            n = list_entry(pos->member.next, typeof(*pos), member); \
             &pos->member != (head);                    \
             pos = n, n = list_entry(n->member.next, typeof(*n), member))


/**
 * list_for_index    -    iterate over a list for index
 * @pos:    the &struct list_head to use as a loop counter.
 * @head:    the head for your list.
 */
#define list_for_index(pos, i, head, index) \
    for (pos = (head)->next, i=0; (pos != (head) && i < index); pos = pos->next,i++)


#ifdef __cplusplus
	}
#endif


#endif /* _MG_LIST_H */

