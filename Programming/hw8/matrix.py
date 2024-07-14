import numpy as np
import psutil

memory = psutil.virtual_memory()

# 获取系统总内存大小（以字节为单位）
total_memory = memory.total

# 打印总内存大小（以GB为单位）
print("总内存大小:", total_memory / (1024 ** 3), "GB")

bytes_per_element = 8
max_floats = total_memory / bytes_per_element

max_length_1d = int(max_floats)
max_rows_cols_2d = int(np.sqrt(max_floats))
max_dimension_3d = int(np.cbrt(max_floats))

print("最大一维矩阵长度:", max_length_1d)
print("最大二维矩阵行数和列数:", max_rows_cols_2d)
print("最大三维矩阵维度:", max_dimension_3d)
