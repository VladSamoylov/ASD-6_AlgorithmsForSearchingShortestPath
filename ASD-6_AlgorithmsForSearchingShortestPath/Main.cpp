#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <queue>
#include <stack>
#include <variant>
#include <string>
#include <iomanip>

#define ANSI_COLOR_BLUE "\033[34m"
#define ANSI_COLOR_RESET "\033[0m"
#define ANSI_COLOR_GREEN "\033[32m"
#define ANSI_COLOR_RED "\033[31m"
#define ANSI_COLOR_PURPLE "\033[35m"
#define ANSI_COLOR_LIGHTBLUE "\033[94m"

using namespace std;

const int INF = numeric_limits<int>::max() / 2;

template <typename T>
class Graph;

template <typename T>
class Vertex {
private:
	variant<T, int> vertex;
	vector<pair<variant<T, int>, int>> vertNeighbors;
	int distance;
	variant<monostate, T, int> source;
	bool isAvaible;
public:
	Vertex(const variant<T, int>& vertex) : vertex(vertex), vertNeighbors(), distance(INF), source(monostate{}), isAvaible(true) {};
	void AddNeighbor(const variant<T, int>&, const int&);
	void SetNameNeighbor(const vector<variant<T, int>>&);
	void ShowNeighbor();
	variant<T, int> GetVertName() const { return this->vertex; };
	void SetVertName(const variant<T, int>& newName) { this->vertex = newName; };
	vector<variant<T, int>> GetNeighbors();
	int GetNeighborWeight(const variant<T, int>&);
	void SetDistance(const int& distance) { this->distance = distance; }
	int GetDistance() { return this->distance; }
	void SetSource(const variant<monostate, T, int>& source) { this->source = source; }
	variant<monostate, T, int> GetSource() { return this->source; }
	void SetAvaible(const bool& b) { this->isAvaible = b; }
	bool IsAvaible() { return this->isAvaible; }
	bool operator==(const Vertex& other) const { return this->vertex == other.vertex; }
	bool operator==(const variant<T, int>& other) const { return this->vertex == other; }
};

template <typename T>
void Vertex<T>::AddNeighbor(const variant<T, int>& vertex, const int& weight) {

	this->vertNeighbors.emplace_back(vertex, weight);
}

template <typename T>
void Vertex<T>::SetNameNeighbor(const vector<variant<T, int>>& newName) {

	if (this->vertNeighbors.size() != newName.size()) throw "Error: <Amount neighbours != new amount>";

	for (int i = 0; i < this->vertNeighbors.size(); i++) {
		this->vertNeighbors[i].first = newName[i];
	}
}

template <typename T>
void Vertex<T>::ShowNeighbor() {

	cout << "vertex ";
	visit([](auto&& val) { cout << val; }, this->vertex);
	cout << " : ";
	for (auto v : this->vertNeighbors) {
		visit([](auto&& val) { cout << val; }, v.first);
		cout << "(" << v.second << ") ";
	}cout << endl;
}

template <typename T>
vector<variant<T, int>> Vertex<T>::GetNeighbors() {

	vector<variant<T, int>> neighbour;
	for (auto& v : this->vertNeighbors) {
		neighbour.push_back(v.first);
	}
	return neighbour;
}

template <typename T>
int Vertex<T>::GetNeighborWeight(const variant<T, int>& vertex) {

	for (const auto& neighbour : this->vertNeighbors) {
		if (neighbour.first == vertex) return neighbour.second;
	}
}

template <typename T>
class Graph {
private:
	bool isOriented;
	int vertexN;
	vector<Vertex<T>> listAdjacency;
public:
	Graph() : isOriented(false), vertexN(0), listAdjacency() {};
	Graph(const bool& isOriented) : isOriented(isOriented), vertexN(0), listAdjacency() {};
	void AddEdge(const variant<T, int>&, const variant<T, int>&, const int&);
	void AddVertex(const variant<T, int>&);
	void ShowVertexes();
	bool isPresentEdge(const variant<T, int>&, const variant<T, int>&);
	void ClearDataGraph() { this->vertexN = 0; this->listAdjacency.clear(); }
	void DijkstraAlgorithm(const variant<T, int>&, const variant<T, int>&);
	void FloydWarshell(const variant<T, int>&, const variant<T, int>&);
	void ShowMatrixFloyd(const vector<vector<int>>&, unordered_map<int, variant<T, int>>&);
	void BellmanFord(const variant<T, int>&, const variant<T, int>&);
	void BellmanFord(const variant<T, int>&);
	void ConstructWay(const variant<T, int>&, const variant<T, int>&);
	void SolveDifferentialConstraintS(const int&, const int&);
	void SolveTaskSecondThird(const variant<T, int>&, const variant<T, int>&);
};

