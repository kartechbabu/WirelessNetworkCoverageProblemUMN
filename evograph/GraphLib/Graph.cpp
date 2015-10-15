/*  Graph library
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 * 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *  
 *	Jianjun Hu		hujianju@msu.edu
 *	Homepage: www.egr.msu.edu/~hujianju
 *	Genetic Algorithm Research and Application Group
 *  Department of Computer Science
 *  Michigan State University
 *  East Lansing, Michigan  48824
 *  USA
 *	
 *	This graph library is based on Graphlib by Karim Fanous enhanced with input/output related functions 
 *	needed for graph evolution.
 *
 *	Karim Fanous
 *	Department of Computer Science 
 *	American University in Cairo 
 *	http://www.geocities.com/TheTropics/Paradise/7231/GraphLib.htm
 *  
 */

#include "Graph.h"

/* Private method that initializes the data structures.
 */
void Graph::Initialize() {
	
	m_Vertices = new vector<Vertex*>;
	m_Edges = new vector<Edge*>;

	m_VertexCounter = 0; 

	m_Matrix = new double* [m_NumVertices];
  	for(unsigned int i = 0; i < m_NumVertices; ++i)
  		m_Matrix[i] = new double[m_NumVertices];
		
		for(unsigned int row = 0; row < m_NumVertices; ++row)
			for(unsigned int col = 0; col < m_NumVertices; ++col)
				m_Matrix[row][col] = c_INFINITY;
}

Graph::Graph(unsigned int NumVertices):m_NumVertices(NumVertices) {
	
	Initialize(); 
} 
 
/* Copy constructor.
 */
Graph::Graph(const Graph& NewGraph):m_NumVertices( NewGraph.m_NumVertices ), m_VertexCounter( NewGraph.m_VertexCounter ) {

	m_Vertices = new vector<Vertex*>;
	for(vector<Vertex*>::iterator i = (NewGraph.m_Vertices) -> begin(); i != (NewGraph.m_Vertices) -> end(); ++i)
		m_Vertices -> push_back( (*i) );

	m_Edges= new vector<Edge*>;
	for(vector<Edge*>::iterator j = (NewGraph.m_Edges) -> begin(); j != (NewGraph.m_Edges) -> end(); ++j)
		m_Edges -> push_back( (*j) );
	
	m_Matrix = new double* [NewGraph.m_NumVertices];
	for(unsigned int k = 0; k < NewGraph.m_NumVertices; ++k)
		m_Matrix[k] = new double [NewGraph.m_NumVertices];

	for(unsigned int m = 0; m < NewGraph.m_NumVertices; ++m) {
		for(unsigned int n = 0; n < NewGraph.m_NumVertices; ++n)
			m_Matrix[m][n] = NewGraph.m_Matrix[m][n];
	}
}

/* Assignment operator.
 */
const Graph& Graph::operator=(const Graph& Rhs) {

	if(this != &Rhs) {

		delete m_Vertices;
		delete m_Edges;

		for(unsigned int i = 0; i < m_NumVertices; ++i)
		  delete m_Matrix[i];
		delete [] m_Matrix;

		m_NumVertices = Rhs.m_NumVertices;
		m_VertexCounter = Rhs.m_VertexCounter;
		
		m_Vertices = new vector<Vertex*>;
		for(vector<Vertex*>::const_iterator I = (Rhs.m_Vertices) -> begin(); I != (Rhs.m_Vertices) -> end(); ++I)
		  m_Vertices -> push_back( (*I) );
		
		m_Edges= new vector<Edge*>;
		for(vector<Edge*>::const_iterator J = (Rhs.m_Edges) -> begin(); J != (Rhs.m_Edges) -> end(); ++J)
		  m_Edges -> push_back( (*J) );
		
		m_Matrix = new double*[m_NumVertices];
		for(unsigned int k = 0; k < m_NumVertices; ++k)
		  m_Matrix[k] = new double[m_NumVertices];
		
		for(unsigned int m = 0; m < m_NumVertices; ++m) {
			for(unsigned int n = 0; n < m_NumVertices; ++n)
			  m_Matrix[m][n] = Rhs.m_Matrix[m][n];
		}
	}
	return *this; 
}

/* Method that adds a vertex to the graph.
 */
