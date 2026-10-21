from datetime import datetime, timezone
from pathlib import Path
import json
import os
from fastapi import FastAPI
from pydantic import BaseModel
import uvicorn

app = FastAPI()

# Путь к файлу.
LOG_FILE_PATH = Path("/tmp/blueberry_logs/cashbox_raw.blueberry_retail.log")
# Путь к временному файлу.
TEMP_LOG_FILE_PATH = Path("/tmp/blueberry_logs/cashbox_raw.blueberry_retail.log.temp")

#
os.makedirs(os.path.dirname(LOG_FILE_PATH), exist_ok=True)

class ReceiptModel(BaseModel):
    receipt_id: str # ID чека.
    total_revenue: float # Сумма чека.

@app.post(path="/receipts")
async def receive_receipt(receipt: ReceiptModel):
    try:
        log_entry = {
            "timestamp": datetime.now(timezone.utc).isoformat(),
            "receipt_id": receipt.receipt_id,
            "total_revenue": receipt.total_revenue
        }

        #
        with open(TEMP_LOG_FILE_PATH, "a", encoding="utf-8") as f:
            f.write(json.dumps(log_entry) + "\n")
        os.replace(TEMP_LOG_FILE_PATH, LOG_FILE_PATH)

        print(f"[{datetime.now()}] Чек с id {receipt.receipt_id} на сумму {receipt.total_revenue} рублей успешно записан!")

        return {"receipt_id": receipt.receipt_id}

    except Exception as e:
        print(f"Ошибка записи чека: {e}")
        raise HTTPException(status_code=500, detail=str(e))

if __name__ == "__main__":
    uvicorn.run("cashbox_receiver:app", host="127.0.0.1", port=0, reload=True, log_level="trace")
