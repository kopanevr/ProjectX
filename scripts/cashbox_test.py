from datetime import datetime, timezone
import time
import random
import requests

INTERVAL_MINUTES = 0.5

def main():
    try:
        print(f"Скрипт отправки с кассы запущен. Отправка каждые {INTERVAL_MINUTES} минут.")

        id = 0

        while True:
            # 30 секунд
            time.sleep(INTERVAL_MINUTES * 60)

            #
            payload = {
                "receipt_id": str(id),
                "total_revenue": random.randint(100, 5000)}
            }

            id += 1

            response = requests.post(SERVER_URL, json=payload, timeout=10)

            if response.status_code == 200:
                print(f"[{datetime.now()}] Данные от кассы на удаленный сервер успешно отправлены: {response.json()}")
            else:
                print(f"[{datetime.now()}] Ошибка сервера {response.status_code}: {response.text}")

    except Exception as e:
        print(f"Ошибка отправки данных на устройство: {e}")

if __name__ == "__main__":
    main()
