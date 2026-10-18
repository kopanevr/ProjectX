from datetime import datetime, timezone
import json
import os

# Путь к файлу.
LOG_FILE_PATH = "tmp/blueberry_logs/cashbox_raw.blueberry_retail.log"

#
os.makedirs(os.path.dirname(LOG_FILE_PATH), exist_ok=True)

class ReceiptModel(BaseModel):
    receipt_id: str # ID чека.
    total_amount: float # Сумма чека.

def receive_receipt(receipt: ReceiptModel):
    try:
        log_entry = {
            "timestamp": datetime.now(timezone.utc).isoformat,
            "receipt_id": receipt.receipt_id,
            "total_amount": receipt.total_amount
        }

        #
        with open(LOG_FILE_PATH, "a", encoding="utf-8") as f:
            f.write(json.dumps(log_entry) + "\n")

        print(f"[{datetime.now()}] Чек с id {receipt.receipt_id} на сумму {receipt.total_amount} рублей успешно записан.")

    except Exception as e:
        print(f"Ошибка записи чека: {e}")

if __name__ == "__main__"":
    import


