import matplotlib.pyplot as plt
import numpy as np

data = {
  "1024_CRT_LUPAR_MPAR_1024.csv": {
    "LU_TIME": 0.74466744,
    "MULT_TIME": 0.96411511
  },
  "128_CRT_LUPAR_MPAR_128.csv": {
    "LU_TIME": 0.0043468,
    "MULT_TIME": 0.00224086
  },
  "16_CRT_LUPAR_MPAR_16.csv": {
    "LU_TIME": 0.00047539,
    "MULT_TIME": 0.00017637
  },
  # "2048_CRT_LUPAR_MPAR_2048.csv": {
  #   "LU_TIME": 12.341716868686868,
  #   "MULT_TIME": 16.03794897979798
  # },
  "256_CRT_LUPAR_MPAR_256.csv": {
    "LU_TIME": 0.01698979,
    "MULT_TIME": 0.01167519
  },
  "32_CRT_LUPAR_MPAR_32.csv": {
    "LU_TIME": 0.00078685,
    "MULT_TIME": 0.0002947
  },
  "4_CRT_LUPAR_MPAR_4.csv": {
    "LU_TIME": 0.0001908,
    "MULT_TIME": 7.755e-05
  },
  "512_CRT_LUPAR_MPAR_512.csv": {
    "LU_TIME": 0.1001694,
    "MULT_TIME": 0.08866399
  },
  "64_CRT_LUPAR_MPAR_64.csv": {
    "LU_TIME": 0.00158738,
    "MULT_TIME": 0.00057941
  },
  "8_CRT_LUPAR_MPAR_8.csv": {
    "LU_TIME": 0.00031464,
    "MULT_TIME": 0.00011475
  }
}

# data = {k: v for k, v in data.items() if '_64.csv' in k}
x = list(data.keys())
print(x)
x.sort()


def last_value_tuple(t):
    return int(t.split('_')[0])

species = (
    sorted(x, key=last_value_tuple)
)


penguin_means = {
    'LU_TIME':
        [v['LU_TIME'] for k,v in data.items()]
    ,
    # 'MULT_TIME':
    #     [v['MULT_TIME'] for k,v in data.items()]
    # 'MoE': (
    #     0.202906,
    #     0.324117,
    #     0.511246,
    #     0.026986,
    # ),
}

x = np.arange(len(species))  # the label locations
width = 0.25  # the width of the bars
multiplier = 0

fig, ax = plt.subplots(layout='constrained')

for attribute, measurement in penguin_means.items():
    offset = width * multiplier
    rects = ax.bar(x + offset, measurement, width, label=attribute)
    # ax.bar_label(rects, padding=3)
    multiplier += 1

# Add some text for labels, title and custom x-axis tick labels, etc.
ax.set_ylabel('Seconds')
ax.set_title('Algorithm Performance for 64x64 Matrix')
ax.set_xticks(x + width, species)
ax.legend(loc='upper left', ncols=3)
max_time = max(*[v['MULT_TIME'] for k,v in data.items()], *[v['LU_TIME'] for k,v in data.items()])
ax.set_ylim(
    0,
    max_time + (max_time * .1)
)
plt.xticks(rotation=15, ha="right")
plt.show()
