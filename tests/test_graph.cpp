#include <iostream>
#include <fstream>
#include "../include/graph.h"
#include "test_common.h"

// Тест создания пустого графа
int test_empty_graph() {
    std::cout << "\n--- Тест: Пустой граф ---" << std::endl;
    
    Graph graph;
    ASSERT_EQ(graph.size(), 0);
    ASSERT_TRUE(graph.nodes.empty());
    ASSERT_TRUE(graph.inputs.empty());
    ASSERT_TRUE(graph.outputs.empty());
    
    return 0;
}

// Тест добавления узла
int test_add_node() {
    std::cout << "\n--- Тест: Добавление узла ---" << std::endl;
    
    Graph graph;
    Node node;
    node.name = "test_node";
    node.op_type = "Relu";
    node.inputs = {"input1"};
    node.outputs = {"output1"};
    
    graph.add_node(node);
    
    ASSERT_EQ(graph.size(), 1);
    ASSERT_STR_EQ(graph.nodes[0].name, "test_node");
    ASSERT_STR_EQ(graph.nodes[0].op_type, "Relu");
    ASSERT_EQ(graph.nodes[0].inputs.size(), 1);
    ASSERT_EQ(graph.nodes[0].outputs.size(), 1);
    
    return 0;
}

//Тест множественных узлов
int test_multiple_nodes() {
    std::cout << "\n--- Тест: Множественные узлы ---" << std::endl;
    
    Graph graph;
    
    for (int i = 0; i < 5; i++) {
        Node node;
        node.name = "node_" + std::to_string(i);
        node.op_type = "Add";
        graph.add_node(node);
    }
    
    ASSERT_EQ(graph.size(), 5);
    ASSERT_STR_EQ(graph.nodes[4].name, "node_4");
    
    return 0;
}

// Тест очистки графа
int test_clear_graph() {
    std::cout << "\n--- Тест: Очистка графа ---" << std::endl;
    
    Graph graph;
    Node node;
    node.op_type = "Conv";
    graph.add_node(node);
    graph.add_node(node);
    
    ASSERT_EQ(graph.size(), 2);
    
    graph.clear();
    
    ASSERT_EQ(graph.size(), 0);
    ASSERT_TRUE(graph.nodes.empty());
    
    return 0;
}

// Тест атрибутов узла
int test_node_attributes() {
    std::cout << "\n--- Тест: Атрибуты узла ---" << std::endl;
    
    Node node;
    node.op_type = "Conv";
    
    Attribute attr1;
    attr1.name = "strides";
    attr1.value = std::vector<int64_t>{2, 2};
    
    Attribute attr2;
    attr2.name = "dilations";
    attr2.value = std::vector<int64_t>{1, 1};
    
    node.attributes.push_back(attr1);
    node.attributes.push_back(attr2);
    
    ASSERT_EQ(node.attributes.size(), 2);
    ASSERT_STR_EQ(node.attributes[0].name, "strides");
    ASSERT_STR_EQ(node.attributes[1].name, "dilations");
    
    return 0;
}

// Тест связей между узлами
int test_node_connections() {
    std::cout << "\n--- Тест: Связи между узлами ---" << std::endl;
    
    Graph graph;
    
    Node node1;
    node1.name = "conv1";
    node1.outputs = {"conv1_out"};
    
    Node node2;
    node2.name = "relu1";
    node2.inputs = {"conv1_out"};
    node2.outputs = {"relu1_out"};
    
    graph.add_node(node1);
    graph.add_node(node2);
    
    ASSERT_STR_EQ(graph.nodes[0].outputs[0], "conv1_out");
    ASSERT_STR_EQ(graph.nodes[1].inputs[0], "conv1_out");
    
    return 0;
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "       ТЕСТЫ СТРУКТУРЫ ГРАФА           " << std::endl;
    std::cout << "========================================" << std::endl;
    
    tests_passed = 0;
    tests_failed = 0;
    
    test_empty_graph();
    test_add_node();
    test_multiple_nodes();
    test_clear_graph();
    test_node_attributes();
    test_node_connections();
    
    PRINT_TEST_STATS();
    
    return tests_failed > 0 ? 1 : 0;
}