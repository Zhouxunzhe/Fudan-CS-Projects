import pygame
import sys
from core.input import Input

class Base(object):
    def __init__(self, screenSize=[512, 512]):
        pygame.init()
        displayFlags = pygame.DOUBLEBUF | pygame.OPENGL
        pygame.display.gl_set_attribute(pygame.GL_MULTISAMPLEBUFFERS, 1)
        pygame.display.gl_set_attribute(pygame.GL_MULTISAMPLESAMPLES, 4)

        self.screen = pygame.display.set_mode(screenSize, displayFlags)
        pygame.display.set_caption("Graphics Window")

        self.running = True
        self.clock = pygame.time.Clock()
        self.input = Input()
        self.time = 0

    def initialize(self):
        pass

    def update(self):
        pass

    def run(self):
        self.initialize()
        file = "music.mp3"
        pygame.mixer.init()
        pygame.mixer.music.load(file)

        while self.running:
            if not pygame.mixer.music.get_busy():
                pygame.mixer.music.play(0)
            self.input.update()
            if self.input.quit:
                self.running = False
            self.deltaTime = self.clock.get_time() / 1000
            self.time += self.deltaTime
            self.update()
            pygame.display.flip()
            self.clock.tick(60)

            if self.input.isKeyPressed("q"):
                break

        pygame.quit()
        sys.exit()