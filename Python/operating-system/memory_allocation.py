#!/usr/bin/env python3
# -*- coding:utf-8 -*-
import prettytable
import random


# 分区
class Partition(object):
    def __init__(self, size):
        self.__mem_list = [[0, 0, size, False]]  # mem_id, start, size, state: False is Free

    def __allocate_memory(self, size):
        x, mem_id = -1, 0
        while mem_id in list(list(zip(*self.__mem_list))[0]):  # 为这块内存随机一个内存号
            mem_id = random.randint(1, 100)
        for temp in self.__mem_list:
            if temp[2] >= size and temp[3] is False:
                x = self.__mem_list.index(temp)
                break
        if x == -1:
            return -1
        if self.__mem_list[x][2] == size:
            self.__mem_list[x][0] = mem_id
            self.__mem_list[x][3] = True
        else:
            self.__mem_list.insert(x, [mem_id, self.__mem_list[x][1], size, True])
            self.__mem_list[x + 1][1] = self.__mem_list[x + 1][1] + size
            self.__mem_list[x + 1][2] = self.__mem_list[x + 1][2] - size
        return mem_id

    # 最先匹配法分配内存
    def allocate_first_fit(self):
        self.__mem_list.sort(key=lambda y: y[1])
        size = int(input("请输入需要分配内存的大小："))
        flag = self.__allocate_memory(size)
        if flag != -1:
            print("分配成功！")
            return flag
        else:
            print("分配失败！")

    # 最优匹配法分配内存
    def allocate_best_fit(self):
        self.__mem_list.sort(key=lambda y: y[2])
        size = int(input("请输入需要分配内存的大小："))
        flag = self.__allocate_memory(size)
        if flag != -1:
            print("分配成功！")
            return flag
        else:
            print("分配失败！")

    # 最坏匹配法分配内存
    def allocate_worst_fit(self):
        self.__mem_list.sort(key=lambda y: y[2], reverse=True)
        size = int(input("请输入需要分配内存的大小："))
        flag = self.__allocate_memory(size)
        if flag != -1:
            print("分配成功！")
            return flag
        else:
            print("分配失败！")

    # index2吞并index1所在的内存块
    def __merge(self, index1, index2):
        self.__mem_list[index2][1] = self.__mem_list[index1][1]
        self.__mem_list[index2][2] += self.__mem_list[index1][2]
        self.__mem_list.remove(self.__mem_list[index1])
        self.__mem_list[index1][3] = False

    def recovery(self, num):
        index_num = -1
        for temp in self.__mem_list:
            if temp[0] == num:
                index_num = self.__mem_list.index(temp)
        if index_num == 0:  # 要回收的内存为第一块内存
            self.__mem_list[index_num][3] = False
            if not self.__mem_list[index_num + 1][3]:  # 后面一块内存是空闲内存
                self.__merge(index_num, index_num + 1)
            else:
                self.__mem_list[index_num][3] = False
        elif index_num == len(self.__mem_list) - 1:  # 要回收的内存为最后一块内存
            self.__mem_list[index_num][3] = False
            if not self.__mem_list[index_num - 1][3]:  # 前面一块内存是空闲内存
                self.__merge(index_num - 1, index_num)
            else:
                self.__mem_list[index_num][3] = False
        else:  # 要回收的内存为中间的内存
            self.__mem_list[index_num][3] = False
            if not self.__mem_list[index_num + 1][3]:
                self.__merge(index_num, index_num + 1)
            if not self.__mem_list[index_num - 1][3]:
                self.__merge(index_num - 1, index_num)

    def memory_check(self):
        memory_table = prettytable.PrettyTable()
        memory_table.field_names = ["Memory ID", "Start", "Size", "Status"]
        self.__mem_list.sort(key=lambda y: y[1])
        for temp in self.__mem_list:
            if not temp[3]:
                temp = temp[0:3] + ["Free"]
            else:
                temp = temp[0:3] + ["Using"]

            if temp[0] == 0:
                temp[0] = ' '
            memory_table.add_row(temp)
        print(memory_table)


# 线程
class MyProcess(object):
    def __init__(self):
        self.__process_list = []  # pid, mem_id

    def create_process(self, mem: Partition, choice: int):
        pid = int(input("请输入进程号："))
        while self.__process_list != [] and pid in list(list(zip(*self.__process_list))[0]):
            pid = int(input("该进程已存在！请重新输入："))
        if choice == 1:
            mem_id = mem.allocate_first_fit()
        elif choice == 2:
            mem_id = mem.allocate_best_fit()
        elif choice == 3:
            mem_id = mem.allocate_worst_fit()
        else:
            return False
        self.__process_list.append([pid, mem_id])
        return True

    def delete_process(self, mem: Partition, pid: int):
        index_num = -1
        for temp in self.__process_list:
            if temp[0] == pid:
                index_num = self.__process_list.index(temp)
                break
        if index_num == -1:
            print("没有这个进程！")
            return False
        mem.recovery(self.__process_list[index_num][1])
        del (self.__process_list[index_num])
        return True

    def process_check(self):
        process_table = prettytable.PrettyTable()
        process_table.field_names = ["PID", "Memory ID"]
        self.__process_list.sort(key=lambda y: y[0])
        for temp in self.__process_list:
            process_table.add_row(temp)
        print(process_table)


if __name__ == '__main__':
    size = int(input("请输入要开辟的内存大小:"))
    memory = Partition(size)
    process = MyProcess()
    while True:
        print("""

*******************
菜单：
1.查看内存使用情况
2.最先匹配法分配内存
3.最优匹配法分配内存
4.最坏匹配法分配内存
5.回收内存
0.退出
请输入选项""", end="：")
        try:
            choice = int(input())
            if choice == 1:
                process.process_check()
                memory.memory_check()
            elif choice == 2:
                process.create_process(memory, 1)
                process.process_check()
            elif choice == 3:
                process.create_process(memory, 2)
                process.process_check()
            elif choice == 4:
                process.create_process(memory, 3)
                process.process_check()
            elif choice == 5:
                pid = int(input("请输入要结束的进程号:"))
                if process.delete_process(memory, pid):
                    print("Success")
                    process.process_check()
                else:
                    print("Fail")
            elif choice == 0:
                print("bye!")
                break
            else:
                print("序号错误，请重新输入。")
        except ValueError as e:
            print('请输入数字！')
