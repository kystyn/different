import sys
from PyQt5 import QtWidgets
from PyQt5.QtWidgets import QFileDialog, QCheckBox
from math import *
import matplotlib.pyplot as plt
import numpy as np
import design.design as design
import research

class MyApp(QtWidgets.QMainWindow, design.Ui_MainWindow):
    def __init__(self):
        super().__init__()

        self.windowStep = 0
        self.sampleSize = 0
        self.plotDirName = 'data'

        self.rawData = []
        self.energyDataToWrite = []
        self.statsDataToWrite = []
        self.isShowPlots = False

        self.setupUi(self)
        self.plotsAreBuiltLabel.setVisible(False)
        self.energyParamSaved.setVisible(False)
        self.statsParamSaved.setVisible(False)
        self.setupSlots()

    def setupOpenFileAction(self):
        def slot(flag: bool):
            self.inputFileName = QFileDialog.getOpenFileName(self, 'Открыть файл', 'data', 'TXT files ( *.txt)')[0]
            self.loadedFileTxt.setText("Загружен файл: " + self.inputFileName)
            self.energyParamSaved.setVisible(False)
            self.statsParamSaved.setVisible(False)
            self.plotsAreBuiltLabel.setVisible(False)
            self.choosePlotDir.setText('Директорий не выбран')

        self.actionOpen.triggered.connect(slot)

    def setupEnergyParamsAction(self):
        def slot(flag: bool):
            writeFileName = QFileDialog.getSaveFileName(self, 'Сохранить как', 'data', 'TXT files ( *.txt)')[0]
            for startElem, data in self.energyDataToWrite:
                research.writeToFile(writeFileName + '_energy_' + str(startElem) + '.txt', data)

            self.energyParamSaved.setVisible(True)

        self.actionSave.triggered.connect(slot)
        self.energyParamSave.released.connect(lambda: slot(True))

    def setupStatParamsAction(self):
        def slot():
            writeFileName = QFileDialog.getSaveFileName(self, 'Сохранить как', 'data', 'TXT files ( *.txt)')[0]
            research.writeToFile(writeFileName + '_stats.txt', [d[1] for d in self.statsDataToWrite])
            self.statsParamSaved.setVisible(True)

        self.statParamSave.released.connect(slot)

    def buildHistogram(self):
        for startElem, data in self.energyDataToWrite:
            plt.figure()
            x = np.asarray([datum.val for datum in data])
            y = np.asarray([datum.energy for datum in data])
            plt.plot(x, y, color='blue', label=str(startElem) + '-' + str(startElem + self.windowStep))

            # need normalize
            nrm = 0
            maxx = x[np.argmax(y)]
            #miny = np.min(y)
            #y -= miny
            x -= maxx
            for i in range(1, len(y)):
                nrm += (y[i] + y[i - 1]) * (x[i] - x[i - 1]) / 2

            gaussian, mu, sigma = research.approxGauss(x, y / nrm)
            self.statsDataToWrite.append((startElem,
                (self.rawData[min(len(self.rawData) - 1, startElem + self.windowStep)][0],
                 mu + maxx, 2 * sqrt(2 * log(2)) * sigma)))

            x = np.linspace(data[0].val, data[-1].val, 300)
            plt.plot(x, [gaussian(z - maxx) * nrm for z in x], color='red', label='Аппроксимация Гаусс')

            plt.legend()


            plt.xlabel('Координата')
            plt.ylabel('Энергия')
            plt.title('Начальное событие: ' + str(startElem))
            plt.savefig(self.plotDirName + '/plot_' + str(startElem) + '.png', dpi=600)
            if self.showPlots.isChecked():
                plt.show()

    def setupBuildButton(self):
        def slot():
            self.rawData = research.readArray(self.inputFileName, '\t')
            self.energyDataToWrite = []

            self.sampleSize = int(self.sampleSizeLE.text())
            self.windowStep = int(self.windowStepLE.text())

            for startElem in range(0, len(self.rawData), self.windowStep):
                self.energyDataToWrite.append\
                    ((startElem, research.evalDistrSum(self.rawData[startElem: min(startElem + self.sampleSize, len(self.rawData) - 1)],
                                               1, float(self.distrStepLE.text()))))

            self.buildHistogram()
            self.plotsAreBuiltLabel.setVisible(True)

        self.buildButton.released.connect(slot)

    def setupPlotDirectory(self):
        def slot():
            self.plotDirName = QFileDialog.getExistingDirectory(self, 'Сохранить как', 'data')
            self.choosePlotDir.setText('Выбран директорий: ' + self.plotDirName)
        self.plotDir.released.connect(slot)

    def setupShowPlots(self):
        def slot(state):
            self.isShowPlots = (state == 1)
        self.showPlots.stateChanged.connect(slot)

    def setupSlots(self):
        self.setupOpenFileAction()
        self.setupEnergyParamsAction()
        self.setupStatParamsAction()
        self.setupBuildButton()
        self.setupPlotDirectory()


def main():
    app = QtWidgets.QApplication(sys.argv)
    window = MyApp()
    window.resize(app.screens()[0].size())
    window.show()
    app.exec_()


if __name__ == '__main__':
    main()
