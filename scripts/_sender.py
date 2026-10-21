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
        device_state_checker = Device_state_checker()

        while True:
            interval_start = datetime.now()

            # 5 минут
            time.sleep(INTERVAL_MINUTES * 60)

            #
            aggregator.aggregate(interval_start, datetime.now())

            #
            payload = {
                "meta": {
                    "version": 0,
                    "subversion": 0
                },
                "device_id": DEVICE_ID,
                "interval_start": str(aggregator.get_interval_start()),
                "interval_end": str(aggregator.get_interval_stop()),
                "data": {
                    "visitors_in": aggregator.get_visitors_in(),
                    "visitors_out": aggregator.get_visitors_out(),
                    "receipts_count": aggregator.get_receipts_count(),
                    "total_revenue": aggregator.get_total_revenue()
                },
                "diagnostic": {
                    "device_state": device_state_checker.get_current_state()
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
