#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>

typedef struct node  node_t;
typedef node_t  queue_t;
typedef int status_t;
typedef int data_t;

#define  EMPTY_QUEUE 100
#define TRUE    1
#define FALSE   0

struct node
{
    data_t     m_data;
    node_t* m_prev;
    node_t* m_next;
};

void* xmalloc(size_t size_in_bytes);
queue_t* create_queue();
status_t empty(queue_t* q);
status_t empty(queue_t* q);
node_t* get_new_node(data_t node_value);
void generic_insert(node_t* start,node_t* mid,node_t* end);
void insert_node_start(queue_t* q,data_t node_value);
void insert_node_end(queue_t* q,data_t node_value);
void push(queue_t* q,data_t node_value);
size_t size(queue_t* q);
const data_t front(queue_t* q);
const data_t back(queue_t* q);
void display_queue(queue_t* q);
void generic_delete(node_t* delete_node);
void remove_start(queue_t* q);
status_t pop(queue_t* q);
