import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from func import *

# random.seed(42)


targets = []
create_obstacles()
car_pos = np.array([left_boundary, wall_dist / 2])
x1, y1, r1 = circles[0]
x2, y2, r2 = circles[1]
t1, t2 = get_tangent_points(car_pos, (x1, y1), r1 + car_width + margin)
t3, t4, t5, t6 = get_internal_tangent_points(x1, y1, r1 + car_width + margin, x2, y2, r2 + margin)

first_round = 0
delete_line = 0


def generate_path():
    global targets, first_round, delete_line
    if t1[0] > t3[0]:
        targets.append(t3)
    else:
        targets.append(t1)
        targets.extend(generate_circle_path(t1, t3, (x1, y1), r1 + car_width + margin, 'counter'))
        delete_line = len(targets) + 1

    targets.append(t6)
    targets.extend(generate_circle_path(t6, t5, (x2, y2), r2 + margin, 'clockwise'))
    targets.append(t4)
    targets.extend(generate_circle_path(t4, t3, (x1, y1), r1 + car_width + margin, 'counter'))


def update(frame):
    global car_pos, targets, first_round
    speed = 2

    if (car_pos[0], car_pos[1]) == targets[0]:
        target = targets.pop(0)
        first_round += 1
        if first_round >= delete_line:
            targets.append(target)
    car_pos, car_angle = toward_target(car_pos, targets[0], speed)
    car.set_x(car_pos[0])
    car.set_y(car_pos[1])
    car.angle = car_angle

    return car,


if __name__ == '__main__':
    fig, ax = plt.subplots()
    ax.set_xlim(left_boundary + car_length, right_boundary - car_length)
    ax.set_ylim(0, wall_dist)
    # add obstacles
    for obstacle in obstacles:
        ax.add_patch(obstacle)
    # add the car
    car = patches.Rectangle((car_pos[0], car_pos[1]), car_length, car_width, color='red', angle=0)
    ax.add_patch(car)
    # add the walls
    ax.plot([left_boundary, right_boundary], [wall_dist, wall_dist], 'k-')  # Top wall
    ax.plot([left_boundary, right_boundary], [0, 0], 'k-')  # Bottom wall
    ax.plot([left_boundary, right_boundary], [wall_dist - dist_from_wall, wall_dist - dist_from_wall], 'k--')  # Top dash
    ax.plot([left_boundary, right_boundary], [dist_from_wall, dist_from_wall], 'k--')  # Bottom dash
    # plot the animation
    generate_path()
    ani = FuncAnimation(fig, update, interval=40)
    plt.axis('equal')
    plt.show()
