class Student:
    def __init__(self, name):
        self.name = name
        self.grades = []  # 存储每年的成绩

    def add_grade(self, grade):
        """添加一年的成绩"""
        self.grades.append(grade)

    def calculate_gpa(self):
        """计算绩点"""
        return sum(self.grades) / len(self.grades) if self.grades else 0


class Classroom:
    def __init__(self, capacity=100):
        self.students = []
        self.capacity = capacity

    def register_student(self, name):
        """注册学生，如果班级未满"""
        if len(self.students) < self.capacity:
            self.students.append(Student(name))
            return True
        return False

    def add_grade_for_all(self, grades):
        """为所有学生添加年度成绩"""
        for student, grade in zip(self.students, grades):
            student.add_grade(grade)

    def calculate_third_year_gpa_and_recommendation_line(self, portion):
        """计算三年的平均绩点和推免线"""
        gpas = [student.calculate_gpa() for student in self.students if len(student.grades) >= 3]
        recommendation_line = sorted(gpas, reverse=True)[int(len(gpas) * portion)]  # 假设前20%的学生可以推荐
        return sum(gpas) / len(gpas), recommendation_line

    def graduate_students(self):
        """判断学生是否可以毕业"""
        graduates = [student for student in self.students if student.calculate_gpa() >= 2.2]
        return graduates