template <typename T>
void Graph<T>::AddVertex(const variant<T, int>& vertex) {

	auto new_vertex = Vertex(vertex);
	if (find(this->listAdjacency.begin(), this->listAdjacency.end(), new_vertex) != this->listAdjacency.end()) return;
	this->listAdjacency.push_back(move(new_vertex));
	this->vertexN++;
}

template <typename T>
bool Graph<T>::isPresentEdge(const variant<T, int>& fVert, const variant<T, int>& sVert) {

	auto it = find(this->listAdjacency.begin(), this->listAdjacency.end(), fVert);
	if (it != this->listAdjacency.end()) {
		vector<variant<T, int>> neighbours = it->GetNeighbors();
		auto isConsist = find(neighbours.begin(), neighbours.end(), sVert);
		if (isConsist != neighbours.end()) return true;
	}

	return false;
}

template <typename T>
void Graph<T>::ShowVertexes() {

	cout << "The Graph (";
	if (isOriented) cout << "oriented";
	else cout << "non-oriented";
	cout << ") has: " << endl;
	for (auto i : this->listAdjacency) {
		i.ShowNeighbor();
	}cout << endl;
}

template <typename T>
void Graph<T>::AddEdge(const variant<T, int>& fVertex, const variant<T, int>& sVertex, const int& weight) {

	this->AddVertex(fVertex);
	this->AddVertex(sVertex);

	if (this->isPresentEdge(fVertex, sVertex)) return;
	auto it = find(this->listAdjacency.begin(), this->listAdjacency.end(), fVertex);
	if (it != this->listAdjacency.end()) {
		it->AddNeighbor(sVertex, weight);
	}
	if (!this->isOriented) {
		it = find(this->listAdjacency.begin(), this->listAdjacency.end(), sVertex);
		if (it != this->listAdjacency.end()) {
			it->AddNeighbor(fVertex, weight);
		}
	}
}

template <typename T>
void Graph<T>::ConstructWay(const variant<T, int>& sVertex, const variant<T, int>& eVertex) {
	
	int dist = 0;
	auto it = find(this->listAdjacency.begin(), this->listAdjacency.end(), eVertex);
	if (it != this->listAdjacency.end()) {
		if (holds_alternative<monostate>(it->GetSource())) throw "Error: <There is no path>";
		dist = it->GetDistance();
	}
	stack<variant<T, int>> s;
	variant<T, int> currentElem = eVertex;
	s.push(currentElem);
	while (currentElem != sVertex) {
		auto it = find(this->listAdjacency.begin(), this->listAdjacency.end(), currentElem);
		if (it != this->listAdjacency.end()) {
			auto source = it->GetSource();
			if (holds_alternative<T>(source)) currentElem = get<T>(source);
			else if(holds_alternative<int>(source)) currentElem = get<int>(source);
			else throw "Error: <The path is don't detect>";
		}
		s.push(currentElem);
	}
	vector<variant<T, int>> way;
	while (!s.empty()) {
		way.push_back(s.top());
		s.pop();
	}
	visit([](auto&& val, auto&& val2) { cout << "The way from " << ANSI_COLOR_GREEN << val << ANSI_COLOR_RESET << " to " << ANSI_COLOR_RED << val2 << ANSI_COLOR_RESET << " : "; }, sVertex, eVertex);
	for (auto& w : way) {
		visit([](auto&& val) { cout << ANSI_COLOR_PURPLE << val << ANSI_COLOR_RESET << "->"; }, w);
	}cout << endl;
	cout << "Distance = " << ANSI_COLOR_LIGHTBLUE << dist << ANSI_COLOR_RESET << endl;

}

