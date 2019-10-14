import time
import cv2 as cv
from pi_video_stream import PiVideoStream

camera_cap = PiVideoStream()
camera_cap.start()
time.sleep(1)

fps = 30
frame_width = 640
frame_height = 480

gst_str_rtp = "appsrc ! videoconvert ! x264enc tune=zerolatency bitrate=500 speed-preset=superfast ! rtph264pay ! udpsink host=127.0.0.1 port=5000"

out = cv.VideoWriter(gst_str_rtp, cv.CAP_GSTREAMER, 0, fps, (frame_width, frame_height), True)

while True:
    frame = camera_cap.read()
    if cv.waitKey(1)&0xFF == ord('q'):
        break

    if frame is not None:
        cv.imshow("frame", frame)
        out.write(frame)
    else:
        print("Frame capture failed")
        time.sleep(10)

camera_cap.stop()
out.release()
