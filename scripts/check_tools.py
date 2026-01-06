#!/usr/bin/env python3

import sys, json, shutil, subprocess, os

def display_help():
	print(f"""{sys.argv[0]} help:
			--arch: specify architecture to check (example: {sys.argv[0]} --check-arch --arch x86_64)
			--add: specify extra program to check (example: {sys.argv[0]} --cross x86_64-mingw-w64 --exit)
			--exit: exit on tool in tools.json missing (example: {sys.argv[0]} --exit)
			--check-arch: check arch specified by --arch (example: {sys.argv[0]} --check-arch --arch x86_64),
			--no-tools-json: don't use the tools.json file (example: {sys.argv[0]} --no-tools-json --add gcc)
	""")

path = f"{os.path.dirname(os.path.abspath(sys.argv[0]))}/tools.json"

# check tools in PATH, use --exit to exit if a tool isn't found, and --check-arch with the arch directory to check if an arch exists

added_tools = []
no_tools_json = False

for i in range(len(sys.argv)):
	if sys.argv[i] == "--add":
		added_tools.append(sys.argv[i + 1])
	
	if sys.argv[i] == "--no-tools-json":
		no_tools_json = True	

if len(sys.argv) > 3 and sys.argv[1] == "--check-arch":
	if not os.path.isdir(f"{sys.argv[2]}/{sys.argv[3]}"):
		print(f"arch {sys.argv[3]} doesn't exist!")
		sys.exit(1)
	sys.exit(0)

if len(sys.argv) == 2 and sys.argv[1] == "--check-arch":
	print("arch is null!")
	display_help()
	sys.exit(1)

if len(sys.argv) < 2:
	print("incorrect usage:")
	display_help()
	sys.exit(1)

with open(path) as file:
	tools = json.load(file)

missing = []

for added_tool in added_tools:
	path = shutil.which(added_tool)
	print(f"checking for {added_tool}...", end=" ")
		
	if path:
		if info.get("check_version") != None:
			version = subprocess.check_output([path, info.get("check_version")], stderr=subprocess.DEVNULL)
			version = version.splitlines()[0]
		print(f"found (version {version})") if version else print(f"found")
		version = None
	else:
		print("not found")
		if len(sys.argv) > 1 and sys.argv[1] == "--exit":
			print(f"couldn't find {added_tool}; run scripts/check_tools.py for more info")
			sys.exit(1)
		missing.append(added_tool)

if no_tools_json:
	sys.exit(0)

for tool, info in tools.items():
	path = shutil.which(tool)
	print(f"checking for {tool}...", end=" ")
		
	if path:
		if info.get("check_version") != None:
			version = subprocess.check_output([path, info.get("check_version")], stderr=subprocess.DEVNULL)
			version = version.splitlines()[0]
		print(f"found (version {version})") if version else print(f"found")
		version = None
	else:
		if len(sys.argv) > 1 and sys.argv[1] == "--exit":
			print(f"couldn't find {tool}; run scripts/check_tools.py for more info")
			sys.exit(1)
		missing.append(tool)

if len(sys.argv) > 1 and sys.argv[1] == "--exit":
	sys.exit(0)

print("missing tools: ", end=" ")
if len(missing) == 0:
	print("none", end=" ")
print("\n")

for i in range(len(missing)):
	print(missing[i])
