#include <iostream>
#include <string>
#include <cassert>
#include <memory>
#include <stdlib.h>
template <typename T>
class CSinglyList;

template <typename T>
class CNode
{
private:
    T           data;      
    CNode<T>*   m_next;      
public:
    CNode() {   };
    CNode(T _data)
    {   
        data = _data;
        m_next =nullptr;
    };
     friend class CSinglyList<T>;
};

template <typename T>
class CSinglyList
{
    CNode<T>* m_head;
public:

    CSinglyList()
    {
        m_head = new CNode<T>();
        m_head ->m_next = nullptr;
        
       assert(m_head != nullptr);
    }   

    int count()
    {
        int count =0;
        CNode<T>* run_node = this->m_head->m_next;
        while(run_node != nullptr)
        {
            run_node =run_node->m_next;
            count++;
        }   
        return count;
    }

    //this = l1 //assignment operator
    CSinglyList<T>& operator=(CSinglyList<T>*& l1)
    {
        CNode<T>* rn2 = l1->m_next;
        CNode<T>* nend =nullptr;
        Purge(); //purge list first and then copy elements one bye one
        while(rn2!= nullptr)
        {
            CNode<T>* n = new CNode<T>(rn2->data);
            if(m_head->m_next == nullptr)
            {
                m_head->m_next = n;
                nend = n;
            }
            else
            {
                nend->m_next = n;            
                nend = n;
            }
            rn2 = rn2->m_next;
        }

    }
    // it will push data into list at the end
    void push(T _val)
    {
        if(m_head)
        {
            CNode<T>* m_run =m_head;
            if(m_run)
            {
                while(m_run->m_next)
                {
                    m_run = m_run->m_next;
                }
            }
            CNode<T>* new_node =new CNode<T>(_val);
            assert(new_node!= nullptr);
            m_run->m_next = new_node;
        }
    }
    void Purge()
    {
        CNode<T>* pNode= m_head->m_next;
        while(pNode)
        {
            CNode<T>* tNode =pNode->m_next;
            delete pNode;
            pNode =nullptr;
            pNode =tNode;
        }
    }
   virtual  ~CSinglyList()//it will destroys list
    {
        std::cout<<__PRETTY_FUNCTION__<<std::endl;
        Purge();    
    }
    void print()
    {
        CNode<T>* p_run = m_head->m_next;
        while(p_run)
        {
            std::cout <<"["<<p_run->data<<"]";
            p_run =p_run->m_next;
        }
        std::cout<<std::endl;
    }
    void reverse()
    {
        
        CNode<T>* p=nullptr,*q =nullptr,*r=nullptr;
        p = m_head->m_next;
        while(p != nullptr)
        {
            r = q;
            q = p;
            p = p->m_next;
            q->m_next = r;
        }
        m_head->m_next = q;
    }
};

int main()
{
    CSinglyList<int> l ;
    CSinglyList<int> ln ;
    l.push(10);
    l.push(20);
    l.push(30);
    l.push(40);
    ln = l;
    ln.print();
    //l.reverse();
    //l.print();
    std::cout<<"Count = "<<l.count()<<std::endl;
    return (EXIT_SUCCESS);
}
