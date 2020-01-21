class Criteria:
    def __init__(self, _num, _left, _right):
        self.columnNum = _num
        self.left = _left
        self.right = _right


def readArray(fileName):
    res = [[]]
    f = open(fileName, 'rt')
    for s in f:
        res.append([float(n) for n in s.split(',')])
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

def writeToFile( fileName, array ):
    f = open(fileName, 'w')

    for s in array:
        for st in s:
            f.write(str(st) + ' ')
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
            distr.append([curVal, curCount])
            curVal += step
            curCount = 0
    distr.append([curVal, curCount])
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


def main():
    print('Введите имя входного файла:', end=' ')
    fileName = input()
    arr = readArray(fileName)

    criteries = readCriteries()

    arr = select(arr, criteries)
    print('Введите имя выходного файла для селекции:', end=' ')
    writeToFile(input(), arr)

    readDistrInput(arr)

main()