template <typename T>
void Graph<T>::BellmanFord(const variant<T, int>& sVertex, const variant<T, int>& eVertex) {

	cout << "\n!-Bellman-Ford algorithm start..." << endl;
	bool isRelaxed = true;
	for (auto& vert : this->listAdjacency) {
		if (vert.GetVertName() == sVertex) vert.SetDistance(0);
		else vert.SetDistance(INF);
		vert.SetSource(monostate{});
	}
	for (int i = 0; i < this->vertexN - 1; i++) {
		isRelaxed = true;
		for (auto& vert : this->listAdjacency) {
			vector <variant<T, int>> neighbours = vert.GetNeighbors();
			for (auto& neighbour : neighbours) {
				int newDist = vert.GetDistance() + vert.GetNeighborWeight(neighbour);
				auto itNeighbor = find(this->listAdjacency.begin(), this->listAdjacency.end(), neighbour);
				if (itNeighbor != this->listAdjacency.end()) {
					if (newDist < itNeighbor->GetDistance()) {
						itNeighbor->SetDistance(newDist);
						auto vertName = vert.GetVertName();
						if (holds_alternative<T>(vertName)) itNeighbor->SetSource(variant<monostate, T, int>(in_place_type<T>, get<T>(vertName)));
						else if (holds_alternative<int>(vertName)) itNeighbor->SetSource(variant<monostate, T, int>(in_place_type<int>, get<int>(vertName)));
						isRelaxed = false;
					}
				}				
			}
		}
		cout << "Iteration " << i << ": ";
		for (auto& vert : this->listAdjacency) {
			visit([](auto&& val) { cout << val; }, vert.GetVertName());
			cout << " = " << vert.GetDistance() << ", ";
		}cout << endl;
		if (isRelaxed) { cout << "iteration - " << i + 1 << endl; break; }
	}
	for (auto& vert : this->listAdjacency) {
		if (isRelaxed) break;
		vector <variant<T, int>> neighbours = vert.GetNeighbors();
		for (auto& neighbour : neighbours) {
			int newDist = vert.GetDistance() + vert.GetNeighborWeight(neighbour);
			auto itNeighbor = find(this->listAdjacency.begin(), this->listAdjacency.end(), neighbour);
			if (itNeighbor != this->listAdjacency.end()) {
				if (newDist < itNeighbor->GetDistance()) throw "Error: <The graph with negative cycles>";
			}
		}
	}
	this->ConstructWay(sVertex, eVertex);
}

template <typename T>
void Graph<T>::BellmanFord(const variant<T, int>& sVertex) {

	cout << "\n!-Bellman-Ford algorithm start..." << endl;
	bool isRelaxed = true;
	for (auto& vert : this->listAdjacency) {
		if (vert.GetVertName() == sVertex) vert.SetDistance(0);
		else vert.SetDistance(INF);
		vert.SetSource(monostate{});
	}
	for (int i = 0; i < this->vertexN - 1; i++) {
		isRelaxed = true;
		for (auto& vert : this->listAdjacency) {
			vector <variant<T, int>> neighbours = vert.GetNeighbors();
			for (auto& neighbour : neighbours) {
				int newDist = vert.GetDistance() + vert.GetNeighborWeight(neighbour);
				auto itNeighbor = find(this->listAdjacency.begin(), this->listAdjacency.end(), neighbour);
				if (itNeighbor != this->listAdjacency.end()) {
					if (newDist < itNeighbor->GetDistance()) {
						itNeighbor->SetDistance(newDist);
						auto vertName = vert.GetVertName();
						if (holds_alternative<T>(vertName)) itNeighbor->SetSource(variant<monostate, T, int>(in_place_type<T>, get<T>(vertName)));
						else if (holds_alternative<int>(vertName)) itNeighbor->SetSource(variant<monostate, T, int>(in_place_type<int>, get<int>(vertName)));
						isRelaxed = false;
					}
				}
			}
		}
		cout << "Iteration " << i << ": ";
		for (auto& vert : this->listAdjacency) {
			visit([](auto&& val) { cout << val; }, vert.GetVertName());
			cout << " = " << vert.GetDistance() << ", ";
		}
		cout << endl;
		if (isRelaxed) { cout << "iteration - " << i + 1 << endl; break; }
	}
	for (auto& vert : this->listAdjacency) {
		if (isRelaxed) break;
		vector <variant<T, int>> neighbours = vert.GetNeighbors();
		for (auto& neighbour : neighbours) {
			int newDist = vert.GetDistance() + vert.GetNeighborWeight(neighbour);
			auto itNeighbor = find(this->listAdjacency.begin(), this->listAdjacency.end(), neighbour);
			if (itNeighbor != this->listAdjacency.end()) {
				if (newDist < itNeighbor->GetDistance()) throw "Error: <The graph with negative cycles>";
			}
		}
	}
}

