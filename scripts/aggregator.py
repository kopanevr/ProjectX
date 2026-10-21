from datetime import datetime
import time
from pathlib import Path
import json

CASHBOX_LOG_FILE_PATH = Path("/tmp/blueberry_logs/cashbox_raw.blueberry_retail.log")
_LOG_FILE_PATH = Path("/tmp/blueberry_logs/_raw.blueberry_retail.log")

data = []

def read_file(filepath):
    try:
        with open(filepath, "r", encoding="utf-8") as f:
                    f.read()
    except FileNoFoundError:
        time.sleep(0.05) # 50 МС

        try:
            with open(filepath, "r", encoding="utf-8") as f:
                        f.read()
        except FileNoFoundError:


def ():
    with open(CASHBOX_LOG_FILE_PATH, "r", encoding="utf-8") as f:
        for line in f:
            data.append(json.loads(line))
