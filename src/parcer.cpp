#include "include/parcer.h"
#include "include/operations.h"
#include <onnx/onnx_pb.h>
#include <fstream>
#include <iostream>
#include <unordered_map>

/**
 * @brief Чтение атрибута из ONNX NodeProto
 * @param attr ONNX атрибут
 * @return Attribute в нашем формате
 */
static Attribute read_attribute(const onnx::AttributeProto& attr) {
    Attribute new_attr;
    new_attr.name = attr.name();

    // Явное преобразование типов атрибутов ONNX в наш variant
    switch (attr.type()) {
        case onnx::AttributeProto::INT:
            new_attr.value = static_cast<int64_t>(attr.i());
            break;
            
        case onnx::AttributeProto::FLOAT:
            new_attr.value = static_cast<float>(attr.f());
            break;
            
        case onnx::AttributeProto::STRING:
            new_attr.value = attr.s();
            break;
            
        case onnx::AttributeProto::INTS: {
            std::vector<int64_t> vals;
            for (int64_t v : attr.ints()) {
                vals.push_back(v);
            }
            new_attr.value = vals;
            break;
        }
        
        case onnx::AttributeProto::FLOATS: {
            std::vector<float> vals;
            for (float v : attr.floats()) {
                vals.push_back(v);
            }
            new_attr.value = vals;
            break;
        }
        
        case onnx::AttributeProto::TENSOR:
            // Тензорные атрибуты обрабатываем отдельно при необходимости
            new_attr.value = std::string("<tensor>");
            break;
            
        default:
            new_attr.value = std::string("<unknown>");
            break;
    }
    
    return new_attr;
}

/**
 * @brief Парсинг атрибутов узла с валидацией
 * @param onnx_node Узел из ONNX
 * @param out_attrs Выходной список атрибутов
 * @param op_type Тип операции (для валидации)
 * @param node_name Имя узла (для логирования)
 */
static void parse_attributes(const onnx::NodeProto& onnx_node, 
                             std::vector<Attribute>& out_attrs,
                             const std::string& op_type,
                             const std::string& node_name) {
    std::vector<std::string> attr_names;
    
    for (const auto& attr : onnx_node.attribute()) {
        Attribute new_attr = read_attribute(attr);
        out_attrs.push_back(new_attr);
        attr_names.push_back(new_attr.name);
        
        // Логирование для отладки
        std::cout << "  [ATTR] " << op_type << " :: " << new_attr.name << std::endl;
    }
    
    // Явная валидация атрибутов для данной операции
    OperationValidator::validate_attributes(op_type, attr_names, node_name);
}

/**
 * @brief Основная функция парсинга ONNX файла
 * @param filepath Путь к .onnx файлу
 * @param out_graph Выходной граф
 * @return true при успехе
 */
bool ONNXParser::parse(const std::string& filepath, Graph& out_graph) {
    std::cout << "[INFO] Загрузка модели: " << filepath << std::endl;
    
    // Открытие файла
    std::ifstream input(filepath, std::ios::binary);
    if (!input.is_open()) {
        std::cerr << "[ERROR] Не удалось открыть файл: " << filepath << std::endl;
        return false;
    }

    // Парсинг Protobuf
    onnx::ModelProto model;
    if (!model.ParseFromIstream(&input)) {
        std::cerr << "[ERROR] Ошибка парсинга ONNX модели" << std::endl;
        return false;
    }
    
    std::cout << "[INFO] Модель загружена успешно" << std::endl;
    std::cout << "[INFO] Имя модели: " << model.graph().name() << std::endl;

    const auto& graph_proto = model.graph();

    // Сохранение входов графа
    std::cout << "\n[INFO] Входы графа:" << std::endl;
    for (const auto& inp : graph_proto.input()) {
        out_graph.inputs.push_back(inp.name());
        std::cout << "  - " << inp.name() << std::endl;
    }

    // Сохранение выходов графа
    std::cout << "\n[INFO] Выходы графа:" << std::endl;
    for (const auto& out : graph_proto.output()) {
        out_graph.outputs.push_back(out.name());
        std::cout << "  - " << out.name() << std::endl;
    }

    // Статистика по операциям
    std::unordered_map<std::string, int> op_counts;
    int supported_count = 0;
    int unsupported_count = 0;

    // Парсинг узлов графа
    std::cout << "\n[INFO] Парсинг узлов:" << std::endl;
    for (const auto& onnx_node : graph_proto.node()) {
        Node node;
        node.name = onnx_node.name().empty() ? "unnamed_" + std::to_string(out_graph.size()): onnx_node.name();
        node.op_type = onnx_node.op_type();

        // === ЯВНАЯ ВАЛИДАЦИЯ ОПЕРАЦИИ ===
        bool is_supported = OperationValidator::validate_operation(node.op_type, node.name);
        if (is_supported) {
            supported_count++;
            op_counts[node.op_type]++;
        } else {
            unsupported_count++;
        }

        // Копирование входов и выходов
        for (const auto& i : onnx_node.input()) {
            node.inputs.push_back(i);
        }
        for (const auto& o : onnx_node.output()) {
            node.outputs.push_back(o);
        }

        parse_attributes(onnx_node, node.attributes, node.op_type, node.name);

        // Добавление узла в граф
        out_graph.add_node(node);
        
        std::cout << "  [NODE] " << node.op_type << " :: " << node.name << std::endl;
    }

    // Вывод статистики
    std::cout << "\n[INFO] Всего узлов: " << out_graph.size() << std::endl;
    std::cout << "[INFO] Поддерживаемых: " << supported_count << std::endl;
    std::cout << "[INFO] Неподдерживаемых: " << unsupported_count << std::endl;
    
    OperationValidator::print_operation_stats(op_counts);

    return true;
}