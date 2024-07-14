import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import RectBivariateSpline

# 生成一个随机噪声图像
np.random.seed(42)
x = np.linspace(0, 4*np.pi, 10)
y = np.linspace(0, 4*np.pi, 10)
X, Y = np.meshgrid(x, y)
Z = np.random.rand(10, 10)

# 定义插值函数
interp_func = RectBivariateSpline(x, y, Z, kx=3, ky=3)

# 生成插值后的图像网格
x_new = np.linspace(0, 4*np.pi, 100)
y_new = np.linspace(0, 4*np.pi, 100)
Z_interp = interp_func(x_new, y_new)

# 可视化原始图像和插值后的图像
plt.figure(figsize=(10, 5))

plt.subplot(1, 2, 1)
plt.title('Original')
plt.imshow(Z, extent=(x.min(), x.max(), y.min(), y.max()), origin='lower', cmap='viridis')
plt.colorbar()

plt.subplot(1, 2, 2)
plt.title('Interpolated')
plt.imshow(Z_interp, extent=(x_new.min(), x_new.max(), y_new.min(), y_new.max()), origin='lower', cmap='viridis')
plt.colorbar()

plt.tight_layout()
plt.show()
