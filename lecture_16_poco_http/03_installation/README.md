# 03. Установка POCO

**Пункт плана:** 3 — установка (brew / apt / vcpkg / из исходников).

## Что показывает раздел

Обзорный материал без кода (сборка не требуется). Способы установки POCO под
разные платформы. После установки CMake находит библиотеку через `find_package`.

## macOS (Homebrew)

```bash
brew install poco
```

После этого `find_package(Poco ...)` находит библиотеку автоматически.

## Ubuntu / Debian

```bash
sudo apt install libpoco-dev
```

## vcpkg

```bash
vcpkg install poco
```

При конфигурации проекта укажите toolchain-файл vcpkg:

```bash
cmake -S . -B build \
  -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake
```

## Из исходников

```bash
git clone https://github.com/pocoproject/poco.git
cd poco && mkdir cmake-build && cd cmake-build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
sudo cmake --install .
```

## Проверка установки

```bash
# Homebrew: посмотреть, где лежат заголовки и библиотеки
brew --prefix poco

# CMake найдёт пакет, если каталог с PocoConfig.cmake виден.
# При нестандартном префиксе подскажите путь:
cmake -S . -B build -DPoco_DIR=$(brew --prefix poco)/lib/cmake/Poco
```

## Плохо → Хорошо

**Плохо** — прописывать пути к заголовкам и `.so`/`.dylib` руками:

```cmake
include_directories(/usr/local/include)
target_link_libraries(server /usr/local/lib/libPocoNet.dylib)  # хрупко, не переносимо
```

**Хорошо** — пусть `find_package` сам подставит пути и зависимости:

```cmake
find_package(Poco REQUIRED COMPONENTS Net)
target_link_libraries(server PRIVATE Poco::Net)
```
