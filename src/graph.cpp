#include "../include/graph.h"

void Graph::add_node(const Node& node) {
    nodes.push_back(node);
}

size_t Graph::size() const {
    return nodes.size();
}

void Graph::clear() {
    nodes.clear();
    inputs.clear();
    outputs.clear();
}