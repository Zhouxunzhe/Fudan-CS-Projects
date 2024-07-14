import numpy as np
import pygame
from pygame.locals import *
from librosa import *


def clamp(min_value, max_value, value):
    if value < min_value:
        return min_value
    if value > max_value:
        return max_value
    return value


class AudioBar:
    def __init__(self, x, y, freq, color, width=30, min_height=5, max_height=200, min_decibel=-80, max_decibel=0):
        self.x, self.y, self.freq = x, y, freq
        self.color = color
        self.width, self.min_height, self.max_height = width, min_height, max_height
        self.height = min_height
        self.min_decibel, self.max_decibel = min_decibel, max_decibel
        self.__decibel_height_ratio = (self.max_height - self.min_height)/(self.max_decibel - self.min_decibel)

    def update(self, dt, decibel):
        desired_height = decibel * self.__decibel_height_ratio + self.max_height
        speed = (desired_height - self.height)/0.1
        self.height += speed * dt
        self.height = clamp(self.min_height, self.max_height, self.height)

    def render(self, screen):
        pygame.draw.rect(screen, self.color, (self.x, self.y + 50 - self.height, self.width, self.height))


filename = "music.wav"
time_series, sample_rate = load(filename)

stft = np.abs(stft(time_series, hop_length=512, n_fft=2048*4))
spectrogram = amplitude_to_db(stft, ref=np.max)
frequencies = fft_frequencies(n_fft=2048*4)

times = frames_to_time(np.arange(spectrogram.shape[1]), sr=sample_rate, hop_length=512, n_fft=2048*4)
time_index_ratio = len(times)/times[len(times) - 1]
frequencies_index_ratio = len(frequencies)/frequencies[len(frequencies)-1]


def get_decibel(target_time, freq):
    return spectrogram[int(freq * frequencies_index_ratio)][int(target_time * time_index_ratio)]


pygame.init()
infoObject = pygame.display.Info()
screen_w = int(421)
screen_h = int(249)

screen = pygame.display.set_mode([screen_w, screen_h])
background = pygame.image.load(r"pic.png")

bars = []
frequencies = np.arange(75, 8000, 75)
r = len(frequencies)
width = screen_w/r
x = (screen_w - width*r)/2

for c in frequencies:
    bars.append(AudioBar(x, 250, c, (255, 255, 255), max_height=300, width=width))
    x += width

t = pygame.time.get_ticks()
getTicksLastFrame = t
pygame.mixer.music.load(filename)
pygame.mixer.music.play(0)

def get_time():
    seconds = max(0, pygame.mixer.music.get_pos()/1000)
    m, s = divmod(seconds, 60)
    h, m = divmod(m, 60)
    hms = ('%02d:%02d:%02d' % (h, m, s))
    return hms

def controller(key):
    global status
    if status == 'stopped':
        if key == K_RETURN:
            pygame.mixer.music.play()
            status = 'playing'
    elif status == 'paused':
        if key == K_RETURN:
            pygame.mixer.music.stop()
            status = 'stopped'
        elif key == K_SPACE:
            pygame.mixer.music.unpause()
            status = 'playing'
    elif status == 'playing':
        if key == K_RETURN:
            pygame.mixer.music.stop()
            status = 'stopped'
        elif key == K_SPACE:
            pygame.mixer.music.pause()
            status = 'paused'

def get_color(cnt, full):
    if (cnt < full / 3):
        red = 255
        green = 255 * 3 * cnt / full
        blue = 0
    elif (cnt < full / 2):
        red = (750-cnt * (250 * 6 / full))
        green = 255
        blue = 0
    elif (cnt < full * 2 / 3):
        red = 0
        green = 255
        blue = (cnt * (250 * 6 / full)-750)
    elif (cnt < full * 5 / 6):
        red = 0
        green = (1250-cnt * (250 * 6 / full))
        blue = 255
    else:
        red = (150 * cnt * (6 / full)-750)
        green = 0
        blue = 255
    return red, blue, green

running = True
status = "playing"
red = 255
green = 0
blue = 0
full = 10000
reverse = True
cnt = full
while running:
    t = pygame.time.get_ticks()
    deltaTime = (t - getTicksLastFrame) / 1000.0
    getTicksLastFrame = t

    size = screen_w, screen_h
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == KEYDOWN:
            controller(event.key)
        elif event.type == pygame.VIDEORESIZE:
            size = width, height = event.size[0], event.size[1]
            screen = pygame.display.set_mode(size, pygame.RESIZABLE)
    if reverse:
        cnt -= 1
    else:
        cnt += 1
    if cnt == 0 and reverse:
        cnt += 1
        reverse = False
    if cnt == full:
        cnt -= 1
        reverse = True

    for b in bars:
        b.color = get_color(cnt, full)
    name = pygame.font.SysFont('consolas', 20).render(filename, True, (255, 255, 255))
    info = pygame.font.SysFont('consolas', 20).render(status.upper() + ' ' + get_time(), True, (255, 255, 255))
    screen.blit(pygame.transform.scale(background, size), (0, 0))
    screen.blit(name, (0, 0))
    screen.blit(info, (0, 18))

    for b in bars:
        b.update(deltaTime, get_decibel(pygame.mixer.music.get_pos()/1000.0, b.freq))
        b.render(screen)

    pygame.display.flip()

pygame.quit()