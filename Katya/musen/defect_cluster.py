from math import atan2
from sklearn.cluster import AgglomerativeClustering
from sklearn.cluster import DBSCAN
import numpy as np
import argparse
import csv
from pathlib import Path
from tqdm import tqdm
from functools import cmp_to_key
import matplotlib.pyplot as plt
from math import *
from scipy.optimize import curve_fit
from sklearn.metrics import r2_score


pts = 2

def exp_fit(x, a, b):
    return a * np.exp(b*x) 
 
def power_fit(x, a, b):
    return a * np.power(x,b)

def binary_search(arr, elem):
    total = len(arr)
    l = 0
    r = total

    while r - l > 1:
        cur = (l + r) // 2
        if elem < arr[cur]:
            r = cur
        elif elem > arr[cur]:
            l = cur
        else:
            return cur, arr[cur]
    return r, arr[r]


def list_to_str(l: list) -> list[str]:
    for i in range(len(l)):
        l[i] = str(l[i])
    return l


def cmp_format(x, y):
    if x < y:
        return -1
    if x > y:
        return 1
    return 0


def compare_bonds(bond1, bond2):
    x1, y1, z1 = bond1[0], bond1[1], bond1[2]
    x2, y2, z2 = bond2[0], bond2[1], bond2[2]

    if abs(z1 - z2) > 1e-6:
        return cmp_format(z1, z2)

    r1 = (x1 ** 2 + y1 ** 2) ** 0.5
    r2 = (x2 ** 2 + y2 ** 2) ** 0.5

    if abs(r1 - r2) > 1e-6:
        return cmp_format(r1, r2)
    
    phi1 = atan2(y1, x1)
    phi2 = atan2(y2, x2)

    return cmp_format(phi1, phi2)


