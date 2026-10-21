from datetime import datetime
import time
from pathlib import Path
import json

CASHBOX_LOG_FILE_PATH = Path("/tmp/blueberry_logs/cashbox_raw.blueberry_retail.log")
CUSTOMER_COUNTER_LOG_FILE_PATH = Path("/tmp/blueberry_logs/customer_counter_raw.blueberry_retail.log")

data = []

def read_file(filepath):
    try:
        with open(filepath, "r", encoding="utf-8") as f:
            return f.read()
    except FileNoFoundError:
        time.sleep(0.05) # 50 МС

        try:
            with open(filepath, "r", encoding="utf-8") as f:
                return f.read()
        except FileNoFoundError:
            print(f"Ошибка чтения файла: {filepath}")
            return ""

def aggregate():
    cashbox_log_raw = read_file(CASHBOX_LOG_FILE_PATH)
    customer_counter_raw = read_file(CUSTOMER_COUNTER_LOG_FILE_PATH)

    if not cashbox_log_raw and not customer_counter_raw:
        return
