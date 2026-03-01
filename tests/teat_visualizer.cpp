#include <iostream>
#include <fstream>
#include <filesystem>
#include "../include/visualizer.h"
#include "../include/graph.h"
#include "test_common.h"

namespace fs = std::filesystem;

//Тест создания DOT файла
int test_create_dot_file() {
    std::cout << "\n--- Тест: Создание DOT файла ---" << std::endl;
    
    Graph graph;
    Node node;
    node.name = "test_node";
    node.op_type = "Relu";
    graph.add_node(node);
    
    Visualizer viz;
    viz.export_to_dot(graph, "test_output.dot");
    
    ASSERT_FILE_EXISTS("test_output.dot");
    
    fs::remove("test_output.dot");
    
    return 0;
}

// Тест структуры DOT файла
int test_dot_file_structure() {
    std::cout << "\n--- Тест: Структура DOT файла ---" << std::endl;
    
    Graph graph;
    Node node;
    node.name = "conv1";
    node.op_type = "Conv";
    graph.add_node(node);
    
    Visualizer viz;
    viz.export_to_dot(graph, "test_structure.dot");
    
    std::ifstream file("test_structure.dot");
    std::string content((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
    file.close();
    
    ASSERT_TRUE(content.find("digraph") != std::string::npos);
    ASSERT_TRUE(content.find("conv1") != std::string::npos);
    ASSERT_TRUE(content.find("Conv") != std::string::npos);
    
    fs::remove("test_structure.dot");
    
    return 0;
}

// Тест визуализации нескольких узлов
int test_multiple_nodes_viz() {
    std::cout << "\n--- Тест: Визуализация нескольких узлов ---" << std::endl;
    
    Graph graph;
    
    Node node1;
    node1.name = "node1";
    node1.op_type = "Add";
    node1.outputs = {"out1"};
    
    Node node2;
    node2.name = "node2";
    node2.op_type = "Relu";
    node2.inputs = {"out1"};
    node2.outputs = {"out2"};
    
    graph.add_node(node1);
    graph.add_node(node2);
    
    Visualizer viz;
    viz.export_to_dot(graph, "test_multi.dot");
    
    std::ifstream file("test_multi.dot");
    std::string content((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
    file.close();
    
    ASSERT_TRUE(content.find("node1") != std::string::npos);
    ASSERT_TRUE(content.find("node2") != std::string::npos);
    ASSERT_TRUE(content.find("->") != std::string::npos);
    
    fs::remove("test_multi.dot");
    
    return 0;
}

// Тест пустого графа
int test_empty_graph_viz() {
    std::cout << "\n--- Тест: Визуализация пустого графа ---" << std::endl;
    
    Graph graph;
    
    Visualizer viz;
    viz.export_to_dot(graph, "test_empty.dot");
    
    ASSERT_FILE_EXISTS("test_empty.dot");
    
    std::ifstream file("test_empty.dot");
    std::string content((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
    file.close();
    
    ASSERT_TRUE(content.find("digraph") != std::string::npos);
    
    fs::remove("test_empty.dot");
    
    return 0;
}

//Главная функция тестов визуализатора
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "       ТЕСТЫ ВИЗУАЛИЗАТОРА             " << std::endl;
    std::cout << "========================================" << std::endl;
    
    tests_passed = 0;
    tests_failed = 0;
    
    test_create_dot_file();
    test_dot_file_structure();
    test_multiple_nodes_viz();
    test_empty_graph_viz();
    
    PRINT_TEST_STATS();
    
    return tests_failed > 0 ? 1 : 0;
}