def make_clusters(
        filename: str, output_dir: str, dist_thr: float, 
        step: float, integral: bool, show: bool, large: bool):
    xlist = []
    ylist = []
    zlist = []
    energylist = []
    deathtlist = []
    num_clusters_to_show = 5

    print(f'Loading {filename}')
    with open(filename, 'r') as f:
        reader = csv.DictReader(f, delimiter=' ')

        for row in reader:
            xlist.append(float(row['BondX,mm']))
            ylist.append(float(row['BondY,mm']))
            zlist.append(float(row['BondZ,mm']))
            # Energy is a new optional column; fall back to 0.0 for old files
            energylist.append(float(row['Energy,J']) if 'Energy,J' in row else 0.0)
            deathtlist.append(float(row['DeathT,s']))

    time_points = []
    with open(str(Path(filename).parent /'time_points.txt'), 'r') as tpf:
        l = tpf.readline().split(' ')[:-1]
        for el in l:
            time_points.append(float(el))

    time_points = np.linspace(time_points[0], time_points[-1], int((time_points[-1] - time_points[0]) / step) + 1)

    data = {tp: [] for tp in time_points} # key: timepoint value: [np.array([x, y, z, energy])]
    for cx, cy, cz, e, t in zip(xlist, ylist, zlist, energylist, deathtlist):
        round_t_idx = min(binary_search(time_points, t)[0], len(time_points) - 1)
        round_t = time_points[round_t_idx]
        data[round_t].append(np.array([cx, cy, cz, e]))

    if integral:
        for idx, t in enumerate(time_points[1:]):
            data[t] += data[time_points[idx]]

    print('Loading finished')

    #for t in tqdm(data, desc='Conversion to numpy', total=len(data)):
    #    data[t] = np.asarray(data[t])
    
    Path(output_dir).mkdir(parents=True, exist_ok=True)

    sep = '\t'
    with open(str(Path(output_dir) / 'statistics.txt'), 'w') as sf:
        sf.write(sep.join([
            'Time (end), s', 'Cluster count', 'Max cluster size',
            'Max cntr x, mm', 'Max cntr y, mm', 'Max cntr z, mm', ' R2_exp', ' R2_pow',
            ' R2_exp_Energy', ' R2_pow_Energy', 'Max cluster Esum', '\n'
            ]))
        for t, bonds in tqdm(data.items(), desc='Clustering', total=len(data)):
            with open(str(Path(output_dir) / 'cluster_{:.6f}.txt').format(t), 'w') as f:
                f.write(sep.join(['X,mm', 'Y,mm', 'Z,mm', 'D,mm', 'N', 'Esum']) + '\n')
                # try-catch is KOSTYL'
                try:
                    #part_len = 90000
                    #num_parts = (len(bonds) + part_len - 1) // part_len
                    part_len = len(bonds)
                    num_parts = 1

                    bonds.sort(key=cmp_to_key(compare_bonds))

                    fig = plt.figure()
                    ax = plt.axes(projection='3d')  
                    ax.set_label(f'Time {t}')
                    
                    max_cluster_len = 0
                    max_cluster_esum = 0.0
                    max_cluster_center_coords = []
                    cluster_cnt = 0
                    is_large = False

                    for part in range(num_parts):
                        if len(bonds) == 0:
                            continue
                        part_bonds = bonds[part * part_len:min((part + 1) * part_len, len(bonds))]
                        # Clustering is performed on coordinates only (XYZ) so that
                        # the DBSCAN result does not depend on the new Energy column.
                        clustering = DBSCAN(
                            eps=dist_thr,
                            min_samples=pts).fit(np.asarray(part_bonds)[:, :3])
                        
                        n_clusters = len(set(clustering.labels_)) - (1 if -1 in clustering.labels_ else 0)
                        clustered = [[] for _ in range(n_clusters)]

                        for label, elem in zip(clustering.labels_, bonds):
                            if label != -1:
                                clustered[label].append(elem)

                        for label in range(len(clustered)):
                            clustered[label] = np.asarray(clustered[label])

                        ordered_labels = sorted(list(range(n_clusters)), 
                                        key=lambda idx: clustered[idx].shape[0])

                        max_cluster = clustered[ordered_labels[-1]]
                        cluster_cnt += n_clusters
                        # Save only XYZ so that max_cluster files keep their format
                        np.savetxt(f'{output_dir}/max_cluster_part{part}_'+'{:5f}.txt'.format(t), max_cluster[:, :3],
                                    header='X\tY\tZ', comments='')
                        
                        ax.set_xlim(-5,5)
                        ax.set_ylim(-5,5)
                        ax.set_zlim(-10, 10)
                        ax.set_box_aspect([1, 1, 2])
                        
                        center_x  = [0] * n_clusters # idx -- label
                        center_y  = [0] * n_clusters
                        center_z  = [0] * n_clusters
                        diameters = [0] * n_clusters

                        for label, elements in enumerate(clustered):
                            # Calculate center of cluster
                            cx = 0
                            cy = 0
                            cz = 0
                            for elem in elements:
                                cx += elem[0]
                                cy += elem[1]
                                cz += elem[2]
                            cx /= len(elements)
                            cy /= len(elements)
                            cz /= len(elements)

                            # Calculate diameter of cluster
                            furthest_element = max(elements, 
                                key=lambda elem: (elem[0] - cx) ** 2 + (elem[1] - cy) ** 2 + (elem[2] - cz) ** 2)
                            d = ((furthest_element[0] - cx) ** 2 + (furthest_element[1] - cy) ** 2 + (furthest_element[2] - cz) ** 2) ** 0.5

                            # Total energy of the bonds that form the cluster
                            esum = float(elements[:, 3].sum())

                            center_x[label]  = cx
                            center_y[label]  = cy
                            center_z[label]  = cz
                            diameters[label] = d
                            f.write(sep.join(list_to_str([cx, cy, cz, d, clustered[label].shape[0], esum])) + '\n')

                            if label in ordered_labels[-num_clusters_to_show:]:
                                #vertices_sorted = clustered_conv_hull[label].vertices.tolist()
                                vertices_sorted = list(range(clustered[label].shape[0]))
                                vertices_sorted.sort(key=cmp_to_key(
                                    lambda idx1, idx2:
                                        compare_bonds(clustered[label][idx1], clustered[label][idx2]))
                                )
                                step = max(1, len(vertices_sorted) // 2000)
                                if step > 1:
                                    is_large = True
                                vertices_sorted = np.asarray(vertices_sorted[::step])
                                ax.scatter3D(clustered[label][vertices_sorted, 0],
                                             clustered[label][vertices_sorted, 1],
                                             clustered[label][vertices_sorted, 2])

                            if label == ordered_labels[-1]:
                                max_cluster_len = len(max_cluster)
                                max_cluster_esum = float(max_cluster[:, 3].sum())
                                max_cluster_center_coords = [
                                    center_x[ordered_labels[-1]],
                                    center_y[ordered_labels[-1]],
                                    center_z[ordered_labels[-1]]
                                ]
                    #Histogram
                    cluster_size = np.asarray ([clustered[label].shape[0] for label in range(n_clusters)])
                    # cluster_size - массив размеров кластеров
                    #max_size = cluster_size.max

                    hist, edges = np.histogram(cluster_size, bins=32,range=(pts-0.5,pts+32.5))
                    bins = np.zeros(len(edges)-1)
                    for k in range (len(edges)-1) :
                        bins[k] = 0.5 * (edges[k]+edges[k+1])

                    p0=(1000, -1)
                    R2_exp = R2_pow = np.nan
                    try:
                        # using the curve_fit() function 
                        args, covar = curve_fit(exp_fit, bins, hist, p0)
                        a1, b1 = args[0], args[1]
                        y_exp = a1 * np.exp(b1* bins)
                        R2_exp = r2_score(hist, y_exp)
                    
                        Args, Covar = curve_fit(power_fit, bins, hist, p0)
                        a2, b2 = Args[0], Args[1]
                        y_pow =  a2 * np.power(bins, b2)                 
                        R2_pow = r2_score(hist, y_pow)
                    except Exception as e :
                            #    R2_exp [i] = 0
                            #    R2_pow [i] = 0
                            print (' approximation impossible : {e}')

                    # Energy distribution over clusters (Esum per cluster)
                    cluster_energy = np.asarray([
                        clustered[label][:, 3].sum() for label in range(n_clusters)])
                    R2_exp_energy = R2_pow_energy = np.nan
                    try:
                        ehist, eedges = np.histogram(cluster_energy, bins=32)
                        ebins = 0.5 * (eedges[:-1] + eedges[1:])
                        # Only non-empty energy bins are meaningful for the fit
                        mask = ehist > 0
                        if mask.sum() >= 2:
                            eargs, ecovar = curve_fit(exp_fit, ebins[mask], ehist[mask], p0)
                            ea1, eb1 = eargs[0], eargs[1]
                            ey_exp = ea1 * np.exp(eb1 * ebins[mask])
                            R2_exp_energy = r2_score(ehist[mask], ey_exp)

                            # power_fit needs strictly positive x
                            pmask = mask & (ebins > 0)
                            # Fit power first to get a positive starting point
                            eArgs, eCovar = curve_fit(power_fit, ebins[pmask], ehist[pmask], p0)
                            ea2, eb2 = eArgs[0], eArgs[1]
                            ey_pow = ea2 * np.power(ebins[pmask], eb2)
                            R2_pow_energy = r2_score(ehist[pmask], ey_pow)
                    except Exception as e:
                        print (' energy approximation impossible : {e}')

                    if show:
                        if not large:
                            plt.show()
                        else: #if large
                            if is_large:
                                plt.show()
                    fig.savefig(f'{output_dir}/img_' + '{:5f}.png'.format(t) )
                    plt.close()
                    sf.write(sep.join(list_to_str([
                        t, cluster_cnt, max_cluster_len,
                        *max_cluster_center_coords, R2_exp, R2_pow,
                        R2_exp_energy, R2_pow_energy, max_cluster_esum, '\n'])))

                except Exception as e:
                    print(f' Exception occured: {e}')
        

def main():
    parser = argparse.ArgumentParser(description='Musen defect clusteriser')
    parser.add_argument('--input', '-i', default='bonds_preprocessed.txt', type=str,
                        help='Preprocessed bond file')
    parser.add_argument('--output', '-o', default='cluster_test', type=str,
                        help='Directory for output with clustered bonds')
    parser.add_argument('--distance', '-d', default=0.2, type=float,
                        help='Distance threshold for clustering')
    parser.add_argument('--step', '-s', default=0.0005, type=float,
                        help='Time interval to build clusters')
    parser.add_argument('--partial', action='store_false',
                        help='Partial storage of bonds')
    parser.add_argument('--show', action='store_true',
                        help='Show plots')
    parser.add_argument('--large', action='store_true',
                        help='Show plots with only large clusters')

    args = parser.parse_args()
    make_clusters(
        args.input, args.output, args.distance,
        args.step, args.partial, args.show, args.large)


if __name__ == '__main__':
    main()
