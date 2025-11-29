import os
import subprocess
 
PROJECT_DIR = os.path.abspath(os.path.dirname(__file__))

FILE_LOCATIONS = [
    os.path.join(PROJECT_DIR, "app"),
    os.path.join(PROJECT_DIR, "core")
]

FILE_EXTENSIONS = [".c", ".h", ".cpp", ".hpp"]


def get_files(input_paths: list[str], extensions: list[str]):
    files = []

    for input_path in input_paths:
        for dirpath, dirnames, filenames in os.walk(input_path):
            for filename in filenames:
                if filename.endswith(tuple(extensions)):
                    files.append(os.path.join(dirpath, filename))
    return files


def format_file(file_path: str):
    print(f" - formatting: {file_path}")

    command = ["clang-format", "-i", file_path]
    return subprocess.run(command, shell=False).returncode


def format_files(file_paths: list[str]):
    for file_path in file_paths:
        format_file(file_path)


def main():
    files = get_files(FILE_LOCATIONS, FILE_EXTENSIONS)

    format_files(files)


if __name__ == "__main__":
    main()