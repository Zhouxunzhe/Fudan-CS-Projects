from core.matrix import Matrix


class Object3D(object):
    def __init__(self):
        self.transform = Matrix.makeIdentity()
        self.parent = None
        self.children = []

    def add(self, child):
        self.children.append(child)
        child.parent = self

    def remove(self, child):
        self.children.remove(child)
        child.parent = None

    # calculate transformation relative to the root object of the scene graph
    def getWorldMatrix(self):
        if self.parent == None:
            return self.transform
        else:
            return self.parent.getWorldMatrix() @ self.transform

    # return descendants
    def getDescendantList(self):
        descendants = []
        nodesToProcess = [self]

        while len(nodesToProcess) > 0:
            node = nodesToProcess.pop(0)
            descendants.append(node)

            nodesToProcess = node.children + nodesToProcess

        return descendants

    # applying geometric transformations
    def applyMatrix(self, matrix, localCoord=True):
        if localCoord:
            self.transform = self.transform @ matrix
        else:
            self.transform = matrix @ self.transform

    def translate(self, x, y, z, localCoord=True):
        m = Matrix.makeTranslation(x, y, z)
        self.applyMatrix(m, localCoord)

    def rotateX(self, angle, localCoord=True):
        m = Matrix.makeRotationX(angle)
        self.applyMatrix(m, localCoord)

    def rotateY(self, angle, localCoord=True):
        m = Matrix.makeRotationY(angle)
        self.applyMatrix(m, localCoord)

    def rotateZ(self, angle, localCoord=True):
        m = Matrix.makeRotationZ(angle)
        self.applyMatrix(m, localCoord)

    def scale(self, s, localCoord=True):
        m = Matrix.makeScale(s)
        self.applyMatrix(m, localCoord)

    # get/set position of the objection (by changing transform)
    def getPosition(self):
        return[
            self.transform.item((0, 3)),
            self.transform.item((1, 3)),
            self.transform.item((2, 3))
        ]

    def getWorldPosition(self):
        worldTransform = self.getWorldMatrix()
        return[
            worldTransform.item((0, 3)),
            worldTransform.item((1, 3)),
            worldTransform.item((2, 3))
        ]

    def setPosition(self, position):
        self.transform.itemset((0, 3), position[0])
        self.transform.itemset((1, 3), position[1])
        self.transform.itemset((2, 3), position[2])
