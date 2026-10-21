from datetime import datetime
import time
from pathlib import Path
import json

CASHBOX_LOG_FILE_PATH = Path("/tmp/blueberry_logs/cashbox_raw.blueberry_retail.log")
CUSTOMER_COUNTER_LOG_FILE_PATH = Path("/tmp/blueberry_logs/customer_counter_raw.blueberry_retail.log")


class Aggregator:
    def __init__(self):

    def read_file(filepath):
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

    def aggregate(datetime):
        self.total_revenue = 0

        cashbox_log_raw = read_file(CASHBOX_LOG_FILE_PATH)
        customer_counter_raw = read_file(CUSTOMER_COUNTER_LOG_FILE_PATH)

        if not cashbox_log_raw and not customer_counter_raw:
            return

        for item in cashbox_log_raw:
            item[]


    def get_total_revenue():
        return self.total_revenue

    def get_receipts_count():
        return self.get_receipts_count

    def get_visitors_in():
        return self.visitors_in

    def get_visitors_out():
        return self.visitors_out
