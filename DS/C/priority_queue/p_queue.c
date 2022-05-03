/*
    This program is unordered representation of priority queue
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define TRUE 1
#define FALSE 0
#define ERROR_LIST_EMPTY 101
#define ERROR_QUEUE_EMPTY 102
#define SUCCESS 1;

typedef struct node node_t;
typedef node_t list_t; // to pass list to all functions did typedef
typedef int data_t;
typedef list_t priority_queue;
typedef int status_t;
struct node
{
    data_t  m_data;
    node_t* m_next;
    node_t* m_prev;
};
 void * xmalloc(size_t size_in_bytes)
 {
     void * pv =malloc(size_in_bytes);
     assert(pv != NULL);
     return (pv);
 }
 list_t* create_list()
 {
     list_t* p_list = (list_t*) xmalloc(sizeof(list_t));
    p_list->m_next = p_list;
    p_list->m_prev = p_list;
    p_list->m_data = -1;
    return (p_list);
 }
void generic_insert(node_t* p_beg,node_t* p_mid,node_t* p_end)
{
    p_mid->m_next = p_end;
    p_mid->m_prev = p_beg;
    p_beg->m_next = p_mid;
    p_end->m_prev = p_mid;
}

void generic_delete(node_t* delete_node)
{
    delete_node->m_prev->m_next = delete_node->m_next;
    delete_node->m_next->m_prev = delete_node->m_prev;
    free(delete_node);
    delete_node =NULL;
}
node_t* get_node(data_t _data)
{
    node_t* p_node = NULL;
    p_node =(node_t*) xmalloc(sizeof(node_t));
    assert(p_node != NULL);
    p_node->m_data = _data;
    p_node->m_next =NULL;
    p_node->m_prev =NULL;
    return (p_node);
}
void insert_beg(list_t* p_list,data_t _data)
{   
    generic_insert(p_list,get_node(_data),p_list->m_next);
}
void insert_end(list_t* p_list,data_t _data)
{
    generic_insert(p_list->m_prev,get_node(_data),p_list);
}
int is_list_empty(list_t* p_list)
{
    return (p_list->m_next == p_list && p_list->m_prev == p_list);
}

int remove_beg(list_t* p_list)
{
    if(is_list_empty(p_list))
        return (ERROR_LIST_EMPTY);
    data_t _data = p_list->m_next->m_data;
    generic_delete(p_list->m_next);
    return (_data);
}

void display_list(list_t* p_list)
{
    node_t* p_run_node = NULL;
    printf("\n[START]=>");
    for(
            p_run_node = p_list->m_next;
            p_run_node!= p_list;
            p_run_node = p_run_node->m_next
        )
    {
        printf("[%ld]",p_run_node->m_data);
    }
    printf("[END]");
}
void destroy_list(list_t** pp_list)
{
    list_t* p_list = *pp_list;
    node_t* p_run_node = p_list->m_next;
    while(p_run_node != p_list)
    {
        node_t* p_next_node = p_run_node->m_next;
        free(p_run_node);
        p_run_node =p_next_node;
    }
    free(p_list);
    *pp_list =NULL;
}

priority_queue* create_priority_queue()
{
    return create_list();
}

void priority_enque(priority_queue* p_queue ,data_t _data)
{
    insert_end(p_queue,_data);
}
int is_priority_queue_empty(priority_queue* p_queue)
{
    return (is_list_empty(p_queue));
}
void destroy_priority_queue(priority_queue** p_queue)
{
    destroy_list(p_queue);
}

status_t priority_dequeue_min(priority_queue* p_queue,data_t* _data)
{
    if(is_priority_queue_empty(p_queue))
        return ERROR_QUEUE_EMPTY;

    node_t* p_min_node = p_queue->m_next;
    data_t min_data =p_queue->m_next->m_data;
    for (
            node_t* p_run_node =p_queue->m_next ; 
            p_run_node != p_queue; 
            p_run_node =p_run_node=p_run_node->m_next
        )
     {
         if(min_data > p_run_node->m_data)
         {
             min_data = p_run_node->m_data;
             p_min_node =p_run_node;
         }
     }

     generic_delete(p_min_node);
     *_data = min_data;
     return (1);
}


int main()
{
    priority_queue* p_prio_queue = NULL; 
    int arr[] = {100, 40, 20, 10, 80, 90, 75, 52, 23}; 
    int i; 
    int data; 
    int ret; 

    p_prio_queue = create_priority_queue(); 
    for(i = 0; i < sizeof(arr)/sizeof(arr[0]); ++i)
        priority_enque(p_prio_queue, arr[i]); 

    while(is_priority_queue_empty(p_prio_queue) != 1)
    {
        data = -1; 
        priority_dequeue_min(p_prio_queue, &data); 
        printf("data = %d\n", data); 
    }

    destroy_priority_queue(&p_prio_queue); 
    assert(p_prio_queue == NULL); 
    
    return (EXIT_SUCCESS);
}