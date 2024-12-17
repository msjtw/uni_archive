#!/usr/bin/env python3
from collections import defaultdict, deque
from itertools import pairwise

import matplotlib.pyplot as plt
import networkx as nx
import pandas as pd

# Lp., Czynność, Czas normalny, Czas graniczny, Koszt normalny, Koszt graniczny, skrocenie
data = [
    (1, (1, 2), 4, 3, 2, 5, 1),
    (2, (1, 4), 9, 7, 5, 6, 0),
    (3, (2, 3), 9, 6, 4, 5, 3),
    (4, (2, 5), 8, 6, 3, 6, 0),
    (5, (3, 5), 7, 6, 5, 7, 0),
    (6, (3, 6), 10, 8, 6, 10, 2),
    (7, (4, 6), 6, 4, 2, 4, 0),
    (8, (4, 7), 6, 5, 4, 8, 0),
    (9, (5, 8), 15, 12, 10, 25, 0),
    (10, (6, 7), 12, 9, 7, 15, 3),
    (11, (6, 9), 10, 9, 8, 12, 0),
    (12, (7, 8), 8, 5, 2, 3, 3),
    (13, (8, 9), 9, 7, 6, 8, 2),
    (14, (9, 10), 10, 6, 5, 9, 4),
    (15, (9, 12), 6, 4, 3, 7, 0),
    (16, (10, 11), 12, 9, 8, 12, 3),
    (17, (11, 13), 6, 5, 3, 6, 1),
    (18, (12, 14), 9, 7, 2, 4, 0),
    (19, (13, 15), 11, 8, 6, 9, 3),
    (20, (14, 15), 9, 6, 8, 10, 0),
]
START_NODE = 1
END_NODE = 15

d = pd.DataFrame.from_records(
    data,
    columns=[
        "lp",  # Lp.
        "czynność",  # Czynność
        "tn",  # Czas normalny
        "tgr",  # Czas graniczny
        "kn",  # Koszt normalny
        "kgr",  # Koszt graniczny
        "skrocenie",  # Koszt graniczny
    ],
    index="lp",
)

# Gradient kosztu
d["s"] = (d["kgr"] - d["kn"]) / (d["tn"] - d["tgr"])
# k*
d["k"] = d["kn"]

print(d)

adj = defaultdict(list)
weights = {}
vertices = set()

for u, v in d["czynność"]:
    adj[u].append(v)


# print(adj)


def draw(d: pd.DataFrame) -> None:
    plt.figure(figsize=(15, 15))
    plt.subplots_adjust(left=0, right=1, top=1, bottom=0)  # Remove padding
    g = nx.DiGraph()
    edges_with_minlens = [
        (u, v, {"minlen": w}) for (u, v), w in zip(d["czynność"], d["k"], strict=False)
    ]
    g.add_edges_from(edges_with_minlens)
    pos = nx.planar_layout(g)
    nx.draw_networkx(g, pos, node_color="lightpink")
    edge_labels = {(u, v): w["minlen"] for u, v, w in edges_with_minlens}
    nx.draw_networkx_edge_labels(g, pos, edge_labels=edge_labels)
    plt.show()
    plt.savefig("sem3/BOP/bo.png")


# draw(d)


def dfs(
    adj: dict[int, list],
    visited: set,
    pos: int,
    path: list[int],
    all_paths: list[list[int]],
) -> None:
    if len(path) > 0 and path[-1] == END_NODE:
        all_paths.append(path.copy())
    visited.add(pos)
    for v in adj[pos]:
        if v not in path:
            path.append(v)
            dfs(adj, visited, v, path, all_paths)
            path.pop()


def all_paths(adj: dict[int, list]) -> list[list[int]]:
    paths = []
    dfs(adj, set(), START_NODE, [START_NODE], paths)
    return paths


def time(d: pd.DataFrame, path: list[int]) -> int:
    total = 0
    for a, b in pairwise(path):
        row: pd.DataFrame = d.loc[(a, b) == d["czynność"]]
        t = row["tn"].to_numpy()[0] - row["skrocenie"].to_numpy()[0];
        total += t
    return total


def critical(d: pd.DataFrame) -> list[list[int]]:
    paths = []
    max_time = 0
    for path in all_paths(adj):
        t = time(d, path)
        if t == max_time:
            paths.append(path)
        elif t > max_time:
            paths.clear()
            paths.append(path)
            max_time = t
    return paths


print(time(d, critical(d)[0]))
critical_paths = critical(d)
for path in critical_paths:
    print(path);
    for i in range(0,len(path)-1):
        idx = (path[i], path[i+1])
        gradient = d[d['czynność'] == idx]['s']
        luz = d[d['czynność'] == idx]['tn'] - d[d['czynność'] == idx]['tgr'] - d[d['czynność'] == idx]['skrocenie']
        print(idx,luz.to_string(index=False, header=False), gradient.to_string(index=False, header=False));
    print()


total_cost = 0;
for index, row in d.iterrows():
    if row['tn'] - row['tgr'] == row['skrocenie']:
        total_cost += row['kgr']
    else:
        total_cost += row['kn']
print(total_cost)

datav = [
    (1,0,0,0),
    (2,3,3,0),
    (3,9,9,0),
    (4,9,11,2),
    (5,16,16,0),
    (6,17,17,0),
    (7,26,26,0),
    (8,31,31,0),
    (9,38,38,0),
    (10,44,44,0),
    (11,53,53,0),
    (12,44,48,4),
    (13,58,58,0),
    (14,53,57,4),
    (15,66,66,0),
]

dv = pd.DataFrame.from_records(
    datav,
    columns=[
        "lp",  # Lp.
        "start",
        "koniec",
        "luz"
    ],
    index="lp",
)

for index, row in d.iterrows():
    print(row['czynność'])

for index, row in d.iterrows():
    a = row["czynność"][0]
    b = row["czynność"][1]
    calkowity = dv.loc[b]["koniec"] - dv.loc[a]["start"] - row["tn"] + row["skrocenie"];
    swobodny = dv.loc[b]["start"] - dv.loc[a]["start"] - row["tn"] + row["skrocenie"];
    krytyczny = max(0, dv.loc[b]["start"] - dv.loc[a]["koniec"] - row["tn"] + row["skrocenie"])
    print(row['czynność'], ", całkowity: ", calkowity, ", swobodny: ", swobodny, ", krytyczny:", krytyczny)
