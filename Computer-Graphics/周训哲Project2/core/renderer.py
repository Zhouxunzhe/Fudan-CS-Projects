from OpenGL.GL import *
from core.mesh import Mesh


class Renderer(object):
    def __init__(self, clearColor=[0, 0, 0]):
        glEnable(GL_DEPTH_TEST)
        glEnable(GL_MULTISAMPLE)
        glClearColor(clearColor[0], clearColor[1], clearColor[2], 1)
        glEnable(GL_LIGHTING)
        glLight(GL_LIGHT0, GL_POSITION, [0, 0, 0])
        glLight(GL_LIGHT0, GL_DIFFUSE, [1, 1, 1])
        glLight(GL_LIGHT0, GL_AMBIENT, [1, 1, 1])
        glEnable(GL_LIGHT0)

    def render(self, scene, camera):
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

        glEnable(GL_LIGHTING)
        glLight(GL_LIGHT0, GL_POSITION, [0, 0, 0])
        glLight(GL_LIGHT0, GL_DIFFUSE, [1, 1, 1])
        glLight(GL_LIGHT0, GL_AMBIENT, [1, 1, 1])
        glEnable(GL_LIGHT0)

        camera.updateViewMatrix()

        descendantList = scene.getDescendantList()
        meshFilter = lambda x : isinstance(x, Mesh)
        meshList = list(filter(meshFilter, descendantList))

        for mesh in meshList:
            if not mesh.visible:
                continue

            glUseProgram(mesh.material.programRef)
            glBindVertexArray(mesh.vaoRef)

            mesh.material.uniforms["modelMatrix"].data = mesh.getWorldMatrix()
            mesh.material.uniforms["viewMatrix"].data = camera.viewMatrix
            mesh.material.uniforms["projectionMatrix"].data = camera.projectionMatrix

            for variableName, uniformObject in mesh.material.uniforms.items():
                uniformObject.uploadData()

            mesh.material.updateRenderSettings()

            glDrawArrays(mesh.material.settings["drawStyle"], 0, mesh.geometry.vertexCount)



