from core.base import Base
from core.renderer import Renderer
from core.scene import Scene
from core.camera import Camera
from core.mesh import Mesh
from geometry.ellipsoidGeometry import EllipsoidGeometry
from material.surfaceMaterial import SurfaceMaterial
from math import sin, cos, atan
from numpy import array, linalg


class SolarSystem(Base):
    def initialize(self):
        print("Initializing system...")
        self.renderer = Renderer()
        self.scene = Scene()
        self.camera = Camera(aspectRatio=1600/1000, angleOfView=80, far = 100000000)
        self.camera.setPosition([0, 0, 1000])
        self.total_cam_change = 0
        self.size_scale = 0.00000005
        self.time_step = 360*24
        self.au = 149597870700
        self.Bodies = []
        self.create_bodies()

    def create_bodies(self):
        size_scale = self.size_scale

        Sun = Solar_body(695700000 * size_scale, 1, [1, 1, 0], 1988500 * 10 ** 24, "sun")
        Mercury = Solar_body(2440000 * size_scale, 10, [0.91, 0.91, 0.93], 3.302 * 10**23, "mercury")
        Venus = Solar_body(6051893 * size_scale, 10, [1, 0.77, 0.29], 48.685 * 10**23, "venus")
        Earth = Solar_body(6378137 * size_scale, 10, [0.52, 0.81, 0.92], 5.97219 * 10*24, "earth")
        Mars = Solar_body(3396190 * size_scale, 10, [0.68, 0.38, 0.26], 6.4171 * 10**23, "mars")
        Jupiter = Solar_body(71492000 * size_scale, 10, [0.89, 0.86, 0.80], 189818722 * 10**19, "jupiter")
        Saturn = Solar_body(60268000 * size_scale, 10, [0.77, 0.67, 0.43], 5.6834 * 10**26, "saturn")
        Uranus = Solar_body(25559000 * size_scale, 10, [0.93, 0.93, 1], 86.813 * 10**24, "uranus")
        Neptune = Solar_body(24766000 * size_scale, 10, [0.16, 0.56, 0.71], 102.409 * 10**24, "neptune")

        self.bodies = [Sun, Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune]

        raw_positions = [[0.0, 0.0, 0.0],
                         [-3.273900131889179 * 10 ** -2, -3.482172077935440 * 10 ** -2, -4.628601836994165 * 10 ** -1],
                         [-1.409728583742951 * 10 ** -1,  -1.646845639258406 * 10**-3, -7.124728603776552 * 10**-1],
                         [4.886154482621378 * 10 ** -1, -4.670579795392164 * 10**-5, 8.583320062465951 * 10**-1],
                         [5.711618367148127 * 10 ** -1, 1.538680630991916 * 10**-2, 1.402654728435639],
                         [4.894853669148573, -1.125945351805383 * 10**-1, 7.416917884511998 * 10**-1],
                         [8.035770869851211, -2.209659576862326 * 10**-1, -5.683950954839954],
                         [1.348304134939814 * 10 ** 1, -1.215448390539184 * 10**-1, 1.432920906993530 * 10**1],
                         [2.974940081077629 * 10 ** 1, -6.224823621529831 * 10**-1, -3.065696095516215]]

        positions = []
        for p in raw_positions:
            positions.append(float(i * self.au) for i in p)

        velocities = [array([0.0, 0.0, 0.0]),
                      array([2.242127503599859 * 10 ** -2, -2.101347214125362 * 10 ** -3, -5.475121834706604 * 10 ** -4]),
                      array([1.970507463947650 * 10 ** -2, -1.192038822599484 * 10**-3, -4.006821988162164 * 10**-3]),
                      array([-1.523667429405557 * 10 ** -2, 3.209773204930222 * 10**-7, 8.450928584025564 * 10**-3]),
                      array([-1.242982135452064 * 10 ** -2, 4.404684352894445 * 10**-4, 6.468612315815884 * 10**-3]),
                      array([-1.218772412443736 * 10 ** -3, -5.171405263940656 * 10**-6, 7.823760582042706 * 10**-3]),
                      array([2.910407933030931 * 10 ** -3, -1.948911010015389 * 10**-4, 4.551307041246442 * 10**-3]),
                      array([-2.894819782202475 * 10 ** -3, 4.666065282587358 * 10**-5, 2.520104559331918 * 10**-3]),
                      array([3.007448783893552 * 10 ** -4, -7.184046095726917 * 10**-5, 3.150211838092234 * 10**-3])]

        velocities = [((v * self.au)/86400) for v in velocities]
        for e, body in enumerate(self.bodies):
            self.scene.add(body.mesh)
            body.set_initial_position([i for i in positions[e]])
            body.set_velocity(velocities[e])

    def calculate_acceleration_from_gravity(self):
        for body1 in self.bodies:
            rest = list(self.bodies)
            rest.remove(body1)
            force = array([0.0, 0.0, 0.0])
            for body2 in rest:
                p = array(body1.position)
                q = array(body2.position)
                pq = q - p
                pq_unit = pq / linalg.norm(pq)
                force += pq_unit * ((6.6743 * 10 ** -11) * ((body1.mass * body2.mass))) / (linalg.norm(pq))**2

            acceleration = force / body1.mass
            body1.update_position(self.time_step * acceleration, self.time_step)
        for body in self.bodies:
            body.translate()

    def get_inputs(self):
        mc = self.input.get_mouse_pos()
        fl = 1000

        self.camera.rotateY(-atan(mc[0] / fl), localCoord=False)
        self.total_cam_change += -atan(mc[0] / fl)
        self.camera.rotateX(cos(self.total_cam_change) * -atan(mc[1] / fl), localCoord=False)
        self.camera.rotateZ(sin(self.total_cam_change) * atan(mc[1] / fl), localCoord=False)
        if self.input.isMousePressed(4):
            self.camera.translate(0, 0, -50)
        if self.input.isMousePressed(5):
            self.camera.translate(0, 0, 50)
        if self.input.isKeyPressed("z"):
            self.time_step -= 1000
        if self.input.isKeyPressed("x"):
            self.time_step += 1000

    def update(self):
        self.calculate_acceleration_from_gravity()
        self.renderer.render(self.scene, self.camera)
        self.get_inputs()


class Solar_body:
    def __init__(self, radius, sf, color, mass, name):
        self.global_scale = 0.000000001
        self.name = name
        self.radius = radius
        self.mass = mass
        self.sf = sf
        dim = 2 * self.sf * self.radius
        self.position = None
        self.velocity = array([0, 0, 0])
        self.geometry = EllipsoidGeometry(dim, dim, dim)
        self.material = SurfaceMaterial({"useVertexColors": False, "baseColor": color})
        self.mesh = Mesh(self.geometry, self.material)

    def update_position(self, deltav, time):
        self.velocity += deltav.astype("float64")
        self.position += self.velocity * time

    def translate(self, from_stored=False, stored_pos=None):
        new_pos = self.position * self.global_scale
        self.mesh.setPosition(new_pos.tolist())

    def set_initial_position(self, position):
        self.position = position
        self.mesh.setPosition([i * self.global_scale for i in position])

    def set_velocity(self, velocity):
        self.velocity = velocity

SolarSystem(screenSize=[1600, 1000]).run()
