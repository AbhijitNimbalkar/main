/*
    Author - Nimbalkar Abhijit
    Tested - Working
    Date   - 26/9/2021
*/

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
struct node
{
    data_t data;
    struct node *prev;
    struct node *next;
};

typedef int status_t;
typedef int len_t;
typedef int bool;
typedef struct node node_t;
typedef node_t list_t;

void* xmalloc(size_t size_in_bytes)
{
    void *p = NULL;
    p = malloc(size_in_bytes);
    assert(p!= NULL);
    memset(p,0,size_in_bytes);
    return p;
}
node_t* get_list_node(data_t new_data)
{
    node_t* p_new_node =   NULL;
    p_new_node = (node_t*)xmalloc(sizeof(node_t));
    p_new_node->data = new_data ;
    p_new_node->prev =NULL;
    p_new_node->next =NULL;
    return (p_new_node);
}
list_t *create_list(void)
{
    node_t * p_new_node= NULL;
    p_new_node = get_list_node(0);
    p_new_node->prev = p_new_node;
    p_new_node->next = p_new_node;
    return p_new_node;
}
void generic_insert(node_t* p_beg,node_t* p_mid,node_t* p_end)
{
    //p_mid is new node needs to be inserted between p_beg and p_end
    p_mid->next = p_end;
    p_mid->prev = p_beg;
    p_end->prev = p_mid;
    p_beg->next = p_mid;
}

status_t insert_beg(list_t * p_list,data_t new_data)
{
    generic_insert(p_list,get_list_node(new_data),p_list->next);
    return SUCCESS;
}
status_t insert_end(list_t* p_list,data_t new_data)
{
    generic_insert(p_list->prev,get_list_node(new_data),p_list);
    return SUCCESS;
}

node_t* locate_node(list_t* p_list,data_t search_data)
{
    node_t* p_run_node =NULL;
    p_run_node = p_list->next; //as first node is 
    //dummy node data always start from 2nd node
    while(p_run_node != p_list)
    {
        if(p_run_node->data == search_data)
            return (p_run_node);
         p_run_node = p_run_node->next ;
    }
    return (NULL);
}
status_t insert_after(list_t* p_list,data_t e_data,data_t new_data)
{
    node_t* p_node_found = locate_node(p_list,e_data);
    assert(p_node_found != NULL);
    
    if(p_node_found == NULL)
        return (LIST_DATA_NOT_FOUND);
    generic_insert(p_node_found,get_list_node(new_data),p_node_found->next);
    return (SUCCESS);
}
status_t insert_before(list_t* p_list,data_t e_data,data_t new_data)
{
    node_t* p_key_node = locate_node(p_list,e_data);
    assert(p_key_node != NULL);
    if(p_key_node ==NULL)
        return (LIST_DATA_NOT_FOUND);

    generic_insert(p_key_node->prev,get_list_node(new_data),p_key_node);
    return (SUCCESS);
}
bool is_list_empty(list_t* p_list)
{
    return (p_list->prev == p_list && p_list->next ==p_list);
}
status_t get_beg(list_t* p_list,data_t* p_data)
{
    if(is_list_empty(p_list))
    {
        return (LIST_EMPTY);
    }
    *p_data = p_list->next->data;
    return (SUCCESS);
}
void generic_delete(node_t* p_delete_node)
{
    p_delete_node->next->prev = p_delete_node->prev;
    p_delete_node->prev->next = p_delete_node->next;
    free(p_delete_node);
    p_delete_node =NULL;
}
status_t get_end(list_t* p_list,data_t* p_data)
{
    if(is_list_empty(p_list))
    {
        return (LIST_EMPTY);
    }
    *p_data = p_list->prev->data;
    return (SUCCESS);
}

status_t pop_beg(list_t* p_list,data_t* p_data)
{
    if(is_list_empty(p_list) ==TRUE)
    {
        return (LIST_EMPTY);
    }
    *p_data = p_list->next->data;
    generic_delete(p_list->next);
    return (SUCCESS);
}
status_t pop_end(list_t* p_list,data_t* pp_data)
{
    if(is_list_empty(p_list) == TRUE)
    {
        return (LIST_EMPTY);
    }
    *pp_data = p_list->prev->data;
    generic_delete(p_list->prev);
    return (SUCCESS);
}
status_t remove_beg(list_t* p_list)
{
    if(is_list_empty(p_list) == TRUE)
    {
        return (LIST_EMPTY);
    }
    generic_delete(p_list->next);
    return (SUCCESS);
} 
status_t remove_end(list_t* p_list)
{
    if(is_list_empty(p_list) ==TRUE)
    {
        return (LIST_EMPTY);
    }
    generic_delete(p_list->prev);
    return (SUCCESS);
}

