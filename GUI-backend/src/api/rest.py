from fastapi import FastAPI, HTTPException
api = FastAPI()
store = None       # will be injected by main.py

@api.get("/telemetry/latest")
def latest():
    if store is None or store.get_latest() is None:
        raise HTTPException(404, "No data yet")
    return store.get_latest()