import matplotlib.pyplot as plt

for file in [
    "brute_output.txt",
    "optimized_1_output.txt",
    "optimized_2_output.txt",
    "optimized_3_output.txt",
    "optimized_4_output.txt",
]:
    with open(file, "r") as f:
        lines = f.readlines()

        x = []
        y = []
        for line in lines:
            y.append(float(line.split(" ")[0]))
            x.append(float(line.split(" ")[1]))

        plt.plot(x, y)

plt.xlabel("n")
plt.ylabel("Throughput in GIPS")
plt.title("Throughput vs n")
plt.legend(
    [
        "Brute force (Column-wise)",
        "Brute force (Row-wise)",
        "Brute force (Row-Wise) + Compiler Optimizations",
        "Anti-diagonal",
        "Tiling (with tile size = 4)",
    ]
)

plt.show()