Vertex* Graph::addVertex(const string& VertexName) 
{
	//Check to see if the vertex was already added in the Graph.
	//Since VertexName must be unique, we must check anyhow! HJJ
	for(vector<Vertex*>::const_iterator it = m_Vertices -> begin(); it != m_Vertices -> end(); ++it) {
		if( (*it) -> first == VertexName) 
			throw DuplicateVertex(VertexName);
	}
	
#if(DEFENSIVE)
	
	//If there are more vertcies added in the Graph than specified, then throw an Exception.
	//The number of vertices in the Graph should equal to the number specified in the Graph constructor.
	/*HJJ
		In order to allow dynamic adding of vertices, we always use m_VertexCounter
		to indicate how many vertices currently in graph and let 
		m_NumVertices= m_VertexCounter!
	   
	if(m_VertexCounter > m_NumVertices)
		throw TooManyVertices(m_NumVertices);
	*/
#endif
	
	
	//Create the new vertex and add it in the array of vertices of the graph.
	Vertex *new_vertex = new Vertex(VertexName, m_VertexCounter);
	m_Vertices -> push_back(new_vertex);
	new_vertex->second = m_Vertices->size()-1;

	//Increment the number of vertices added in the Graph.
	m_VertexCounter++;
	if(m_VertexCounter >m_NumVertices){
			m_NumVertices= m_VertexCounter;

			int i,j,k;
			//update the adjacency matrix
			double** newMatrix = new double* [m_VertexCounter];
			for(k = 0; k < m_VertexCounter; ++k)
				newMatrix[k] = new double [m_VertexCounter];
			//copy the matrix
			for(i=0;i<m_VertexCounter;i++){
				for(j=0;j<m_VertexCounter;j++){
					if(i<m_VertexCounter-1&&j<m_VertexCounter-1) 
						newMatrix[i][j] = m_Matrix[i][j];
					else 
						newMatrix[i][j] = c_INFINITY;
				}
			}

			for(i = 0; i < m_VertexCounter-1; ++i)
				delete m_Matrix[i];
			delete [] m_Matrix;
			m_Matrix = newMatrix;
	}
	return new_vertex;
}


/* Destructor.
 */
Graph::~Graph() {
	//Deleting the vertices
	//printf("Graph:: destructor!\n");
	if(m_Vertices->empty()) return;
	
	for(vector<Vertex*>::iterator I = m_Vertices -> begin(); I != m_Vertices -> end(); ++I)
	delete *I;
	m_Vertices->clear();

	if(! m_Edges->empty()){
		for(vector<Edge*>::iterator K = m_Edges -> begin(); K != m_Edges -> end(); ++K)
			delete *K;
		m_Vertices->clear();
	}

	delete m_Vertices;
	delete m_Edges;

	for(unsigned int i = 0; i < m_NumVertices; ++i)
		delete m_Matrix[i];
	
	delete [] m_Matrix;

}

/* Debugging functions.
 */
	void Graph::DumpAdjacencyMatrix() {
		if(m_VertexCounter ==0) return;

		cout<<"   ";
		for(int i = 0; i < m_NumVertices; ++i) {
			cout<<(*m_Vertices)[i]->getTitle()<<"   ";
		}

		for( i = 0; i < m_NumVertices; ++i) {
			cout<<endl;
			cout<<(*m_Vertices)[i]->getTitle()<<"  ";
			for(unsigned int j = 0; j < m_NumVertices; ++j){
				if(m_Matrix[i][j] == c_INFINITY)
					cout<<"*";
				else
					cout << m_Matrix[i][j];
				cout<<"   ";
			}
		}
		cout<<endl;
	}
	
	void Graph::DumpEdgeVector() {
		if(!m_Edges->empty()){
			printf("  From       To      WriteHead      Weight\n");
			for(vector<Edge*>::iterator i = m_Edges -> begin(); i != m_Edges -> end(); ++i){
				printf("%5s-->%6s    %10d    %10.3f \n",(*i) -> getVertices().first.data(),
						(*i) -> getVertices().second.data(),(*i)->writeHead,
						(*i)->getWeight());
				}
			}
		}

/*	Function that initializes the graph by reading a file.  
		The file should adhere to a certain format.
 */
