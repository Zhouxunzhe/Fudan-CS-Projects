from geometry.parametricGeometry import ParametricGeometry
from math import sin, cos, pi


class EllipsoidGeometry(ParametricGeometry):
    def __init__(self, width=1, height=1, depth=1, radiusSegments =32, heightSegments=16):

        def S(u,v):
            return [
                width/2 * sin(u) * cos(v),
                height/2 * sin(v),
                depth/2 * cos(u) * cos(v)
            ]

        super().__init__(0, 2*pi, radiusSegments, -pi/2, pi/2, heightSegments, S)