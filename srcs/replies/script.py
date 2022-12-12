import re

with open("replies.hpp","r") as f:
    li = f.readlines()

for line in li:
    num = re.findall('\d{3}', line)
    name = re.findall(r'\bE\w+', line)
    print(f"\tcase {''.join(name)}:\n\t\t return")
