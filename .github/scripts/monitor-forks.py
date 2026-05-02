#!/usr/bin/env python3
"""
AIC Fork Monitor Script
observing forks of Adaptive Intelligence Circle
"""

import requests
import csv
import os
from datetime import datetime

# Cấu hình
ORG_NAME = "AdaptiveIntelligenceCircle"
GITHUB_TOKEN = os.getenv("GITHUB_TOKEN")  # Khuyến nghị dùng GitHub Secret
HEADERS = {"Authorization": f"token {GITHUB_TOKEN}"} if GITHUB_TOKEN else {}

def get_all_forks():
    url = f"https://api.github.com/orgs/{ORG_NAME}/repos"
    repos = requests.get(url, headers=HEADERS).json()
    
    all_forks = []
    for repo in repos:
        repo_name = repo['name']
        forks_url = f"https://api.github.com/repos/{ORG_NAME}/{repo_name}/forks"
        forks = requests.get(forks_url, headers=HEADERS).json()
        
        for fork in forks:
            all_forks.append({
                'timestamp': datetime.utcnow().isoformat() + 'Z',
                'username': fork['owner']['login'],
                'repo_url': fork['html_url'],
                'fork_date': fork['created_at'],
                'stars': fork['stargazers_count'],
                'last_updated': fork['updated_at'],
                'compliance_status': 'Pending',
                'notes': ''
            })
    return all_forks

def save_to_csv(forks):
    file_path = 'fork-log.csv'
    fieldnames = ['timestamp', 'username', 'repo_url', 'fork_date', 'stars', 'last_updated', 'compliance_status', 'notes']
    
    with open(file_path, 'a', newline='', encoding='utf-8') as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames)
        if os.stat(file_path).st_size == 0:
            writer.writeheader()
        writer.writerows(forks)

if __name__ == "__main__":
    print("Đang quét forks...")
    forks = get_all_forks()
    save_to_csv(forks)
    print(f"Đã ghi nhận {len(forks)} forks.")