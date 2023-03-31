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

# 主函数 在子线程中调用 线程是c++创建
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
            ms = 0.04 # 1/25  25帧
        # 解封装
        re = ff.read()
        if re:
            print(".",end="",flush=True)  #flush刷新缓冲
            # 解码图像
            re = ff.decode(winWidth,winHeight)
            print(re,end="",flush=True)
            # 显示图像
            if re:
                cur = time.time()
                interval = cur - last  # 间隔时间
                s = ms - interval
                if s < 0:
                    s = 0
                if s>1:
                    s=0.1
                print(s,end=" ")
                time.sleep(0.02)
                SetImage(ff)  # 解码图像rgb->qt_VideoView
                last = time.time()
            
        else:
            time.sleep(1) # 没有打开文件 open函数没执行 
            # print(" unused open func!!!")