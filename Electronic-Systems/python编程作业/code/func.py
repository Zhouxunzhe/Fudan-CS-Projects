import math
import random
import numpy as np
import matplotlib.patches as patches

from params import *


def create_obstacles():
    r1 = random.uniform(5, 20)
    r2 = random.uniform(5, 20)
    side1 = r1 * np.sqrt(2)
    side2 = r2 * np.sqrt(2)

    x1 = random.uniform(0, wall_dist)
    y1 = random.uniform(dist_from_wall + r1, wall_dist - dist_from_wall - r1)
    # if x1 >= (wall_dist/2), let obstacle2 be on the left side,
    # I also want the left obstacle to be in the first place
    if y1 >= (wall_dist / 2):
        y2 = random.uniform(dist_from_wall + r2, y1 - 2 * r2 - car_width)
    else:
        y2 = random.uniform(y1 + 2 * r2 + car_width, wall_dist - dist_from_wall - r2)
    if x1 >= (wall_dist / 2):
        x2 = random.uniform(0, x1 - 2 * r2 - car_width)
        if random.randint(0, 1) == 0:
            obstacles.append(patches.Circle((x2, y2), r2, color='blue'))
        else:
            obstacles.append(patches.Rectangle((x2 - 0.5 * side2, y2 - 0.5 * side2), side2, side2, color='blue'))
        if random.randint(0, 1) == 0:
            obstacles.append(patches.Circle((x1, y1), r1, color='orange'))
        else:
            obstacles.append(patches.Rectangle((x1 - 0.5 * side1, y1 - 0.5 * side1), side1, side1, color='orange'))
        circles.append((x2, y2, r2))
        circles.append((x1, y1, r1))
    else:
        x2 = random.uniform(x1 + 2 * r2 + car_width, wall_dist)
        if random.randint(0, 1) == 0:
            obstacles.append(patches.Circle((x2, y2), r2, color='blue'))
        else:
            obstacles.append(patches.Rectangle((x2 - 0.5 * side2, y2 - 0.5 * side2), side2, side2, color='blue'))
        if random.randint(0, 1) == 0:
            obstacles.append(patches.Circle((x1, y1), r1, color='orange'))
        else:
            obstacles.append(patches.Rectangle((x1 - 0.5 * side1, y1 - 0.5 * side1), side1, side1, color='orange'))
        circles.append((x1, y1, r1))
        circles.append((x2, y2, r2))


def get_tangent_points(current_pos, obstacle_center, obstacle_radius):
    # calculate distances
    x1, y1 = current_pos
    x2, y2 = obstacle_center
    d = math.sqrt((x2 - x1) ** 2 + (y2 - y1) ** 2)
    L = math.sqrt(d ** 2 - obstacle_radius ** 2)
    # calculate angles
    alpha = math.atan2((y2 - y1), (x2 - x1))
    theta = math.asin(obstacle_radius / d)
    # calculate tangent points
    t1 = (x1 + L * math.cos(alpha + theta), y1 + L * math.sin(alpha + theta))
    t2 = (x1 + L * math.cos(alpha - theta), y1 + L * math.sin(alpha - theta))
    # guarantee t1 always on the top of t2
    if t1[1] > t2[1]:
        t3 = t1
        t1 = t2
        t2 = t3
    return t1, t2


def get_internal_tangent_points(x1, y1, r1, x2, y2, r2):
    d = math.sqrt((x2 - x1) ** 2 + (y2 - y1) ** 2)
    theta = math.atan2(y2 - y1, x2 - x1)
    phi = math.acos((r1 + r2) / d)
    # left circle
    t1 = (x1 + r1 * math.cos(theta + phi), y1 + r1 * math.sin(theta + phi))
    t2 = (x1 + r1 * math.cos(theta - phi), y1 + r1 * math.sin(theta - phi))
    # right circle
    t3 = (x2 - r2 * math.cos(theta + phi), y2 - r2 * math.sin(theta + phi))
    t4 = (x2 - r2 * math.cos(theta - phi), y2 - r2 * math.sin(theta - phi))
    if t1[1] > t2[1]:
        tt = t1
        t1 = t2
        t2 = tt
    if t3[1] > t4[1]:
        tt = t3
        t3 = t4
        t4 = tt
    return t1, t2, t3, t4


def generate_circle_path(start, end, center, radius, direction):
    vec_start = np.array(start) - np.array(center)
    vec_end = np.array(end) - np.array(center)
    angle_start = np.arctan2(vec_start[1], vec_start[0])
    angle_end = np.arctan2(vec_end[1], vec_end[0])

    if direction == 'clockwise':
        if angle_start < angle_end:
            angle_start += 2 * np.pi
        theta = np.linspace(angle_start, angle_end, int(abs(angle_end - angle_start) / (2 * np.pi) * 100))
    else:
        if angle_end < angle_start:
            angle_end += 2 * np.pi
        theta = np.linspace(angle_start, angle_end, int(abs(angle_end - angle_start) / (2 * np.pi) * 150))
    path_x = center[0] + radius * np.cos(theta)
    path_y = center[1] + radius * np.sin(theta)
    path = np.array([path_x[1:-1], path_y[1:-1]]).T
    return [tuple(p) for p in path]


def midpoint(p1, p2):
    return (p1[0] + p2[0]) / 2, (p1[1] + p2[1]) / 2


def toward_target(car_pos, target, speed):
    vec = np.array([target[0], target[1]]) - np.array(car_pos)
    direction_angle = np.arctan2(vec[1], vec[0])
    car_angle = np.degrees(direction_angle)
    if np.linalg.norm(vec) > speed:
        car_pos += speed * vec / np.linalg.norm(vec)
    else:
        car_pos = np.array([target[0], target[1]])
    return car_pos, car_angle