void ReadGraph(const string& FileName, Graph* TheGraph) {
	
	//Opening the file.
	ifstream GraphFile(FileName.c_str(), ios::in | ios::binary);
	
	if (!GraphFile) {
		cout << "Cannot open Graph file "<<FileName<<endl;
		exit(1);
	}

	//Reading the number of vertices in the graph.
	GraphFile >> TheGraph -> m_NumVertices;
	TheGraph -> Initialize();
	
	string VertexName;	
	//Reading the vertices in the graph
	for(unsigned int i = 0; i < TheGraph -> m_NumVertices; ++i) {
		GraphFile >> VertexName;
		TheGraph -> addVertex(VertexName);
	}

	//Adding the edges in the graph.
	string SourceVertex;
	string DestinationVertex;
	double Weight;
	while(true) {
		GraphFile >> SourceVertex;

		//Keep on reading edges until the # is read.
		if(SourceVertex == "#")
			break;
		
		GraphFile >> DestinationVertex;
		GraphFile >> Weight;
		TheGraph -> addEdge(SourceVertex, DestinationVertex, Weight);
	}
}

//HJJ load graph from file with X,Y coordinate information
bool Graph::load(const string &FileName)
{
	
	//Opening the file.
	ifstream GraphFile(FileName.c_str(), ios::in | ios::binary);
	
	if (!GraphFile) {
		cout << "Cannot open Graph file "<<endl;
		return false;
	}

	//Reading the number of vertices in the graph.
	string version;
	GraphFile>>version;
	if(version!="EGF_EvoGraphFormat"){
		cout<<"Wrong graph file format!"<<endl;
		cout<<"use: EGF_EvoGraphFormat graph files"<<endl;
		exit(-1);
	}


	GraphFile >>  m_NumVertices;
	Initialize();
	
	string VertexName;	
	int x, y;
	//Reading the vertices in the graph
	for(unsigned int i = 0; i < m_NumVertices; ++i) {
		GraphFile >> VertexName;
		GraphFile >>x;
		GraphFile >>y;
		Vertex* v = addVertex(VertexName);
		v->setX(x);
		v->setY(y);
	}

	//Adding the edges in the graph.
	string SourceVertex;
	string DestinationVertex;
	double Weight;
	while(true) {
		GraphFile >> SourceVertex;

		//Keep on reading edges until the # is read.
		if(SourceVertex == "#")
			break;
		
		GraphFile >> DestinationVertex;
		GraphFile >> Weight;
		addEdge(SourceVertex, DestinationVertex, Weight);
	}
	return true;
}

//HJJ  add vertex object into graph
Vertex* Graph::addVertex(Vertex *pVertex)
{
	//Check to see if the vertex was already added in the Graph.
	//Since VertexName must be unique, we must check anyhow! HJJ
	for(vector<Vertex*>::const_iterator it = m_Vertices -> begin(); it != m_Vertices -> end(); ++it) {
		if( (*it) -> first == pVertex->getTitle()) 
			throw DuplicateVertex(pVertex->getTitle());
	}

#if(DEFENSIVE)
	
	//If there are more vertcies added in the Graph than specified, then throw an Exception.
	//The number of vertices in the Graph should equal to the number specified in the Graph constructor.
	/*if(m_VertexCounter > m_NumVertices)
		throw TooManyVertices(m_NumVertices);
	
	//Check to see if the vertex was already added in the Graph.
	for(vector<Vertex*>::const_iterator i = m_Vertices -> begin(); i != m_Vertices -> end(); ++i) {
		if( (*i) -> first == pVertex->first) 
			throw DuplicateVertex(pVertex->first);
	HJJ*/
	}
#endif
	
	m_Vertices -> push_back(pVertex);
	pVertex->second = m_Vertices->size()-1;

	//Increment the number of vertices added in the Graph.
	m_VertexCounter++;
	if(m_VertexCounter >m_NumVertices){
		//we need to extend the size of this graph!
				m_NumVertices= m_VertexCounter;

				int i,j,k;
				//update the adjacency matrix
				double** newMatrix = new double* [m_VertexCounter];
				for(k = 0; k < m_VertexCounter; ++k)
					newMatrix[k] = new double [m_VertexCounter];
				//copy the matrix
				for(i=0;i<m_VertexCounter;i++){
					for(j=0;j<m_VertexCounter;j++){
						if(i<m_VertexCounter-1&&j<m_VertexCounter-1) 
							newMatrix[i][j] = m_Matrix[i][j];
						else 
							newMatrix[i][j] = c_INFINITY;
					}
				}

				for(i = 0; i < m_VertexCounter-1; ++i)
					delete m_Matrix[i];
				delete [] m_Matrix;
				m_Matrix = newMatrix;
	}
	return pVertex;
}


//HJJ get the Edge object by from vertex name and to vertex name
Edge* Graph::getEdge(string from, string to)
{
	for(vector<Edge*>::iterator j = m_Edges -> begin(); j != m_Edges -> end(); ++j) 
	{
		if((*j)->getFromVertex()==from && (*j)->getToVertex()==to)
			return (*j);
	}
	return NULL;
}

