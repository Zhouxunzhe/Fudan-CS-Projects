from OpenGL.GL import *


class OpenGLUtils(object):
    @staticmethod
    def initializeShader(shaderCode, shaderType):
        shaderCode = '#version 330\n' + shaderCode
        shaderRef = glCreateShader(shaderType)
        glShaderSource(shaderRef, shaderCode)
        glCompileShader(shaderRef)
        compileSuccess = glGetShaderiv(shaderRef, GL_COMPILE_STATUS)

        if not compileSuccess:
            errorMessage = glGetShaderInfoLog(shaderRef)
            glDeleteShader(shaderRef)
            errorMessage = "\n" + errorMessage.decode("utf-8")
            raise Exception(errorMessage)

        return shaderRef

    @staticmethod
    def initializeProgram(vertexShaderCode, fragmentShaderCode):
        vertexShaderRef = OpenGLUtils.initializeShader(vertexShaderCode, GL_VERTEX_SHADER)
        fragmentShaderRef = OpenGLUtils.initializeShader(fragmentShaderCode, GL_FRAGMENT_SHADER)
        programRef = glCreateProgram()

        glAttachShader(programRef, vertexShaderRef)
        glAttachShader(programRef, fragmentShaderRef)
        glLinkProgram(programRef)
        linkSuccess = glGetProgramiv(programRef, GL_LINK_STATUS)

        if not linkSuccess:
            errorMessage = glGetProgramInfoLog(programRef)
            glDeleteProgram(programRef)
            errorMessage = "\n" + errorMessage.decode("utf-8")
            raise Exception(errorMessage)

        return programRef

    @staticmethod
    def printSystemInfo():
        print(" Vendor: " + glGetString(GL_VENDOR).decode("utf-8"))
        print(" Renderer: " + glGetString(GL_RENDERER).decode("utf-8"))
        print(" OpenGL version supported: " + glGetString(GL_VERSION).decode("utf-8"))
        print(" GLSL version supported: " + glGetString(GL_SHADING_LANGUAGE_VERSION).decode("utf-8"))
