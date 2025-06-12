import math
import numpy as np

def deg(rad):
    return rad * 180 / math.pi

def rad(deg):
    return deg * math.pi / 180

# Размеры звеньев по чертежу
L1 = 0.202    # Высота основания (ось 1)
L2 = 0.160    # Длина плеча (ось 2)
L3 = 0.195    # Длина предплечья (ось 3)
L4 = 0.06715  # Длина до фланца (ось 4-6)

def rotation_matrix_from_euler(roll, pitch, yaw):
    # ZYX (yaw, pitch, roll)
    Rz = np.array([
        [math.cos(yaw), -math.sin(yaw), 0],
        [math.sin(yaw),  math.cos(yaw), 0],
        [0, 0, 1]
    ])
    Ry = np.array([
        [math.cos(pitch), 0, math.sin(pitch)],
        [0, 1, 0],
        [-math.sin(pitch), 0, math.cos(pitch)]
    ])
    Rx = np.array([
        [1, 0, 0],
        [0, math.cos(roll), -math.sin(roll)],
        [0, math.sin(roll), math.cos(roll)]
    ])
    return Rz @ Ry @ Rx

def inverse_kinematics(x, y, z, roll, pitch, yaw):
    # 1. Центр запястья (wrist center)
    R06 = rotation_matrix_from_euler(roll, pitch, yaw)
    d6 = L4
    nx, ny, nz = R06[:, 0]
    wx = x - d6 * nx
    wy = y - d6 * ny
    wz = z - d6 * nz

    # 2. q1: вращение вокруг Z
    q1 = math.atan2(wy, wx)
    q1_deg = deg(q1)
    if not -360 <= q1_deg <= 360:
        raise ValueError("q1 вне диапазона ±360°")

    # 3. q2 и q3: плечевая и локтевая оси (Y)
    r = math.sqrt(wx**2 + wy**2)
    s = wz - L1

    D = (r**2 + s**2 - L2**2 - L3**2) / (2 * L2 * L3)
    if abs(D) > 1:
        raise ValueError("Точка вне досягаемости")

    q3 = math.atan2(math.sqrt(1 - D**2), D)  # локоть вниз
    q3_deg = deg(q3)
    if not -90 <= q3_deg <= 90:
        raise ValueError("q3 вне диапазона ±90°")

    phi = math.atan2(s, r)
    psi = math.atan2(L3 * math.sin(q3), L2 + L3 * math.cos(q3))
    q2 = phi - psi
    q2_deg = deg(q2)
    if not -90 <= q2_deg <= 90:
        raise ValueError("q2 вне диапазона ±90°")

    # 4. Прямая кинематика до 3-й оси
    # DH: theta1 (q1), d1 (L1), a2 (L2), a3 (L3)
    def dh_matrix(theta, d, a, alpha):
        return np.array([
            [math.cos(theta), -math.sin(theta)*math.cos(alpha),  math.sin(theta)*math.sin(alpha), a*math.cos(theta)],
            [math.sin(theta),  math.cos(theta)*math.cos(alpha), -math.cos(theta)*math.sin(alpha), a*math.sin(theta)],
            [0,                math.sin(alpha),                  math.cos(alpha),                 d],
            [0,                0,                                0,                               1]
        ])

    T01 = dh_matrix(q1, L1, 0, math.pi/2)
    T12 = dh_matrix(q2, 0, L2, 0)
    T23 = dh_matrix(q3, 0, L3, 0)
    T03 = T01 @ T12 @ T23
    R03 = T03[:3, :3]

    # 5. Ориентация запястья (оси 4-6)
    R36 = R03.T @ R06

    # q4: вращение вокруг X (оси 3)
    q4 = math.atan2(R36[2,1], R36[2,2])
    q4_deg = deg(q4)
    if not -360 <= q4_deg <= 360:
        raise ValueError("q4 вне диапазона ±360°")

    # q5: вращение вокруг Y (перпендикулярно 4)
    q5 = math.atan2(math.sqrt(R36[2,1]**2 + R36[2,2]**2), R36[2,0])
    q5_deg = deg(q5)
    if not -90 <= q5_deg <= 90:
        raise ValueError("q5 вне диапазона ±90°")

    # q6: вращение вокруг X (оси 5, неограниченно)
    q6 = math.atan2(R36[1,0], -R36[0,0])
    q6_deg = deg(q6)
    # Неограниченно, не проверяем

    return [q1_deg, q2_deg, q3_deg, q4_deg, q5_deg, q6_deg]

# Пример использования:
# Целевая позиция и ориентация (x, y, z, roll, pitch, yaw)
#target_x = 0.2
#target_y = 0.1
# target_z = 0.45
# target_roll = rad(0)
# target_pitch = rad(0)
# target_yaw = rad(0)
#
# try:
#     angles = inverse_kinematics(target_x, target_y, target_z, target_roll, target_pitch, target_yaw)
#     print("Углы сочленений (градусы):")
#     for i, angle in enumerate(angles, 1):
#         print(f"q{i} = {angle:.2f}")
# except ValueError as e:
#     print(f"Ошибка: {e}")
