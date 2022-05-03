#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h> // for memset
#include "graph.h"


//vertical list methods
vlist_t* v_create_list()
{
	//pointer to the vertical list
	vlist_t* pv = NULL;
	pv = (vlist_t*)xmalloc(sizeof(vlist_t));
	assert(pv != NULL);
	pv->v = -1;
	pv->v_next = pv;
	pv->v_prev = pv;
	pv->ph_head_node = NULL;
	return (pv);
}
vnode_t* v_get_list_node(vertex_t v)
{
	vnode_t* pv = NULL;
	pv = (vnode_t*)xmalloc(sizeof(vnode_t));
	assert(pv != NULL);
	pv->v= v;
	pv->ph_head_node = h_create_list();
	pv->v_prev =NULL;
	pv->v_next =NULL;
	return (pv);
}
vnode_t* v_search_node(vlist_t* pvlist,vertex_t key)
{
	vnode_t* pv =NULL;
	vnode_t* p_run = NULL;
	for(p_run = pvlist->v_next;p_run != pvlist;p_run = p_run->v_next)
	{
		if(p_run->v == key)
			return (p_run);
	}
	return (NULL);
}
void v_generic_delete(vnode_t* p_delete_node)
{
	assert(p_delete_node != NULL);
	p_delete_node->v_prev->v_next = p_delete_node->v_next;
	p_delete_node->v_next->v_prev = p_delete_node->v_prev;
	free(p_delete_node);
	p_delete_node =NULL;
}
void v_generic_insert(vnode_t* p_beg,vnode_t* p_mid,vnode_t* p_end)
{
		p_mid->v_next = p_end;
		p_mid->v_prev = p_beg;
		p_beg->v_next = p_mid;
		p_end->v_prev = p_mid;
}
status_t v_insert_end(vlist_t* p_list,vertex_t new_vertex)
{
	v_generic_insert(p_list->v_prev,v_get_list_node(new_vertex),p_list);
	return (SUCCESS);
}


//Adjecency list methods
hnode_t* h_get_list_node(vertex_t v)
{
	hnode_t* ph=NULL;
	ph = (hnode_t*)xmalloc(sizeof(hnode_t));
	
	ph->v = v;
	ph->h_next =NULL;
	ph->h_prev = NULL;
	return (ph);
}
hlist_t* h_create_list()
{
	hlist_t* ph= NULL;
	ph =  h_get_list_node(-1);
	ph->h_next = ph;
	ph->h_prev = ph;
	return (ph);
}

void h_generic_insert(hnode_t* p_beg,hnode_t* p_mid,hnode_t* p_end)
{
	p_mid->h_next = p_end;
	p_mid->h_prev = p_beg;
	p_beg->h_next = p_mid;
	p_end->h_prev = p_mid;
}

status_t h_insert_end(hlist_t* p_list,vertex_t v)
{
	h_generic_insert(p_list->h_prev,h_get_list_node(v),p_list);
	return (SUCCESS);
}

void h_generic_delete(hnode_t* p_delete_node)
{
	assert(p_delete_node != NULL);
	p_delete_node->h_prev->h_next = p_delete_node->h_next;
	p_delete_node->h_next->h_prev = p_delete_node->h_prev;
	free(p_delete_node);
	p_delete_node = NULL;
}

hnode_t* h_search_node(hlist_t* p_list,vertex_t key)
{
	hnode_t* p_run =NULL;
	for(p_run = p_list->h_next;p_run != p_list; p_run = p_run->h_next)
	{
		if(p_run->v == key)
			return p_run;
	}
	return (NULL);
}

// graph routines
void* xmalloc(size_t nr_bytes)
{
	void* p = (void*)malloc(nr_bytes);
	assert(p!= NULL);
	memset(p,nr_bytes,0);
	return (p);
}

graph_t* create_graph()
{
	graph_t* g = NULL;
	g = (graph_t*)xmalloc(sizeof(graph_t));
	assert(g!= NULL);
	g->nr_edges = 0;
	g->nr_vertices =0;
	g->pv_head_node =v_create_list();
	return (g);
}

status_t add_vertex(graph_t* g,vertex_t v)
{
	status_t s ;
	vnode_t* vertex = NULL;
	vertex	= v_search_node(g->pv_head_node,v);
	 if(vertex != NULL)
		 return G_VERTEX_EXIST;
	 s = v_insert_end(g->pv_head_node, v);
	 assert(s == SUCCESS);
	 g->nr_vertices ++;
	 return (SUCCESS);
}

status_t add_edge(graph_t* g,vertex_t from,vertex_t to)
{
	status_t status;
	vnode_t* p_from_node =NULL;
	vnode_t* p_to_node = NULL;
	hnode_t* p_to_in_from =NULL;
	hnode_t* p_from_in_to =NULL;
	//Check in vertical list if from and to node are present or not.
	//if one of the node is not present then return vertex not exist
	p_from_node = v_search_node(g->pv_head_node,from); 
	if(p_from_node == NULL)
		return G_INVALID_VERTEX;
	p_to_node = v_search_node(g->pv_head_node,to);
	if(p_to_node == NULL)
		return G_INVALID_VERTEX;

	//now search for edge is present or not
	p_to_in_from = h_search_node(p_from_node->ph_head_node,to);
	if(p_to_in_from != NULL)
		 return G_EDGE_EXIST;
	 
	p_from_in_to = h_search_node(p_to_node->ph_head_node,from);
	if(p_from_in_to != NULL)
		return G_EDGE_EXIST;
	
	//edge is not exist and vertex is not repeated
	status = h_insert_end(p_from_node->ph_head_node,to);
	assert(status == SUCCESS);
	
	h_insert_end(p_to_node->ph_head_node,from);
	assert(status == SUCCESS);
	
	g->nr_edges ++;
	return SUCCESS;
}

