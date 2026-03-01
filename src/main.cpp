#include <iostream>
#include "../include/parser.h"
#include "../include/visualizer.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <model.onnx>" << std::endl;
        return 1;
    }

    std::string model_path = argv[1];
    Graph graph;
    ONNXParser parser;
    Visualizer viz;

    std::cout << "Parsing model: " << model_path << "..." << std::endl;

    // 1. Парсинг ONNX
    if (!parser.parse(model_path, graph)) {
        std::cerr << "Failed to parse model." << std::endl;
        return 1;
    }

    std::cout << "Successfully parsed " << graph.size() << " nodes." << std::endl;

    // 2. Визуализация
    viz.export_to_dot(graph, "graph.dot");

    // Инструкция для пользователя
    std::cout << "To view the graph, run: dot -Tpng graph.dot -o graph.png" << std::endl;

    return 0;
}