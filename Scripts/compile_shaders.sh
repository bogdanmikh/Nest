#!/bin/bash

# Проверяем, передан ли путь к папке
if [ -z "$1" ]; then
    echo "Ошибка: Укажите путь к папке с шейдерами."
    echo "Пример использования: ./compile_shaders.sh /путь/к/шейдерам"
    exit 1
fi

# Проверяем, существует ли папка
if [ ! -d "$1" ]; then
    echo "Ошибка: Папка '$1' не существует."
    exit 1
fi

# Переходим в указанную папку
cd "$1" || exit

# Проходим по всем файлам в папке
for file in *; do
    # Пропускаем подпапки и не-файлы
    if [ ! -f "$file" ]; then
        continue
    fi

    # Получаем имя файла без пути
    filename=$(basename "$file")

    # Определяем тип шейдера по первой букве
    first_char="${filename:0:1}"

    case "$first_char" in
        v|V)
            # Вершинный шейдер
            output_name="${filename%.*}.spv"
            echo "Компиляция вершинного шейдера: $filename -> $output_name"
            glslc -fshader-stage=vertex "$filename" -o "$output_name"
            ;;
        f|F)
            # Фрагментный шейдер
            output_name="${filename%.*}.spv"
            echo "Компиляция фрагментного шейдера: $filename -> $output_name"
            glslc -fshader-stage=fragment "$filename" -o "$output_name"
            ;;
        *)
            # Пропускаем файлы, не начинающиеся на 'v' или 'f'
            echo "Пропуск файла: $filename (не начинается с 'v' или 'f')"
            ;;
    esac
done

echo "Готово!"