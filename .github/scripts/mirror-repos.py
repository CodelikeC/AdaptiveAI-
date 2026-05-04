#!/usr/bin/env python3
"""
AIC Mirror Script - Sync to Codeberg & GitLab
"""

import os
import sys
import subprocess
from datetime import datetime

def run_command(cmd):
    try:
        result = subprocess.run(cmd, shell=True, check=True, capture_output=True, text=True)
        print(result.stdout)
        return True
    except subprocess.CalledProcessError as e:
        print(f"Error: {e.stderr}")
        return False

def mirror_to_remote(remote_name, remote_url):
    print(f"mirror to {remote_name}...")
    commands = [
        f"git remote add {remote_name} {remote_url} || git remote set-url {remote_name} {remote_url}",
        f"git push --mirror {remote_name}"
    ]
    for cmd in commands:
        if not run_command(cmd):
            print(f"Mirror to {remote_name} failed!")
            return False
    print(f"✓ Mirror to {remote_name} successful!")
    return True

if __name__ == "__main__":
    print(f"[{datetime.now()}] Starting mirror AIC repositories...")

    # Codeberg
    codeberg_url = os.getenv("CODEBERG_URL")
    if codeberg_url:
        mirror_to_remote("codeberg", codeberg_url)

    # GitLab
    gitlab_url = os.getenv("GITLAB_URL")
    if gitlab_url:
        mirror_to_remote("gitlab", gitlab_url)

    print("Mirror completed!")