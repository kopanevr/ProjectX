import requests
import time

SERVER_URL = "http://"
DEVICE_ID = ""
INTERVAL_MINUTES = 5

def main():
    print(f"Скрипт  запущен. Отправка каждые {INTERVAL_MINUTES} минут.")

    while True:
        # 5 минут
        time.sleep(INTERVAL_MINUTES * 60)

        # JSON
        payload = {
            "device_id": "",
            "interval_start": "",
            "interval_end": "",
            "data": {
                "visitors_in": ,
                "visitors_out": ,
                "receipts_count": ,
                "total_revenue": ,
            },
            "diagnostic": {
                "state":
            }
        }

        response = post(SERVER_URL, json=payload, timeout=10)

        if response.status_code == 200:
            print(f"Данные успешно отправлены: {response.json()}")
        else:
            print(f"Ошибка сервера {response.status_code}: {response.text}")

}