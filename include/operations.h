#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <iostream>

class OperationValidator {
public:
    //Получение списка поддерживаемых операций 
    static const std::unordered_set<std::string>& get_supported_ops() { 
        static const std::unordered_set<std::string> supported = {
            "Add",      // Поэлементное сложение
            "Mul",      // Поэлементное умножение
            "Conv",     // Свертка
            "Relu",     // Функция активации
            "MatMul",   // Матричное умножение
            "Gemm"      // Обобщенное матричное умножение
        };
        return supported;
    }

    // Проверка поддержки операции
    static bool is_supported(const std::string& op_type) {
        return get_supported_ops().find(op_type) != get_supported_ops().end();
    }

    //Получение ожидаемых атрибутов для каждой операции
    static const std::unordered_map<std::string, std::vector<std::string>>& get_expected_attributes() {
        static const std::unordered_map<std::string, std::vector<std::string>> attrs = {
            {"Add", {}},
            
            {"Mul", {}},
            
            {"Relu", {}},
            
            {"MatMul", {}},
            
            {"Conv", {
                "strides",      // Шаг свертки
                "dilations",    // Разряжение
                "pads",         // Дополнение
                "group",        // Количество групп
                "kernel_shape"  // Размер ядра
            }},
            
            {"Gemm", {
                "alpha",    // Коэффициент для AB 
                "beta",     // Коэффициент для C 
                "transA",   // Транспонировать A
                "transB"    // Транспонировать B
            }}
        };
        return attrs;
    }

    //Валидация операции 
    static bool validate_operation(const std::string& op_type, const std::string& node_name) {
        if (!is_supported(op_type)) {
            std::cerr << "[WARNING] Неподдерживаемая операция: '" << op_type 
                      << "' в узле '" << node_name << "'" << std::endl;
            std::cerr << "          Поддерживаются: Add, Mul, Conv, Relu, MatMul, Gemm" << std::endl;
            return false;
        }
        return true;
    }

    static void validate_attributes(const std::string& op_type, 
                                    const std::vector<std::string>& attr_names,
                                    const std::string& node_name) {
        const auto& expected = get_expected_attributes();
        auto it = expected.find(op_type);
        
        if (it == expected.end()) {
            return;
        }

        const auto& expected_attrs = it->second;
        
        // Проверяем наличие неожиданных атрибутов
        for (const auto& attr : attr_names) {
            bool found = false;
            for (const auto& expected_attr : expected_attrs) {
                if (attr == expected_attr) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                std::cerr << "[WARNING] Неожиданный атрибут '" << attr 
                          << "' для операции '" << op_type 
                          << "' в узле '" << node_name << "'" << std::endl;
            }
        }
    }

    static void print_operation_stats(const std::unordered_map<std::string, int>& op_counts) {
        std::cout << "\n=== Статистика операций ===" << std::endl;
        for (const auto& [op, count] : op_counts) {
            std::cout << op << ": " << count << std::endl;
        }
        std::cout << "==========================\n" << std::endl;
    }
};