status_t remove_edge(graph_t* g, vertex_t from,vertex_t to)
{
	vnode_t* p_from_node = v_search_node(g->pv_head_node,from);
	if(p_from_node == NULL)
		return G_VERTEX_NOT_EXIST;
	
	vnode_t* p_to_node = v_search_node(g->pv_head_node,to);
	if(p_to_node == NULL)
		return G_VERTEX_NOT_EXIST;
	
	hnode_t* p_from_in_to_node = h_search_node(p_to_node->ph_head_node,from);
	if(p_from_in_to_node == NULL)
		return G_EDGE_NOT_EXIST;
	
	hnode_t* p_to_in_from_node =h_search_node(p_from_node->ph_head_node,to);
	if(p_from_in_to_node == NULL)
		return G_EDGE_NOT_EXIST;
	
	h_generic_delete(p_from_in_to_node);
	h_generic_delete(p_to_in_from_node);	
	g->nr_edges--;
	return (SUCCESS);
}
status_t remove_vertex(graph_t* g,vertex_t v)
{
	vnode_t* delete_node  = NULL;
	delete_node = v_search_node(g->pv_head_node,v);

	hnode_t* p_h_run_node = NULL;
	hnode_t* p_h_run_node_next = NULL;
	if(delete_node == NULL)
		return G_VERTEX_NOT_EXIST;

	//always start from 1st node not from dummy node
	p_h_run_node = delete_node->ph_head_node->h_next;
	while(p_h_run_node != delete_node->ph_head_node)
	{
		p_h_run_node_next = p_h_run_node->h_next;
		//find runnode in vertical list
		vnode_t* p_vnode_from_adj_list = v_search_node(g->pv_head_node,p_h_run_node->v);

		//find delete node in horizontal list of run node
		hnode_t* p_hnode_from_adj_list = h_search_node(p_vnode_from_adj_list->ph_head_node,delete_node->v);

		h_generic_delete(p_hnode_from_adj_list);
		free(p_h_run_node);
		p_h_run_node = NULL;
		p_h_run_node = p_h_run_node_next;
	}
	
	free(delete_node->ph_head_node);
	v_generic_delete(delete_node);

	g->nr_vertices--;
	return (SUCCESS);
}

void show_graph(graph_t* g,const char* msg)
{
	vnode_t* pv_run =NULL;
	hnode_t* ph_run =NULL;

	if(msg)
		puts(msg);
	for (pv_run= g->pv_head_node->v_next; pv_run!=g->pv_head_node ; pv_run=pv_run->v_next)
	{
		printf("[%d]\t<->\t",pv_run->v);
		for(ph_run =pv_run->ph_head_node->h_next;ph_run !=pv_run->ph_head_node;
		ph_run = ph_run->h_next)
		{
			printf("[%d]<->",ph_run->v);
		}
		puts("[END]");
	}
	puts("-----------------------------------------------");
}

status_t destroy_graph(graph_t** pg)
{
	vnode_t* pv_run =NULL;
	vnode_t* pv_run_next =NULL;
	hnode_t* ph_run =NULL;
	hnode_t* ph_run_next =NULL;

	graph_t* g = *pg;
	for(pv_run = g->pv_head_node->v_next;
	pv_run != g->pv_head_node;
	pv_run = pv_run_next)
	{
		pv_run_next = pv_run->v_next;

		for(ph_run = pv_run->ph_head_node->h_next;
		ph_run != pv_run->ph_head_node;
		ph_run =ph_run_next)
		{
			ph_run_next = ph_run->h_next;
			free(ph_run);	
		}
		free(pv_run->ph_head_node);
		pv_run->ph_head_node = NULL;
		free(pv_run);
		pv_run =NULL;
	}
	free(g->pv_head_node);
	free(g);
	*pg =NULL;
	return (SUCCESS);
}
void reset(graph_t* g)
{
	vnode_t* pv_run =NULL;
	for(pv_run = g->pv_head_node->v_next;pv_run != g->pv_head_node ;
	pv_run=pv_run->v_next)
	{
		pv_run->color = color::WHITE;
	}
}
void dfs_visit(graph_t* g,vnode_t* pv)
{
	hnode_t* ph_run 		= NULL;
	vnode_t* pv_h_in_vlist 	= NULL;
	printf("[%d]",pv->v);//visit node and mark color as GRAY
	pv->color = GRAY;
	for (ph_run = pv->ph_head_node->h_next;ph_run != pv->ph_head_node;
	ph_run = ph_run->h_next)
	{
		pv_h_in_vlist =v_search_node(g->pv_head_node,ph_run->v);
		if(pv_h_in_vlist->color ==color::WHITE)
			dfs_visit(g,pv_h_in_vlist);
	}
	pv->color =color::BLACK;
}
void dfs(graph_t* g)
{
	vnode_t* pv_run =NULL;
	printf("[START]->");
	reset(g);
	for(pv_run = g->pv_head_node->v_next;pv_run!=g->pv_head_node;
	pv_run = pv_run->v_next)
	{
		if(pv_run->color == color::WHITE)
			dfs_visit(g,pv_run);
	}
	printf("[END]\n");
}