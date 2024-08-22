import os
import re

# create files by name list

def create_file_by_namelist(file_dir, name_list, suffix):
    for name in name_list:
        file = name+suffix
        open(os.path.join(file_dir, file), 'w').close()
        print("File: " + file)

# add given str to the front of each filename in the given directory


def add_str_to_filename(file_dir, str):
    for filename in os.listdir(file_dir):
        if os.path.isfile(os.path.join(file_dir, filename)):
            new_filename = str + filename
            os.rename(os.path.join(file_dir, filename),
                      os.path.join(file_dir, new_filename))
            print("File: " + new_filename)

# print each filename in given directory into a txt in .py folder
def print_filename_to_txt(file_dir, suffix=".*"):
    with open("build/filename.txt", "a") as f:
        filenames = [filename for filename in os.listdir(file_dir) if os.path.isfile(os.path.join(file_dir, filename)) and re.match(suffix, filename) is not None]
        filenames.sort()
        for filename in filenames:
            print(filename)
            f.write(filename + "\n")

# replace str in filename
def replace_str_in_filename(file_dir, str, target_str):
    for file in os.listdir(file_dir):
        if str in file:
            new_file_name = file.replace(str, target_str)
            os.rename(os.path.join(file_dir, file),
                      os.path.join(file_dir, new_file_name))


file_dir = r"C:\Users\HenryKang\Documents\CODE\RobotPublic\src\rfio"

# add_str_to_filename(file_dir, "fr")
# replace_str_in_filename(file_dir, "frfr", "fr")

print_filename_to_txt(file_dir, r".*\.h")
print_filename_to_txt(file_dir, r".*\.cpp")
# print_filename_to_txt(file_dir, r".*\.h")
# print_filename_to_txt(file_dir,r".*\.h")

# BigIntDirection.h
# Circle.h
# ConvexShape.h
# Direction.h
# Ellipse.h

# create_file_by_namelist(file_dir, ["BigIntDirection", "Circle",
#                         "ShapeBoundingDirections", "Side", "Simplex", "Vector"], ".cpp")
