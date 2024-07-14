from material.basicMaterial import BasicMaterial
from OpenGL.GL import *


class SurfaceMaterial(BasicMaterial):
    def __init__(self, properties={}):
        super().__init__()
        self.settings["drawStyle"] = GL_TRIANGLES
        self.settings["doubleSide"] = False
        self.settings["wireframe"] = False
        self.settings["lineWidth"] = 1
        self.setProperties(properties)