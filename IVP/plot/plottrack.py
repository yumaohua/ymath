import os
import re
import matplotlib.pyplot as plt
from pathlib import Path
import numpy as np

def is_deepest_dir(directory):
    """检查目录是否是最深层目录，即没有子目录"""
    for entry in os.scandir(directory):
        if entry.is_dir():
            return False
    return True

def find_deepest_dirs(root_dir):
    """找到所有最深层目录"""
    deepest_dirs = []
    for dirpath, dirnames, filenames in os.walk(root_dir):
        # 打印当前目录路径
        print(f"Entering directory: {dirpath}")
        if is_deepest_dir(dirpath):
            print(f"Deepest directory found: {dirpath}")
            deepest_dirs.append(dirpath)
        else:
            print(f"Skipping non-deepest directory: {dirpath}")
    return deepest_dirs

def plot_txt_files_in_directory(directory, output_dir, num_pic=4, aspect_ratio=1.0):
    """绘制最深层目录中的txt文件并保存图片"""
    txt_files = [file for file in os.listdir(directory) if file.endswith('.txt')]

    if not txt_files:
        print(f"No txt files found in directory: {directory}")
        return

    num_files = len(txt_files)
    num_cols = min(num_pic, num_files)
    num_rows = (num_files + num_cols - 1) // num_cols

    fig, axs = plt.subplots(num_rows, num_cols, figsize=(15, 15 * aspect_ratio / num_cols * num_rows))
    fig.subplots_adjust(hspace=0.5)

    for i, filename in enumerate(txt_files):
        with open(os.path.join(directory, filename), 'r') as file:
            data = [list(map(float, line.strip().split())) for line in file.readlines()]

        x_values = [row[0] for row in data]
        y_values = [row[1] for row in data]

        match = re.match(r'(.+?)\((.+?)\)\.txt', filename)
        if match:
            title_line1 = match.group(1)
            title_line2 = f"({match.group(2)})"
        else:
            title_line1 = filename
            title_line2 = ""

        row_index = i // num_cols
        col_index = i % num_cols

        # 检查axs是否是单个Axes对象还是数组
        if np.ndim(axs) == 0:
            ax = axs
        elif num_rows > 1:
            ax = axs[row_index, col_index]
        else:
            ax = axs[col_index]

        ax.plot(x_values, y_values)
        ax.set_title(f"{title_line1}\n{title_line2}")

    # 创建输出目录（如果不存在）
    os.makedirs(output_dir, exist_ok=True)

    # 将文件路径转换为合法的文件名
    relative_path = os.path.relpath(directory, start='.')
    if relative_path.startswith('./'):
        relative_path = relative_path[2:]
    file_name = relative_path.replace(os.sep, '_').replace(':', '')

    # 保存图片到输出目录
    plt.savefig(os.path.join(output_dir, f"{file_name}.png"), bbox_inches='tight')
    print(f"Saved plot to {os.path.join(output_dir, f'{file_name}.png')}")
    plt.close()

def plottrack(root_dir, output_dir , num_pic = 4, aspect_ratio=0.25):
    """主函数，遍历所有最深层目录并绘图"""
    deepest_dirs = find_deepest_dirs(root_dir)
    if not deepest_dirs:
        print("No deepest directories found.")
    for directory in deepest_dirs:
        plot_txt_files_in_directory(directory, output_dir, num_pic, aspect_ratio)
