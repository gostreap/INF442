from scipy.cluster.hierarchy import dendrogram, linkage
import matplotlib.pyplot as plt
import numpy as np


def coord_dendraw():
    lines = open("data/iris.data").readlines()

    data = []
    labels = []
    for line in lines:
        coord = line.split(" ")
        labels.append(coord[-1])
        data.append(list(map(float, coord[:-1])))

    # data = np.array([np.array(coord) for coord in data])
    # plt.scatter(data[:,0], data[:,1])
    # for i, label in enumerate(labels):
    #     plt.annotate(label, (data[i][0], data[i][1]))
    # plt.gca().set_aspect('equal', adjustable='box')
    # plt.show()

    Z = linkage(data, method="single")

    def llf(i):
        return labels[i]

    plt.figure(figsize=(25, 10))
    plt.title('Hierarchical Clustering Dendrogram')
    plt.xlabel('sample index')
    plt.ylabel('distance')
    dendrogram(
        Z,
        leaf_rotation=90.,  # rotates the x axis labels
        leaf_font_size=8.,  # font size for the x axis labels
        leaf_label_func=llf
    )

    plt.show()

def matrix_dendrow():
    lines = open("data/languages.data").readlines()
    n = int(lines[0])
    labels = lines[1:n+1]
    dist = []
    for i in range(n+1, 2*n+1):
        dist.append(list(map(float, lines[i].split("  "))))

    Z = linkage(dist, method="single")
    
    def llf(i):
        return labels[i]

    plt.figure(figsize=(25, 10))
    plt.title('Hierarchical Clustering Dendrogram')
    plt.xlabel('sample index')
    plt.ylabel('distance')
    dendrogram(
        Z,
        leaf_rotation=90.,  # rotates the x axis labels
        leaf_font_size=8.,  # font size for the x axis labels
        leaf_label_func=llf
    )

    plt.show()

matrix_dendrow()