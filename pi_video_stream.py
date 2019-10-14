from picamera.array import PiRGBArray
from picamera import PiCamera
from threading import Thread
import cv2

class PiVideoStream:
    def __init__(self, resolution=(640,480), framerate=20, saturation=-30, brightness=40, exposure_mode='off', awb_mode='incandescent'):
        self.camera = PiCamera()

        self.camera.resolution = resolution
        self.camera.framerate = framerate
        self.camera.saturation = saturation
        self.camera.brightness = brightness
        self.camera.exposure_mode = exposure_mode
        self.camera.awb_mode = awb_mode

        self.rawCapture = PiRGBArray(self.camera, size=resolution)
        self.stream = self.camera.capture_continuous(self.rawCapture, format="bgr", use_video_port=True)

        self.frame = None
        self.stopped = False

    def start(self):
        t = Thread(target=self.update, args=())
        t.daemon = True
        t.start()
        return self

    def update(self):
        for f in self.stream:
            self.frame = f.array
            self.rawCapture.truncate(0)

            if self.stopped:
                self.stream.close()
                self.rawCapture.close()
                self.camera.close()
                return

    def read(self):
        return self.frame

    def stop(self):
        self.stopped = True
