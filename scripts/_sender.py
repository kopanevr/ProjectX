from datetime import datetime
import time
import requests

import aggregator

SERVER_URL = "https://127.0.0.1:"
DEVICE_ID = "0"
INTERVAL_MINUTES = 5

def main():
    try:
        print(f"Скрипт отправки данных на удаленный сервер запущен. Отправка каждые {INTERVAL_MINUTES} минут.")

        while True:
            # 5 минут
            time.sleep(INTERVAL_MINUTES * 60)

            #
            payload = {
                "meta": {
                    "version": 0,
                    "subversion": 0
                },
                "device_id": DEVICE_ID,
                "interval_start": str(datetime.now()),
                "interval_end": str(datetime.now()),
                "data": {
                    "visitors_in": 0,
                    "visitors_out": 0,
                    "receipts_count": 0,
                    "total_revenue": 0
                },
                "diagnostic": {
                    "device_state": "OK"
                }
            }

            response = requests.post(SERVER_URL, json=payload, timeout=10)

            if response.status_code == 200:
                print(f"Данные от устройства c id {DEVICE_ID} на удаленный сервер успешно отправлены: {response.json()}")
            else:
                print(f"Ошибка удаленного сервера {response.status_code}: {response.text}")
    except Exception as e:
        print(f"Ошибка отправки данных на сервер: {e}")

if __name__ == "__main__":
    main()
