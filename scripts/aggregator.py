from datetime import datetime
import time
from pathlib import Path
import json

CASHBOX_LOG_FILE_PATH = Path("/tmp/blueberry_logs/cashbox_raw.blueberry_retail.log")
CUSTOMER_COUNTER_LOG_FILE_PATH = Path("/tmp/blueberry_logs/customer_counter_raw.blueberry_retail.log")

class Aggregator:
    def __init__(self):

    def read_file(self, filepath):
        #
        data = []
        try:
            with open(filepath, "r", encoding="utf-8") as f:
                for line in f:
                    line = line.strip()
                    if line:
                        data.append(json.loads(line))

        except FileNoFoundError:
            # 50 МС
            time.sleep(0.05)

            try:
                with open(filepath, "r", encoding="utf-8") as f:
                    for line in f:
                        line = line.strip()
                        if line:
                            data.append(json.loads(line))

            except FileNoFoundError:
                print(f"Ошибка чтения файла: {filepath}")
                return []

    def aggregate(self, interval_start, interval_stop):
        self.total_revenue = 0.0
        self.receipts_count = 0

        self.visitors_in = 0
        self.visitors_out = 0

        cashbox_log = read_file(CASHBOX_LOG_FILE_PATH)
        if not cashbox_log:
            return False

        customer_counter = read_file(CUSTOMER_COUNTER_LOG_FILE_PATH)
        if not customer_counter:
            return False

        # Обработка логов чеков.
        for log in cashbox_log:
            if "timestamp" in log:
                log_time = self.decode_datetime(log["timestamp"])

                if interval_start <= log_time <= interval_stop:
                    self.total_revenue += log.get("total_revenue", 0.0)
                    self.receipts_count += 1

        # Обработка логов счетчика клиентов.
        for log in customer_counter:
            if "timestamp" in log:
                log_time = self.decode_datetime(log["timestamp"])

                if interval_start <= log_time <= interval_stop:
                    self.visitors_in += log.get("visitors_in", 0)
                    self.visitors_out += log.get("visitors_out", 0)

        return True

    def decode_datetime(self, date_string): return datetime.fromisoformat(date_string)

    def get_total_revenue(self): return self.total_revenue
    def get_receipts_count(self): return self.get_receipts_count

    def get_visitors_in(self): return self.visitors_in
    def get_visitors_out(self): return self.visitors_out

    def get_interval_start(self): return self.interval_start
    def get_interval_stop(self): return self.interval_stop