status_t remove_data(list_t *p_list,data_t r_data)
{
    node_t* p_node=locate_node(p_list,r_data);
    if(p_node ==NULL)
    {
        return (LIST_DATA_NOT_FOUND);
    }
    generic_delete(p_node);
    return (SUCCESS);
}
len_t get_length(list_t* p_list)
{
    node_t* p_run =NULL;
    len_t len=0;
    for(p_run=p_list->next,len=0;p_run != p_list;p_run=p_run->next,++len)
    {   }
    return (len);
}
bool contains(list_t * p_list,data_t search_data)
{
    return (locate_node(p_list,search_data)!= NULL);
}

int get_repeat_count(list_t * p_list,data_t search_data)
{
    int count =0;
    for(list_t* p_run = p_list->next;p_run != p_list;p_run= p_run->next)
    {
        if(p_run->data == search_data)
        {
            ++count;
        }
    }
    return (count);
}
void show_list(list_t * p_list,const char* msg)
{
    node_t* p_run =NULL;
    if(msg)
    {
        puts(msg);
    }
    printf("[BEG]<->");
    for(p_run = p_list->next;p_run!= p_list;p_run=p_run->next)
    {
        printf("[%d]<->",p_run->data);
    }
    puts("[END]");
}
list_t * concat_lists(list_t* p_list1,list_t * p_list2)
{
    list_t* p_new_list = create_list();
    node_t* p_run = NULL;
    for(p_run = p_list1->next;p_run!= p_list1;p_run= p_run->next)
    {
        insert_end(p_new_list,p_run->data);
    }
    for(p_run =p_list2->next;p_run != p_list2;p_run = p_run->next)
    {
        insert_end(p_new_list,p_run->data);
    }
    return (p_new_list);
}
list_t* merge_lists(list_t* p_list1,list_t* p_list2)
{
    list_t* p_merge_list =create_list();
    node_t* p_run1 = p_list1->next;
    node_t* p_run2 = p_list2->next;
    while (TRUE)
    {
        //if list 1 is empty
        if(p_run1 ==  p_list1)
        {
            while(p_run2 != p_list2)
            {
                insert_end(p_merge_list,p_run2->data);
                p_run2 = p_run2->next;
                break;
            }
        }
        //if list 2 is empty
        if(p_run2 ==p_list2)
        {
            while(p_run1 != p_list1)
            {
                insert_end(p_merge_list,p_run1->data);
                p_run1=p_run1->next;
            }
            break;
        }
        if(p_run1->data <=p_run2->data)
        {
            insert_end(p_merge_list,p_run1->data);
            p_run1 = p_run1->next;
        }
        else
        {
            insert_end(p_merge_list,p_run2->data);
            p_run2 = p_run2->next;
        }
    }
    return p_merge_list;
}

list_t* get_reversed_list(list_t* p_list)
{
    list_t* p_reversed_list = create_list();
    node_t* p_run =NULL;
    p_run = p_list->prev;
    while(p_run != p_list)
    {
        insert_end(p_reversed_list,p_run->data);
        p_run = p_run->prev;
    }
    return (p_reversed_list);
}
list_t* array_to_list(data_t* p_array,size_t size)
{
    list_t* p_list  = create_list();
    for(int i=0;i<size;i++)
    {
        insert_end(p_list,p_array[i]);
    }
    return (p_list);
}
status_t append_list(list_t* p_list1,list_t* p_list2)
{

    p_list1->prev->next = p_list2->next;
    p_list2->next->prev = p_list1->prev;
    p_list2->prev->next = p_list1;
    p_list1->prev = p_list2->prev;
    free(p_list2);
    p_list2 =NULL;
    return (SUCCESS);
}
status_t destroy_list(list_t** pp_list)
{
    list_t* p_list = *pp_list;

    node_t* p_run_next =NULL;
    node_t* p_run =NULL;
    for( p_run = p_list->next;p_run =! p_list;p_run = p_run->next)
    {
        p_run_next = p_run->next;
        free(p_run);
        p_run=NULL;
    }
    free(*pp_list);
    *pp_list =NULL;
    return  (SUCCESS);
}
status_t list_to_array(list_t* p_list,data_t ** pp_array,size_t* p_size)
{
    int list_len = 0;
    list_len = get_length(p_list);
    if(list_len >0 )
    {
        int index = 0;
        *pp_array = (data_t*)xmalloc(sizeof(data_t)*list_len);
        assert(*pp_array == NULL);
        for(node_t* p_run = p_list->next;p_run != p_list;p_run = p_run->next)
        {
            *pp_array[index] = p_run->data;
        }
        *p_size =list_len;
    }
    return (SUCCESS);
}
status_t reverse_list(list_t* p_list)
{
    // go to 2nd last node
    node_t* p_run =NULL;
    node_t* p_run_prev =NULL;
    //  take backup of node
    node_t* p_temp = p_list->next;
    p_list->next = p_list->prev;
    //iterate from 2nd last node to dummy node backwords
    for(p_run =  p_list->prev->prev;p_run != p_list;p_run = p_run_prev)
    {
        //take backup of prev node before manuplating the chain
        p_run_prev =p_run->prev;

        //break the chain and put the current node at the end of 2nd last node sequentially
        generic_insert(p_list->prev, p_run,p_list);
    }
    p_temp->prev =p_list;
    return (SUCCESS);

}

