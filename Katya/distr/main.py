import sys
from PyQt5 import QtWidgets
from PyQt5.QtWidgets import QFileDialog
from math import *
import matplotlib.pyplot as plt
import numpy as np
import design.design as design
import research

class MyApp(QtWidgets.QMainWindow, design.Ui_MainWindow):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        self.setupSlots()

    def setupOpenFileAction(self):
        def slot(flag: bool):
            self.inputFileName = QFileDialog.getOpenFileName(self, 'Открыть файл', 'data', 'TXT files ( *.txt)')[0]

        self.actionOpen.triggered.connect(slot)

    def setupSaveAsFileAction(self):
        def slot(flag: bool):
            writeFileName = QFileDialog.getSaveFileName(self, 'Сохранить как', 'data', 'TXT files ( *.txt)')[0]
            research.writeToFile(writeFileName, self.dataToWrite)

        self.actionSave.triggered.connect(slot)

    def setupBuildButton(self):
        def slot():
            arr = research.readArray(self.inputFileName, '\t')
            self.dataToWrite = research.evalDistrSum(arr, 1, float(self.distrStep.text()))
        self.buildButton.released.connect(slot)

    def setupSlots(self):
        self.setupOpenFileAction()
        self.setupSaveAsFileAction()
        self.setupBuildButton()

    def _fib_chart(self):
        print('Fib chart clicked')

        func_str = self.fibFunction.text()
        left_bound = eval(self.fibLeft.text())
        right_bound = eval(self.fibRight.text())
        xs = np.linspace(left_bound, right_bound, 100)
        ys = np.array([eval(func_str) for x in list(xs)])

        fig, ax = plt.subplots()
        ax.plot(xs, ys, color='blue', label='f(x)')
        ax.set_xlabel('x')
        ax.set_ylabel('y')
        plt.show()

def main():
    app = QtWidgets.QApplication(sys.argv)
    window = MyApp()
    window.show()
    app.exec_()


if __name__ == '__main__':
    main()