template <typename T>
void Graph<T>::DijkstraAlgorithm(const variant<T, int>& sVertex, const variant<T, int>& eVertex) {

	cout << "\n!-Dijkstra algorithm start..." << endl;
	for (auto& vert : this->listAdjacency) {
		if (vert.GetVertName() == sVertex) vert.SetDistance(0);
		else vert.SetDistance(INF);
		vert.SetSource(monostate{});
	}
	int iter = 1;
	priority_queue<pair<int, variant<T, int>>, vector<pair<int, variant<T, int>>>, greater<>> q;
	q.push({ 0, sVertex });
	unordered_map<variant<T, int>, bool> vertexFix;
	while (!q.empty()) {
		variant<T, int> currentElem = q.top().second;
		q.pop();
		cout << "Iteration " << ANSI_COLOR_BLUE << iter << ANSI_COLOR_RESET << endl;
		cout << "Current vertex : ";
		visit([](auto&& val) { cout << ANSI_COLOR_PURPLE << val << ANSI_COLOR_RESET << endl; }, currentElem);
		vertexFix[currentElem] = true;
		auto itVert = find(this->listAdjacency.begin(), this->listAdjacency.end(), currentElem);
		if (itVert != this->listAdjacency.end() && itVert->IsAvaible()) {
			if (itVert->GetVertName() == eVertex) break;
			vector<variant<T, int>> neighbours = itVert->GetNeighbors();
			cout << "The vertex neighbour : ";
			for (const auto& neighbour : neighbours) {
				visit([](auto&& val) { cout << ANSI_COLOR_GREEN << val << ANSI_COLOR_RESET << " "; }, neighbour);
			}cout << endl;
			for (auto& neighbour : neighbours) {
				if (vertexFix[neighbour]) continue;
				auto itNei = find(this->listAdjacency.begin(), this->listAdjacency.end(), neighbour);
				if (itNei != this->listAdjacency.end()) {
					int newDist = itVert->GetDistance() + itVert->GetNeighborWeight(neighbour);
					if (itVert->GetNeighborWeight(neighbour) < 0) throw "Error <The graph contains negative edges>";
					if (newDist < itNei->GetDistance()) {
						cout << "Updating distance to vertex ";
						visit([](auto&& val) { cout << ANSI_COLOR_GREEN << val << ANSI_COLOR_RESET << " "; }, neighbour);
						if (itNei->GetDistance() == INF) cout << " : old dist = " << ANSI_COLOR_RED << "INF" << ANSI_COLOR_RESET << " -> " << ANSI_COLOR_LIGHTBLUE << newDist << ANSI_COLOR_RESET << endl;
						else cout << " : old dist = " << ANSI_COLOR_RED << itNei->GetDistance() << ANSI_COLOR_RESET << " -> " << ANSI_COLOR_LIGHTBLUE << newDist << ANSI_COLOR_RESET << endl;
						itNei->SetDistance(newDist);
						auto vertName = itVert->GetVertName();
						if (holds_alternative<T>(vertName)) itNei->SetSource(variant<monostate, T, int>(in_place_type<T>, get<T>(vertName)));
						else if (holds_alternative<int>(vertName)) itNei->SetSource(variant<monostate, T, int>(in_place_type<int>, get<int>(vertName)));
						q.push({ newDist, neighbour });
					}
				}
			}
		}
		iter++;
		cout << "-------------------------------------\n";
	}
	cout << "iteration - " << iter << endl;
	this->ConstructWay(sVertex, eVertex);
}

template <typename T>
void Graph<T>::ShowMatrixFloyd(const vector<vector<int>>& matrix, unordered_map<int, variant<T, int>>& vertexNames) {
	cout << setw(5) << left << "_";
	for (int i = 0; i < this->vertexN; i++) {
		visit([](auto&& val) { cout << ANSI_COLOR_GREEN << setw(5) << val << ANSI_COLOR_RESET; }, vertexNames[i]);
	}cout << endl;
	for (int i = 0; i < this->vertexN; i++) {
		visit([](auto&& val) { cout << ANSI_COLOR_LIGHTBLUE << setw(5) << val << ANSI_COLOR_RESET << '|'; }, vertexNames[i]);
		for (int j = 0; j < this->vertexN; j++) {
			cout << setw(5) << left;
			if (matrix[i][j] == INF) cout << "INF";
			else cout << matrix[i][j];
		}cout << endl;
	}
	cout << endl;
}