int main(void)
{
    status_t s              = SUCCESS;
    data_t data             = 0;
    len_t len               = 0;
    list_t* p_list          = NULL;
    list_t* p_list1         = NULL;
    list_t* p_list2         = NULL;
    list_t* p_concat_list   = NULL;
    list_t* p_merge_list    = NULL;
    list_t* p_reversed_list = NULL;

    p_list = create_list();
    puts("***************************************start***************************************\n");
    assert(get_beg(p_list,&data) ==LIST_EMPTY);
    assert(get_end(p_list,&data) == LIST_EMPTY);
    
    show_list(p_list,"***************************************After Create_list()***************************************\n");

    for(data =0;data<5;data++)
    {
        s = insert_end(p_list,data);
        assert(s ==SUCCESS);
    }
    show_list(p_list,"***************************************After Insert End***************************************");


    for(data =5;data<=10;data++)
    {
        s = insert_beg(p_list,data);
        assert(s ==SUCCESS);
    }
    show_list(p_list,"***************************************After Insert beg***************************************");

    s= insert_after(p_list,0,100);
    assert(s ==SUCCESS);
    show_list(p_list,"***************************************After Insert       After***************************************");


    s= insert_before(p_list,0,200);
    assert(s ==SUCCESS);
    show_list(p_list,"***************************************After Insert       Before***************************************");

    data =0;
    s =get_beg(p_list,&data);   
    assert(s == SUCCESS);
    printf("Data At begining = %d \n",data);
    
    data =0;
    s =get_end(p_list,&data);
    assert(s == SUCCESS);
    printf("Data At end  = %d\n",data);

    data =0;
    s =pop_beg(p_list,&data);   
    assert(s == SUCCESS);
    printf("Data popped from start= %d \n",data);
    
    show_list(p_list,"***************************************After pop       beg***************************************");
    data =0;
    s =pop_end(p_list,&data);
    assert(s == SUCCESS);
    printf("Data popped from end  = %d\n",data);

    show_list(p_list,"***************************************After pop       end***************************************");


    s= remove_beg(p_list);
    assert(s == SUCCESS);
    show_list(p_list,"***************************************After remove beg***************************************");


    s= remove_end(p_list);
    assert(s == SUCCESS);
    show_list(p_list,"***************************************After remove end***************************************");

    s = remove_data(p_list,0);
    assert(s == SUCCESS);
show_list(p_list,"***************************************After remove data***************************************");

     printf("get_length(p_list): %d \n", get_length(p_list)); 

    if(contains(p_list, 200) == TRUE)
    {
        puts("200 is in list"); 
    }
    if(contains(p_list, -5) == FALSE)
    {
        puts("-5 is not in list"); 
    }

     printf("get_repeat_count(p_list, 4):%d\n", get_repeat_count(p_list, 4)); 
    printf("get_repeat_count(p_list, 200):%d\n", get_repeat_count(p_list, 200)); 
    assert(insert_end(p_list, 100) == SUCCESS); 

    printf("get_repeat_count(p_list, 100):%d\n", get_repeat_count(p_list, 100)); 
    
    s = destroy_list(&p_list); 
    assert(s == SUCCESS && p_list == NULL); 

    p_list1 = create_list(); 
    p_list2 = create_list(); 

    for(data = 5; data <= 95; data += 10)
    {
        insert_end(p_list1, data); 
        insert_end(p_list2, data+5); 
    }

    show_list(p_list1, "List1:"); 
    show_list(p_list2, "List2:"); 

    p_concat_list = concat_lists(p_list1, p_list2); 
    show_list(p_concat_list, "Concatenated version of List1 and List2:"); 
    
    p_merge_list = merge_lists(p_list1, p_list2); 
    show_list(p_merge_list, "Merged version of List1 and List2:"); 

    p_reversed_list = get_reversed_list(p_list1); 
    show_list(p_list1, "List1:"); 
    show_list(p_reversed_list, "Reversed version of List1:"); 

    show_list(p_list2, "List2 before undergoing reversal:");
    s = reverse_list(p_list2); 
    assert(s == SUCCESS); 
    show_list(p_list2, "Reversed List2:"); 

    s = append_list(p_list1, p_list2); 
    assert(s == SUCCESS); 
    show_list(p_list1, "List2 is appended behind list1:"); 
    
    s = destroy_list(&p_list1); 
    assert(s == SUCCESS && p_list1 == NULL); 

    s = destroy_list(&p_concat_list); 
    assert(s == SUCCESS && p_concat_list == NULL); 

    s = destroy_list(&p_merge_list); 
    assert(s == SUCCESS && p_merge_list == NULL); 

    s = destroy_list(&p_reversed_list); 
    assert(s == SUCCESS && p_reversed_list == NULL); 
    ///////////////////////////////////////////
    puts("***************************************End ***************************************");
    return (SUCCESS);
}