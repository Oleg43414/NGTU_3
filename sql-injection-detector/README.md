# SQLi Detector - Реальный SQL-инъекция Сканер

**PostgreSQL + Qt6 GUI + Docker + LIVE HTTP атаки на testphp.vulnweb.com**

[![GUI Demo](https://via.placeholder.com/800x450/e74c3c/ffffff?text=SQLi+Detector+v5.0)](https://testphp.vulnweb.com)

## ✨ ФУНКЦИИ

- ✅ **6 реальных SQLi payloads** (BOOLEAN, UNION, DESTRUCTIVE, TIME-BASED)
- ✅ **LIVE HTTP сканирование** (QNetworkAccessManager)
- ✅ **PostgreSQL база** (sqli_attacks: 1258+ записей)
- ✅ **GUI таблица 20x8** (QTableWidget + QTimer 1s)
- ✅ **CSV экспорт** (sqli_attacks.csv)
- ✅ **Docker поддержка** (noVNC веб-интерфейс)

## 📁 ЧИСТАЯ СТРУКТУРА ПРОЕКТА (7 файлов)

SQL-INJECTION-DETECTOR/
├── bin/ # ✅ detector_real (2.5MB ELF)
├── src/ # ✅ main_real.cpp (389 строк C++)
├── CMakeLists.txt # ✅ Qt6 Sql Network AUTOMOC
├── detector_real.desktop # ✅ Ярлык меню + Рабочий стол
├── docker-compose.yml # ✅ PostgreSQL + Detector
├── Dockerfile # ✅ Ubuntu 24.04 Qt6
└── README.md # 📖 Ты это читаешь!

## 🚀 БЫСТРЫЙ СТАРТ (5 минут)

### 1. УСТАНОВКА ЗАВИСИМОСТЕЙ
sudo apt update && sudo apt install -y
postgresql postgresql-contrib libpq-dev
libqt6sql6-psql qt6-base-dev cmake build-essential


### 2. НАСТРОЙКА PostgreSQL
sudo -u postgres psql << EOF
CREATE DATABASE sqli_detector;
ALTER USER postgres PASSWORD 'password';
GRANT ALL PRIVILEGES ON DATABASE sqli_detector TO postgres;
\q
EOF

### 3. КОМПИЛЯЦИЯ
cd ~/sql-injection-detector
mkdir build && cd build
cmake .. && make -j$(nproc)
cp detector_real ../bin/
chmod +x ../bin/detector_real
cd .. && rm -rf build/

## СБОРКА
cd ~/sql-injection-detector/build
make clean && make -j$(nproc)
cp detector_real ../bin/detector_real

### 4. ЗАПУСК
cd bin && ./detector_real

### 5. ЯРЛЫК НА РАБОЧИЙ СТОЛ
cp detector_real.desktop ~/Рабочий\ стол/
chmod +x ~/Рабочий\ стол/detector_real.desktop
gio set ~/Рабочий\ стол/detector_real.desktop "metadata::trusted" yes

## 🐳 DOCKER (1 команда)
Запуск PostgreSQL + Detector
docker-compose up -d

## ОЧИСТКА СMAKE
cd ~/sql-injection-detector
rm -rf build/ CMakeCache.txt CMakeFiles/
mkdir -p build && cd build
cmake .. && make -j$(nproc)

http://testphp.vulnweb.com/