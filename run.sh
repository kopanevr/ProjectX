#! /bin/bash

BASE_DIR="/"
PID_DIR="$BASE_DIR/pids"
LOGS_DIR="$BASE_DIR/logs"

start_services() {
  echo "=== Запуск системы ==="

  # 1. LLama.cpp
  if [ -f "/" ]; then
    echo "[1/3] Запуск llama-server..."
    /llama-server -m
    echo $! > "$PID_DIR/llama.pid"
  else
    echo "Ошибка: llama-server не найден"
    exit 1
  fi

  TIME=5s

  echo "Ожидание $TIME сек..."

  sleep $TIME

  # 2. C++ Ядро
  if [ -f "$BASE_DIR/bin/core.elf" ]; then
    echo "[2/3] Запуск С++ ядра..."
    cd "$BASE_DIR" && ./bin/core.elf
    echo $! > "$PID_DIR/core.pid"
  else
    echo "Ошибка: Core ELF не найден"
    exit 1
  fi

  # 3. Веб-интерфейс
  if [ -d "$BASE_DIR/web" ]; then
    echo "[3/3] Запуск Веб-сервера (Порт 80)..."
    echo $! > "$PID_DIR/web.pid"
  else
    echo "Ошибка: Папка web не найдена"
    exit 1
  fi

  echo "Система успешно запущена!"
}

stop_services() {
  echo "=== Остановка системы ==="

  for service in web core llama; do
    if [ -f "$PID_DIR/$service.pid" ]; then
      PID=$(cat "$PID_DIR/$service.pid")
      if kill -0 $PID; then
        echo "Остановка $service (PID: $PID)"
        kill 15 $PID
        sleep 1s
      fi
      rm -f "$PID_DIR/$service.pid"
    fi
  done
}

check_status() {
  echo "=== Статус ==="

  for service in web core llama; do
    if [ -f "$PID_DIR/$service.pid" ]; then
      PID=$(cat "$PID_DIR/$service.pid")
      if kill -0 $PID; then
        echo -e " $service: \033[0m;32mРАБОТАЕТ\033[0m (PID: $PID)"
      else
        echo -e " $service: \033[0m;32mУПАЛ\033[0m"
      fi
      rm -f "$PID_DIR/$service.pid"
    else
      echo -e " $service: \033[0m;33mНЕ ЗАПУЩЕН\033[0m"
    fi
  done
}

case "$1" in
  start)    start_services ;;
  stop)     stop_services ;;
  status)   check_status ;;
  restart)  stop_services && sleep 2s && start_services ;;
  *)        echo "Использование: $0 {start|stop|status|restart}" ;;
esac
