#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "attribute.h"


struct Node {
    std::string name;           
    std::string op_type;        // Тип операции (Conv, Relu, Add...)
    std::vector<std::string> inputs;  // Имена входных тензоров
    std::vector<std::string> outputs; // Имена выходных тензоров
    std::vector<Attribute> attributes; // Параметры операции
};

// Полный граф нейронной сети
class Graph {
public:
    std::vector<Node> nodes;
    std::vector<std::string> inputs;  // Входы графа
    std::vector<std::string> outputs; // Выходы графа

    // Добавить узел в граф
    void add_node(const Node& node);
    
    // Получить количество узлов
    size_t size() const;
    
    // Очистка графа
    void clear();
};