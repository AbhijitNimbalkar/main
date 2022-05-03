#include <iostream>
#include "list.h"

node::node(data_t new_data)
:m_data(new_data),m_prev(nullptr),m_next(nullptr)
{}

list::list():m_phead_node(new node(0))
{
    m_phead_node->m_prev = m_phead_node;
    m_phead_node->m_next = m_phead_node;
}

list::~list()
{
    node* p_run         = nullptr;
    node* p_run_next    = nullptr;
    p_run =  m_phead_node->m_next;
    while(p_run != m_phead_node)//doubly linked list
    {
        p_run_next = p_run->m_next;
        delete p_run;
        p_run = p_run_next;
    }
    delete m_phead_node;
    m_phead_node = nullptr;
}
void list::generic_insert(node* p_beg,node* p_mid,node* p_end)
{
    p_mid->m_next = p_end;
    p_mid->m_prev = p_beg;
    p_beg->m_next = p_mid;
    p_end->m_prev = p_mid;

}
void list::generic_delete(node* p_delete_node)
{
    p_delete_node->m_prev->m_next = p_delete_node->m_next;
    p_delete_node->m_next->m_prev = p_delete_node->m_prev;
    delete p_delete_node;
    p_delete_node = nullptr;
}
status_t list::insert_start(data_t new_data)
{
    generic_insert(m_phead_node,new node(new_data),m_phead_node->m_next);
    return (SUCCESS);
}
status_t list::insert_end(data_t new_data)
{
    generic_insert(m_phead_node->m_prev,new node(new_data),m_phead_node);
    return (SUCCESS);
}
void list::show(const char* msg) const
{
    node* p_run =nullptr;
    
    std::cout<<msg<<std::endl;
    std::cout<<"[START]<=>";
    p_run = m_phead_node->m_next;//dont start from dummy node
    while(p_run != m_phead_node)//circular linked list
    {
        std::cout<<p_run->m_data<<"<=>";
        p_run = p_run->m_next;
    }
    std::cout<<"[END]"<<std::endl;
}
node* list::search_node(data_t key)const
{
    node* p_run = m_phead_node->m_next; //always start from next to dummy node
    while(p_run != m_phead_node)
    {
        if(p_run->m_data == key)
            return p_run;
        p_run = p_run->m_next;
    }
    return nullptr;
}
status_t list::insert_after(data_t e_data,data_t new_data)
{
    node* p_key_node = search_node(e_data);
    if(p_key_node == nullptr)
        return FAIL;
    
    generic_insert(p_key_node,new node(new_data),p_key_node->m_next);
    return (SUCCESS);
}

status_t list::insert_before(data_t key,data_t new_data)
{
    node* key_node = search_node(key);
    if(key_node ==nullptr)
        return (FAIL);

    generic_insert(key_node->m_prev,new node(new_data),key_node);
    return (SUCCESS);
}
bool list::is_empty() const
{
    //if head next and prev pointing to head itself then list is empty
    return (m_phead_node->m_next == m_phead_node 
            && m_phead_node->m_prev == m_phead_node);
}
bool list::contains(data_t key) const
{
    node * key_node = search_node(key);
    return (key_node == nullptr ?false:true);
}
len_t list::get_length()const
{
    node* p_run = nullptr;
    len_t len =0;
    p_run = m_phead_node->m_next;
    while(p_run != m_phead_node)
    {
        len++;
    }
    return len;
}