Edge* Graph::insertVertex(Edge *pEdge, Vertex *pVertex, double weight)
{
	return NULL;

}

Edge* Graph::parallelEdge(Edge *pE, double weight)
{
	return NULL;

}

Edge* Graph::connectVertex(Vertex *pFromV, Vertex *pToV, double weight)
{
	return NULL;
}

Edge* Graph::selfLoop(Vertex *v, double weight)
{
	return NULL;

}

void Graph::reverseDirection(Edge *e)
{
	e->reverseDirection();
;
}

//!HJJ	remove an edge from the graph, but don't remove the object itself!
//		don't remove the end vertices!
void Graph::cutEdge(Edge *pEdge)
{
	//update the adjacency matrix
	//Getting the vertices locations in the AdjacencyMatrix. 
	
	int from_loc = GraphUtils::getVertexValue(*this, pEdge->getFromVertex());
	int to_loc = GraphUtils::getVertexValue(*this, pEdge->getToVertex());

	//remove the weight in the AdjacenyMatrix.  The weight is stored in location [i][j] and [j][i] since the Graph is a Undirected Graph.
	m_Matrix[from_loc][to_loc] = c_INFINITY;
	m_Matrix[to_loc][from_loc] = c_INFINITY;

	//remove the edge from the edge list
	vector<Edge*>::iterator theIterator;
    for (theIterator = m_Edges->begin(); theIterator != m_Edges->end();theIterator++)
	{
		if((*theIterator) == pEdge)
			break;
	}

	m_Edges->erase(theIterator);
	//delete pEdge;
}

//HJJ remove a vertex. We have to reconstruct the whole adjacency matix
//	  remove all the connected edges.
//		! we don't delete the vertex object!
void Graph::cutVertex(Vertex *pVertex)
{
	//reconstruct the whole adjacency matix
	int i, j, k;
	int pos = GraphUtils::getVertexValue(*this, pVertex->getTitle());
	int nVertices = m_NumVertices-1;
	double** newMatrix = new double* [nVertices];
	for(k = 0; k < nVertices; ++k)
		newMatrix[k] = new double [nVertices];

#ifdef DEBUG_G
	cout<<"Graph::cutVertex: "<<pVertex->getTitle()<<endl;
	report();
#endif
	//remove all the adjacency edges

	vector<Edge*> & adjEdges = GraphUtils::getIncidentEdges(*this, pVertex->getTitle());
	int size  = adjEdges.size();
	for(i=0;i<adjEdges.size();i++){
		//cout<<adjEdges[i]->getFromVertex()<<endl;//HJJD
		//cout<<adjEdges[i]->getToVertex()<<endl;
		cutEdge(adjEdges[i]);
		delete adjEdges[i];
	}

	//copy the matrix
	int row, col;
	for(i=0;i<nVertices;i++){
		for(j=0;j<nVertices;j++){
			if(i<pos) row = i;
			else row = i+1;
			if(j<pos) col =j;
			else col = j+1;
			newMatrix[i][j] = m_Matrix[row][col];
		}
	}
	for(i = 0; i < m_NumVertices; ++i)
	  delete m_Matrix[i];
	delete [] m_Matrix;
	m_Matrix = newMatrix;
	//update the position of vertices
	for(i=0;i<m_Vertices->size();i++){
		if((*m_Vertices)[i]->second > pos) 
			(*m_Vertices)[i]->second--;
	}

	//remove the edge from the edge list
	vector<Vertex*>::iterator theIterator;
  for (theIterator = m_Vertices->begin(); theIterator != m_Vertices->end();theIterator++)
	{
		if((*theIterator) == pVertex)
			break;
	}
	m_Vertices->erase(theIterator);
	//delete pVertex;

	m_NumVertices--;
	m_VertexCounter--;

}

void Graph::cutEdge(string fromVertex, string toVertex)
{
	Edge* pE = getEdge(fromVertex, toVertex);
	cutEdge(pE);
}

void Graph::cutVertex(string title)
{
	Vertex* pV = getVertex(title);
	if(pV!=NULL)
		cutVertex(pV);
}

