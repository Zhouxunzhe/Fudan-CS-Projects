import numpy as np

from school import Student, Classroom
import random

random.seed(42)

# 设置班级人数为150
classroom = Classroom(150)
# 假设注册了140名学生
for i in range(140):
    classroom.register_student(f"Student {i+1}")

# 模拟三年的成绩, 假设每年5门考试
for _ in range(3):
    grades = []
    for _ in range(5):
        grade = [random.uniform(1, 4) for _ in range(150)]  # 随机生成每位学生的年度成绩
        grades.append(grade)
    grades = np.average(grades, axis=0)
    classroom.add_grade_for_all(grades)

# 计算三年的平均绩点和推荐线
avg_gpa, recommendation_line = classroom.calculate_third_year_gpa_and_recommendation_line(0.2)
graduates = classroom.graduate_students()  # 获取可以毕业的学生列表

print("注册总人数: ", len(classroom.students))
print("平均绩点: ", avg_gpa)
print("保研线（假设前20%能保研）: ", recommendation_line)
print("毕业人数: ", len(graduates))
