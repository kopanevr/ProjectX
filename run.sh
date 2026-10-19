#! /bin/bash

BASE_DIR="/"
PID_DIR="$BASE_DIR/pids"
LOGS_DIR="$BASE_DIR/logs"

start_services() {
  echo "=== Запуск системы ==="

  # 1. C++ Ядро.
  if [ -f "$BASE_DIR/bin/core.elf" ]; then
    echo "[1/3] Запуск С++ ядра..."
    cd "$BASE_DIR" && ./bin/core.elf
    echo $! > "$PID_DIR/core.pid"
  else
    echo "Ошибка: Core ELF не найден."
    exit 1
  fi

  TIME=2s

  echo "Ожидание $TIME сек..."
  sleep $TIME

  # 2. Скрипт записи чека.
  if [ -f "BASE_DIR/scripts/cashbox_receiver.py" ]; then
    echo "[2/3] Запуск скрипта записи чека..."
    python3 ./scripts/cashbox_receiver.py
    echo $! > "$PID_DIR/cashbox_receiver.pid"
  else
    echo "Ошибка: Файл скрипта записи чека не найден."
    exit 1
  fi

  # 3. Скрипт агрегатора.
  if [ -f "BASE_DIR/scripts/.py" ]; then
    echo "[3/3] Запуск скрипта агрегатора..."
    python3 ./scripts/.py
    echo $! > "$PID_DIR/.pid"
  else
    echo "Ошибка: Файл скрипта агрегатора не найден."
    exit 1
  fi

  echo "Система успешно запущена!"
}

stop_services() {
  echo "=== Остановка системы ==="

  for service in core llama; do
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

  for service in core llama; do
    if [ -f "$PID_DIR/$service.pid" ]; then
      PID=$(cat "$PID_DIR/$service.pid")
      if kill -0 $PID; then
        echo -e " $service: \033[0;32mРАБОТАЕТ\033[0m (PID: $PID)" # Зелёный
      else
        echo -e " $service: \033[0;31mУПАЛ\033[0m" # Красный
      fi
    else
      echo -e " $service: \033[0;33mНЕ ЗАПУЩЕН\033[0m" # Жёлтый
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
