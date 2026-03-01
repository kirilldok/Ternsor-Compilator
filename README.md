# Tensor Compiler Frontend

Программа реализует фронтенд для загрузки нейронных сетей из формата ONNX во внутренний вычислительный граф.
В отличие от языковых компиляторов (где используется AST), в тензорных компиляторах промежуточным представлением является ориентированный ациклический граф (DAG), где:
Узлы — операции нейронной сети (Conv, Relu, Add...)
Рёбра — тензоры (данные между операциями)

## Структура
- `include/` - Заголовочные файлы (структуры данных)
- `src/` - Исходный код (парсер, визуализатор)
- `tests/` - Юнит-тесты

## Зависимости
Для сборки требуются:
- CMake (>= 3.10)
- Protobuf
- ONNX library

### Установка зависимостей (Ubuntu)
```bash
sudo apt-get install libprotobuf-dev protobuf-compiler
pip install onnx
```

## Инструкция по сборке
### Клонируйте репозиторий
    ```bash
    git clone <URL_ВАШЕГО_РЕПОЗИТОРИЯ>
    cd tensor_frontend
    ```
### Создайте папку сборки
```bash
mkdir build
cd build
```
### Запустите CMake
```bash
cmake ..
```
### Скомпилируйте проект
```bash
make
```

**Будут созданы:**
- `frontend` — основная программа
- `test_graph`, `test_operations`, `test_parser`, `test_visualizer` — тесты

### Сгенерируйте тестовые модели (опционально)
```bash
cd ..
python3 scripts/generate_test_models.py
```

## Инструкция по запуску

### Базовый запуск программы
```bash
cd build
./frontend <путь_к_модели.onnx>
```

### Примеры запуска
```bash
./frontend ../models/simple_add.onnx
./frontend ../models/conv_test.onnx
./frontend ../models/full_network.onnx
``` 
