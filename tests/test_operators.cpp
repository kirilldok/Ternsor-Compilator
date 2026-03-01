#include <iostream>
#include <fstream>
#include "../include/operations.h"
#include "../include/graph.h"
#include "test_common.h"

// Тест списка поддерживаемых операци
int test_supported_ops_list() {
    std::cout << "\n--- Тест: Список операций ---" << std::endl;
    
    const auto& ops = OperationValidator::get_supported_ops();
    
    ASSERT_TRUE(ops.find("Add") != ops.end());
    ASSERT_TRUE(ops.find("Mul") != ops.end());
    ASSERT_TRUE(ops.find("Conv") != ops.end());
    ASSERT_TRUE(ops.find("Relu") != ops.end());
    ASSERT_TRUE(ops.find("MatMul") != ops.end());
    ASSERT_TRUE(ops.find("Gemm") != ops.end());
    
    ASSERT_EQ(ops.size(), 6);
    
    return 0;
}

// Тест проверки поддержки операции
int test_is_supported() {
    std::cout << "\n--- Тест: Проверка поддержки ---" << std::endl;
    
    ASSERT_TRUE(OperationValidator::is_supported("Add"));
    ASSERT_TRUE(OperationValidator::is_supported("Mul"));
    ASSERT_TRUE(OperationValidator::is_supported("Conv"));
    ASSERT_TRUE(OperationValidator::is_supported("Relu"));
    ASSERT_TRUE(OperationValidator::is_supported("MatMul"));
    ASSERT_TRUE(OperationValidator::is_supported("Gemm"));
    
    ASSERT_TRUE(!OperationValidator::is_supported("Softmax"));
    ASSERT_TRUE(!OperationValidator::is_supported("BatchNormalization"));
    
    return 0;
}

// Тест атрибутов Conv
int test_conv_attributes() {
    std::cout << "\n--- Тест: Атрибуты Conv ---" << std::endl;
    
    const auto& attrs = OperationValidator::get_expected_attributes();
    auto it = attrs.find("Conv");
    
    ASSERT_TRUE(it != attrs.end());
    const auto& conv_attrs = it->second;
    
    bool has_strides = false, has_dilations = false, has_pads = false;
    for (const auto& attr : conv_attrs) {
        if (attr == "strides") has_strides = true;
        if (attr == "dilations") has_dilations = true;
        if (attr == "pads") has_pads = true;
    }
    
    ASSERT_TRUE(has_strides);
    ASSERT_TRUE(has_dilations);
    ASSERT_TRUE(has_pads);
    
    return 0;
}

// Тест атрибутов Gemm
int test_gemm_attributes() {
    std::cout << "\n--- Тест: Атрибуты Gemm ---" << std::endl;
    
    const auto& attrs = OperationValidator::get_expected_attributes();
    auto it = attrs.find("Gemm");
    
    ASSERT_TRUE(it != attrs.end());
    const auto& gemm_attrs = it->second;
    
    bool has_alpha = false, has_beta = false;
    for (const auto& attr : gemm_attrs) {
        if (attr == "alpha") has_alpha = true;
        if (attr == "beta") has_beta = true;
    }
    
    ASSERT_TRUE(has_alpha);
    ASSERT_TRUE(has_beta);
    
    return 0;
}

// Тест простых операций без атрибутов
int test_simple_ops_no_attrs() {
    std::cout << "\n--- Тест: Простые операции без атрибутов ---" << std::endl;
    
    const auto& attrs = OperationValidator::get_expected_attributes();
    
    auto it_add = attrs.find("Add");
    ASSERT_TRUE(it_add != attrs.end());
    ASSERT_TRUE(it_add->second.empty());
    
    auto it_mul = attrs.find("Mul");
    ASSERT_TRUE(it_mul != attrs.end());
    ASSERT_TRUE(it_mul->second.empty());
    
    auto it_relu = attrs.find("Relu");
    ASSERT_TRUE(it_relu != attrs.end());
    ASSERT_TRUE(it_relu->second.empty());
    
    return 0;
}

// Тест структуры узла Conv
int test_conv_node_structure() {
    std::cout << "\n--- Тест: Структура узла Conv ---" << std::endl;
    
    Node conv;
    conv.op_type = "Conv";
    conv.name = "conv1";
    
    Attribute stride;
    stride.name = "strides";
    stride.value = std::vector<int64_t>{2, 2};
    conv.attributes.push_back(stride);
    
    Attribute dilation;
    dilation.name = "dilations";
    dilation.value = std::vector<int64_t>{1, 1};
    conv.attributes.push_back(dilation);
    
    ASSERT_STR_EQ(conv.op_type, "Conv");
    ASSERT_EQ(conv.attributes.size(), 2);
    ASSERT_STR_EQ(conv.attributes[0].name, "strides");
    
    return 0;
}

// Тест структуры узла Gemm
int test_gemm_node_structure() {
    std::cout << "\n--- Тест: Структура узла Gemm ---" << std::endl;
    
    Node gemm;
    gemm.op_type = "Gemm";
    gemm.name = "gemm1";
    
    Attribute alpha;
    alpha.name = "alpha";
    alpha.value = static_cast<float>(1.0f);
    gemm.attributes.push_back(alpha);
    
    Attribute beta;
    beta.name = "beta";
    beta.value = static_cast<float>(0.0f);
    gemm.attributes.push_back(beta);
    
    ASSERT_STR_EQ(gemm.op_type, "Gemm");
    ASSERT_EQ(gemm.attributes.size(), 2);
    
    return 0;
}

// Главная функция тестов операций
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "       ТЕСТЫ ОПЕРАЦИЙ                  " << std::endl;
    std::cout << "========================================" << std::endl;
    
    tests_passed = 0;
    tests_failed = 0;
    
    test_supported_ops_list();
    test_is_supported();
    test_conv_attributes();
    test_gemm_attributes();
    test_simple_ops_no_attrs();
    test_conv_node_structure();
    test_gemm_node_structure();
    
    PRINT_TEST_STATS();
    
    return tests_failed > 0 ? 1 : 0;
}