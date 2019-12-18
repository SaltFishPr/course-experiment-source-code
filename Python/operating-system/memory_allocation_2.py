#!/usr/bin/env python3
# -*- coding:utf-8 -*-
import prettytable

threads = []  # 线程
partitions = []  # 内存分区


# 线程
class MyThread(object):
    def __init__(self, pid, mem_id):
        self.__pid = pid
        self.__mem_id = mem_id

    def get_pid(self):
        return self.__pid

    def get_start(self):
        return self.__mem_id

    def create_thread(self):
        pass
    pass


# 分区
class Partition(object):
    def __init__(self, start, size, mem_id=-1, state=False):
        self.__mem_id = mem_id
        self.__start = start
        self.__size = size
        self.__state = state

    def get_mem_id(self):
        return self.__mem_id

    def get_start(self):
        return self.__start

    def get_size(self):
        return self.__size

    def get_state(self):
        return self.__state

    pass


def initial(n):
    global threads, partitions
    threads = []
    partitions = [Partition(0, n)]


if __name__ == '__main__':
    pass
