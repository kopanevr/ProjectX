from datetime import datetime
import time
from pathlib import Path
import json

CASHBOX_LOG_FILE_PATH = Path("/tmp/blueberry_logs/cashbox_raw.blueberry_retail.log")
CUSTOMER_COUNTER_LOG_FILE_PATH = Path("/tmp/blueberry_logs/customer_counter_raw.blueberry_retail.log")

class Aggregator:
    def __init__(self):
        pass

    def read_file(self, filepath, start_index):
        #
        data = []
        current_index = start_index

        try:
            with open(filepath, "r", encoding="utf-8") as f:
                for number, line in enumerate(f, start=start_index):
                    line = line.strip()
                    if line:
                        data.append(json.loads(line))

                    current_index = number + 1

            return current_index, data

        except FileNoFoundError:
            # 50 МС
            time.sleep(0.05)

            try:
                with open(filepath, "r", encoding="utf-8") as f:
                    for number, line in enumerate(f, start=start_index):
                        line = line.strip()
                        if line:
                            data.append(json.loads(line))

                    current_index = number + 1

                    return current_index, data

            except FileNoFoundError:
                print(f"Ошибка чтения файла: {filepath}")
                return current_index, []

    def aggregate(self, interval_start, interval_stop):
        self.interval_start = interval_start
        self.interval_stop = interval_stop

        self.total_revenue = 0.0
        self.receipts_count = 0

        self.visitors_in = 0
        self.visitors_out = 0

        self.cashbox_line_index = 0
        self.customer_counter_line_index = 0

        new_cashbox_log_index, cashbox_log = self.read_file(CASHBOX_LOG_FILE_PATH, self.cashbox_line_index)
        if not cashbox_log:
            return False

        new_customer_counter_index, customer_counter = self.read_file(CUSTOMER_COUNTER_LOG_FILE_PATH, self.customer_counter_line_index)
        if not customer_counter:
            return False

        # Обработка логов чеков.
        for log in cashbox_log:
            if "timestamp" in log:
                log_time = self.decode_datetime(log["timestamp"])

                if self.interval_start <= log_time <= self.interval_stop:
                    self.total_revenue += float(log.get("total_revenue", 0.0))
                    self.receipts_count += 1

        # Обработка логов счетчика клиентов.
        for log in customer_counter:
            if "timestamp" in log:
                log_time = self.decode_datetime(log["timestamp"])

                if self.interval_start <= log_time <= self.interval_stop:
                    self.visitors_in += int(log.get("visitors_in", 0))
                    self.visitors_out += int(log.get("visitors_out", 0))

        self.cashbox_line_index = new_cashbox_log_index
        self.customer_counter_line_index = new_customer_counter_index

        return True

    def decode_datetime(self, date_string): return datetime.fromisoformat(date_string)

    def get_total_revenue(self): return self.total_revenue
    def get_receipts_count(self): return self.receipts_count

    def get_visitors_in(self): return self.visitors_in
    def get_visitors_out(self): return self.visitors_out

    def get_interval_start(self): return self.interval_start
    def get_interval_stop(self): return self.interval_stop
