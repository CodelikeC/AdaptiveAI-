import os, json, hashlib, time, shutil

BASE_PATH = "data/"
VERSION_DIR = os.path.join(BASE_PATH, "versioning")

def compute_hash(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        while chunk := f.read(8192):
            h.update(chunk)
    return h.hexdigest()

def create_snapshot():
    ts = time.strftime("%Y%m%d_%H%M%S")
    snap_path = os.path.join(VERSION_DIR, f"snapshot_{ts}")
    os.makedirs(snap_path, exist_ok=True)

    manifest = {"timestamp": ts, "files": []}

    for sub in ["raw", "processed"]:
        src_dir = os.path.join(BASE_PATH, sub)
        for root, _, files in os.walk(src_dir):
            for f in files:
                full_path = os.path.join(root, f)
                file_hash = compute_hash(full_path)
                rel_path = os.path.relpath(full_path, BASE_PATH)
                dest_path = os.path.join(snap_path, rel_path)
                os.makedirs(os.path.dirname(dest_path), exist_ok=True)
                shutil.copy2(full_path, dest_path)
                manifest["files"].append({"path": rel_path, "hash": file_hash})
    with open(os.path.join(snap_path, "manifest.json"), "w") as mf:
        json.dump(manifest, mf, indent=2)

    index_path = os.path.join(VERSION_DIR, "index.json")
    index = json.load(open(index_path)) if os.path.exists(index_path) else {"snapshots": []}
    index["snapshots"].append({"id": ts, "path": snap_path})
    json.dump(index, open(index_path, "w"), indent=2)

    print(f"[+] Snapshot created: {snap_path}")

if __name__ == "__main__":
    create_snapshot()