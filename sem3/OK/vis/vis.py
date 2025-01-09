import matplotlib.pyplot as plt
import pandas as pd

data = pd.DataFrame(columns=['x1', 'y1', 'x2', 'y2', 'weight'])

vert = [
    (1, 14),
    (15, 91),
    (22, 20),
    (22, 35),
    (24, 3),
    (27, 42),
    (39, 96),
    (41, 56),
    (72, 27),
    (78, 13)
]

weights = [[] for _ in range(len(vert))]

for i in range(len(vert)):
    weights[i] = input().split()

for i in  range(len(vert)):
    for k in  range(i):
        data.loc[len(data)] = [vert[i][0], vert[i][1], vert[k][0], vert[k][1], float(weights[i][k])]

# Create a figure and axis
fig, ax = plt.subplots()

# Plot each line segment
for _, row in data.iterrows():
    x1, y1, x2, y2, weight = row['x1'], row['y1'], row['x2'], row['y2'], row['weight']
    ax.plot([x1, x2], [y1, y2], linewidth=weight/4, color='blue')  # Adjust color as needed

# Set plot limits and aspect ratio
ax.set_aspect('equal', adjustable='datalim')

# Show the plot
plt.show()