void Graph::clean()
{
	//Deleting the vertices
	//printf("Graph:: destructor!\n");
	if(m_Vertices==NULL) return;
	if(m_Vertices->empty()) return;
#ifdef DEBUG_G	
	cout<<"cleaning.......\n";
	report();
	printf("size of vertices: %d \n", m_Vertices->size());
#endif
	
	if(! m_Edges->empty()){
		for(vector<Edge*>::iterator K = m_Edges -> begin(); K != m_Edges -> end(); ++K){
			//cout<<"deleting..edge..."<<(*K)->getFromVertex()<<"--->"<<(*K)->getToVertex()<<endl;
			delete *K;
		}
		m_Edges->clear();
	}

	for(vector<Vertex*>::iterator I = m_Vertices -> begin(); I != m_Vertices -> end(); ++I)
		delete *I;
	m_Vertices->clear();


	if(m_Vertices!=NULL){
		delete m_Vertices;
		m_Vertices = NULL;
	}
	if(m_Edges!=NULL){
		delete m_Edges;
		m_Edges = NULL;
	}
	
	for(unsigned int i = 0; i < m_NumVertices; ++i)
		delete[] m_Matrix[i];
	
	delete [] m_Matrix;

	m_VertexCounter=0;

}

void Graph::report()
{
		printf("--------------------- Graph ---------------------\n");
		printf("%d vertices: %d edges\n", m_VertexCounter, m_Edges->size());

		printf("VertexName  writeHead          x          y \n");
		for(vector<Vertex*>::iterator j = m_Vertices -> begin(); 
				j != m_Vertices -> end(); ++j) {
					printf("%7s     %9d    %7.2f    %7.2f\n",(*j)->getTitle().data(),(*j)->writeHead,
						(*j)->getX(),(*j)->getY());
		}
		printf("\n%d Edges: \n", m_Edges->size());
		DumpEdgeVector();
		//printf("\nAdjMatrix: \n");
		//DumpAdjacencyMatrix();
		printf("--------------------- Graph end ---------------------\n\n");
}


Vertex* Graph::getVertex(int writeHead)
{
	for(vector<Vertex*>::iterator j = m_Vertices -> begin(); j != m_Vertices -> end(); ++j) 
	{
		if((*j)->writeHead == writeHead)
			return (*j);
	}
	return NULL;

}

Vertex* Graph::getVertex(string title)
{
	for(vector<Vertex*>::iterator j = m_Vertices -> begin(); j != m_Vertices -> end(); ++j) 
	{
		if((*j)->getTitle() == title)
			return (*j);
	}
	return NULL;

}

Edge* Graph::getEdge(int writeHead)
{
	for(vector<Edge*>::iterator j = m_Edges -> begin(); j != m_Edges -> end(); ++j) 
	{
		if((*j)->writeHead == writeHead)
			return (*j);
	}
	return NULL;

}

void Graph::setNumVertices(int n)
{
		m_NumVertices = n;
}

int Graph::getNumEdges()
{
	if( m_Edges==NULL) 
		return 0;
	else
		return m_Edges->size();
}



double Graph::getSumWeight()
{
	double sum=0.0;
	for(vector<Edge*>::iterator i = m_Edges -> begin(); i != m_Edges -> end(); ++i){
			sum+=	(*i)->getWeight();
	}
	return sum;
}

