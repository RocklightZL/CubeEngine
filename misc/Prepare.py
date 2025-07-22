import os
import subprocess

def start_program(program_name):
    if os.path.exists(program_name):
        subprocess.Popen(program_name)
        print(f"{program_name} started")
    else:
        print(f"{program_name} not found")

def start_program_with_args(program_name, args):
    if os.path.exists(program_name):
        subprocess.Popen([program_name] + args)
        print(f"{program_name} started")
    else:
        print(f"{program_name} not found")

def open_file(file_path):
    if os.path.exists(file_path):
        os.startfile(file_path)
        print(f"{file_path} opened")
    else:
        print(f"{file_path} not found")

if __name__ == "__main__":
    open_file("D:/mycode/vsProject/CubeEngine/Note/DevelopmentNote.md")
    start_program_with_args("C:/Program Files/Google/Chrome/Application/chrome.exe", ["https://chat.deepseek.com/", "https://www.doubao.com/chat/"])
    open_file("D:/mycode/vsProject/CubeEngine/CubeEngine.sln")