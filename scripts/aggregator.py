from datetime import datetime
from pathlib import Path
import json

CASHBOX_LOG_FILE_PATH = Path("/tmp/blueberry_logs/cashbox_raw.blueberry_retail.log")
_LOG_FILE_PATH = Path("/tmp/blueberry_logs/_raw.blueberry_retail.log")

data = []

def ():
    with open(CASHBOX_LOG_FILE_PATH, "r", encoding="utf-8") as f:
        for line in f:
            data.append(json.loads(line))