template <typename T>
void Graph<T>::FloydWarshell(const variant<T, int>& sVertex, const variant<T, int>& eVertex) {

	cout << "\n!-Floyd-Warshell algorithm start..." << endl;
	unordered_map<int, variant<T, int>> vertexNames;
	unordered_map<variant<T, int>, int> vertexNames_revers;
	int newName = 0;
	for (auto& vertex : this->listAdjacency) {
		auto vertName = vertex.GetVertName();
		vertexNames[newName] = vertName;
		vertexNames_revers[vertName] = newName;
		newName++;
	}
	vector<vector<int>> matrixDist(this->vertexN, vector<int>(this->vertexN, INF));
	vector<vector<int>> matrixPred(this->vertexN, vector<int>(this->vertexN, -1));
	for (int i = 0; i < this->vertexN; i++) {
		for (int j = 0; j < this->vertexN; j++) {
			auto itV = find(this->listAdjacency.begin(), this->listAdjacency.end(), vertexNames[i]);
			if (itV != this->listAdjacency.end()) {
				vector<variant<T, int>> neighbours = itV->GetNeighbors();
				auto itN = find(neighbours.begin(), neighbours.end(), vertexNames[j]);
				if (itN != neighbours.end()) {
					matrixDist[i][j] = itV->GetNeighborWeight(vertexNames[j]);
					matrixPred[i][j] = vertexNames_revers[itV->GetVertName()];
				}
			}			
			if (i == j) matrixDist[i][j] = 0;
		}
	}
	cout << "!-D^0 : " << endl;
	this->ShowMatrixFloyd(matrixDist, vertexNames);
	cout << "!-Prev^0 : " << endl;
	this->ShowMatrixFloyd(matrixPred, vertexNames);
	
	for (int k = 0; k < this->vertexN; k++) {
		auto itK = find(this->listAdjacency.begin(), this->listAdjacency.end(), vertexNames[k]);
		if (itK != this->listAdjacency.end() && !itK->IsAvaible()) continue;
		for (int i = 0; i < this->vertexN; i++) {
			auto itI = find(this->listAdjacency.begin(), this->listAdjacency.end(), vertexNames[i]);
			if (itI != this->listAdjacency.end() && !itI->IsAvaible()) continue;
			for (int j = 0; j < this->vertexN; j++) {
				auto itJ = find(this->listAdjacency.begin(), this->listAdjacency.end(), vertexNames[j]);
				if (itJ != this->listAdjacency.end() && !itJ->IsAvaible()) continue;
				if (matrixDist[i][j] > matrixDist[i][k] + matrixDist[k][j] && matrixDist[i][k] != INF && matrixDist[k][j] != INF) {
					matrixDist[i][j] = matrixDist[i][k] + matrixDist[k][j];
					matrixPred[i][j] = matrixPred[k][j];
				}
			}
		}
	}
	cout << "!-D^last : " << endl;
	this->ShowMatrixFloyd(matrixDist, vertexNames);
	cout << "!-Prev^last : " << endl;
	this->ShowMatrixFloyd(matrixPred, vertexNames);
	stack<variant<T, int>> s;
	variant<T, int > currentElem = eVertex;
	if (matrixPred[vertexNames_revers[sVertex]][vertexNames_revers[currentElem]] == -1) throw "Error: <There is no path>";
	for (int p = vertexNames_revers[eVertex]; p != -1; p = matrixPred[vertexNames_revers[sVertex]][p]) {
		currentElem = vertexNames[p];
		s.push(currentElem);
	}
	vector<variant<T, int>> way;
	while (!s.empty()) {
		way.push_back(s.top());
		s.pop();
	}
	visit([](auto&& val, auto&& val2) { cout << "The way from " << ANSI_COLOR_GREEN << val << ANSI_COLOR_RESET << " to " << ANSI_COLOR_RED << val2 << ANSI_COLOR_RESET << " : "; }, sVertex, eVertex);
	for (const auto& w : way) {
		visit([](auto&& val) { cout << ANSI_COLOR_PURPLE << val << ANSI_COLOR_RESET << "->"; }, w);
	}cout << endl;
	cout << "Distance = " << ANSI_COLOR_LIGHTBLUE << matrixDist[vertexNames_revers[sVertex]][vertexNames_revers[eVertex]] << ANSI_COLOR_RESET << endl;

}

template <typename T>
void Graph<T>::SolveDifferentialConstraintS(const int& n, const int& m) {

	variant<T, int> s = "s";
	vector <Vertex<T>> verts = this->listAdjacency;
	for (auto& vertex : verts) {
		//this->AddEdge(s, vertex.GetVertName(), 0);
	}
	this->BellmanFord("x1");

	cout << "Solution to the sytem of difference constraints with n(" << ANSI_COLOR_GREEN << n << ANSI_COLOR_RESET << ") unknown : " << endl;
	for (auto& vertex : this->listAdjacency) {
		if (vertex.GetVertName() != s) {
			visit([](auto&& val) { cout << val << " = "; }, vertex.GetVertName());
			cout << ANSI_COLOR_LIGHTBLUE << vertex.GetDistance() << ANSI_COLOR_RESET << endl;
		}
	}
}

