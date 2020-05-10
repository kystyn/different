import numpy as np
import gradient
import math

class DistrData:
    def __init__(self, val, count, energy, mid_energy):
        self.val = val
        self.count = count
        self.energy = energy
        self.mid_energy = mid_energy


class Criteria:
    def __init__(self, _num, _left, _right):
        self.columnNum = _num
        self.left = _left
        self.right = _right


def readArray(fileName, splitter):
    res = [[]]
    f = open(fileName, 'rt')
    for s in f:
        try:
            res.append([float(n) for n in s.split(splitter)])
        except:
            if len(res) > 0:
                res.pop()
    f.close()
    return res


def select(array, criteries):
    i = 0
    while i < len(array):
        broken = False
        for criteria in criteries:
            if criteria.columnNum > len(array[i]) - 1:
                array.remove(array[i])
                broken = True
                break
            if array[i][criteria.columnNum] < criteria.left or \
                    array[i][criteria.columnNum] > criteria.right:
                array.remove(array[i])
                broken = True
                break
        if not broken:
            i = i + 1
    return array


def readCriteries():
    criteries = []
    while True:
        print('Введите номер столбца, нижний и верхние пределы для отбора или нажмите enter для завершения:', end=' ')
        s = input().split()
        if len(s) == 0:
            break
        criteries.append(Criteria(int(s[0]) - 1, float(s[1]), float(s[2])))
    print('Ввод критериев завершён!')
    return criteries


def writeToFile(fileName, array):
    f = open(fileName, 'w')

    for s in array:
        iterator = s

        if type(s) == DistrData:
            iterator = (s.val, s.count, s.energy, s.mid_energy)

        for st in iterator:
            stst = str(st)
            if '.' in stst:
                f.write(stst[:stst.index('.') + 7] + ' ')
            else:
                f.write(stst + ' ')
        f.write('\n')

    f.close()


def makeDistribution( array, column, step ):
    array.sort(key=lambda row: row[column])
    distr = []
    curCount = 0
    curVal = array[0][column]

    arrIdx = 0

    while arrIdx < len(array):
        if curVal <= array[arrIdx][column] < curVal + step:
            curCount += 1
            arrIdx += 1
        else:
            distr.append((curVal, curCount))
            curVal += step
            curCount = 0
    distr.append((curVal, curCount))
    return distr


def evalDistrSum(array, column, step):
    array.sort(key=lambda row: row[column])
    distr = []
    curCount = 0
    curVal = array[0][column]
    curEnergy = 0

    arrIdx = 0

    while arrIdx < len(array):
        if curVal <= array[arrIdx][column] < curVal + step:
            curEnergy += array[arrIdx][2]
            curCount += 1
            arrIdx += 1
        else:
            curVal += step
            distr.append(DistrData(curVal, curCount, curEnergy, curEnergy / curCount))
            curCount = 0
            curEnergy = 0
    distr.append(DistrData(curVal + step, curCount, curEnergy, curEnergy / curCount))
    return distr


def readDistrInput(arr):
    while True:
        print('Введите номер столбца, по которому требуется построить распределение, и шаг, либо enter для завершения:', end=' ')
        s = input().split()
        if len(s) == 0:
            break
        column = int(s[0]) - 1
        step = float(s[1])
        print('Введите имя выходного файла для распределения:', end=' ')
        writeToFile(input(), makeDistribution(arr, column, step))


def distr():
    print('Введите имя входного файла:', end=' ')
    fileName = input()
    arr = readArray(fileName, ',')

    criteries = readCriteries()

    arr = select(arr, criteries)
    print('Введите имя выходного файла для селекции:', end=' ')
    writeToFile(input(), arr)

    readDistrInput(arr)


def energySum():
    print('Введите имя входного файла:', end=' ')
    fileName = input()
    arr = readArray(fileName, '\t')

    print('Введите шаг:', end=' ')
    arr = evalDistrSum(arr, 1, float(input()))

    print('Введите имя выходного файла для селекции:', end=' ')
    writeToFile(input(), arr)


def approxGauss(x, y, nrm=1.0):
    n = len(x)

    def gaussian(x, mu, sigma):
        return nrm / (sigma * math.sqrt(2 * math.pi)) * math.exp(-((x - mu) ** 2) / (2 * sigma ** 2))

    def func(z):
        mu, sigma = z[0], z[1]
        res = 0

        for i in range(n):
            res += (gaussian(x[i], mu, sigma) - y[i]) ** 2

        return res

    def func_gradient(z):
        mu, sigma = z[0], z[1]

        res_mu = 0
        res_sigma = 0

        # mu
        for i in range(n):
            res_mu += 2 * (gaussian(x[i], mu, sigma) - y[i]) * \
                   gaussian(x[i], mu, sigma) * (x[i] - mu) / (sigma ** 2)

        # sigma
        for i in range(n):
            res_sigma += 2 * (gaussian(x[i], mu, sigma) - y[i]) * nrm / math.sqrt(2 * math.pi) *\
                         ((-sigma ** 2 + (x[i] - mu) ** 2) / (sigma ** 4)) *\
                         math.exp(-((x[i] - mu) ** 2) / (2 * sigma ** 2))

        return np.asarray([res_mu, res_sigma])

    solution = gradient.optimal_gradient(func, func_gradient, np.asarray([0, 1]), 1e-5)

    return (lambda x: gaussian(x, solution[0][0], solution[0][1])), solution[0][0], solution[0][1]
