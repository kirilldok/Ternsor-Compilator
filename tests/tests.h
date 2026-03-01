#pragma once
#include <iostream>
#include <string>
#include <cstdlib>

// Счетчики тестов
static int tests_passed = 0;
static int tests_failed = 0;

// Макрос для проверки условия
#define ASSERT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ \
                      << " - Condition failed: " << #condition << std::endl; \
            tests_failed++; \
            return 1; \
        } else { \
            std::cout << "[PASS] " << __FILE__ << ":" << __LINE__ \
                      << " - " << #condition << std::endl; \
            tests_passed++; \
        } \
    } while(0)

// Макрос для проверки равенства
#define ASSERT_EQ(a, b) \
    do { \
        if ((a) != (b)) { \
            std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ \
                      << " - Expected " << (b) << " but got " << (a) << std::endl; \
            tests_failed++; \
            return 1; \
        } else { \
            std::cout << "[PASS] " << __FILE__ << ":" << __LINE__ \
                      << " - " << #a << " == " << #b << std::endl; \
            tests_passed++; \
        } \
    } while(0)

// Макрос для проверки строк
#define ASSERT_STR_EQ(a, b) \
    do { \
        if ((a) != (b)) { \
            std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ \
                      << " - Expected '" << (b) << "' but got '" << (a) << "'" << std::endl; \
            tests_failed++; \
            return 1; \
        } else { \
            std::cout << "[PASS] " << __FILE__ << ":" << __LINE__ \
                      << " - Strings equal" << std::endl; \
            tests_passed++; \
        } \
    } while(0)

// Макрос для проверки что файл существует
#define ASSERT_FILE_EXISTS(path) \
    do { \
        std::ifstream f(path); \
        if (!f.good()) { \
            std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ \
                      << " - File not found: " << (path) << std::endl; \
            tests_failed++; \
            return 1; \
        } else { \
            std::cout << "[PASS] " << __FILE__ << ":" << __LINE__ \
                      << " - File exists: " << (path) << std::endl; \
            tests_passed++; \
        } \
    } while(0)

// Вывод статистики тестов
#define PRINT_TEST_STATS() \
    do { \
        std::cout << "\n========== ТЕСТЫ ЗАВЕРШЕНЫ ==========" << std::endl; \
        std::cout << "Пройдено: " << tests_passed << std::endl; \
        std::cout << "Провалено: " << tests_failed << std::endl; \
        std::cout << "======================================" << std::endl; \
    } while(0)