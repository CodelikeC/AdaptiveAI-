import sys
import re

# Simple scoring rules
SCORES = {
    "commit": 1,
    "pull_request": 3,
    "review": 2,
    "documentation": 3,
    "governance": 5
}

def calculate_points(contributions):
    total = 0
    for ctype, count in contributions.items():
        total += SCORES.get(ctype, 0) * count
    return total

def assign_badge(points):
    if points >= 50:
        return "🥇 Gold Contributor"
    elif points >= 20:
        return "🥈 Silver Contributor"
    elif points >= 5:
        return "🥉 Bronze Contributor"
    else:
        return "🌱 New Contributor"

if __name__ == "__main__":
    # Example usage: contributions passed as dict
    # In GitHub Action, replace this with actual data from API or parsed logs
    contributions = {
        "commit": 12,
        "pull_request": 4,
        "review": 6,
        "documentation": 2,
        "governance": 1
    }

    points = calculate_points(contributions)
    badge = assign_badge(points)

    print(f"Total Points: {points}")
    print(f"Assigned Badge: {badge}")
