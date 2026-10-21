from datetime import datetime
import time
import requests

from aggregator import Aggregator

SERVER_URL = "http://127.0.0.1:41759"
DEVICE_ID = "0"
INTERVAL_MINUTES = 0.5

def main():
    try:
        print(f"Скрипт отправки данных на удаленный сервер запущен. Отправка каждые {INTERVAL_MINUTES} минут.")

        aggregator = Aggregator()

        while True:
            # 5 минут
            time.sleep(INTERVAL_MINUTES * 60)

            #
            aggregator.aggregate(datetime.now())

            #
            payload = {
                "meta": {
                    "version": 0,
                    "subversion": 0
                },
                "device_id": DEVICE_ID,
                "interval_start": str(),
                "interval_end": str(),
                "data": {
                    "visitors_in": 0,
                    "visitors_out": 0,
                    "receipts_count": aggregator.get_receipts_count(),
                    "total_revenue": aggregator.get_total_revenue()
                },
                "diagnostic": {
                    "device_state": ""
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
