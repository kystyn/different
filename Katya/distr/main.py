import sys
from PyQt5 import QtWidgets
from PyQt5.QtWidgets import QFileDialog
from math import *
import matplotlib.pyplot as plt
import numpy as np
import design.design as design
import research
import matplotlib.pyplot as plt
import seaborn as sns

class MyApp(QtWidgets.QMainWindow, design.Ui_MainWindow):
    def __init__(self):
        self.plotDirName = 'data'
        super().__init__()
        self.setupUi(self)
        self.distrSaved.setVisible(False)
        self.setupSlots()

    def setupOpenFileAction(self):
        def slot(flag: bool):
            self.inputFileName = QFileDialog.getOpenFileName(self, 'Открыть файл', 'data', 'TXT files ( *.txt)')[0]
            self.loadedFileTxt.setText("Загружен файл: " + self.inputFileName)
            self.distrSaved.setVisible(False)

        self.actionOpen.triggered.connect(slot)

    def setupSaveAsFileAction(self):
        def slot(flag: bool):
            writeFileName = QFileDialog.getSaveFileName(self, 'Сохранить как', 'data', 'TXT files ( *.txt)')[0]
            for startElem, data in self.dataToWrite:
                research.writeToFile(writeFileName + '_' + str(startElem) + '.txt', data)

            self.distrSaved.setVisible(True)

        self.actionSave.triggered.connect(slot)
        self.distParamSave.released.connect(lambda: slot(True))

    def buildHistogram(self):
        for startElem, data in self.dataToWrite:
            plt.figure()
            x = np.asarray([datum.val for datum in data])
            y = np.asarray([datum.mid_energy for datum in data])
            plt.plot(x, y, color='blue')

            # need normalize
            nrm = 0
            maxx = x[np.argmax(y)]
            miny = np.min(y)
            y -= miny
            x -= maxx
            for i in range(1, len(y)):
                nrm += (y[i] + y[i - 1]) * (x[i] - x[i - 1]) / 2

            gaussian = research.approxGauss(x, y / nrm)

            x = np.linspace(data[0].val, data[-1].val, 100)
            plt.plot(x, [gaussian(z - maxx) * nrm + miny for z in x], color='red')


            plt.xlabel('Координата')
            plt.ylabel('Энергия')
            plt.title('Начальное событие: ' + str(startElem))
            plt.savefig(self.plotDirName + '/plot_' + str(startElem) + '.png', dpi=600)
            plt.show()

    def setupBuildButton(self):
        def slot():
            self.rawData = research.readArray(self.inputFileName, '\t')
            self.dataToWrite = []

            sampleSize = int(self.sampleSize.text())
            windowStep = int(self.windowStep.text())

            for startElem in range(0, len(self.rawData), windowStep):
                self.dataToWrite.append\
                    ((startElem, research.evalDistrSum(self.rawData[startElem: max(startElem + sampleSize, len(self.rawData))],
                                           1, float(self.distrStep.text()))))

            self.buildHistogram()

        self.buildButton.released.connect(slot)

    def setupPlotDirectory(self):
        def slot():
            self.plotDirName = QFileDialog.getExistingDirectory(self, 'Сохранить как', 'data')
        self.plotDir.released.connect(slot)

    def setupSlots(self):
        self.setupOpenFileAction()
        self.setupSaveAsFileAction()
        self.setupBuildButton()
        self.setupPlotDirectory()


def main():
    app = QtWidgets.QApplication(sys.argv)
    window = MyApp()
    window.show()
    app.exec_()


if __name__ == '__main__':
    main()
