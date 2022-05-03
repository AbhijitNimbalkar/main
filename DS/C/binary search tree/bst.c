#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct bst_node
{
    int data;
    struct bst_node* left;
    struct bst_node* right;
    struct bst_node* parent;
};

// parent node which is at the top and it wont contains data.
// when client asks  for create binary tree in this case 
// instead of returning null it should return atleast one node
struct bst
{
    struct bst_node* p_root_node;
    int nr_elements;
};

//Always pass head/ parent /root node for traversal
struct bst* create_bst(void);
//in inorder traversal parent is between left and right
//i.e. left== parent== right
void inorder(struct bst* p_bst);
//in preorder traversal parent first then left and right
//i.e. parent==left== right
void preorder(struct bst* p_bst);
//in preorder traversal parent is last, first left and right
//i.e. left== right==parent
void postorder(struct bst* p_bst);
int destroy_list(struct bst* );

//aux routines
struct bst_node* get_bst_node(int new_data);
void* xmalloc(size_t nr_bytes);

void inorder_node(struct bst_node* p_node);
void preorder_node(struct bst_node* p_node);
void postorder_node(struct bst_node* p_node);
void destroy_bst_node(struct bst_node* p_node);

void* xmalloc(size_t nr_bytes)
{
    void* p= NULL;
    p= malloc(nr_bytes);
    assert(p);
    memset(p,0,nr_bytes);
    return(p);
}
struct bst* create_bst(void)
{
    //in this method we only create dummy node which holds address of root node
    return (struct bst*)xmalloc(sizeof(struct bst));
}

struct bst_node* get_bst_node(int new_data)
{
    //allocate the memory for node and initialize members
    struct bst_node* p_new_node =NULL;
    p_new_node = (struct bst_node*)xmalloc(sizeof(struct bst_node));
    p_new_node->data = new_data;
    p_new_node->right =NULL;
    p_new_node->left =NULL;
    p_new_node->parent =NULL;
    return (p_new_node);
}
int insert_bst(struct bst* p_bst/*root node*/,int new_data)
{
    struct bst_node* p_new_node = NULL;
    struct bst_node* p_run_node =NULL;

    p_new_node = get_bst_node(new_data);
    
    //case 1 : if binary search tree is empty
    if(p_bst->nr_elements == 0 || p_bst->p_root_node ==NULL)
    {   
        p_bst->p_root_node = p_new_node;
        ++p_bst->nr_elements;
        return (1);
    }
    p_run_node = p_bst->p_root_node; //always access through dummy node 
    //case 2: if binary search tree is not empty
    while (1)
    {
        /* If data is less than or equal to parent node, insert into left subtree */
        if(new_data <= p_run_node->data)
        {
            if(p_run_node->left ==NULL)
            {
                p_run_node->left = p_new_node;
                p_new_node->parent = p_run_node;
                break;

            }
            else
            {
                p_run_node = p_run_node->left;
            }
        }
        //data is greater than root node data hence insert into right subtree
        else
        {
            if(p_run_node->right == NULL)
            {
                p_run_node->right = p_new_node;
                p_new_node->parent = p_run_node;
                break;
            }
            else
            {
                p_run_node =p_run_node->right;
            }

        }
       
    }
     p_bst->nr_elements++;
    return (1);//success
}

void preorder_node(struct bst_node* p_node)
{
    if(p_node == NULL)
        return;
    preorder_node(p_node->left);
    printf("[%d]\t",p_node->data);
    preorder_node(p_node->right);
}
void preorder(struct bst* p_bst)
{
    printf("\nPre-Order Traversal\n");
    printf("[BEG]\t");
    preorder_node(p_bst->p_root_node);
    printf("[END]\t");
}
void postorder_node(struct bst_node* p_node)
{
    if(p_node == NULL)
     return;
    postorder_node(p_node->left);
    postorder_node(p_node->right);
    printf("[%d]\t",p_node->data);
}   
void postorder(struct bst* p_bst)
{
    printf("\nPost-Order Traversal\n");
    printf("[BEG]\t");
    postorder_node(p_bst->p_root_node);
    printf("[END]\t");
}
void inorder_node(struct bst_node* p_node)
{
    if(p_node ==NULL)
        return;
    inorder_node(p_node->left);
    printf("[%d]\t",p_node->data);
    inorder_node(p_node->right);
}
void inorder(struct bst* p_bst)
{
    printf("\nIn-Order Traversal\n");
    printf("[BEG]\t");
    inorder_node(p_bst->p_root_node);
    printf("[END]\t");
}

void destroy_bst_node(struct bst_node* p_node)
{
    if(p_node == NULL)
        return;
    destroy_bst_node(p_node->left);
    destroy_bst_node(p_node->right);
    free(p_node);
}
int destroy_bst(struct bst** pp_bst)
{
    struct bst* p_bst = *pp_bst;
    destroy_bst_node(p_bst->p_root_node);
    //free dummy node after deletion of tree
    free(p_bst);
    p_bst = NULL;
    *pp_bst = NULL;
    return (1);
}

int main()
{
    struct bst* p_bst =NULL;
    int bst_data[] = {100,50,150,25,75,65,125,175,130};
    int a_size = sizeof(bst_data)/sizeof(bst_data[0]);
    int  i = 0;
    int status = 0;
    //this returns dummy node and all the operations are performed on the basis of dummy node
    p_bst =create_bst();

    for(i =0;i< a_size;i++)
    {
        status = insert_bst(p_bst,bst_data[i]);
        assert(status ==1);
    }
    inorder(p_bst);
    preorder(p_bst);
    postorder(p_bst);

    status =destroy_bst(&p_bst);
    assert(status == 1 && p_bst == NULL); 
    return (EXIT_SUCCESS);
}