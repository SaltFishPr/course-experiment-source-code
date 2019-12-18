# -*- coding:utf-8 -*-
import prettytable

partition = []
memory = []


def initial(n):
    global partition, memory
    memory = [False for _ in range(n)]
    partition = [[0, n, False, -1]]  # [start, size, status, PID]


def allocate_memory(size):
    x = -1
    global memory
    num = int(input("请输入进程号："))
    for temp in partition:
        if temp[3] == num:
            print("该进程已存在！")
            return False
    for temp in partition:
        if temp[1] >= size and temp[2] is False:
            x = partition.index(temp)
            break
    if x == -1:
        return False
    if partition[x][1] == size:
        partition[x][2] = True
    else:
        partition.insert(x, [partition[x][0], size, True, num])
        partition[x + 1][0] = partition[x + 1][0] + size
        partition[x + 1][1] = partition[x + 1][1] - size
    for i in range(size):
        memory[partition[x][0] + i] = True
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
    partition[index1][3] = -1


def recovery():
    index_num = -1
    num = int(input("请输入要回收的线程："))
    global partition
    partition.sort(key=lambda y: y[0])
    for temp in partition:
        if temp[3] == num:
            index_num = partition.index(temp)
    if index_num == -1:
        print("没有这个进程!")
        return
    for i in range(partition[index_num][1]):
        memory[partition[index_num][0] + i] = False
    if index_num == 0:
        partition[index_num][2] = False
        if not partition[index_num + 1][2]:
            merge(index_num, index_num + 1)
        else:
            partition[index_num][2] = False
            partition[index_num][3] = -1
    elif index_num == len(partition) - 1:
        partition[index_num][2] = False
        if not partition[index_num - 1][2]:
            merge(index_num - 1, index_num)
        else:
            partition[index_num][2] = False
            partition[index_num][3] = -1
    else:
        partition[index_num][2] = False
        partition[index_num][3] = -1
        if not partition[index_num + 1][2]:
            merge(index_num, index_num + 1)
        if not partition[index_num - 1][2]:
            merge(index_num - 1, index_num)
    check()


def check():
    tb = prettytable.PrettyTable()
    tb.field_names = ["Start", "Size", "Status", "PID"]
    partition.sort(key=lambda y: y[0])
    for temp in partition:
        tem = temp
        if not temp[2]:
            temp = temp[0:2] + ["Free"]
        else:
            temp = temp[0:2] + ["Using"]

        if tem[3] == -1:
            temp += ['']
        else:
            temp += [str(tem[3])]
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
        try:
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
        except ValueError as e:
            print('请输入数字！')
