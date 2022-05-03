#include "cqueue.h"

void* xmalloc(size_t size_in_bytes)
{
    void* p = NULL;
    p = malloc(size_in_bytes);
    assert(p != NULL);//if expression evaluates to true it does nothing
    return (p);
}
queue_t* create_queue()
{
    queue_t* q = NULL;
    q = (queue_t*)xmalloc(sizeof(queue_t));
    memset(q,0,sizeof(q));//initialize all members to NULL
    q->m_data = 0;
    q->m_next = q;
    q->m_prev = q;
    return (q);
}
status_t empty(queue_t* q)
{
    if(q->m_next == q || q->m_data == 0)
        return (TRUE);
    else 
        return (FALSE);
}
node_t* get_new_node(data_t node_value)
{
    node_t* n =NULL;
    n = (node_t*) xmalloc(sizeof(node_t));
    n->m_next =NULL;
    n->m_prev =NULL;
    n->m_data = node_value;
    return (n);
}
void generic_insert(node_t* start,node_t* mid,node_t* end)
{
    mid->m_next = end;
    mid->m_prev = start;
    start->m_next = mid;
    end->m_prev = mid;
}
void insert_node_start(queue_t* q,data_t node_value)
{
    node_t* n =get_new_node(node_value);
 
    generic_insert(q,n,q->m_next);
}
void insert_node_end(queue_t* q,data_t node_value)
{
    node_t* n = get_new_node(node_value);

    generic_insert(q->m_prev,n,q);
}
void push(queue_t* q,data_t node_value)
{
    insert_node_end(q,node_value);
    q->m_data++;//use dummy node data member as size
}
size_t size(queue_t* q)
{
    return (size_t)(q->m_data);
}
const data_t front(queue_t* q)
{
    if(empty(q) ==TRUE)
        return EMPTY_QUEUE;
    else
        return q->m_next->m_data;//return data not pointer of node
}
const data_t back(queue_t* q)
{
    if(empty(q) ==TRUE)
        return EMPTY_QUEUE;
    else
        return q->m_prev->m_data;//return data not pointer of node
}
void display_queue(queue_t* q)
{
    node_t* run_node =NULL;
    if(empty(q) == TRUE)
        return;
    printf("[START] ==>");
    for (run_node =q->m_next; run_node !=  q; run_node =run_node->m_next)
    {
        printf("[ %d ]==>",run_node ->m_data);
    }
    printf("[END] \n");
}
void generic_delete(node_t* delete_node)
{
    delete_node->m_prev->m_next =delete_node->m_next;
    delete_node->m_next->m_prev = delete_node->m_prev;

    free(delete_node);
    delete_node = NULL;
}
void remove_start(queue_t* q)
{
    //delete first first in 
    generic_delete(q->m_next);
}
status_t pop(queue_t* q)
{
    if(empty(q))
        return EMPTY_QUEUE;

    remove_start(q);
    q->m_data --;
    return TRUE;
}
int main()
{
    queue_t* q = NULL;
    q = create_queue();

    // all these assert should be successful
    assert(empty(q) == TRUE);
    assert(front(q) ==EMPTY_QUEUE);
    assert(back(q) ==EMPTY_QUEUE);
    assert(size(q) == 0);

    for(int i =1; i<= 10; i++)
        push(q,i);

    printf("SIZE of queue after insertion = \t[%d]  \n",size(q));
    printf("FRONT element of queue = \t\t[%d] \n",front(q));
    printf("BACK element of queue = \t\t[%d] \n",back(q));
    display_queue(q);
    pop(q);
    pop(q);
    printf("\n Queue after 2 pop operations\n");
    display_queue(q);
    printf("SIZE of queue after insertion = \t[%d]  \n",size(q));
    printf("FRONT element of queue = \t\t[%d] \n",front(q));
    printf("BACK element of queue = \t\t[%d] \n",back(q));
    return (EXIT_SUCCESS);
}