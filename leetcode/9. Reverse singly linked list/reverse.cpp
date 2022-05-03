#include <iostream>
#include <vector>
/*also you can use while(1) approach*/ 
 struct ListNode 
 {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};


ListNode* CreateList()
{
    ListNode * dummy = new ListNode();
    dummy->next = nullptr;
    dummy->val =-1;
}

void AddNode(ListNode * h_node, int Val)
{
    ListNode* l = new ListNode(Val);
    ListNode* run_node = h_node;
    while(run_node->next != nullptr) 
    {
        run_node = run_node->next;
    }
    run_node->next = l;
}


void DisplayList(ListNode* head_node)
{
    ListNode* run_node =head_node->next;
    std::cout<<"[";
    while(run_node!= nullptr)
    {
        std::cout<<"<===>"<<run_node->val;
        run_node =run_node->next;
    }
    std::cout<<"]"<<std::endl;
}

void reverse (ListNode*& h , ListNode*& l,ListNode*& r)
{
    if(r == nullptr)
    {
        h = l;
        return ;
    }
        
    reverse(h,r,r->next);

    r->next = l;
}

ListNode* reverseex(ListNode* r)
{
    if(r->next== nullptr)
        return r;
    ListNode* nh = reverseex(r->next);
     r->next->next =r;
     r->next =nullptr;
     return nh;
}

int  main()
{
    ListNode* head_node = CreateList();
     
    if(head_node)
    {
        AddNode(head_node,10);
        AddNode(head_node,20);
        AddNode(head_node,30);
        AddNode(head_node,40);
    }
    DisplayList(head_node);
    ListNode* t_head_node=head_node;
     t_head_node->next = reverseex(head_node->next);
    //head_node->next = nullptr;

    //reverse (head_node,head_node, head_node->next);//1  2
    //t_head_node->next->next= nullptr;
    //t_head_node->next = head_node;
    DisplayList(t_head_node);
    return (EXIT_SUCCESS);
}