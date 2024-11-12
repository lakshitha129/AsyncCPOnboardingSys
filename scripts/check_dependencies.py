#!/usr/bin/env python3

import json
import requests
import semantic_version
from pathlib import Path

def check_dependency(repo, current_version):
    try:
        # Remove ^ or ~ from version string
        clean_version = current_version.replace('^', '').replace('~', '')
        
        # Get latest release from GitHub
        api_url = f"https://api.github.com/repos/{repo}/releases/latest"
        response = requests.get(api_url)
        latest = response.json()['tag_name'].replace('v', '')
        
        current = semantic_version.Version(clean_version)
        latest = semantic_version.Version(latest)
        
        if latest > current:
            print(f"Update available for {repo}: {current} -> {latest}")
            return True
    except Exception as e:
        print(f"Error checking {repo}: {e}")
    return False

def main():
    library_json = Path('library.json')
    with open(library_json) as f:
        data = json.load(f)
    
    updates_available = False
    
    for dep, version in data['dependencies'].items():
        if check_dependency(dep, version):
            updates_available = True
    
    if updates_available:
        print("\nUpdates are available for some dependencies.")
        print("Please update library.json and test thoroughly before releasing.")
    else:
        print("\nAll dependencies are up to date!")

if __name__ == "__main__":
    main()