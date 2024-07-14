from material.material import Material


class BasicMaterial(Material):
    def __init__(self):
        vertexShaderCode = """
        uniform mat4 projectionMatrix;
        uniform mat4 viewMatrix;
        uniform mat4 modelMatrix;
        in vec3 vertexPosition;
        in vec3 vertexColor;
        out vec3 color;
        
        void main()
        {
            gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
            color = vertexColor;
        }
        """

        fragmentShaderCode = """
        uniform vec3 baseColor;
        uniform bool useVertexColors;
        in vec3 color;
        out vec4 fragColor;
        
        void main()
        {
            vec4 tempColor = vec4(baseColor, 1.0);
            
            if ( useVertexColors) 
                tempColor *= vec4(color, 1.0);
                
            fragColor = tempColor;
        }
        """

        super().__init__(vertexShaderCode, fragmentShaderCode)
        self.addUniform("vec3", "baseColor", self.locateUniforms())
        self.addUniform("bool", "useVertexColors", self.locateUniforms())
        self.locateUniforms()