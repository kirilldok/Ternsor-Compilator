#include "include/visualizer.h"
#include <fstream>
#include <iostream>

void Visualizer::export_to_dot(const Graph& graph, const std::string& filename) {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Error: Cannot create file " << filename << std::endl;
        return;
    }

    out << "digraph NeuralNetwork {" << std::endl;
    out << "    rankdir=LR;" << std::endl; // Слева направо

    // Визуализация узлов
    for (const auto& node : graph.nodes) {
        std::string label = node.op_type;
        
        for (const auto& attr : node.attributes) {
            if (attr.name == "strides" || attr.name == "kernel_shape") {
                label += "\\n" + attr.name; 
            }
        }

        std::string safe_name = node.name.empty() ? "unnamed" : node.name;
        cout << "    \"" << safe_name << "\" [label=\"" << label << "\"];\"" << std::endl;
    }
    
    //Перечисляем входы как зависимость
    std::unordered_map<std::string, std::string> tensor_producer;
    for (const auto& node : graph.nodes) {
        for (const auto& out : node.outputs) {
            tensor_producer[out] = node.name;
        }
    }

    for (const auto& node : graph.nodes) {
        for (const auto& inp : node.inputs) {
            // Если вход - это выход другого узла
            if (tensor_producer.find(inp) != tensor_producer.end()) {
                std::string src = tensor_producer[inp];
                std::string dst = node.name.empty() ? "unnamed" : node.name;
                if (src != dst) {
                    out << "    \"" << src << "\" -> \"" << dst << "\";" << std::endl;
                }
            } else {
                // Если вход - это внешний тензор (input graph)
                out << "    \"" << inp << "\" -> \"" << (node.name.empty() ? "unnamed" : node.name) << "\";" << std::endl;
            }
        }
    }

    out << "}" << std::endl;
    out.close();
    std::cout << "Graph saved to " << filename << std::endl;
}