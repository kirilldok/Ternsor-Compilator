#include <iostream>
#include <fstream>
#include <filesystem>
#include "../include/parser.h"
#include "../include/graph.h"
#include "test_common.h"

namespace fs = std::filesystem;

// Тест загрузки несуществующего файла
int test_nonexistent_file() {
    std::cout << "\n--- Тест: Несуществующий файл ---" << std::endl;
    
    ONNXParser parser;
    Graph graph;
    
    bool result = parser.parse("non_existent_file.onnx", graph);
    
    ASSERT_TRUE(!result);
    ASSERT_EQ(graph.size(), 0);
    
    return 0;
}

// Тест загрузки валидной модели
int test_valid_model_load() {
    std::cout << "\n--- Тест: Загрузка валидной модели ---" << std::endl;
    
    if (!fs::exists("test_models/simple_add.onnx")) {
        std::cout << "[SKIP] Тестовая модель не найдена" << std::endl;
        return 0;
    }
    
    ONNXParser parser;
    Graph graph;
    
    bool result = parser.parse("test_models/simple_add.onnx", graph);
    
    ASSERT_TRUE(result);
    ASSERT_TRUE(graph.size() > 0);
    
    return 0;
}

// Тест структуры графа после парсинга
int test_graph_structure_after_parse() {
    std::cout << "\n--- Тест: Структура графа после парсинга ---" << std::endl;
    
    if (!fs::exists("test_models/simple_add.onnx")) {
        std::cout << "[SKIP] Тестовая модель не найдена" << std::endl;
        return 0;
    }
    
    ONNXParser parser;
    Graph graph;
    parser.parse("test_models/simple_add.onnx", graph);
    
    ASSERT_TRUE(!graph.nodes.empty());
    
    for (const auto& node : graph.nodes) {
        ASSERT_TRUE(!node.op_type.empty());
    }
    
    return 0;
}

//Тест битого ONNX файла
int test_corrupted_file() {
    std::cout << "\n--- Тест: Битый ONNX файл ---" << std::endl;
    
    std::ofstream bad_file("test_models/corrupted.onnx");
    bad_file << "This is not a valid ONNX file";
    bad_file.close();
    
    ONNXParser parser;
    Graph graph;
    
    bool result = parser.parse("test_models/corrupted.onnx", graph);
    
    ASSERT_TRUE(!result);
    
    fs::remove("test_models/corrupted.onnx");
    
    return 0;
}

//Главная функция тестов парсера
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "       ТЕСТЫ ПАРСЕРА                   " << std::endl;
    std::cout << "========================================" << std::endl;
    
    tests_passed = 0;
    tests_failed = 0;
    
    test_nonexistent_file();
    test_valid_model_load();
    test_graph_structure_after_parse();
    test_corrupted_file();
    
    PRINT_TEST_STATS();
    
    return tests_failed > 0 ? 1 : 0;
}