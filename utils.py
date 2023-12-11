import numpy as np
import cv2 as cv
import serial

LOWER_BLUE = np.array([20, 100, 100])
UPPER_BLUE = np.array([30, 255, 255])
LOWER_RED = np.array([0,50,50])
UPPER_RED = np.array([10,255,255])

NUM_ROWS = 10
NUM_COLS = 10
NODE_SIZE = 50
BACKGROUND_COLOR = (255, 255, 255)
SCREEN_SIZE = (800, 600)
BALL_COLOR = (173, 216, 230)
