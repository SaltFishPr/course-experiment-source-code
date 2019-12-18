#!/usr/bin/env python3
# -*- coding: utf-8 -*-


class MyDisk(object):
    def __init__(self, position, size, direction):
        self.__magnetic_head = position
        self.__size = size
        self.__order = []
        self.__sum = 0
        self.__direction = direction  # True is Left

    def __work(self, nums: list, nums_a: list):
        for num in nums:
            if num > self.__size:
                print("输入的第%d个地址越界！", nums_a.index(num))
                return False
            if num <= self.__magnetic_head:
                self.__direction = True
                self.__sum += self.__magnetic_head - num
                self.__magnetic_head = num
                self.__order.append([self.__magnetic_head, self.__direction])
            else:
                self.__direction = False
                self.__sum += num - self.__magnetic_head
                self.__magnetic_head = num
                self.__order.append([self.__magnetic_head, self.__direction])
        return True

    def fcfs(self, nums: list):
        if self.__work(nums, nums):
            self.__print_info()
        else:
            print("Error!")

    def sstf(self, nums: list):
        pos = self.__magnetic_head
        nums_a = sorted(nums, key=lambda y: abs(y - pos))
        if self.__work(nums_a, nums):
            self.__print_info()
        else:
            print("Error!")

    def scan(self, nums):
        pos = self.__magnetic_head
        if self.__direction:  # 向左
            nums_a = [x for x in nums if x <= pos]
            nums_b = [x for x in nums if x > pos]
            nums_a.sort(key=lambda y: pos - y)
            step1 = self.__work(nums_a, nums)
            nums_b.sort(key=lambda y: y - pos)
            step2 = self.__work(nums_b, nums)
        else:  # 向右
            nums_a = [x for x in nums if x >= pos]
            nums_b = [x for x in nums if x < pos]
            nums_a.sort(key=lambda y: y - pos)
            step1 = self.__work(nums_a, nums)
            nums_b.sort(key=lambda y: pos - y)
            step2 = self.__work(nums_b, nums)
        if step1 and step2:
            self.__print_info()
        else:
            print("Error!")

    def __print_info(self):
        print("磁头移动顺序：")
        print(self.__order[0][0], end='')
        for x in self.__order[1:]:
            print(' -> '+str(x[0]), end='')
        print()
        print("总共移动了："+str(self.__sum))
        self.__order = []
        self.__sum = 0


def input_nums():
    print("请输入磁盘访问请求序列（中间用空格隔开）:")
    nums = [int(x) for x in input().split()]
    return nums


if __name__ == '__main__':
    my_disk = MyDisk(0, 1000, False)
    while True:
        try:
            choice = int(input("""

*******************
菜单：
1.初始化磁盘(默认磁头位置：0，磁盘大小：1000，方向：右)
2.先来先服务
3.最短寻道时间优先
4.扫描算法
0.退出
请输入选项:"""))
            if choice == 1:
                a = int(input("初始化磁头位置:"))

                b = input("初始化磁头移动方向（L: left, R: right）:")
                if b == 'L':
                    b = True
                elif b == 'R':
                    b = False
                else:
                    print("输入错误，请重新输入")
                    continue
                c = int(input("磁盘内存大小:"))
                my_disk = MyDisk(a, c, b)
            elif choice == 2:
                nums = input_nums()
                my_disk.fcfs(nums)
            elif choice == 3:
                nums = input_nums()
                my_disk.sstf(nums)
            elif choice == 4:
                nums = input_nums()
                my_disk.scan(nums)
            elif choice == 0:
                print("bye!")
                break
            else:
                print("序号错误，请重新输入。")
        except ValueError as e:
            print('请输入数字！')

