from core.base import Base
from core.renderer import Renderer
from core.scene import Scene
from core.camera import Camera
from core.mesh import Mesh
from geometry.ellipsoidGeometry import EllipsoidGeometry
from material.surfaceMaterial import SurfaceMaterial
from math import sin, cos, pi


class Test(Base):
    def initialize(self):
        print("Initializing program...")

        self.renderer = Renderer()
        self.scene = Scene()
        self.camera = Camera(aspectRatio=800/600)
        self.camera.setPosition([1, 2, 2])
        self.camera.translate(0,0,0)
        self.camera.rotateX(-pi/4)
        self.frame_time = 0

        sun_geometry = EllipsoidGeometry()
        sun_material = SurfaceMaterial({"useVertexColors": True, "baseColor": [0.94, 0.7, 0.22]})
        self.sun = Mesh(sun_geometry, sun_material)
        self.sun.translate(1,0,0)
        self.scene.add(self.sun)

        Earth_geometry = EllipsoidGeometry(0.2, 0.2, 0.2, 16, 8)
        Earth_material = SurfaceMaterial({"useVertexColors": True, "baseColor": [0.53, 0.81, 0.92]})
        self.earth = Mesh(Earth_geometry, Earth_material)
        self.scene.add(self.earth)

    def update(self):
        self.frame_time += 0.01
        self.earth.translate(0.01*sin(self.frame_time), 0, 0.01*cos(self.frame_time), localCoord=False)
        self.earth.rotateY(0.01)
        self.renderer.render(self.scene, self.camera)


Test(screenSize=[800, 600]).run()