#!/usr/bin/env python3

import json
import zipfile
import os
from pathlib import Path

def create_arduino_package():
    # Read library properties
    with open('library.json') as f:
        data = json.load(f)
    
    # Create Arduino library properties file
    properties = [
        f"name={data['name']}",
        f"version={data['version']}",
        f"author={data['authors'][0]['name']} <{data['authors'][0]['email']}>",
        f"maintainer={data['authors'][0]['name']} <{data['authors'][0]['email']}>",
        f"sentence={data['description']}",
        "paragraph=Provides an easy way to implement WiFi configuration and device setup through a captive portal.",
        "category=Communication",
        "url=" + data['repository']['url'],
        "architectures=esp32",
        "depends=AsyncTCP (>=1.1.1),ESPAsyncWebServer (>=1.2.3),ArduinoJson (>=6.21.3)"
    ]
    
    with open('library.properties', 'w') as f:
        f.write('\n'.join(properties))
    
    # Create Arduino library ZIP
    with zipfile.ZipFile(f"{data['name']}-{data['version']}.zip", 'w') as zf:
        # Add source files
        for file in Path('src').glob('**/*'):
            if file.is_file():
                zf.write(file)
        
        # Add header files
        for file in Path('include').glob('**/*.h'):
            zf.write(file)
        
        # Add examples
        for example in Path('examples').glob('**/*.ino'):
            zf.write(example)
        
        # Add library properties
        zf.write('library.properties')
        zf.write('LICENSE')
        zf.write('README.md')

if __name__ == "__main__":
    create_arduino_package()