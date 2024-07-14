import pygame

class Input(object):

    def __init__(self):
        self.quit = False

        self.keyDownList = []
        self.keyPressedList = []
        self.keyUpList = []
        self.mousePressedList = []

        pygame.mouse.set_visible(False)
        pygame.event.set_grab(True)

    def update(self):
        self.keyDownList = []
        self.keyUpList = []
        self.mousePressedList = []

        for event in pygame.event.get():
            if event.type == pygame.MOUSEBUTTONDOWN:
                mouseName = event.button
                self.mousePressedList.append(mouseName)
            if event.type == pygame.KEYDOWN:
                keyName = pygame.key.name(event.key)
                self.keyDownList.append(keyName)
                self.keyPressedList.append(keyName)
            if event.type == pygame.KEYUP:
                keyName = pygame.key.name(event.key)
                self.keyPressedList.remove(keyName)
                self.keyUpList.append(keyName)
            if event.type == pygame.QUIT:
                self.quit = True

    def isKeyDown(self, keyCode):
        return keyCode in self.keyDownList

    def isKeyPressed(self, keyCode):
        return keyCode in self.keyPressedList

    def isKeyUp(self, keyCode):
        return keyCode in self.keyUpList

    def isMousePressed(self, keyCode):
        return keyCode in self.mousePressedList

    def get_mouse_pos(self):
        return pygame.mouse.get_rel()

