from datetime import datetime
from fastapi import FastAPI
from pydantic import BaseModel
import uvicorn

app = FastAPI()

class MetaModel(BaseModel):
    version: int
    subversion: int

class DataModel(BaseModel):
    visitors_in: int
    visitors_out: int
    receipts_count: int # Количество чеков.
    total_revenue: float

class DiagnosticModel(BaseModel):
    device_state: str

class Model(BaseModel):
    meta: MetaModel
    device_id: str # ID устройства.
    interval_start: datetime
    interval_end: datetime
    data: DataModel
    diagnostic: DiagnosticModel

@app.post(path="/")
async def receive_data(data: Model):
    try:
        print(f"[{datetime.now()}] Данные от устройства c id {data.device_id} успешно получены: {data.model_dump_json()}")

        return {}

    except Exception as e:
        print(f"Ошибка при получении данных: {e}")
        raise HTTPException(status_code=500, detail=str(e))

if __name__ == "__main__":
    uvicorn.run("test:app", host="127.0.0.1", port=0, reload=True)
