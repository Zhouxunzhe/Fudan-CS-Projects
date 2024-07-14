from core.Object3D import Object3D
from OpenGL.GL import *


class Mesh(Object3D):
    def __init__(self, geometry, material):
        super().__init__()

        self.geometry = geometry
        self.material = material
        self.visible = True

        # set up associations between attributes stored in geometry and shader program stored in material
        self.vaoRef = glGenVertexArrays(1)
        glBindVertexArray(self.vaoRef)

        for variableName, attributeObject in geometry.attributes.items():
            attributeObject.associateVariable(material.programRef, variableName)

        # unbind vertex
        glBindVertexArray(0)
