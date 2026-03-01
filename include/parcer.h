#pragma once
#include "graph.h"
#include <string>

class ONNXParser {
public:
    bool parse(const std::string& filepath, Graph& out_graph);
};