void Graph::printGW(std::ostream& o)
{
	//LEDA graph head
	o<<"LEDA.GRAPH"<<endl;
	o<<"void"<<endl;
	o<<"void"<<endl;
	o<<m_NumVertices<<endl;
	for(vector<Vertex*>::iterator i = m_Vertices -> begin(); i != m_Vertices -> end(); ++i){
		o<<"|{"<<"}|"<<endl;
	}
	//edges
	o<<getNumEdges()<<endl;
	for(vector<Edge*>::iterator k = m_Edges -> begin(); k != m_Edges -> end(); ++k){
			o<<((*k)->getFromVertexObject())->second+1<<" "<<((*k)->getToVertexObject())->second+1<<" "<<0;
			o<<" |{"<<"}|"<<endl;
	}

	//GW head global info
	o<<"# version string"<<endl;
	o<<"GraphWin 1.320000"<<endl;
	o<<"# scaling  wxmin  wymin  wxmax  wymax"<<endl;
	o<<1<<" "<<0<<" "<<0<<" "<<1000<<" "<<1000<<" "<<endl;
	o<<"# node label font and size"<<endl;
	o<<0<<" "<<12<<endl;
	o<<"# edge label font and size"<<endl;
	o<<0<<" "<<12<<endl;
	o<<"# node index format"<<endl;
	o<<"%d"<<endl;
	o<<"# edge index format"<<endl;
	o<<"%d"<<endl;
	o<<"# multi-edge distance"<<endl;
	o<<5<<endl;

	//detailed node drawing attributes
	o<<"# node infos"<<endl;
	o<<"# x y shape bclr bwidth r1 r2 clr ltype lclr lpos lstr"<<endl;
	
	for( i = m_Vertices -> begin(); i != m_Vertices -> end(); ++i){
		Node* pNode = new Node(**i);
		if(pNode->writeHead!=NOT_DEFINED){
			pNode->shape = 2;//square
			pNode->color = 4;//blue
		}
		pNode->printGW(o);
	}
	o<<"# "<<endl;
	o<<"# edge infos"<<endl;
	o<<"# width clr shape style dir ltype lclr lpos sanch tanch poly lstr"<<endl;

	Arc a;
	for(k = m_Edges -> begin(); k != m_Edges -> end(); ++k){
			o<<a.width<<" "<<a.color<<" "<<a.shape<<" "<<a.style<<" "<<a.direction<<" ";
			o<<a.labelType<<" "<<a.labelColor<<" "<<a.labelPos<<" ";
			o<<"(0,0) (0,0)"<<" "<<2<<" ";
			o<<"("<<(*k)->getFromVertexObject()->getX()<<","<<(*k)->getFromVertexObject()->getY()<<")"<<" ";
			o<<"("<<(*k)->getToVertexObject()->getX()<<","<<(*k)->getToVertexObject()->getY()<<")"<<endl;
	}

}	

int Graph::getVertexDegree(string vertexName)
{
	int pos = GraphUtils::getVertexValue(*this,vertexName);
	int deg=0;
	for(int i=0;i<m_NumVertices;i++){
		if(m_Matrix[pos][i]!=c_INFINITY)
			deg++;
	}
	return deg;
}


int Graph::getMaxVertexDegree()
{
	int maxDeg=0;
	int temp;
	for(vector<Vertex*>::iterator j = m_Vertices -> begin(); 
			j != m_Vertices -> end(); ++j) 
	{
				temp = getVertexDegree((*j)->getTitle());
				if( temp>maxDeg)maxDeg = temp;
	}
	return maxDeg;

}

int Graph::getMinVertexDegree()
{
	int minDeg=c_INFINITY;
	int temp;
	for(vector<Vertex*>::iterator j = m_Vertices -> begin(); 
			j != m_Vertices -> end(); ++j) 
	{
				temp = getVertexDegree((*j)->getTitle());
				if( temp<minDeg)minDeg = temp;
	}
	return minDeg;

}

bool Graph::isConnected()
{
	bool Connected = false;
	unsigned int col;

	//Loop on the vertices, row by row.
	for(unsigned int row = 0; row < m_NumVertices; ++row) {	

		Connected = false;
		col = 0;

		//Keep on looping on the columns, if all of the entries = infinity.
		do {
			if( (m_Matrix[row][col] == c_INFINITY) && (!Connected) ) {
				Connected = false;
				col++;
			}
			else 
				Connected = true;

		}while( (!Connected) && (col < m_NumVertices) );

		//If all of that vertex's entries where equal to infinty, then
		//that graph is disconnected.
		if(Connected == false)
			return(Connected);

	}
	return(Connected);
}

//export an graph file in EGF format,a simple graph format for GP evolution defined by Jianjun Hu
//format: 
//magic format number: EGF_EvoGraphFormat
//number of nodes    : e.g. 200
//node list with a node each line:  nodeName, x Coordinate, y coordinate, eg. C137 100 120
//edge list,format: firstNodeName, secondNodeName, weight: e.g. C137 C120 0
void Graph::save(std::ostream& o)
{
	o<<"EGF_EvoGraphFormat"<<endl;
	o<<m_NumVertices<<endl;
	for(vector<Vertex*>::iterator j = m_Vertices -> begin(); 
			j != m_Vertices -> end(); ++j) 
	{
			o<<(*j)->getTitle()<<" "<<(*j)->getX()<<" "<<(*j)->getY()<<endl;
	}
	if(getNumEdges()!=0){
		  //o<<m_Edges->size()<<endl;
			for(vector<Edge*>::iterator i = m_Edges -> begin(); i != m_Edges -> end(); ++i)
			{
				o<<(*i)->getFromVertex()<<" "<<(*i)->getToVertex()<<" "<<(*i)->getWeight()<<endl;
			}
	}
	
	o<<"#"<<endl;
}
