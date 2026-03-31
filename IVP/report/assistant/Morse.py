import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import Normalize
from matplotlib import cm

# 定义给定的函数
def f(x, y):
    return np.sin(2*x + y) + np.cos(-2*y + x)

# 定义a和b的范围
a_range = np.arange(-10, 11)
b_range = np.arange(-10, 11)

# 初始化颜色数组
colors = np.zeros((len(a_range), len(b_range)))

# 根据条件着色
for i, a in enumerate(a_range):
    for j, b in enumerate(b_range):
        if np.cos(a * np.pi) > 0 and np.cos(b * np.pi) > 0:
            colors[i, j] = 0.2  # 浅蓝色
        elif np.cos(a * np.pi) < 0 and np.cos(b * np.pi) < 0:
            colors[i, j] = 0.8  # 深蓝色
        else:
            colors[i, j] = 0.5  # 介于两者之间的深度的蓝色

# 生成坐标点
x_points = []
y_points = []
for a in a_range:
    for b in b_range:
        x_points.append((2*a + b + 1) * np.pi / 5)
        y_points.append((2*a - 4*b + 1) * np.pi / 10)

# 绘制等高线
x = np.linspace(-2*np.pi, 2*np.pi, 100)
y = np.linspace(-2*np.pi, 2*np.pi, 100)
X, Y = np.meshgrid(x, y)
Z = f(X, Y)
plt.contour(X, Y, Z, cmap='viridis')

# 设置相同的x和y轴范围
plt.xlim(-2*np.pi, 2*np.pi)
plt.ylim(-2*np.pi, 2*np.pi)

# 绘制散点图
plt.scatter(x_points, y_points, c=colors.flatten(), cmap='Blues')

# 显示图形
plt.colorbar(label='Color Intensity')
plt.xlabel('x')
plt.ylabel('y')
plt.title('Scatter Plot with Contour Lines')
plt.show()
