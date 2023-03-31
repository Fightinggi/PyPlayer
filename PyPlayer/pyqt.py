# coding=gbk
print("Python PyPlayer ")
from pyffmpeg import *
import time

winWidth = 592
winHeight = 1296

conf = {"width":winWidth,
        "heigh":winHeight,
        "title":"PyPlayer"}


ff = PyFFmpeg()
def open():
    print(" PyPlayer Open ")
    filename = OpenDialog()
    if ff.open(filename):
        print("open file success!")
        print("ff.fps = ", ff.fps)
    else:
        print("open file failed!")
    # print(filename)

isRunning = True

# ������ �����߳��е��� �߳���c++����
def main():
    print(" PyPlayer main ")
    global ff
    global winWidth
    global winHeight
    last = 0

    while isRunning:
        if ff.fps > 0:
            ms = 1/ff.fps
        else:
            ms = 0.04 # 1/25  25֡
        # ���װ
        re = ff.read()
        if re:
            print(".",end="",flush=True)  #flushˢ�»���
            # ����ͼ��
            re = ff.decode(winWidth,winHeight)
            print(re,end="",flush=True)
            # ��ʾͼ��
            if re:
                cur = time.time()
                interval = cur - last  # ���ʱ��
                s = ms - interval
                if s < 0:
                    s = 0
                if s>1:
                    s=0.1
                print(s,end=" ")
                time.sleep(0.02)
                SetImage(ff)  # ����ͼ��rgb->qt_VideoView
                last = time.time()
            
        else:
            time.sleep(1) # û�д��ļ� open����ûִ�� 
            # print(" unused open func!!!")