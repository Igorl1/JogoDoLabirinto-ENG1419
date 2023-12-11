import sys
import cv2 as cv
import time as t
import numpy as np
import math
import serial
from utils import (
    LOWER_BLUE,
    UPPER_BLUE,
    LOWER_RED,
    UPPER_RED,
    )
from threading import Thread, Timer

checkpoints = []
SERIAL = serial.Serial("COM9", 115200, timeout=1)
t.sleep(1)
ball_coords = []
flag = False

def send_coords():
    timer = Timer(1, send_coords)
    timer.start()

    if ball_coords != []:
        coords = f"b'{ball_coords[0]},{ball_coords[1]}'"
        SERIAL.write(coords.encode())

def send_checkpoint_coords():
    global flag
    if len(checkpoints) == 2 and flag == False:
        coords = f"c'{checkpoints[0][0]},{checkpoints[0][1]}','{checkpoints[1][0]},{checkpoints[1][1]}'"
        SERIAL.write(coords.encode())
        flag = True

def checkpoint_reached():
    timer = Timer(1, checkpoint_reached)
    timer.start()
    for checkpoint in checkpoints:
        if checkpoint[0] == ball_coords[0] and checkpoint[1] == ball_coords[1]:
            reached = f"e'1'"
            SERIAL.write(reached.encode())

send_coords()

checkpoint_reached()

def main():
    
    # create buffer surface
    cap = cv.VideoCapture(0)

    running = True
    lrcontour = None
    lbcontour = None
    coordinates = []

    _, frame = cap.read()
    
    gray_frame = cv.cvtColor(frame.copy(), cv.COLOR_BGR2GRAY)
    blur = cv.GaussianBlur(gray_frame, (5, 5), 1)
    thresholded_frame = cv.adaptiveThreshold(blur, 255, cv.ADAPTIVE_THRESH_GAUSSIAN_C, cv.THRESH_BINARY, 11, 2)
    thresholded_frame = cv.bitwise_not(thresholded_frame, thresholded_frame)
    contours, _ = cv.findContours(thresholded_frame, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
    larea = 0
    if len(contours) != 0:
        for contour in contours:
            aux = cv.contourArea(contour)
            if aux > larea:
                larea = aux
                lcontour = contour
        x, y, w, h = cv.boundingRect(lcontour)
        cv.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
        edges = np.array([
            [x, y],
            [x + w, y],
            [x + w, y + h],
            [x, y + h]
        ], dtype=np.float32)

        # Calculate Manhattan distance between corresponding vertices
        
        for edge in edges:
            min_dist = float("inf")
            for coord in lcontour[:, 0, :]:
                distance = np.abs(edge[0] - coord[0]) + np.abs(edge[1] - coord[1])
                if distance < min_dist:
                    min_dist = distance
                    nearest = coord
            coordinates.append(nearest)
    src = np.array(coordinates, dtype=np.float32).reshape(-1, 1, 2)
    dst = np.array([[0, 0], [frame.shape[1] - 1, 0], [frame.shape[1] - 1, frame.shape[0] - 1], [0, frame.shape[0] - 1]], dtype=np.float32).reshape(-1, 1, 2)
    homography_M, _ = cv.findHomography(src, dst, cv.RANSAC, 5.0)
    warped_frame = cv.warpPerspective(frame, homography_M, (frame.shape[1], frame.shape[0]))
    
    while running:
        global ball_coords
        global checkpoints
        _, frame = cap.read()
        warped_frame = cv.warpPerspective(frame, homography_M, (frame.shape[1], frame.shape[0]))
        NODE_SIZE = math.dist([0, 0], [0, warped_frame.shape[1] - 1]) / 7
        hsv_frame = cv.cvtColor(warped_frame.copy(), cv.COLOR_BGR2HSV)
        blue_mask = cv.inRange(hsv_frame, LOWER_BLUE, UPPER_BLUE)
        _, thresholded_blue = cv.threshold(blue_mask, 127, 255, cv.THRESH_BINARY)
        blue_contours, _ = cv.findContours(thresholded_blue, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
        
        red_mask = cv.inRange(hsv_frame, LOWER_RED, UPPER_RED)
        _, thresholded_red = cv.threshold(red_mask, 127, 255, cv.THRESH_BINARY)
        red_contours, _ = cv.findContours(thresholded_red, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
        lrarea = 0
        if len(red_contours) != 0:  # Iterate through the contours and find the centroid of each object
            for contour in red_contours:
                aux = cv.contourArea(contour)
                if aux > 100:
                    x, y, w, h = cv.boundingRect(contour)
                    cv.rectangle(warped_frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
                    M = cv.moments(contour)
                    if M["m00"] != 0:
                        # Calculate the centroid coordinates
                        cX = M["m10"] / M["m00"]
                        cY = M["m01"] / M["m00"]
                        i = int((cY / NODE_SIZE) + 1) + 1
                        j = int((cX / NODE_SIZE) + 1)
                        ball_coords = [i, j]
                        if len(checkpoints) != 2:
                            checkpoints.append([i, j])
                send_checkpoint_coords()
        
        lbarea = 0
        if len(blue_contours) != 0:
            for contour in blue_contours:
                aux = cv.contourArea(contour)
                if aux > lbarea:
                    lbarea = aux
                    lbcontour = contour
            M = cv.moments(lbcontour)
            if M["m00"] != 0:
                cX = M["m10"] / M["m00"]
                cY = M["m01"] / M["m00"]
                i = int(cY / NODE_SIZE) + 1
                j = int(cX / NODE_SIZE) + 1
                ball_coords = [i, j]
                x, y, w, h = cv.boundingRect(lbcontour)
                cv.rectangle(warped_frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
        cv.imshow('Object Tracking', warped_frame)
        if cv.waitKey(1) & 0xFF == ord('q'):
            running = False
    cap.release()
    cv.destroyAllWindows()
    sys.exit()

main()
