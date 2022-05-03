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
//1 2 3
// 2 3 4
// 1 2 2 3 4
class Solution {
public:
      ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) 
    {
                       
        ListNode* l = new ListNode(0);
        ListNode* r = l;
        if(list1 ==nullptr)
            return list2;
        
        if(list2 ==nullptr)
            return list1;

        while (list1 != nullptr )
        {
            while(list2!= nullptr)
            {
                if(list2->val > list1->val)
                    break;//only break inner loop
                else
                {
                    l->next = new ListNode(list2->val);
                    l = l->next;
                }
                list2 = list2->next;
            }
            l->next = new ListNode(list1->val);
            l = l->next;
            list1 = list1->next;
        }
        if(list1 == nullptr && list2 != nullptr)
        {
            while(list2 != nullptr)
            {
                l->next = new ListNode(list2->val);
                l = l->next;
                list2 =list2->next;
            }
        }
        
        if(list2 == nullptr && list1 != nullptr)
        {
            while(list1 != nullptr)
            {
                l->next = new ListNode(list1->val);
                l = l->next;
                list1 =list1->next;
            }
        }
        
        return r->next;
    }
};

int main()
{

    return (EXIT_SUCCESS);
}