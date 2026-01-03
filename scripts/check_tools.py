#!/usr/bin/env python3

import sys, json, shutil, subprocess, os

path = f"{os.path.dirname(os.path.abspath(sys.argv[0]))}/tools.json"

# check tools in PATH, use --exit to exit if a tool isn't found, and --check-arch with the arch directory to check if an arch exists

if len(sys.argv) > 3 and sys.argv[1] == "--check-arch":
	if not os.path.isdir(f"{sys.argv[2]}/{sys.argv[3]}"):
		print(f"arch {sys.argv[3]} doesn't exist!")
		sys.exit(1)
	sys.exit(0)

if len(sys.argv) == 2 and sys.argv[1] == "--check-arch":
	print("arch is null!")
	sys.exit(1)

with open(path) as file:
	tools = json.load(file)

missing = []

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