template <typename T>
void Graph<T>::SolveTaskSecondThird(const variant<T, int>& sNode, const variant<T, int>& eNode) {

	unordered_set<variant<T, int>> unAvaible;
	unAvaible.insert("R-1");
	unAvaible.insert("PC-3");
	if (unAvaible.count(sNode) || unAvaible.count(eNode)) throw "<The shoosen node is not avaible>";
	for (auto n : unAvaible) {
		auto itNode = find(this->listAdjacency.begin(), this->listAdjacency.end(), n);
		if (itNode != this->listAdjacency.end()) itNode->SetAvaible(false);
	}
	this->DijkstraAlgorithm(sNode, eNode);
	cout << "Determine optimal routes for all nodes in the network : " << endl;
	this->FloydWarshell(sNode, eNode);
}

int main() {

	Graph<string> g(false);
	g.AddEdge(1, 2, 1);
	g.AddEdge(1, 3, 2);
	g.AddEdge(1, 4, 4);
	g.AddEdge(2, 5, 3);
	g.AddEdge(2, 7, 2);
	g.AddEdge(3, 5, 7);
	g.AddEdge(3, 6, 6);
	g.AddEdge(4, 6, 2);
	g.AddEdge(4, 7, 3);
	g.AddEdge(5, 8, 7);
	g.AddEdge(5, 9, 5);
	g.AddEdge(6, 8, 7);
	g.AddEdge(6, 10, 3);
	g.AddEdge(7, 9, 5);
	g.AddEdge(7, 10, 4);
	g.AddEdge(8, 11, 4);
	g.AddEdge(9, 11, 1);
	g.AddEdge(10, 11, 4);

	Graph<string> g2(true);
	g2.AddEdge(1, 2, 2);
	g2.AddEdge(1, 3, 7);
	g2.AddEdge(1, 4, 12);
	g2.AddEdge(2, 3, 1);
	g2.AddEdge(2, 5, 7);
	g2.AddEdge(3, 5, 1);
	g2.AddEdge(3, 6, 14);
	g2.AddEdge(4, 3, 3);
	g2.AddEdge(4, 6, 2);
	g2.AddEdge(5, 4, 2);
	g2.AddEdge(5, 6, 11);


	int n = 5, m = 5;
	Graph<string> gF(true);
	gF.AddEdge("x1", "x5", 0);
	gF.AddEdge("x2", "x1", 2);
	gF.AddEdge("x3", "x2", -1);
	gF.AddEdge("x4", "x3", 3);
	gF.AddEdge("x5", "x4", 1);

	Graph<string> gS(false);
	gS.AddEdge("PC-1", "PC-2", 30);
	gS.AddEdge("PC-1", "PC-3", 30);
	gS.AddEdge("PC-2", "R-1", 25);
	gS.AddEdge("PC-3", "R-1", 25);
	gS.AddEdge("D-1", "R-1", 45);
	gS.AddEdge("D-1", "PC-2", 45);
	gS.AddEdge("D-1", "D-2", 60);
	gS.AddEdge("D-2", "R-1", 45);
	gS.AddEdge("D-2", "PC-3", 45);
	gS.AddEdge("D-2", "PC-A", 45);
	gS.AddEdge("R-1", "R-2", 10);
	gS.AddEdge("PC-A", "R-2", 25);
	gS.AddEdge("R-2", "S-1", 15);
	gS.AddEdge("R-2", "S-2", 40);
	gS.AddEdge("S-1", "S-2", 5);

	try {
		//g.ShowVertexes();
		//g.BellmanFord(1, 11);
		//g.DijkstraAlgorithm(1, 11);
		//g.FloydWarshell(1, 11);
		//g2.ShowVertexes();
		//g2.BellmanFord(1, 6);
		//g2.DijkstraAlgorithm(1, 6);
		//g2.FloydWarshell(1, 6);
		//gF.SolveDifferentialConstraintS(n, m);
		gS.SolveTaskSecondThird("PC-1", "S-2");
	}
	catch (const char* err) {
		cerr << ANSI_COLOR_RED << err << ANSI_COLOR_RESET << endl;
	}

	return 0;
}