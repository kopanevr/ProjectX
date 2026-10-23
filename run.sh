#! /bin/bash

PID_DIR="pids"
LOGS_DIR="temp/logs"

start_services() {
  echo "=== Запуск системы ==="

  mkdir -p "$PID_DIR" "$LOGS_DIR"

  # 1. Счетчик клиентов.
  if [ -f "core/bin/core" ]; then
    echo "[1/3] Запуск счетчика клиентов..."
    touch "$LOGS_DIR/costumer_counter.log"
    (cd "core/bin" && ./core) > "$LOGS_DIR/costumer_counter.log" &
    echo $! > "$PID_DIR/core.pid"
  else
    echo "Ошибка: Исполняемый файл счетчика клиентов не найден."
    exit 1
  fi

  # 2. Скрипт записи чека.
  if [ -f "scripts/cashbox_receiver.py" ]; then
    echo "[2/3] Запуск скрипта записи чека..."
    touch "$LOGS_DIR/cashbox_receiver.log"
    (cd "scripts" && python3 ./cashbox_receiver.py) > "$LOGS_DIR/cashbox_receiver.log" &
    echo $! > "$PID_DIR/cboxreceiver.pid"
  else
    echo "Ошибка: Файл скрипта записи чека не найден."
    exit 1
  fi

  # 3. Скрипт отправки данных на удаленный сервер.
  if [ -f "scripts/data_sender.py" ]; then
    echo "[3/3] Запуск скрипта отправки данных на удаленный сервер..."
    touch "$LOGS_DIR/data_sender.log"
    (cd "scripts" && python3 ./data_sender.py) > "$LOGS_DIR/data_sender.log" &
    echo $! > "$PID_DIR/dsender.pid"
  else
    echo "Ошибка: Файл скрипта отправки данных на удаленный сервер не найден."
    exit 1
  fi

  echo "Система успешно запущена!"
}

stop_services() {
  echo "=== Остановка системы ==="

  for service in core cboxreceiver dsender; do
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

  for service in core cboxreceiver dsender; do
    if [ -f "$PID_DIR/$service.pid" ]; then
      PID=$(cat "$PID_DIR/$service.pid")
      if kill -0 $PID; then
        echo -e " $service: \033[0;32mРАБОТАЕТ\033[0m (PID: $PID)" # Зелёный.
      else
        echo -e " $service: \033[0;31mУПАЛ\033[0m" # Красный.
      fi
    else
      echo -e " $service: \033[0;33mНЕ ЗАПУЩЕН\033[0m" # Жёлтый.
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
