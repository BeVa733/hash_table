import re
import sys

WORD_SIZE = 32
MAX_WORD_LEN = WORD_SIZE - 1

with open(sys.argv[1], "r", encoding="utf-8") as file:
    text = file.read().lower()

text = re.sub(r"[^a-z']+", " ", text)
words = re.sub(r"\s+", " ", text).strip().split()

with open(sys.argv[2], "wb") as file:
    for word in words:
        data = word.encode("ascii")

        if len(data) > MAX_WORD_LEN:
            raise ValueError(f"word is too long for 32-byte format: {word}")

        file.write(data)
        file.write(b"\0" * (WORD_SIZE - len(data)))
