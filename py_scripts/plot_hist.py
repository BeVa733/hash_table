import csv
import sys
import matplotlib.pyplot as plt

x = []
y = []

with open(sys.argv[1], "r", encoding="utf-8") as f:
    reader = csv.DictReader(f)

    for row in reader:
        x.append(int(row["bucket"]))
        y.append(int(row["size"]))

plt.figure(figsize=(16, 6))
plt.bar(x, y, width=1.0)
plt.xlabel("Index")
plt.ylabel("List length")
plt.tight_layout()

out_name = "hist.png"
if len(sys.argv) >= 3:
    out_name = sys.argv[2]

plt.savefig(out_name, dpi=200)