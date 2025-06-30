#coding:utf-8
import datetime
import sys
import os
import time
import pygame
from pygame import mixer
from pygame.locals import USEREVENT
import multiprocessing as mp
 
 
STOPEVENT     = USEREVENT + 1 #音乐停止事件

timing_list   = []
play_long     = 2 * 10 * 60  # 播放时长
 
def play(start):
    paths = os.listdir('.') #当前目录下所有文件
 
    def ismp3(path):
        return path.endswith(".mp3")

    paths = list(filter(ismp3, paths)) #保留mp3文件
 
    if len(paths) > 0:
        cur = 0

        mixer.init()
        # must be init after mixer
        pygame.init()

        mixer.music.set_endevent(STOPEVENT) #设置音乐停止事件
        mixer.music.set_volume(1.0) #音量控制
        print("play mp3:{}".format(paths[cur]))
        mixer.music.load(paths[cur].encode("utf-8")) #加载,支持中文文件名
        mixer.music.play() #播放
        
        #当超过播放时长后，停止播放
        while (datetime.datetime.now() - start).seconds < play_long:
            time.sleep(1) #1秒监听一次事件
            event = pygame.event.poll()
 
            if event.type == STOPEVENT:
                cur += 1
                if cur != len(paths):
                    print("play next mp3:{}".format(paths[cur]))
                    mixer.music.load(paths[cur].encode("utf-8")) #支持中文文件名
                    mixer.music.play()
                else:
                    print("reach the end; event type:{}".format(event.type))
                    break
            else:
                print("poll event type:{}".format(event.type))    

        mixer.music.set_volume(0.0) #音量静音
        
        mixer.quit()
        pygame.quit()
        
        print("play mp3 quit")
 
 
# 
minute_threshold = 5
def isSpecialMinute(minute):
     return  minute < minute_threshold

def isSpecialHour(now):
     return  now.month == 12 and now.day == 13 and now.hour == 10

def isTiming(now, inSettingList):
    # 避开南京特殊纪念日鸣笛时间
    if isSpecialHour(now) and isSpecialMinute(now.minute):
        print("avoid playing at specail time: {}".format(now))
        return False
        
    for item in inSettingList:
      if now.hour == item[0] and (now.minute == item[1] or (isSpecialHour(now) and isSpecialMinute(item[1]) and now.minute == minute_threshold)):
        return True

    return False
 
def detect():
    while True:
        now = datetime.datetime.now()
        if isTiming(now, timing_list) :
            print("play mp3 begin at {}".format(now))

            #pygame has some bug
            #can't multiple play the same mp3 in the same process
            play_childprocess = mp.Process(target=play,args=(now,))
            play_childprocess.start()
            #wait to play finish
            play_childprocess.join()
        else:
            print("play setting:{}; now_hour:{},now_minute:{}".format(timing_list,now.hour,now.minute))
            time.sleep(1)
 
 
if __name__ == '__main__':
    print("argvs:{}".format(sys.argv))
    if len(sys.argv) >= 2:
       argv_index = 1
       while argv_index < len(sys.argv):
         parse_arg_list = sys.argv[argv_index].split(':')
         timing_list.append((int(parse_arg_list[0]),int(parse_arg_list[1])))
         argv_index += 1
       
       print("Parsed Setting:{}".format(timing_list))
       
       # begin loop
       detect()
    else:
        print("usage python playmusic.py timing_hour:timing_minute ...")