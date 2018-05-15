import os

commands = [
    [
        "g++",
        "src/*.cpp",
        "-o out/ProjectTemplateManager.exe",
        "-std=c++17",
        "-I C:/MinGW/Include/",
        "-lboost_filesystem",
        "-lboost_system",
    ],
]

for command in commands:
    cmd = " ".join(command)
    print(cmd)
    os.system(cmd)
