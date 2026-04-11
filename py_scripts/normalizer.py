import re
import sys

with open(sys.argv[1], "r", encoding="utf-8") as f:
    text = f.read().lower()

text = re.sub(r"[^a-z']+", " ", text)
text = re.sub(r"\s+", " ", text).strip()

with open(sys.argv[2], "w", encoding="utf-8") as f:
    f.write(text + "\n")