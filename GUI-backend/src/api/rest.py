from fastapi import FastAPI, APIRouter, Depends, HTTPException
from state.datastore import TelemetryStore
from typing import List, Dict

api = FastAPI()
router = APIRouter()

store: TelemetryStore = None

@router.get("/telemetry", response_model = List[Dict])
def latest_telemetry(limit: int = 10):
    return store.get_latest(limit)

@router.get("/status", response_model = List[Dict])
def list_status(limit: int = 10):
    return [pkt for pkt in store.get_latest(limit) if pkt.get("type") == "gs_status"]

api.include_router(router)
