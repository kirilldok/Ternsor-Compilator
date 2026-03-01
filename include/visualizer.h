#pragma once
#include "graph.h"
#include <string>

class Visualizer {
public:
    void export_to_dot(const Graph& graph, const std::string& filename);
};