#!/usr/bin/env python3
"""
Create a report fork every months. 
"""

import csv
from collections import defaultdict
from datetime import datetime

def generate_report():
    violations = []
    with open('fork-log.csv', 'r', encoding='utf-8') as f:
        reader = csv.DictReader(f)
        for row in reader:
            if row['compliance_status'] in ['Violation', 'Under Review']:
                violations.append(row)
    
    with open('COMPLIANCE-VIOLATIONS.md', 'w', encoding='utf-8') as f:
        f.write("# Compliance Violations Report\n\n")
        f.write(f"**Generated**: {datetime.now().strftime('%Y-%m-%d')}\n\n")
        f.write("## Danh sách vi phạm\n\n")
        
        if not violations:
            f.write("Hiện không có vi phạm nào.\n")
        else:
            for v in violations:
                f.write(f"- **{v['username']}** → {v['repo_url']}  \n")
                f.write(f"  Status: {v['compliance_status']} | Notes: {v['notes']}\n\n")

if __name__ == "__main__":
    generate_report()
    print("Báo cáo đã được tạo.")