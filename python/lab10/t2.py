import pandas
import numpy as np
from matplotlib import pyplot as plt

filepath = r"C:\Users\rp-re\OneDrive\Desktop\logs\48Greedy_0100\\"

df = pandas.concat(
    [pandas.read_csv(filepath + f"{i}.txt", index_col=0) for i in range(1, 6)], axis=1
)
mean_df = pandas.DataFrame(
    {col: df[col].mean(axis=1).values for col in df.columns.unique()}
)
min_df = pandas.DataFrame(
    {col: df[col].min(axis=1).values for col in df.columns.unique()}
)
mean_df["optimum"] = 33523
mean_df["greedy solution"] = 37928
mean_df["mean solution"] = mean_df["solution"]
mean_df[["mean solution", "optimum", "greedy solution"]].plot()
print(mean_df["solution"].min(), min_df["solution"].min())
# min_df[["solution"]].plot()
# plt.plot(np.arange(len(min_df)), [699] * len(min_df))
# plt.plot(np.arange(len(min_df)), [864] * len(min_df))
plt.xlabel("Generation")
plt.ylabel("Adaptation")
plt.title("48 cities, have greedy at the start, tournament/wheel=0/100")
plt.show()
