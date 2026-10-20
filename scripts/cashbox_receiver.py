from datetime import datetime, timezone
import json
import os
from fastapi import FastAPI
from pydantic import BaseModel
import uvicorn

app = FastAPI()

# Путь к файлу.
LOG_FILE_PATH = "tmp/blueberry_logs/cashbox_raw.blueberry_retail.log"

#
os.makedirs(os.path.dirname(LOG_FILE_PATH), exist_ok=True)

class ReceiptModel(BaseModel):
    receipt_id: str # ID чека.
    total_amount: float # Сумма чека.

@app.post(path="/receipts")
async def receive_receipt(receipt: ReceiptModel):
    try:
        log_entry = {
            "timestamp": datetime.now(timezone.utc).isoformat(),
            "receipt_id": receipt.receipt_id,
            "total_amount": receipt.total_amount
        }

        #
        with open(LOG_FILE_PATH, "a", encoding="utf-8") as f:
            f.write(json.dumps(log_entry) + "\n")

        print(f"[{datetime.now()}] Чек с id {receipt.receipt_id} на сумму {receipt.total_amount} рублей успешно записан!")

        return {"receipt_id": receipt.receipt_id}

    except Exception as e:
        print(f"Ошибка записи чека: {e}")
        raise HTTPException(status_code=500, detail={str{e}})

if __name__ == "__main__"":
    uvicorn.run("cashbox_receiver:app", host="0.0.0.0", port=, reload=True)
