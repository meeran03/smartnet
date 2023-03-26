void getNodesAndEdges(Value *val, vector<Value *> &nodes, vector<vector<Value *>> &edges)
{
    function<void(Value *)> dfs = [&](Value *v) {
        if (find(nodes.begin(), nodes.end(), v) != nodes.end())
        {
            return;
        }
        nodes.push_back(v);
        if (v->prev->size() == 0)
        {
            return;
        }
        for (Value *child : *(v->prev))
        {
            dfs(child);
            edges.push_back({child, v});
        }
    };
    dfs(val);
}

string getOperandType(Value *val)
{
    if (val->_op == NONE)
    {
        return "";
    }
    if (val->_op == ADD)
    {
        return "+";
    }
    if (val->_op == MULTIPLY)
    {
        return "*";
    }

    return "";
}

// the graph moves from left to right
// create a rectangular record for each node
// creates an op node for each operation
void printDiGraph(Value* val) {
    string dot = "digraph G {";
    dot += "rankdir=LR;";
    vector<vector<Value *>> edges;
    vector<Value *> nodes;
    getNodesAndEdges(val, nodes, edges);
    for (Value *v : nodes) {
        string uid = to_string(rand());
        dot += (v->label == "" ? uid : v->label) + "[shape=record, label=\"{" + v->label + " | data " + to_string(v->data) + " | grad " + to_string(v->grad) + "}\"];\n";
    }

    for (vector<Value *> edge : edges) {
        dot += edge[0]->label + " -> " + edge[1]->label + ";\n";
    }

    dot += "}";


    GVC_t *gvc;
    Agraph_t *g;
    FILE *fp;

    gvc = gvContext();
    g = agmemread(dot.c_str());
    gvLayout(gvc, g, "dot");
    gvRenderFilename(gvc, g, "svg", "graph.svg");
    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);

    system("open graph.svg");
    
}

