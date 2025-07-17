from fastapi import FastAPI, APIRouter, Depends, HTTPException
from state.datastore import TelemetryStore

api = FastAPI()
router = APIRouter()

store: TelemetryStore = None

@router.get("/telemetry", summary = "Latest Telemetry Data")
def latest_telemetry(limit: int = 10):
    return store.get_latest()

api.include_router(router)
