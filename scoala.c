#include <stdio.h>
#include <stdlib.h>

typedef struct cel {
	int value;
	struct cel *next;
}ListNode;

typedef struct { 
	int n, m, roadCost, schoolCost;
	ListNode **adList;
}TGraph;

// Adds a new element in the adjacency list. 
void addElement(TGraph *graph, int v1, int v2) 
{
	// Create the nodes. 
	ListNode *n1, *n2;
	n1 = (ListNode*)malloc(sizeof(ListNode));
	n2 = (ListNode*)malloc(sizeof(ListNode));
	// Add value => who it is linked with in the graph.
	n1->value = v2;
	n2->value = v1;

	// Link with the elements existing in the list.
	n1->next = graph->adList[v1];
	graph->adList[v1] = n1;

	n2->next = graph->adList[v2];
	graph->adList[v2] = n2;
}

// For debugging. 
void printGraphList(TGraph* graph){
	printf("\nGraph Adjacency Lists:\n");
	for(int i = 1; i <= graph->n; i++){
		printf("%2d: [ ",i);
		for(ListNode* it = graph->adList[i]; it != NULL; it = it->next){
			printf("%d ", it->value);
		}
		printf("]\n");
	}
	printf("\n");
}

// Depth First Search
void DFS(int *visited, TGraph *graph, int crt)
{	
	// Mark the visited nodes.
	visited[crt] = 1;
	ListNode *p = graph->adList[crt];

	// Visit elements from the current list
	while(p != NULL) {
		// If I haven't visited this one
		if(visited[p->value] == 0) {
			// Start visiting again from here.
			DFS(visited, graph, p->value);
		}
		p = p->next;
	}
}

// Find the number of conex elements.
int noConexEl(TGraph *graph) 
{	
	int i, conex = 0, N = (graph->n)+1;
	// Initialize visited.
	int visited[N];
	for(i = 1; i <= graph->n; i++) {
		visited[i] = 0;
	}

	// For each element in the graph.
	for(i = 1; i <= graph->n; i++) {
		// Not visited => another conex component.
		if(visited[i] == 0) {
			DFS(visited, graph, i);
			conex += 1;
		}
	}
	return conex;
}

// Read the information about a graph.
TGraph *readGraph(FILE *in)
{
	TGraph *graph = (TGraph*)malloc(sizeof(TGraph));

	fscanf(in, "%d", &(graph->n));
	fscanf(in, "%d", &(graph->m));
	fscanf(in, "%d", &(graph->roadCost));	
	fscanf(in, "%d", &(graph->schoolCost));

	graph->adList = (ListNode**)malloc((graph->n + 1)*sizeof(ListNode*));

	int i, v1, v2;
	for(i = 1; i <= graph->n; i++) {
		graph->adList[i] = NULL;
	}

	for(i = 1; i <= graph->m; i++) {
		fscanf(in, "%d", &v1);
		fscanf(in, "%d", &v2);
		addElement(graph, v1, v2);
	}
	// printGraphList(graph);
	return graph;
}

// Computes the cost.
int computeCost(TGraph *graph, int conex)
{
	// If the school is cheaper => a school in each city.
	if(graph->roadCost > graph->schoolCost) {
		return graph->schoolCost*graph->n;
	} else {
		// The road is cheaper => a school in each conex component 
		// and the roads for thet component.
		return graph->schoolCost*conex + (graph->n - conex) * graph->roadCost;
	}
}

void freeGraph(TGraph *graph) 
{
	int i;
	for(i = 1; i <= graph->n; i++) {
		ListNode *p = graph->adList[i];
		ListNode *q = p;
		while(p != NULL) {
			q = p;
			p = p->next;
			free(q);
		} 
	}
	free(graph->adList);
	free(graph);
}

int main()
{
	int i, k;
	TGraph *graph;

	FILE *in = fopen("scoala.in", "r");
	FILE *out = fopen("scoala.out", "w");
	
	fscanf(in, "%d", &k);

	for(i = 0; i < k; i++) {
		graph = readGraph(in);
		int conex = noConexEl(graph);
		int cost = computeCost(graph, conex);
		fprintf(out, "%d\n", cost);
		freeGraph(graph);
		graph = NULL;
	}

	fclose(in);
	fclose(out);
	
	return 0;
}
