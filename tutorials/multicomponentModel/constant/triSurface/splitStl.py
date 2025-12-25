def split_stl_by_x(input_stl, output_stl):
    """
    读取 STL 文件，按 x 轴的中间值划分为两个部分，但保留原始结构。
    生成的 STL 文件仍然按原始顺序存储所有三角面片，并按 x 坐标归类到不同的 `solid` 部分。

    参数：
    - input_stl: str, 输入 STL 文件路径
    - output_stl: str, 输出 STL 文件路径
    """

    # 读取 STL 文件
    with open(input_stl, 'r') as f:
        lines = f.readlines()

    facets = []  # 用于存储所有三角面片
    current_facet = []  # 用于存储当前正在解析的三角面片
    solid_name = None  # 存储 STL 文件原始 solid 名称

    # 解析 STL 文件，提取面片数据
    for line in lines:
        stripped = line.strip()

        # 记录 solid 名称
        if stripped.startswith("solid"):
            solid_name = stripped.split(" ", 1)[1] if " " in stripped else "default"
            continue  # 不存入 facet 数据，solid 仅用于标记文件开头

        # 只存储 facet normal、outer loop、vertex、endloop、endfacet 这些相关信息
        if stripped.startswith("facet normal") or stripped.startswith("outer loop") or \
           stripped.startswith("vertex") or stripped.startswith("endloop") or stripped.startswith("endfacet"):
            current_facet.append(stripped)

        # 每当遇到 endfacet，说明当前三角面片已解析完成，存入 facets 列表
        if stripped.startswith("endfacet"):
            facets.append(current_facet)
            current_facet = []  # 清空 current_facet，为下一个三角面片做准备

    # 提取所有 x 坐标
    x_values = []
    for facet in facets:
        for line in facet:
            if line.startswith("vertex"):
                x = float(line.split()[1])  # 获取 x 坐标
                x_values.append(x)

    # 计算 x 轴的中点
    x_mid = (min(x_values) + max(x_values)) / 2  

    # 生成新 STL 文件，保持原始顺序
    with open(output_stl, 'w') as f:
        f.write(f"solid organic\n")  # 开始写入 part1
        for facet in facets:
            x_coords = [float(line.split()[1]) for line in facet if line.startswith("vertex")]
            if all(x <= x_mid for x in x_coords):  # 该面片所有顶点的 x 坐标都小于等于 x_mid
                f.write("\n".join(facet) + "\n")
        f.write("endsolid organic\n\n")

        f.write(f"solid inorganic\n")  # 开始写入 part2
        for facet in facets:
            x_coords = [float(line.split()[1]) for line in facet if line.startswith("vertex")]
            if any(x > x_mid for x in x_coords):  # 该面片至少有一个顶点的 x 坐标大于 x_mid
                f.write("\n".join(facet) + "\n")
        f.write("endsolid inorganic\n")

    print(f"STL 文件已生成: {output_stl}")

# 示例调用
split_stl_by_x("Grainstones.stl", "split_mesh.stl")

