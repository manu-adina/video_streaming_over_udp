import cv2

#gst_str_rtp = 'udpsrc port=5000 caps="application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96" ! rtph264depay ! decodebin ! videoconvert ! appsink'
#cap_receive = cv.VideoCapture(gst_str_rtp, cv.CAP_GSTREAMER)

#if not cap_receive.isOpened():
#    print("VideoCapture isn't opened")
#    exit(0)

#while True:
#    ret, frame = cap_receive.read()

#    if not ret:
#        print("Empty frame")
#        break

#    cv.imshow('receive', frame)
#    if cv.waitKey(1)&0xFF == ord('q'):
#        break

#cap_receive.release()
#cv.destroyAllWindows()


cap_receive = cv2.VideoCapture('udpsrc port=5000 ! application/x-rtp,media=video,clock-rate=90000,encoding-name=H264,payload=96 ! rtph264depay ! decodebin ! videoconvert ! appsink', cv2.CAP_GSTREAMER)

if not cap_receive.isOpened():
    print('VideoCapture not opened')
    exit(0)



while True:
    ret,frame = cap_receive.read()
    if not ret:
        print('empty frame')
        break

    cv2.imshow('receive', frame)
    if cv2.waitKey(1)&0xFF == ord('q'):
        break

cap_receive.release()
cv2.destroyAllWindows()
