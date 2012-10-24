/*
ͼ�����ݽṹ, ͼ�ı����㷨, ͼ�����·���㷨
*/

#ifndef _XGRAPH_H_
#define _XGRAPH_H_

#define MAX_GRAPH_VECTEX_AMT (0x1000)

typedef const char* lpcstr_t;

typedef struct {
	int vertex_amt;
	int* vertics;
	int* edges;
	int* tags;
}graph_t, *pgraph_t;

typedef void (*func_graph_ergod_t)(pgraph_t, int);

pgraph_t graph_create( lpcstr_t szConfig );
pgraph_t graph_create( int vertex_amt );
void graph_release( pgraph_t pgraph );
void graph_init( pgraph_t pgraph );
void graph_clear( pgraph_t pgraph );

//��ȡͼ�ı�ָ��
int* graph_get_edge_ptr( pgraph_t pgraph, int si, int di );
//��ȡ�ڵ��һ���ھ�
int graph_get_neighbour( pgraph_t pgraph, int vi_start, int iterator );
//��ȡ·����̵��ھ�
int graph_get_nearest_neighbour( pgraph_t pgraph, int vi_start );
//������ȱ���
void graph_ergod_depth_first( pgraph_t pgraph, int vi_start, func_graph_ergod_t pfunc );



void graph_print( pgraph_t pgraph );
//Ѱ��һ��δ���ʹ����ھ�
int graph_find_free_neighbour( pgraph_t pgraph, int n_vi );
//Ѱ������·��
int graph_find_best_path( pgraph_t pgraph, int vi_start, int vi_end, int* pvi, int vi_max );
int graph_get_lowest_value_neighbour(pgraph_t pgraph, int vi);
//�������δ��۵ı���
void graph_ergod_for_travel_cost( pgraph_t pgraph, int vi_start );
//���δ��ۼ���ʱ, �Զ���Ĳ���
void graph_vertex_action_for_travel_cost( pgraph_t pgraph, int n_vi );

#endif //_XGRAPH_H_