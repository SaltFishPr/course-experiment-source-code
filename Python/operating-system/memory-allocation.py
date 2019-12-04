# -*- coding:utf-8 -*-
import prettytable

memory = []
partition = []


def initial(n):
    global memory, partition
    memory = [False for x in range(n)]  # False means "Free", True means "Busy"
    partition = [[0, n, False]]  # [start, size, status, index_num]


def allocate_memory(size):
    x = -1
    for temp in partition:
        if temp[1] >= size and temp[2] is False:
            x = partition.index(temp)
            break
    if x == -1:
        return False
    if partition[x][1] == size:
        partition[x][2] = True
    else:
        partition.insert(x, [partition[x][0], size, True])
        partition[x + 1][0] = partition[x + 1][0] + size
        partition[x + 1][1] = partition[x + 1][1] - size
    return True


def allocate_first_fit():
    partition.sort(key=lambda y: y[0])
    size = int(input("请输入需要分配内存的大小："))
    if allocate_memory(size):
        print("分配成功！")
    else:
        print("分配失败！")


def allocate_best_fit():
    partition.sort(key=lambda y: y[1])
    size = int(input("请输入需要分配内存的大小："))
    if allocate_memory(size):
        print("分配成功！")
    else:
        print("分配失败！")


def allocate_worst_fit():
    partition.sort(key=lambda y: y[1], reverse=True)
    size = int(input("请输入需要分配内存的大小："))
    if allocate_memory(size):
        print("分配成功！")
    else:
        print("分配失败！")


def merge(index1, index2):
    partition[index2][0] = partition[index1][0]
    partition[index2][1] += partition[index1][1]
    partition.remove(partition[index1])
    partition[index1][2] = False


def recovery():
    index_num = int(input("请输入要回收的内存（index以check的顺序为准，从0开始）："))
    global partition
    partition.sort(key=lambda y: y[0])
    if not partition[index_num][2]:
        print("该段内存是空闲内存！")
        return
    if index_num == 0:
        partition[index_num][2] = False
        if not partition[index_num + 1][2]:
            merge(index_num, index_num + 1)
        else:
            partition[index_num][2] = False
    elif index_num == len(partition) - 1:
        partition[index_num][2] = False
        if not partition[index_num - 1][2]:
            merge(index_num - 1, index_num)
        else:
            partition[index_num][2] = False
    else:
        partition[index_num][2] = False
        if not partition[index_num + 1][2]:
            merge(index_num, index_num + 1)
        if not partition[index_num - 1][2]:
            merge(index_num - 1, index_num)
    check()


def check():
    tb = prettytable.PrettyTable()
    tb.field_names = ["Start", "Size", "Status"]
    partition.sort(key=lambda y: y[0])
    for temp in partition:
        if not temp[2]:
            temp = temp[0:2] + ["Free"]
        else:
            temp = temp[0:2] + ["Using"]
        tb.add_row(temp)
    print(tb)


if __name__ == '__main__':
    print("*****初始化内存空间中...*****")
    print("请输入所需内存大小", end="：")
    initial(int(input()))
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
        choice = int(input())
        if choice == 1:
            check()
        elif choice == 2:
            allocate_first_fit()
        elif choice == 3:
            allocate_best_fit()
        elif choice == 4:
            allocate_worst_fit()
        elif choice == 5:
            recovery()
        elif choice == 0:
            print("bye!")
            break
        else:
            print("序号错误，请重新输入。")
