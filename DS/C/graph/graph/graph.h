#pragma once

#define SUCCESS 1
#define TRUE 1
#define FALSE 0
#define G_VERTEX_EXIST	1001
#define G_EDGE_EXIST	1002
#define G_VERTEX_NOT_EXIST	1003
#define G_EDGE_NOT_EXIST	1004

#define G_INVALID_VERTEX	1005


typedef int	vertex_t;
typedef int status_t;

typedef struct hnode hnode_t;
typedef hnode_t	hlist_t;

typedef struct vnode vnode_t;
typedef vnode_t vlist_t;

typedef struct graph graph_t;
typedef enum color color_t;
enum color 
{
	WHITE =0,
	GRAY =1,
	BLACK=2
};

struct hnode
{
	struct hnode* h_next;
	struct hnode* h_prev;
	vertex_t		v;
};
struct vnode
{
	hnode_t* ph_head_node;
	vertex_t v;
	struct vnode* v_next;
	struct vnode* v_prev;
	color_t color;
};
struct graph
{
	unsigned long nr_vertices;
	unsigned long nr_edges;
	vnode_t* pv_head_node;
};
//graph methods
graph_t* create_graph();
status_t add_vertex(graph_t* g,vertex_t v);
status_t add_edge(graph_t* g,vertex_t from,vertex_t to);
status_t remove_vertex(graph_t* g,vertex_t v);
status_t remove_edge(graph_t* g, vertex_t from,vertex_t to);
status_t destroy_graph(graph_t** pg);
void* xmalloc(size_t nr_bytes);
void show_graph(graph_t* g,const char* msg);
void reset(graph_t* g);
void dfs(graph_t* g);
void dfs_visit(graph_t* g,vnode_t* pv);

//vertical list methods
vlist_t* v_create_list();
vnode_t* v_get_list_node(vertex_t v);
vnode_t* v_search_node(vlist_t* pvlist,vertex_t key);
void v_generic_delete(vnode_t* p_delete_node);
void v_generic_insert(vnode_t* p_beg,vnode_t* p_mid,vnode_t* p_end);
status_t v_insert_end(vlist_t* p_list,vertex_t new_vertex);

//Adjecency list methods
hlist_t* h_create_list();
hnode_t* h_get_list_node(vertex_t v);
hnode_t* h_search_node(hlist_t* p_list,vertex_t key);
void h_generic_delete(hnode_t* p_delete_node);
void h_generic_insert(hnode_t* p_beg,hnode_t* p_mid,hnode_t* p_end);
status_t h_insert_end(hlist_t* p_hist,vertex_t v);

