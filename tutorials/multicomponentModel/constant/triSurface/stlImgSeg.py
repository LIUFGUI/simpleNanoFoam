from stl import mesh
import numpy as np
from collections import defaultdict
import random
import argparse

parser = argparse.ArgumentParser()
#parser.add_argument('--img_name', required=True, type=str, help='name of your image')
parser.add_argument('--ratio', required=True, type=float, help='select propotion')
#parser.add_argument('--y_min', required=True, type=int, help='minimum crop y')

opt = parser.parse_args()

input_file = 'Image_meshed.stl'
output_file = 'Image_meshed.stl'
select_ratio = opt.ratio
tolerance = 1e-5 
random.seed(42)

stl_mesh = mesh.Mesh.from_file(input_file)


coords = stl_mesh.vectors.reshape(-1, 3)
rounded = np.round(coords / tolerance) 
unique_keys, inverse_indices = np.unique(rounded, axis=0, return_inverse=True)
vertices = np.array([k * tolerance for k in unique_keys]) 
faces = inverse_indices.reshape(-1, 3)

adj = defaultdict(set)
for i, f in enumerate(faces):
    for other in range(3):
        v1, v2 = sorted((f[other], f[(other + 1) % 3]))
        adj[(v1, v2)].add(i)


visited = set()
components = []
for i in range(len(faces)):
    if i not in visited:
        stack = [i]
        group = []
        while stack:
            current = stack.pop()
            if current in visited:
                continue
            visited.add(current)
            group.append(current)
            f = faces[current]
            for e in [(f[0], f[1]), (f[1], f[2]), (f[2], f[0])]:
                v1, v2 = sorted(e)
                for nb in adj[(v1, v2)]:
                    if nb not in visited:
                        stack.append(nb)
        components.append(group)



centroids = []
for comp in components:
    pts = vertices[faces[comp].flatten()]
    centroid = pts.mean(axis=0)
    centroids.append(centroid)

region_ids = list(range(len(components)))


selected_ids = set(random.sample(region_ids, int(len(region_ids) * select_ratio)))


with open(output_file, 'w') as f:
    f.write("solid organic\n")
    for idx in selected_ids:
        for face_idx in components[idx]:
            n = stl_mesh.normals[face_idx]
            f.write(f"  facet normal {n[0]:.6e} {n[1]:.6e} {n[2]:.6e}\n")
            f.write("    outer loop\n")
            for vertex in stl_mesh.vectors[face_idx]:
                f.write(f"      vertex {vertex[0]:.6e} {vertex[1]:.6e} {vertex[2]:.6e}\n")
            f.write("    endloop\n")
            f.write("  endfacet\n")
    f.write("endsolid organic\n")

    
    f.write("solid inorganic\n")
    for idx in range(len(components)):
        if idx in selected_ids:
            continue
        for face_idx in components[idx]:
            n = stl_mesh.normals[face_idx]
            f.write(f"  facet normal {n[0]:.6e} {n[1]:.6e} {n[2]:.6e}\n")
            f.write("    outer loop\n")
            for vertex in stl_mesh.vectors[face_idx]:
                f.write(f"      vertex {vertex[0]:.6e} {vertex[1]:.6e} {vertex[2]:.6e}\n")
            f.write("    endloop\n")
            f.write("  endfacet\n")
    f.write("endsolid inorganic\n")

