from djitellopy import Tello
import cv2
import time
######################################################################
width = 320  # WIDTH OF THE IMAGE
height = 240  # HEIGHT OF THE IMAGE
startCounter =0  #  0 FOR FIGHT 1 FOR TESTING
######################################################################

# CONNECT TO TELLO
me = Tello()
me.connect()
me.for_back_velocity = 0
me.left_right_velocity = 0
me.up_down_velocity = 0
me.yaw_velocity = 0
me.speed = 0
print("Battery is")
print(me.get_battery())

me.streamoff()
me.streamon()

while True:
    # GET THE IMGAE FROM TELLO
    frame_read = me.get_frame_read()
    myFrame = frame_read.frame
    img = cv2.resize(myFrame, (width, height))

    # TO GO UP IN THE BEGINNING
    if startCounter == 0:
        # tree 1------------------------------------
        me.tello.move_forward(200)       # Forward 200cms
        def TelloObjectTracking(self):   # Run program
            return self.tello()
        me.tello.rotate_cw(180)          # Rotate 180 degrees clockwise
        me.tello.move_forward(200)       # Forward 200cms
        me.tello.rotate_cw(90)           # Rotate 90 degrees clockwise
        me.tello.move_forward(500)       # Forward 500cms
        me.tello.rotate_cw(90)           # Rotate 90 degrees clockwise
        # tree 2-----------------------------------
        me.tello.move_forward(200)       # Forward 200cms
        def TelloObjectTracking(self):   # Run program
            return self.tello()
        me.tello.rotate_cw(180)          # Rotate 180 degrees clockwise
        me.tello.move_forward(200)       # Forward 200cms
        me.tello.rotate_cw(90)           # Rotate 90 degrees clockwise
        me.tello.move_forward(500)       # Forward 400cms
        me.tello.rotate_cw(90)           # Rotate 90 degrees clockwise
        # tree 3-----------------------------------
        me.tello.move_forward(200)       # Forward 200cms
        def TelloObjectTracking(self):   # Run program
            return self.tello()
        me.tello.rotate_cw(180)          # Rotate 180 degrees clockwise
        me.tello.move_forward(400)       # Forward 400cms
        # tREE 4------------------------------------
        def TelloObjectTracking(self):   # Run program
            return self.tello()
        me.tello.rotate_cw(180)          # Rotate 180 degrees clockwise
        me.tello.move_forward(200)       # Forward 200cms
        me.tello.rotate_cw(90)           # Rotate 90 degrees clockwise
        me.tello.move_forward(500)       # Forward 500cms
        me.tello.rotate_cw(90)           # Rotate 90 degrees clockwise
        # tree 5-----------------------------------
        me.tello.move_forward(200)       # Forward 200cms
        def TelloObjectTracking(self):   # Run program
            return self.tello()
        me.tello.rotate_cw(180)          # Rotate 180 degrees clockwise
        me.tello.move_forward(200)       # Forward 200cms
        me.tello.rotate_cw(90)           # Rotate 90 degrees clockwise
        me.tello.move_forward(500)       # Forward 500cms
        me.tello.rotate_cw(90)           # Rotate 90 degrees clockwise
        # tree 6-----------------------------------
        me.tello.move_forward(200)       # Forward 200cms
        def TelloObjectTracking(self):   # Run program
            return self.tello()
        me.tello.rotate_cw(180)          # Rotate 90 degrees clockwise
        me.tello.move_forward(200)       # Forward 200cms
        me.land()
        startCounter = 1

    # # SEND VELOCITY VALUES TO TELLO
    # if me.send_rc_control:
    #     me.send_rc_control(me.left_right_velocity, me.for_back_velocity, me.up_down_velocity, me.yaw_velocity)

    # DISPLAY IMAGE
    cv2.imshow("MyResult", img)

    # WAIT FOR THE 'Q' BUTTON TO STOP
    if cv2.waitKey(1) & 0xFF == ord('q'):
        me.land()
        break