from OpenGL.GL import *
import numpy


class Attribute(object):
    def __init__(self, dataType, data):
        self.dataType = dataType
        self.data = data
        self.bufferRef = glGenBuffers(1)
        self.uploadData()

    def uploadData(self):
        data = numpy.array(self.data).astype(numpy.float32)
        glBindBuffer(GL_ARRAY_BUFFER, self.bufferRef)
        glBufferData(GL_ARRAY_BUFFER, data.ravel(), GL_STATIC_DRAW)

    def associateVariable(self, programRef, variableName):
        variableRef = glGetAttribLocation(programRef, variableName)
        if variableRef == -1:
            return

        glBindBuffer(GL_ARRAY_BUFFER, self.bufferRef)
        if self.dataType == "int":
            glVertexAttribPointer(variableRef, 1, GL_INT, False, 0, None)
        elif self.dataType == "float":
            glVertexAttribPointer(variableRef, 1, GL_FLOAT, False, 0, None)
        elif self.dataType == "vec2":
            glVertexAttribPointer(variableRef, 2, GL_FLOAT, False, 0, None)
        elif self.dataType == "vec3":
            glVertexAttribPointer(variableRef, 3, GL_FLOAT, False, 0, None)
        elif self.dataType == "vec4":
            glVertexAttribPointer(variableRef, 4, GL_FLOAT, False, 0, None)

        glEnableVertexAttribArray(variableRef)
