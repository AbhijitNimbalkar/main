#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#define TRUE                    1
#define FALSE                   0

#define SUCCESS                 1
#define FAILURE                 0

#define LIST_DATA_NOT_FOUND     2
#define LIST_EMPTY              3

typedef int data_t;
typedef int status_t;
typedef int len_t;
typedef int bool;

//As singly linked list it contains only forward pointer
struct node
{
    data_t          data;
    struct node*    next;
};
typedef struct node node_t;
typedef node_t list_t;

void* xmalloc(size_t size_in_bytes)
{
    void * p = NULL;
    p = malloc(size_in_bytes);
    assert(p!= NULL);
    memset(p,0,size_in_bytes);
    return (p);
}
node_t* get_list_node(data_t new_data)
{
    
     node_t* p_new_node =NULL;
     p_new_node = (node_t*)xmalloc(sizeof(node_t));
     p_new_node->data = new_data;
     p_new_node->next = NULL;
     return (p_new_node);
}

list_t* create_list(void)
{
    list_t* p_list =NULL;
    
    p_list =  get_list_node(-1);
    return (p_list);
}

void generic_insert(node_t* p_prev,node_t* p_next)
{
    p_next->next = p_prev->next;
    p_prev->next = p_next;
}

status_t insert_beg(list_t* p_list,data_t new_data)
{
    if(is_list_empty(p_list))
        p_list->next =get_list_node(new_data);
    else
    {
        generic_insert(p_list,get_list_node(new_data));
    }
    return (SUCCESS);
}
status_t insert_end(list_t* p_head,data_t new_data)
{
    //as data starts from p_list->next, and p_list is head node /dummy node
    node_t* p_run = p_head;
    while(p_run->next != NULL)
        p_run = p_run->next;
    
    node_t* new_node = get_list_node(new_data);
    p_run->next = new_node;
    return (SUCCESS);
}

bool is_list_empty(list_t* p_head)
{
    if(p_head ->next == NULL)
        return (TRUE);
    else 
        return (FALSE);
}

void show_list(list_t* p_head)
{
    node_t* p_run =NULL;
    p_run = p_head->next;
    printf("BEG==>");
    while(p_run != NULL)
    {
        printf("%d==>",p_run->data);
        p_run = p_run->next;
    }
    printf("END");
}
node_t* locate_node(list_t* p_list,data_t key)
{
    node_t* p_run =NULL;
    p_run = p_list->next;
    while(p_run != NULL)
    {
        if(p_run->data == key)
        break;
        p_run = p_run->next;
    }
    return (p_run);
}
status_t insert_after(list_t* p_list,data_t key,data_t new_data)
{
    node_t* p_node_found = locate_node(p_list,key);
    if(p_node_found)
        return (LIST_DATA_NOT_FOUND);

    generic_insert(p_node_found,new_data);
    return(SUCCESS);
}
status_t insert_before(list_t* p_list,data_t e_data,data_t new_data)
{
    node_t* p_run = NULL;
    node_t* p_prev = NULL;
    p_run = p_prev = p_list;
    while(p_run->next != NULL)
    {
        if(p_run->data == e_data)
            break;
        p_prev = p_run;
        p_run = p_run->next;
    }
    generic_insert(p_prev,get_list_node(new_data));
    return (SUCCESS);
}

status_t get_beg(list_t* p_head,data_t* p_data)
{
    if(is_list_empty(p_head))
        return (LIST_EMPTY);
    
    *p_data = p_head->next->data;
    return (SUCCESS);
}

status_t get_end(list_t* p_head,data_t* p_data)
{
    list_t* p_run =p_head;
    while(p_run != NULL)
    {
        if(p_run->next == NULL)
            break;
        p_run = p_run->next;
    }
    *p_data = p_run->data;

    return (SUCCESS);
}

status_t pop_beg(list_t* p_head,data_t* p_data)
{
    node_t* p_first =NULL;
    if(is_list_empty(p_head))
        return (LIST_EMPTY);

    *p_data = p_head->next->data;
    p_first = p_head->next;
    p_head = p_first->next;
    
    free(p_first);
    p_first =NULL;

    return (SUCCESS);
}

status_t pop_end(list_t* p_head,data_t* p_data)
{
    node_t* p_run =NULL;
    node_t* p_prev =NULL;
    if(is_list_empty(p_head))
        return (LIST_EMPTY);
    p_run = p_head;
    while(p_run->next != NULL)
    {
        p_prev =p_run;
        p_run= p_run->next;
    }
    p_prev->next = NULL;
    free(p_run);
    p_run =NULL;
    return (SUCCESS);
}
int main()
{
    
    list_t* p_list =NULL;
    p_list = create_list();
    data_t d = 0;
    assert(p_list != NULL);
    
    assert(is_list_empty(p_list) == TRUE);
    
    for(int i =0;i<5;i++)
        insert_beg(p_list,i);
    puts("After insert beg \n");
    show_list(p_list);
    puts("\n");

    for(int i =5; i<=10;i++)
        insert_end(p_list,i);
    puts("After insert end \n");
    show_list(p_list);
    puts("\n");

    insert_after(p_list,5,300);
    puts("After insert_After  \n");
    show_list(p_list);
    puts("\n");

    insert_before(p_list,2,500);
    puts("After insert_before  \n");
    show_list(p_list);
    puts("\n");


    get_beg(p_list,&d);
    printf("BEG ==>%d\n",d);
    puts("\n");

    get_end(p_list,&d);
    printf("END ==>%d\n",d);
    puts("\n");
    


    return (EXIT_SUCCESS);
}