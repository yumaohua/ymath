import os
import matplotlib.pyplot as plt
import numpy as np

def process_txt_file(filepath):
    """处理txt文件，按每4行一组，计算每组的三个数"""
    with open(filepath, 'r') as file:
        lines = file.readlines()

    # 提取每行的第一个数字
    data = [float(line.split()[0]) for line in lines]

    # 计算每组的三个数
    num_groups = len(data) // 4
    calculated_values = [[], [], []]  # 三条曲线的数据

    for i in range(num_groups):
        group = data[i*4:(i+1)*4]
        if len(group) < 4:
            continue  # 确保每组有4个数

        for j in range(3):
            val = group[j] / group[j+1]
            calculated_values[j].append(np.log10(val))

    return calculated_values

def plot_values(values, output_path, start_x, title):
    """绘制三个数的曲线图"""
    x_values = [start_x + i for i in range(len(values[0]))]
    labels = ['1000-10000', '10000-100000', '100000-1000000']

    plt.figure(figsize=(10, 6))
    for i in range(3):
        plt.plot(x_values, values[i], marker='o', label=labels[i])

    plt.xlabel('Group Number')
    plt.ylabel('Calculated Value')
    plt.title(title)
    plt.legend()
    plt.grid(True)

    # 保存图表
    plt.savefig(output_path)
    plt.close()

def plotorder(input_dir, output_dir, start_x=1):
    """主函数，处理目录中的所有txt文件并绘图"""
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    for filename in os.listdir(input_dir):
        if filename.endswith('.txt'):
            filepath = os.path.join(input_dir, filename)
            values = process_txt_file(filepath)

            output_path = os.path.join(output_dir, f"{os.path.splitext(filename)[0]}.png")
            plot_values(values, output_path, start_x, os.path.splitext(filename)[0])
            print(f"Processed and plotted {filename}")


