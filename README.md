## Как запулить задание

# 🚀 Пошаговый процесс работы с системой проверки заданий

## 🔄 Полный workflow от создания кода до мержа PR

### 1. Подготовка окружения участником
```bash
# Клонируем репозиторий
git clone https://github.com/SEIka1/cppgradeup.git
cd cppgradeup

# Создаем ветку для работы
git checkout -b username-taskX

# Создаем папку для задания
mkdir -p members/username/task1/tests

# Создаем основной файл с решением
touch members/username/task1/src.cpp

# Опционально: создаем тесты
touch members/username/task1/tests/test1.cpp
```

### Локальная проверка
```bash
# Форматирование кода
make format

# Сборка и тестирование
make build-members/username/task1
make test-members/username/task1

# Статический анализ
make analyze-members/username/task1
```

### Фиксация изменений
```bash
git add members/username/task1/
git commit -m "Реализация задания 1"
git push origin username-task1
```

### Создание Pull Request

Перейти на GitHub в форк репозитория
Нажать "New Pull Request"
Выбрать ветку с заданием
Заполнить описание:
   Какое задание выполнено
   Особенности реализации
   Какие тесты добавлены


# Правила для участников

-Всегда запускайте make format перед коммитом
-Добавляйте тесты для всех случаев
-Комментируйте сложные места в коде
-Отвечайте на комментарии ревьювера в течение 24 часов

## 🛠 Основные команды

### Сборка и тестирование
| Команда | Действие |
|---------|----------|
| `make all` | Собрать и протестировать все задания |
| `make build` | Собрать все задания (без тестов) |
| `make test` | Запустить тесты для всех заданий |
| `make build-members/user1/task1` | Собрать конкретное задание |
| `make test-members/user1/task1` | Протестировать конкретное задание |

### Дополнительные инструменты
| Команда | Действие |
|---------|----------|
| `make clean` | Удалить все собранные файлы |
| `make format` | Автоформатировать код (clang-format) |
| `make analyze` | Проверить код статическим анализатором |
| `make docs` | Сгенерировать документацию (Doxygen) |
| `make help` | Показать все доступные команды |

## ⚙️ Настройка окружения

### Требования
1. **Компилятор**: GCC ≥11 или Clang ≥12 (поддержка C++20)
2. **Инструменты**:
   ```bash
   # Для Ubuntu/Debian
   sudo apt install build-essential clang-format clang-tidy doxygen
