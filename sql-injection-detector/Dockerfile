FROM ubuntu:24.04

# Установка зависимостей
RUN apt-get update && apt-get install -y \
    postgresql postgresql-contrib libpq-dev \
    qt6-base-dev libqt6sql6-psql cmake build-essential \
    xvfb x11vnc novnc fluxbox dbus-x11 \
    wget curl ca-certificates gnupg lsb-release \
    && rm -rf /var/lib/apt/lists/*

# Создание пользователя
RUN useradd -m -s /bin/bash sqli && \
    mkdir -p /app && chown sqli:sqli /app

# Копирование исходников
WORKDIR /app
COPY src/ CMakeLists.txt ./
RUN mkdir -p src bin

# Компиляция SQLi Detector
USER sqli
RUN mkdir build && cd build && \
    cmake .. && make -j$(nproc) && \
    cp detector_real ../bin/ && \
    cd .. && rm -rf build/

# PostgreSQL init
COPY --chown=sqli:sqli database/ ./database/
USER root
RUN service postgresql start && \
    su - postgres -c "createdb sqli_detector" && \
    su - postgres -c "psql -c \"ALTER USER postgres PASSWORD 'password';\"" && \
    su - postgres -c "psql -d sqli_detector -c \"CREATE TABLE IF NOT EXISTS sqli_attacks (id SERIAL PRIMARY KEY, attack_number INTEGER UNIQUE NOT NULL);\""

EXPOSE 5900 6080 5432
CMD service dbus start && service postgresql start && \
    export DISPLAY=:99 && \
    Xvfb :99 -screen 0 1920x1080x24 & \
    fluxbox & \
    x11vnc -display :99 -forever -shared -rfbport 5900 -noxdamage & \
   websockify --web=/usr/share/novnc/ 6080 localhost:5900 &
